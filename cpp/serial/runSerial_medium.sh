#!/bin/bash

make
time (./fractal --height 5000 --width 5000 --colors 10 --iterations 80 --left -1.5 --right -1.0 --top 0.0 --bottom -0.5)
