//
//  main.c
//  Matrix Transposition
//
//  Created by Matteo Gottardelli on 16/11/24.
//
//Access to the same node (pbs script and insert select=1:ncpus=64:ompthreads=64:mem=1Gb)
//Too avoid outlayer
//qsub -I -q short_cpuQ -l select=1:ncpus=64:ompthreads=64:mem=1Gb
//NO -O3
//mpicc hello.c -o hello
//mpirun --oversubscribe -np 8 ./hello
#include <stdio.h>
#include <mpi.h>
//#include "functions.h"

int main(int argc, const char * argv[]) {
    //Initialization
    int count=0;
    double time=0.0;
    float** M=NULL;
    float** T=NULL;
    //struct timeval start_tv, end_tv;
    double tw_start=0.0, tw_end=0.0;
    //Input parameters
    inputParameters(argc);
    const char* CODE=argv[1];
    const Mode MODE=valueInputed(argc, argv[2], 2);
    const int N=valueInputed(argc, argv[3], 3);
    const int TESTING=valueInputed(argc, argv[4], 4);
    const int SAMPLES=valueInputed(argc, argv[5], 5);
    const int NUM_THREADS=threadInit(argv[6], argc, MODE);
    if(N/2<MINIMUMSUBLENGTH) {
        fprintf(stderr, "Error, input dimension under the limit\n");
        exit(1);
    }
    int SUBLENGTH=MAX(MIN(N/2, MAXIMUMSUBLENGTH/NUM_THREADS), MINIMUMSUBLENGTH);
    printf("%d\n\n", SUBLENGTH);
    double* results=malloc(sizeof(double)*SAMPLES);
    if (results==NULL) {
        fprintf(stderr, "ERROR Memory Allocation");
        MPI_Finalize();
        exit(1);
    }
    int rank;
    //Optimized Length Function
    while (count<SAMPLES) {
        //Allocation in memory
        clearAllCache();
        M=allocateMatrixPerMode(MODE, N);
        T=allocateMatrixPerMode(MODE, N);
        initializeMatrix(M, TESTING, N);
        MPI_Init(&argc, &argv);
        //Setuping
        MPI_Comm_size(MPI_COMM_WORLD, &NUM_THREADS);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        printf("Hello from rank %d out of %d processes\n", rank, NUM_THREADS);
        //Starting
        tw_start=MPI_Wtime();
        //Modalities
        bool symmetric=executionProgram(M, T, MODE, N, my_rank, size);
        //Ending
        tw_end=MPI_Wtime();
        MPI_Finalize();
        time=(tw_end-tw_start);
        printf("Time Elapsed (get time)=%.12f\n", time);
        //Free Memory
        freeMemory(M, N);
        if(!symmetric) {
            freeMemory(T, N);
        }
        //Incrementing
        openFilesResultsPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_THREADS, time);
        results[count]=time;
        count++;
    }
    //Printing
    //Take the 10 values in the middle
    const int TAKE_SAMPLES=SAMPLES/5*2;//40%
    int start=SAMPLES/2-TAKE_SAMPLES/2;
    int end=SAMPLES/2+TAKE_SAMPLES/2;
    double total_time=0.0;
    bubbleSort(results, SAMPLES);
    for (int i=start; i<end; i++) {
        total_time+=results[i];
        printf("%.12lf\n", results[i]);
    }
    printf("\n\nFINAL RESULTS WITH:\nMODE %d\nDimension: %d\nTesting: %d\nSamples: %d\nThreads: %d\nAverage Time: %.12lf secs\n\n", MODE, N, TESTING, SAMPLES, NUM_THREADS, total_time/TAKE_SAMPLES);
    openFilesAvgPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_THREADS, total_time/TAKE_SAMPLES);
    free(results);
    return 0;
}
