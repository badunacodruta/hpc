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
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <mpi.h>
#include <vector>
#include <algorithm>

using namespace std;

#include "defaults.h"
#include "fractal.h"


void Fractal::printHelp(){
	cout << "Fractal Help:" << endl;
	cout << endl;
	cout << "\t-h\t\t - Prints this help message." << endl;
	cout << "\t--h\t\t - Prints this help message." << endl;
	cout << "\t-help\t\t - Prints this help message." << endl;
	cout << "\t--help\t\t - Prints this help message." << endl;	
	cout << endl;
	cout << "\t--out\t\t - The name of the output files." << endl;
	cout << "\t\t\t\t eg: --out fractal.pgm" << endl;
	cout << "\t\t\t\t OBS: The output type is PGM." << endl;
	cout << endl;
	cout << "\t--width\t\t - The width of the final picture in pixels." << endl;
	cout << "\t\t\t\t eg: --width 10000" << endl;
	cout << "\t--height\t - The height of the final picture in pixels." << endl;
	cout << "\t\t\t\t eg: --height 10000" << endl;
	cout << endl;
	cout << "\t--left\t\t - The left coordinate of the fractal." << endl;
	cout << "\t\t\t\t eg: --left -2.0" << endl;
	cout << "\t--right\t\t - The right coordinate of the fractal." << endl;
	cout << "\t\t\t\t eg: --right 1.0" << endl;
	cout << "\t--top\t\t - The top coordinate of the fractal." << endl;
	cout << "\t\t\t\t eg: --top 1.0" << endl;
	cout << "\t--bottom\t - The bottom coordinate of the fractal." << endl;
	cout << "\t\t\t\t eg: --bottom -1.0" << endl;
	cout << endl;
	cout << "\t--colors\t - The number of colors to use." << endl;
	cout << "\t\t\t\t eg: --colors 10" << endl;
	cout << endl;
	cout << "\t--map_percent\t - The division percent of the resolution." << endl;
	cout << "\t\t\t\t eg: --map_percent 0.1" << endl;
	cout << endl;
	cout << "\t--mode\t\t - The mode we run the program." << endl;
	cout << "\t\t\t\t eg: --mode simple/complex" << endl;
	cout << "\t--threads\t - If we run the loops with multiple threads. Default is off" << endl;
	cout << "\t\t\t\t eg: --threads on/off" << endl;
	cout << "\t--thread_type\t - If we use threads to run the loops, what kind of threads do we use." << endl;
	cout << "\t\t\t\t eg: --thread_type openmp/tbb/cilk" << endl;
	cout << "\t\t\t\t OBS: Be sure to use the correct compiler and libraries." << endl;
	cout << endl;
	cout << "\t--process_dim\t - The pre-process dimmension. Default is 32 pixels." << endl;
	cout << "\t\t\t\t eg: --process_dim 32" << endl;
	cout << endl;

	return;
}

