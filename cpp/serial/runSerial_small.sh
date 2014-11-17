#!/bin/bash

make
time (./fractal --height 20 --width 20 --colors 5 --iterations 80 --left -1.4 --right -1.1 --top 0.0 --bottom -0.2)
