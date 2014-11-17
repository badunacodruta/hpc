#!/bin/bash

module load libraries/openmpi-1.6-gcc-4.4.6

mpic++ -Wall -g -lm fractal.cpp -o fractal

/opt/tools/libraries/openmpi/1.6-gcc-4.4.6/bin/mpirun -np 8 fractal --height 10000 --width 10000 --colors 10 --iterations 80 --left -2.0 --right 1.0 --top 1.0 --bottom -1.0
