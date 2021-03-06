#!/bin/bash

module load libraries/openmpi-1.6-gcc-4.4.6

mpic++ -Wall -g -lm fractal.cpp -o fractal

time (mpirun -np 7 fractal --height 1000 --width 1000 --colors 5 --iterations 80 --left -2.0 --right 1.0 --top 1.0 --bottom -1.0 --mode complex )
