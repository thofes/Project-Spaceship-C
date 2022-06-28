#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


    double acc[100];
    double acc_x[100];
    double acc_y[100];
    double speed_new = 0;
    double speed_max = 0;
    double rot[100];
    double rot_new = 0;
    double temp[100];
    double temp_max = 0;
    double temp_min = 0;
    double temp_ges = 0;
    double temp_average = 0;
    double pos_x[100];
    double pos_y[100];
    double dist = 0;
    double dist_max = 0;
    double dist_ges = 0;
    double var = 0;
    double temp_sekt[136];
    double counter[136];
    double temp_av[136];
    int s = 0;




void temperatur (double *temp){
     for (int i=0; i<100; i++){
        if (i==0){
            temp_max = temp[i];
            temp_min = temp[i];
            temp_ges += temp[i];
        } else {
        temp_ges += temp[i];
        if (temp[i] > temp_max)
            temp_max = temp[i];
        if (temp[i] < temp_min)
            temp_min = temp[i];

        }

    }

    temp_average = temp_ges/100;

}

void speed (double *acc_x, double *acc_y){
for (int i=0; i<100; i++){

    speed_new = hypot(acc_x[i], acc_y[i]);
        if (speed_new > speed_max)
            speed_max = speed_new;
}


}

void distanz (double *pos_x, double *pos_y){
for (int i=0; i<100; i++){
    dist = hypot(pos_x[i], pos_y[i]);
    if (dist > dist_max)
        dist_max = dist;
    dist_ges += hypot(acc_x[i], acc_y[i]);
}

}

void varianz (double *temp){
    for (int i=0; i<100; i++){
        var += (temp[i] - temp_ges/100) * (temp[i] - temp_ges/100);

    }
var = var / ( 100 );
}

void csv (double *rot, double *acc){
    for (int i=0; i<100; i++){
        rot_new += rot[i];
        if (i==0){
            acc_x[i] = cos(rot_new)*acc[i];
            acc_y[i] = sin(rot_new)*acc[i];
            pos_x[i] = acc_x[i];
            pos_y[i] = acc_y[i];
        } else {
            acc_x[i] = cos(rot_new)*acc[i] + acc_x[i-1];
            acc_y[i] = sin(rot_new)*acc[i] + acc_y[i-1];
            pos_x[i] = pos_x[i-1] + acc_x[i];
            pos_y[i] = pos_y[i-1] + acc_y[i];

        }





        FILE *file_new = fopen("spaceship_data_new.csv", "a");
            if (file_new != NULL) {
                    if (i==0)
                        fprintf(file_new, "PositionX, PositionY, Orientierung\n%.18f, %.18f, %.18f\n", pos_x[i], pos_y[i], rot_new);
                    else
                        fprintf(file_new, "%.18f, %.18f, %.18f\n", pos_x[i], pos_y[i], rot_new);
            } else {
                printf("Fehler beim Öffnen der Datei!\n");
                }
        fclose(file_new);


    }
}

void latex (double dist_ges, double dist_max, double speed_max, double temp_min, double temp_max, double temp_average, double var){

char start[] = "\\begin{document}";
char end[] = "\\end{document}";
char doc[] = "\\documentclass{minimal}";

FILE *file_latex = fopen("Raumschiff_Data.tex", "w");
            if (file_latex != NULL) {
                fprintf(file_latex, "%s\n", doc);
                fprintf(file_latex, "%s\n", start);
                fprintf(file_latex, "Raumschiffdaten:\n \\\\");
                fprintf(file_latex, "Gesamte Distanz = %.18f \\\\ Maximale Distanz = %.18f \\\\ Maximale Geschwindigkeit = %.18f \\\\ Minimale Temperatur = %.15f \\\\ Maximale Temperatur = %.14f \\\\ Durchschnittliche Temperatur = %.18f \\\\ Varianz = %.15f\n", dist_ges, dist_max, speed_max, temp_min, temp_max, temp_average, var);
                fprintf(file_latex, "%s\n", end);
            } else {
                printf("Fehler beim Öffnen der Datei!\n");
                }
        fclose(file_latex);


}

void svg (double *pos_x, double *pos_y){
    for (int i=0;i<99;i++){
    FILE *file_svg = fopen("Raumschiff_Data.svg", "a");
            if (file_svg != NULL) {
                if (i==0){
                    fprintf(file_svg, "<svg height=\"170\" width=\"85\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n");
                    fprintf(file_svg, "<line x1=\"%.18f\" y1=\"%.18f\" x2=\"%.18f\" y2=\"%.18f\" stroke=\"blue\" stroke-width=\"5\"/>\n", 25+pos_x[i], pos_y[i]*(-1), 25+pos_x[i+1], pos_y[i+1]*(-1)); // Nullpunkt um 25 nach rechts und Y-Achse invertieren für SVG
                }
                else
                    fprintf(file_svg, "<line x1=\"%.18f\" y1=\"%.18f\" x2=\"%.18f\" y2=\"%.18f\" stroke=\"blue\" stroke-width=\"5\"/>\n", 25+pos_x[i], pos_y[i]*(-1), 25+pos_x[i+1], pos_y[i+1]*(-1));
                if (i==98)
                    fprintf(file_svg, "</svg>");

            } else {
                printf("Fehler beim Öffnen der Datei!\n");
                }
        fclose(file_svg);
}

}