int Fractal::init(int argc, char *argv[]) {

    for (i = 1; i < argc; i++) {
	    if (strcmp(argv[i], "-h")     == 0  ||
	    	strcmp(argv[i], "--h")    == 0  || 
	    	strcmp(argv[i], "-help")  == 0 ||
	    	strcmp(argv[i], "--help") == 0
	    ){
	    	printHelp();
            return -1;
        }

		if (strcmp(argv[i], "--process_dim") == 0 && i + 1 < argc) {
            complexProcessSize = atoi(argv[i + 1]);
        }

        if (strcmp(argv[i], "--out") == 0 && i + 1 < argc) {
            f = fopen(argv[i + 1], "w");
        }

		if (strcmp(argv[i], "--mode") == 0 && i + 1 < argc) {
            if (strcmp(argv[i+1], "simple") == 0){
            	runningMode = SIMPLE;
            }
            else if (strcmp(argv[i+1], "complex") == 0){
            	runningMode = COMPLEX;
            }
            else{
            	cout << "ERROR: Mode (" << argv[i] << ") can only be 'simple' or 'complex'." << endl;
                return 5;
            }
        }

		if (strcmp(argv[i], "--threads") == 0 && i + 1 < argc) {
			if (strcmp(argv[i+1], "on") == 0){
				withThreads = true;
            }
            else if (strcmp(argv[i+1], "off") == 0){
	            withThreads = false;
            }
            else{
            	cout << "ERROR: Threads can only be 'on' or 'off'." << endl;
                return 6;
            }
		}

		if (strcmp(argv[i], "--thread_type") == 0 && i + 1 < argc) {
			if (strcmp(argv[i+1], "openmp") == 0){
				withThreads = OPENMP;
            }
            else if (strcmp(argv[i+1], "tbb") == 0){
	            withThreads = TBB;
            }
            else if (strcmp(argv[i+1], "cilk") == 0){
            	withThreads = CILK;
            }
            else{
            	cout << "ERROR: Threads type can only be 'openmp', 'tbb' or 'cilk'." << endl;
                return 7;
            }
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

		if (strcmp(argv[i], "--map_percent") == 0 && i + 1 < argc) {
            map_percent = strtold(argv[i + 1], NULL) / 100;
        }		
    }

	if (!withThreads && withThreads != NONE){
		cout << "WARNING: You specified to use a type of threads, but did not enabled the threads. The program will run without threads." << endl;
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

	// Start MPI Communication
	MPI_Init (&argc, &argv);
	// Get the current process ID
	MPI_Comm_rank (MPI_COMM_WORLD, &mpiRank);
	// Get the total number of processes
	MPI_Comm_size (MPI_COMM_WORLD, &mpiSize);

    return 0;
}

void Fractal::mapMPIv1(){
	int widthStep  = width  * map_percent;
	int heightStep = height * map_percent;
	
	long double horizontalStep = (right - left) * map_percent;
	long double verticalStep   = (top - bottom) * map_percent;

	cout << "Width Step = " << widthStep << endl;
	cout << "Height Step = " << heightStep << endl;
	cout << "H Step = " << horizontalStep << endl;
	cout << "V Step = " << verticalStep << endl;

	cout << "top = " << top << endl;
	cout << "bottom = " << bottom << endl;
	cout << "left = " << left << endl;
	cout << "right = " << right << endl;
	
	long double h = left;
	long double v = top;
	for (int i=0 ; i<width; i+=widthStep){
		h = left;
		for (int j=0 ; j<height; j+=heightStep){
			InputData mapData;
			mapData.width  = widthStep;
			mapData.height = heightStep;
			mapData.left   = h;
			mapData.right  = h + horizontalStep;
			mapData.top	   = v;
			mapData.bottom = v - verticalStep;
			mapData.i	   = i;
			mapData.j	   = j;
			mapData.msgType = PROCESS_TASK;
			
			dataPool.push_back(mapData);
			
			h += horizontalStep;
		}
		v -= verticalStep;
	}
}

void Fractal::mapMPIv2(){
	mapMPIv1();

	for (std::vector<InputData>::iterator it = dataPool.begin() ; it != dataPool.end(); ++it){
		(*it).severity = processData(*it);
	}

	sort(dataPool.begin(), dataPool.end(), dataSort());

	for(int i=0 ; i<mpiSize ; i++){
		vector<InputData> job;
		jobs.push_back(job);
	}

	InputData mapData;
	int minJobID;
	int minSum = -1;
	int sum;

	cout << endl;
	cout << "Total number of jobs: " << dataPool.size() << endl;
	while(!dataPool.empty ()){
		mapData = dataPool.back ();
		dataPool.pop_back ();		

		for (unsigned int i=0 ; i<jobs.size() ; i++){
			vector<InputData> job = jobs[i];
			
			sum = 0;
			for (unsigned int j=0 ; j<job.size() ; j++){
				InputData data = job[j];
				sum += data.severity;
			}

			if(minSum == -1 || minSum >= sum){
				minJobID = i;
				minSum = sum;
			}
		}

		jobs[minJobID].push_back(mapData);
		minSum += mapData.severity;
	}

	for (unsigned int i=0 ; i<jobs.size() ; i++){
		vector<InputData> job = jobs[i];
		cout << "Jobs for process " << i+1 << ": " << job.size() << "\t Severities: ";

		sum = 0;
		for (unsigned int j=0 ; j<job.size() ; j++){
			InputData data = job[j];
			cout << " " << data.severity;
			sum += data.severity;
		}
		cout <<"  - Sev Sum: "<< sum << endl;
	}
}

void printData(InputData data){
	cout << " Width = " << data.width << endl;
	cout << " height = " << data.height << endl;
	cout << " left = " << data.left << endl;
	cout << " right = " << data.right << endl;
	cout << " top = " << data.top << endl;
	cout << " bottom = " << data.bottom << endl;
}

int Fractal::processData(InputData mapData){
	int width  = complexProcessSize;
	int height = complexProcessSize;

	int insideCounter = 0;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			zr = 0.0;
			zi = 0.0;
			cr = mapData.left + x * (mapData.right - mapData.left) / width;

			ci = mapData.top + y * (mapData.bottom - mapData.top) / height;
			rsquared = zr * zr;
			isquared = zi * zi;

			for (count = 0; rsquared + isquared <= 4.0 && count < max_iterations; count++) {
				zi = zr * zi * 2;
				zi += ci;
				zr = rsquared - isquared;
				zr += cr;

				rsquared = zr * zr;
				isquared = zi * zi;
			}

			// If we are inside the fractal we make the color black.
			if (rsquared + isquared <= 4.0) {
				insideCounter ++;
			}
		}
	}

	return ((3*insideCounter)/(complexProcessSize * complexProcessSize)) + 1;
}

