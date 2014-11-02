#!/bin/bash

make
./fractal  --height 1000 --width 1000 --colors 5 --iterations 80 --left -1.5 --right -1.0 --top 0.0 --bottom -0.5
