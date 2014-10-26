#include "stdio.h"
#include "math.h"

#define MaxIters 80
#define SIZE     4000
#define BLACK    -1
#define LEFT     -2.0
#define RIGHT    1.0
#define TOP      1.0
#define BOTTOM   -1.0

int interpolate(int c1, int c2, double X){
    int Z = c1 + ((c2 - c1) * X);
    if(Z > 255 || Z<0){
        printf("%d %d %f %d\n", c1,c2,X,Z);
    }
    return c1 + ((c2 - c1) * X);
}

main(int argc, char *argv[])
{
    short   x, y, count;
    long double zr, zi, cr, ci;
    long double rsquared, isquared;

int R,G,B;
            double iteration = count;
            int max_iteration = MaxIters;
            int *color1, *color2;
            double nu, zn;

            int nrCol = 5;
            int palette[5][3]= {{255,0,0},
                                {255,100,0},
                                {255,255,0},
                                {0,100,255},
                                {0,255,255}};
                                
    FILE *f;
    f = fopen("test.pgm", "w");

    fprintf(f,"P3\n");
    fprintf(f,"%d %d\n", SIZE, SIZE);
    fprintf(f,"255\n");    
    
    for (y = 0; y < SIZE; y++)
    {
        for (x = 0; x < SIZE; x++)
        {
            zr = 0.0;
            zi = 0.0;
            cr = LEFT + x * (RIGHT - LEFT) / SIZE;

            ci = TOP + y * (BOTTOM - TOP) / SIZE;
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

           if (rsquared + isquared <= 4.0){
                fprintf(f,"0 0 0 ");
            }
            else{
                //fprintf(f,"255 0 0 ");
                iteration = count;
                iteration = (iteration-1) / ((MaxIters)/(nrCol-1));

              color1 = palette[(int)floor(iteration)];
              color2 = palette[(int)floor(iteration) + 1];
              
              //printf("count:  %d", count);
              
              if((int)floor(iteration)>3){
                printf("boom");
                return 0;
              }
              
              fprintf(f,"%d %d %d ",
                interpolate(color1[0], color2[0], iteration - (int)iteration),
                interpolate(color1[1], color2[1], iteration - (int)iteration),
                interpolate(color1[2], color2[2], iteration - (int)iteration));
           }   
        }
        fprintf(f,"\n");
    }
    return 0;
}
