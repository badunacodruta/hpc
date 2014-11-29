//============================================================================
// Name        : fractal.h
// Author      : Alexandru Cozma
// Version     :
// Copyright   : (C) UPD Project
// Description : Fractal Generator in C++, Ansi-style
//============================================================================
#ifndef FRACTAL_H_
#define FRACTAL_H_

class Fractal {
    public:
        void init();
        int init(int argc, char *argv[]);
        int processFractal(int argc, char *argv[]);

        Fractal() {
            f = NULL;
            max_iterations = MAX_ITERATIONS;

            noCol = 1;
            left = LEFT;
            right = RIGHT;
            top = TOP;
            bottom = BOTTOM;
			
			map_percent = 0.1;
			withThreads = false;
			threadType = NONE;
			runningMode = SIMPLE;
			complexProcessSize = 32;
        }

    private:
        int interpolate(int c1, int c2, double x);
		void mpiMap();
		void master();
		void slave();
		void masterComplex();
		void slaveComplex();
		void mapMPIv1();
		void mapMPIv2();
		void printHelp();
		int  processData(InputData mapData);

		int complexProcessSize;

		bool withThreads;
		int threadType;
		int runningMode;
		
        int x, y;
        double count;
        int i, colorNo;
        double colorPercent;
        long double zr, zi, cr, ci;
        long double rsquared, isquared;

        int max_iterations;
        int *color1, *color2;

        FILE *f;
        int width;
        int height;

		long double map_percent;
        int noCol;
        long double left;
        long double right;
        long double top;
        long double bottom;

		int mpiRank;
		int mpiSize;

		vector<InputData> dataPool;
		vector<InputData> jobsToProcess;
		vector< vector<InputData> > jobs;
		
		MPI_Datatype InputDataType;
};

#endif /* FRACTAL_H_ */