void Fractal::masterComplex(){
	mapMPIv2();

	for (int i=1 ; i<mpiSize ; i++){
		vector<InputData> jobsv = jobs[i];
		for (unsigned int j=0 ; j<jobsv.size() ; j++){
			InputData job = jobsv[j];

			MPI_Send(&job, 1, InputDataType, i, 1, MPI_COMM_WORLD);
		}

		InputData startProcess;
		startProcess.msgType = START_PROCESS;
		MPI_Send(&startProcess, 1, InputDataType, i, 1, MPI_COMM_WORLD);
	}

	jobsToProcess = jobs[0];
	slaveComplex();
}

void Fractal::slaveComplex(){
	InputData incomingData, data;
	MPI_Status status;

	if(mpiRank != BOSS){
		while(true){
			MPI_Recv(&incomingData, 1, InputDataType, 0, 1, MPI_COMM_WORLD, &status);
			if(incomingData.msgType == START_PROCESS){
				break;
			}

			jobsToProcess.push_back(incomingData);
		}
	}

	while(!jobsToProcess.empty ()){
		data = jobsToProcess.back ();
		jobsToProcess.pop_back ();

		width  = data.width;
		height = data.height;
		left   = data.left;
		right  = data.right;
		top    = data.top;
		bottom = data.bottom;

		char buff[100];
		sprintf(buff, "fractal-%d-%d.pgm", data.i, data.j);
		cout << "Writing to " << buff << endl;
			
		f = fopen(buff, "w");
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
	}
}

void Fractal::master(){
	mapMPIv1();

	InputData mapData;
	InputData incomingData;
	MPI_Status status;

	while(!dataPool.empty ()){
		MPI_Recv(&incomingData, 1, InputDataType, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
		if(incomingData.msgType == REQUEST_DATA){
			mapData = dataPool.back ();
			dataPool.pop_back ();
			MPI_Send(&mapData, 1, InputDataType, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
		}
	}

	for(int i = 1; i < mpiSize ; i++){
		MPI_Recv(&incomingData, 1, InputDataType, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
		mapData.msgType = STOP_PROCESS;
		MPI_Send(&mapData, 1, InputDataType, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
	}
}

void Fractal::slave(){
	InputData incomingData, requestData;
	MPI_Status status;

	while(true){
		requestData.msgType = REQUEST_DATA;
		MPI_Send(&requestData, 1, InputDataType, 0, 1, MPI_COMM_WORLD);
			
		MPI_Recv(&incomingData, 1, InputDataType, 0, 1, MPI_COMM_WORLD, &status);
		if(incomingData.msgType == STOP_PROCESS){
			return;
		}
		if(incomingData.msgType == PROCESS_TASK){
			width  = incomingData.width;
			height = incomingData.height;
			left   = incomingData.left;
			right  = incomingData.right;
			top    = incomingData.top;
			bottom = incomingData.bottom;

			char buff[100];
			sprintf(buff, "fractal-%d-%d.pgm", incomingData.i, incomingData.j);
			cout << "Writing to " << buff << endl;
			
			f = fopen(buff, "w");
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
		}
	}
}

void Fractal::mpiMap(){
	char hostname[MPI_MAX_PROCESSOR_NAME];
	int len;
	MPI_Get_processor_name(hostname, &len);
	cout << "This is MPI Process " << mpiRank << " on hostname " <<  hostname << endl;
	
	InputData mpiData;

    MPI_Datatype type[2] = { MPI_LONG_DOUBLE, MPI_INT};
	int blocklen[2] = { 4, 6 };

	MPI_Aint array_of_displaysments[2];
	MPI_Aint address1, address2, address3;
	MPI_Get_address(&mpiData,&address1);
	MPI_Get_address(&mpiData.left,&address2);
	MPI_Get_address(&mpiData.width,&address3);
	array_of_displaysments[0] = address2 - address1;
	array_of_displaysments[1] = address3 - address2;

    MPI_Type_create_struct(2, blocklen, array_of_displaysments, type, &InputDataType);
    MPI_Type_commit(&InputDataType);
	
	if(mpiRank == BOSS){
		if (runningMode == SIMPLE){
			master();
		}
		else if (runningMode == COMPLEX){
			masterComplex();
		}
	}
	else{
		if (runningMode == SIMPLE){
			slave();
		}
		else if (runningMode == COMPLEX){
			slaveComplex();
		}
	}
}

int Fractal::processFractal(int argc, char *argv[]) {
    //----------------------------------->>
    //      INIT
    //----------------------------------->>

	int initValue = init(argc, argv);
    if ( initValue != 0) {
    	if (initValue < 0){
	    	return initValue;
	    }

        cout << "Initialization returned error value: " << initValue << endl;
        return initValue;
    }

	//----------------------------------->>
    //      MPI
    //----------------------------------->>

    mpiMap();

	MPI_Finalize();
    return 0;
}

int Fractal::interpolate(int c1, int c2, double x){
    return c1 + ((c2 - c1) * x);
}

int main(int argc, char *argv[]) {

    Fractal fractal;

    return fractal.processFractal(argc, argv);
}