void svg_heatmap (double *pos_x, double *pos_y, double *temp){

for (int i=0; i<100; i++){


for(int x=0; x<170; x+=10){

	for(int k=0; k<80; k+=10){
		s++;

		if (pos_x[i] <= -10+k && pos_x[i] >= -20+k && pos_y[i] <= 0-x && pos_y[i] >= -10-x){

			temp_sekt[s] += temp[i];
            counter[s] +=1;
			temp_av[s] = temp_sekt[s]/counter[s];

        }

	}

}

s = 0;
}




for (int i=0; i<99; i++){

FILE *file_svg_heatmap = fopen("Raumschiff_Data_Heatmap.svg", "a");
            if (file_svg_heatmap != NULL) {
                if (i==0){
                    fprintf(file_svg_heatmap, "<svg height=\"170\" width=\"85\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n");
                    fprintf(file_svg_heatmap, "<rect x=\"5\" y=\"0\"  width=\"85\" height=\"170\" style=\"fill:gray\" />\n");
                }



for(int x=0; x<170; x+=10){
	for(int k=0; k<80; k+=10){
		s++;
		int a = 5+k; //Verschiebung des Nullpunkts um 25 nach rechts
		int b = 0+x; //Invertieren der Y-Achse
		if (pos_x[i] <= -10+k && pos_x[i] >= -20+k && pos_y[i] <= 0-x && pos_y[i] >= -10-x){

			if(temp_av[s] <10)
                fprintf(file_svg_heatmap, "<rect x=\"%d\" y=\"%d\"  width=\"10\" height=\"10\" style=\"fill:blue\" />\n", a, b);
            if(temp_av[s] >10 && temp_av[s] <20)
                fprintf(file_svg_heatmap, "<rect x=\"%d\" y=\"%d\"  width=\"10\" height=\"10\" style=\"fill:yellow\" />\n", a, b);
			if(temp_av[s] >20 && temp_av[s] <30)
                fprintf(file_svg_heatmap, "<rect x=\"%d\" y=\"%d\"  width=\"10\" height=\"10\" style=\"fill:orange\" />\n", a, b);
			if(temp_av[s] >30 && temp_av[s] <40)
                fprintf(file_svg_heatmap, "<rect x=\"%d\" y=\"%d\"  width=\"10\" height=\"10\" style=\"fill:red\" />\n", a, b);


        }


	}


}
    if (i==98)
        fprintf(file_svg_heatmap, "</svg>");

	} else {
		printf("Fehler beim Öffnen der Datei!\n");
        }
        fclose(file_svg_heatmap);

s = 0;
}


}

