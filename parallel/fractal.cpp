//============================================================================
// Name        : fractal.cpp
// Author      : Alexandru Cozma
// Version     :
// Copyright   : (C) UPD Project
// Description : Fractal Generator in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

#include "defaults.h"
#include "fractal.h"

int Fractal::init(int argc, char *argv[]) {

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--out") == 0 && i + 1 < argc) {
            f = fopen(argv[i + 1], "w");
        }

        if (strcmp(argv[i], "--width") == 0 && i + 1 < argc) {
            width = atoi(argv[i + 1]);
        }

        if (strcmp(argv[i], "--height") == 0 && i + 1 < argc) {
            height = atoi(argv[i + 1]);
        }

        if (strcmp(argv[i], "--iterations") == 0 && i + 1 < argc) {
            max_iterations = atoi(argv[i + 1]);
        }

        if (strcmp(argv[i], "--left") == 0 && i + 1 < argc) {
            left = strtold(argv[i + 1], NULL);
        }

        if (strcmp(argv[i], "--right") == 0 && i + 1 < argc) {
            right = strtold(argv[i + 1], NULL);
        }

        if (strcmp(argv[i], "--top") == 0 && i + 1 < argc) {
            top = strtold(argv[i + 1], NULL);
        }

        if (strcmp(argv[i], "--bottom") == 0 && i + 1 < argc) {
            bottom = strtold(argv[i + 1], NULL);
        }

        if (strcmp(argv[i], "--colors") == 0 && i + 1 < argc) {
            noCol = atol(argv[i + 1]);
            if (noCol < 1 || noCol > 10) {
                cout << "ERROR: Maximum number of colors is 10." << endl;
                return 1;
            }
        }
    }

    if (left < LEFT) {
        cout << "ERROR: The left start point is not valid. Left can't be lower than "" << LEFT << ""." << endl;
        return 4;
    }

    if (right > RIGHT) {
        cout << "ERROR: The right start point is not valid. Right can't be greater than " << RIGHT << "." << endl;
        return 4;
    }

    if (top > TOP) {
        cout << "ERROR: The top start point is not valid. Top can't be greater than " << TOP << "." << endl;
        return 4;
    }

    if (bottom < BOTTOM) {
        cout << "ERROR: The bottom start point is not valid. Bottom can't be lower than " << BOTTOM << "." << endl;
        return 4;
    }

    if (max_iterations < 1) {
        cout << "ERROR: The number of iterations must be greater than zero." << endl;
        return 3;
    }

    if (width == 0 || height == 0) {
        cout << "ERROR: The size of the output image was not specified: ./main --height <INT> --width <INT>" << endl;
        return 2;
    }

    if (f == NULL) {
        f = fopen("fractal.pgm", "w");
        cout << "WARNING: No specified output file. Using \"fractal.pgm\" as output file." << endl;
    }

    return 0;
}

int Fractal::processFractal(int argc, char *argv[]) {
    //----------------------------------->>
    //      INIT
    //----------------------------------->>

    if (int initValue = init(argc, argv) != 0) {
        cout << "Initialization returned error value: " << initValue << endl;
        return initValue;
    }

    //----------------------------------->>
    //      PROCESSING...
    //----------------------------------->>

    fprintf ( f, "P3\n");
    fprintf ( f, "%d %d\n", width, height);
    fprintf ( f, "255\n");

    for (y = 0; y < width; y++) {
        for (x = 0; x < height; x++) {
            zr = 0.0;
            zi = 0.0;
            cr = left + x * (right - left) / width;

            ci = top + y * (bottom - top) / height;
            rsquared = zr * zr;
            isquared = zi * zi;

            for (count = 0;
                    rsquared + isquared <= 4.0 && count < max_iterations;
                    count++) {
                zi = zr * zi * 2;
                zi += ci;

                zr = rsquared - isquared;
                zr += cr;

                rsquared = zr * zr;
                isquared = zi * zi;
            }

            // If we are inside the fractal we make the color black.
            if (rsquared + isquared <= 4.0) {
                fprintf ( f, "0 0 0 ");
            }
            // Else, if we are outside the fractal, we need to choose the gradient color.
            else {
                if (noCol == 1) {
                    color1 = COLORS[0];
                    color2 = COLORS[0];
                    colorPercent = 1.0;
                }
                else {
                    count = (count * (noCol - 2.0)) / max_iterations;
                    colorNo = (int) floor(count);
                    color1 = COLORS[colorNo];
                    color2 = COLORS[colorNo + 1];
                    colorPercent = count / (floor(count) + 1);
                }

                fprintf ( f, "%d %d %d ",
                        interpolate(color1[0], color2[0], colorPercent),
                        interpolate(color1[1], color2[1], colorPercent),
                        interpolate(color1[2], color2[2], colorPercent));
            }
        }

        fprintf (f, "\n");
    }

    fclose(f);
    return 0;
}

int Fractal::interpolate(int c1, int c2, double x){
    return c1 + ((c2 - c1) * x);
}

int main(int argc, char *argv[]) {

    Fractal fractal;

    return fractal.processFractal(argc, argv);
}
