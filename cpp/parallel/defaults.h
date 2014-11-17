//============================================================================
// Name        : defaults.h
// Author      : Alexandru Cozma
// Version     :
// Copyright   : (C) UPD Project
// Description : Fractal Generator in C++, Ansi-style
//============================================================================
#ifndef DEFAULTS_H_
#define DEFAULTS_H_

#define MAX_ITERATIONS 80

#define LEFT     -2.0
#define RIGHT     1.0
#define TOP       1.0
#define BOTTOM   -1.0

#define WORKTAG 1
#define DIETAG 2

#define REQUEST_DATA	1
#define STOP_PROCESS	2
#define PROCESS_TASK	3

int COLORS[10][3] = {{255, 0  , 0  },  // 0
                      {255, 100, 0  },  // 1
                      {255, 255, 0  },  // 2
                      {0  , 100, 255},  // 3
                      {0  , 255, 255},  // 4
                      {25 , 10 , 40 },  // 5
                      {55 , 10 , 0  },  // 6
                      {25 , 156, 30 },  // 7
                      {15 , 95 , 80 },  // 8
                      {0  , 80 , 24 }}; // 9

struct InputData{
	long double left;
    long double right;
    long double top;
    long double bottom;
	int width;
    int height;
	int i;
	int j;
	int msgType;
};

#endif /* DEFAULTS_H_ */