void svg_temperatur (double *pos_x, double *pos_y, double *temp){
    for (int i=0;i<99;i++){
    FILE *file_svg_temperatur = fopen("Raumschiff_Data_Temperatur.svg", "a");
            if (file_svg_temperatur != NULL) {
                if (i==0){
                    fprintf(file_svg_temperatur, "<svg height=\"170\" width=\"85\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n");
                    if (temp[i]<10)
                        fprintf(file_svg_temperatur, "<line x1=\"%.18f\" y1=\"%.18f\" x2=\"%.18f\" y2=\"%.18f\" stroke=\"blue\" stroke-width=\"5\"/>\n", 25+pos_x[i], pos_y[i]*(-1), 25+pos_x[i+1], pos_y[i+1]*(-1)); // Nullpunkt um 25 nach rechts und Y-Achse invertieren für SVG
                    if (temp[i]>10 && temp[i]<20)
                        fprintf(file_svg_temperatur, "<line x1=\"%.18f\" y1=\"%.18f\" x2=\"%.18f\" y2=\"%.18f\" stroke=\"yellow\" stroke-width=\"5\"/>\n", 25+pos_x[i], pos_y[i]*(-1), 25+pos_x[i+1], pos_y[i+1]*(-1));
                    if (temp[i]>20 && temp[i]<30)
                        fprintf(file_svg_temperatur, "<line x1=\"%.18f\" y1=\"%.18f\" x2=\"%.18f\" y2=\"%.18f\" stroke=\"orange\" stroke-width=\"5\"/>\n", 25+pos_x[i], pos_y[i]*(-1), 25+pos_x[i+1], pos_y[i+1]*(-1));
                    if (temp[i]>30)
                        fprintf(file_svg_temperatur, "<line x1=\"%.18f\" y1=\"%.18f\" x2=\"%.18f\" y2=\"%.18f\" stroke=\"red\" stroke-width=\"5\"/>\n", 25+pos_x[i], pos_y[i]*(-1), 25+pos_x[i+1], pos_y[i+1]*(-1));


                }
                else
                    if (temp[i]<10)
                        fprintf(file_svg_temperatur, "<line x1=\"%.18f\" y1=\"%.18f\" x2=\"%.18f\" y2=\"%.18f\" stroke=\"blue\" stroke-width=\"5\"/>\n", 25+pos_x[i], pos_y[i]*(-1), 25+pos_x[i+1], pos_y[i+1]*(-1));
                    if (temp[i]>10 && temp[i]<20)
                        fprintf(file_svg_temperatur, "<line x1=\"%.18f\" y1=\"%.18f\" x2=\"%.18f\" y2=\"%.18f\" stroke=\"yellow\" stroke-width=\"5\"/>\n", 25+pos_x[i], pos_y[i]*(-1), 25+pos_x[i+1], pos_y[i+1]*(-1));
                    if (temp[i]>20 && temp[i]<30)
                        fprintf(file_svg_temperatur, "<line x1=\"%.18f\" y1=\"%.18f\" x2=\"%.18f\" y2=\"%.18f\" stroke=\"orange\" stroke-width=\"5\"/>\n", 25+pos_x[i], pos_y[i]*(-1), 25+pos_x[i+1], pos_y[i+1]*(-1));
                    if (temp[i]>30)
                        fprintf(file_svg_temperatur, "<line x1=\"%.18f\" y1=\"%.18f\" x2=\"%.18f\" y2=\"%.18f\" stroke=\"red\" stroke-width=\"5\"/>\n", 25+pos_x[i], pos_y[i]*(-1), 25+pos_x[i+1], pos_y[i+1]*(-1));

                    if (i==98)
                        fprintf(file_svg_temperatur, "</svg>");

            } else {
                printf("Fehler beim Öffnen der Datei!\n");
                }
        fclose(file_svg_temperatur);
}

}



int main()
{


//--------------------------------------------- Daten einlesen --------------------------------

    FILE *file = fopen("spaceship_data.csv", "r");
    if (file != NULL) {
        for (int i=0;i<100;i++){

        fscanf(file, "%lf,%lf,%lf\n", &acc[i], &rot[i], &temp[i]);

        }


    } else {
        printf("Fehler beim öffnen der Datei!\n");
    }
    fclose(file);



//---------------------------------------- Berechnungen -------------------------------------------------------------------------

csv (rot, acc);
temperatur (temp);
speed (acc_x, acc_y);
distanz (pos_x, pos_y);
varianz (temp);
latex (dist_ges, dist_max, speed_max, temp_min, temp_max, temp_average, var);
svg_temperatur (pos_x, pos_y, temp);
svg (pos_x, pos_y);
svg_heatmap (pos_x, pos_y, temp);


//------------------------------- Racketen Animation ---------------------------


const char rocket[] =
"           ^ \n\
          /^\\\n\
          |-|\n\
          | |\n\
          | |\n\
          | |\n\
          | |\n\
          | |\n\
         /| |\\\n\
        / | | \\\n\
       |  | |  |\n\
        `-\"\"\"-`\n\
";

for (int i = 0; i < 30; i++)
    {
        printf("\n");
    }

fputs(rocket,stdout);

for (int i=0; i<2; i++)
    {
        for (int delay=0; delay<170000000; delay++){

        }
        printf("        ()()()\n");
    }

for (int i=0; i<5; i++)
    {
        for (int delay=0; delay<150000000; delay++){

        }
        printf("      ()()()()()\n");
    }

for (int i=0; i<5; i++)
    {
        for (int delay=0; delay<100000000; delay++){

        }
        printf("    ()()()()()()()\n");
    }

for (int i=0; i<5; i++)
    {
        for (int delay=0; delay<80000000; delay++){

        }
        printf("    ()()()()()()()\n");
    }

for (int i=0; i<10; i++)
    {
        for (int delay=0; delay<70000000; delay++){

        }
        printf("    ()()()()()()()\n");
    }

for (int i=0; i<10; i++){
    printf("\n");
}

//------------------------------- Ergebnisse -------------------------------


        printf("Gesamte Distanz = %.18f\n", dist_ges);
        printf("Maximale Distanz = %.18f\n", dist_max);
        printf("Maximale Geschwindigkeit = %.18f\n", speed_max);
        printf("Minimale Temperatur = %.15f\n", temp_min);
        printf("Maximale Temperatur = %.14f\n", temp_max);
        printf("Durchschnittliche Temperatur = %.18f\n", temp_average);
        printf("Varianz = %.15f\n", var);




    return 0;
}
