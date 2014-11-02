#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>

#define MaxIters 80

#define LEFT     -2.0
#define RIGHT    1.0
#define TOP      1.0
#define BOTTOM   -1.0

int interpolate(int c1, int c2, double X){
    int Z = c1 + ((c2 - c1) * X);
    if(Z > 255 || Z<0){
        //  printf("%d %d %f %d\n", c1,c2,X,Z);
    }
    return c1 + ((c2 - c1) * X);
}

main(int argc, char *argv[])
{

    //----------------------------------->>
    //       DECLARATIONS
    //----------------------------------->>
    
    short   x, y, count, i;
    long double zr, zi, cr, ci;
    long double rsquared, isquared;

    int R, G, B;
    double iteration = count;
    int max_iteration = MaxIters;
    int *color1, *color2;
    double nu, zn;

    FILE *f = NULL;
    int width = 0, height = 0;

    int noCol;
    long double left   = LEFT,
                right  = RIGHT,
                top    = TOP,
                bottom = BOTTOM;

    int palette[10][3] = {{255, 0  , 0  },  // 0
                          {255, 100, 0  },  // 1
                          {255, 255, 0  },  // 2
                          {0  , 100, 255},  // 3
                          {0  , 255, 255},  // 4
                          {25 , 10 , 40 },  // 5
                          {55 , 10 , 0  },  // 6
                          {25 , 156, 30 },  // 7
                          {15 , 95 , 80 },  // 8
                          {0  , 80 , 24 }}; // 9

    //----------------------------------->>
    //      INIT
    //----------------------------------->>

    for (i=1 ; i<argc; i++){
        if (strcmp(argv[i], "--out") == 0 && i+1 < argc){
            f = fopen(argv[i+1], "w");
        }

        if (strcmp(argv[i], "--width") == 0 && i+1 < argc){
            width = atoi(argv[i+1]);
        }

        if (strcmp(argv[i], "--height") == 0 && i+1 < argc){
            height = atoi(argv[i+1]);
        }

        if (strcmp(argv[i], "--left") == 0 && i+1 < argc){
            left = strtold (argv[i+1], NULL);
        }

        if (strcmp(argv[i], "--right") == 0 && i+1 < argc){
            right = strtold (argv[i+1], NULL);
        }

        if (strcmp(argv[i], "--top") == 0 && i+1 < argc){
            top = strtold (argv[i+1], NULL);
        }

        if (strcmp(argv[i], "--bottom") == 0 && i+1 < argc){
            bottom = strtold (argv[i+1], NULL);
        }

        if (strcmp(argv[i], "--colors") == 0 && i+1 < argc){
            noCol = atol(argv[i+1]);
            if(noCol < 1 || noCol > 10){
                printf("ERROR: Maximum number of colors is 10.\n");
                return 1;
            }
        }
    }

    if (width == 0 || height == 0){
        printf("ERROR: The size of the output image was not specified: ./main --height <INT> --width <INT>\n");
        return 2;
    }

    if (f == NULL){
        f = fopen("fractal.pgm", "w");
        printf("WARNING: No specified output file. Using \"frctal.pgm\" as output file.\n");
    }

    //----------------------------------->>
    //      PROCESSING...
    //----------------------------------->>
    
    fprintf(f,"P3\n");
    fprintf(f,"%d %d\n", width, height);
    fprintf(f,"255\n");    


    for (y = 0; y < width; y++)
    {
        for (x = 0; x < height; x++)
        {
            zr = 0.0;
            zi = 0.0;
            cr = LEFT + x * (right - left) / width;

            ci = TOP + y * (bottom - top) / height;
            rsquared = zr * zr;
            isquared = zi * zi;

            for (count = 0; rsquared + isquared <= 4.0
                            && count < MaxIters; count++)
            {
                zi = zr * zi * 2;
                zi += ci;

                zr = rsquared - isquared;
                zr += cr;

                rsquared = zr * zr;
                isquared = zi * zi;
            }

            // If we are inside the fractal we make the color black.
            if (rsquared + isquared <= 4.0){
                fprintf(f,"0 0 0 ");
            }
            // Else, if we are outside the fractal, we need to choose the gradient color.
            else{
                iteration = count;
                iteration = (iteration-1) / ((MaxIters)/(noCol-1));

                color1 = palette[(int)floor(iteration)];
                color2 = palette[(int)floor(iteration) + 1];
              
                if ( (int)floor(iteration) > 3 ){
                    printf ("Boom \n");
                    return 0;
                }

                fprintf (f, "%d %d %d ",
                         interpolate(color1[0], color2[0], iteration - (int)iteration),
                         interpolate(color1[1], color2[1], iteration - (int)iteration),
                         interpolate(color1[2], color2[2], iteration - (int)iteration));
           }   
        }

        fprintf (f, "\n");
    }

    fclose (f);
    return 0;
}


