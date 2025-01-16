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
#include "functions.h"

float* globalsendptr=NULL;
float* localrecvptr=NULL;
float* globalrecvptr=NULL;
float* localsendptr=NULL;
MPI_Comm actual_comm;
Transposer transposer;

int main(int argc, char * argv[]) {
    //Initialization
    int i, j;
    int count=0;
    float** M=NULL;
    float** MGEN=NULL;
    float** T=NULL;
    float** TGEN=NULL;
    float** tempM=NULL;
    double time=0.0;
    //struct timeval start_tv, end_tv;
    double tw_start=0.0, tw_end=0.0;
    //Input parameters
    inputParameters(argc);
    const char* CODE=argv[1];
    const Mode MODE=valueInputed(argc, argv[2], 2);
    const int N=valueInputed(argc, argv[3], 3);
    const int TESTING=valueInputed(argc, argv[4], 4);
    const int SAMPLES=valueInputed(argc, argv[5], 5);
    double* results;
    results=(double*)malloc(sizeof(double)*SAMPLES);
    if (results==NULL) {
        fprintf(stderr, "ERROR Memory Allocation");
        MPI_Finalize();
        exit(1);
    }
    int rank, NUM_PROCS;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_PROCS);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n_x=1, n_y=1;
    int rows=1;
    int over=0;
    int ranges[1][3];
    if (NUM_PROCS>N) {
        ranges[0][0]=N;
        
    }else {
        ranges[0][0]=NUM_PROCS;
    }
    ranges[0][1]=NUM_PROCS-1;
    ranges[0][2]=1;
    if(rank==0) {
        if (NUM_PROCS > N) {
            printf("Number of threads %d exceeds N=%d\n", NUM_PROCS, rank);
            for (i=ranges[0][0]; i<=ranges[0][1]; i++) {
                printf("Drop rank %d out of %d\n", i+1, NUM_PROCS);
            }
        }
    }
    if(NUM_PROCS>N) {
        MPI_Group all_procs;
        MPI_Comm_group(MPI_COMM_WORLD, &all_procs);
        MPI_Group exceed_procs;
        MPI_Group_range_excl(all_procs, 1, ranges, &exceed_procs);
        MPI_Comm_create(MPI_COMM_WORLD, exceed_procs, &actual_comm);
    }
    else {
        int color=0;
        MPI_Comm_split(MPI_COMM_WORLD, color, rank, &actual_comm);
    }
    over=ranges[0][1]-ranges[0][0]+1;
    if(actual_comm==MPI_COMM_NULL) {
        MPI_Finalize();
        exit(1);
    }
    NUM_PROCS=MIN(NUM_PROCS, N);
    if(MODE==MPI_BLOCK || MODE==MPI_BLOCK_OPT) {
        n_x=(int)sqrt(NUM_PROCS);
        n_y=(int)sqrt(NUM_PROCS);
        rows=N/n_y;
        if(N%(n_x*n_y)!=0) {
            printf("Number of threads %d is not the square root of N=%d\n", NUM_PROCS, rank);
            MPI_Finalize();
            return 0;
        }
        int dims[2]={n_x, n_y};
        int periods[2]={1, 1};
        int reorder=1;
        MPI_Cart_create(actual_comm, 2, dims, periods, reorder, &actual_comm);
        MPI_Comm_rank(actual_comm, &transposer.rank_start);
        MPI_Cart_coords(actual_comm, transposer.rank_start, 2, transposer.coords_start);
        transposer.coords_dest[1]=transposer.coords_start[0];
        transposer.coords_dest[0]=transposer.coords_start[1];
        MPI_Cart_rank(actual_comm, transposer.coords_dest, &transposer.rank_dest);
        printf("Rank %d -> Cartesian Rank Start %d, Coordinates (%d, %d) - Cartesian Rank End %d, Coordinates (%d, %d)\n", rank, transposer.rank_start, transposer.coords_start[0], transposer.coords_start[1], transposer.rank_dest, transposer.coords_dest[0], transposer.coords_dest[1]);
        MPI_Barrier(actual_comm);
    }
    else {
        if(MODE==SEQ && NUM_PROCS!=1) {
            printf("This mode can be run only with 1 process", NUM_PROCS, rank);
            MPI_Finalize();
            return 1;
        }
        if (N % NUM_PROCS == 0) {
            rows=N/NUM_PROCS;
        } else {
            rows=N/NUM_PROCS+(rank<N%NUM_PROCS ? 1 : 0);
        }
        n_x=1;
        n_y=NUM_PROCS;
        printf("Hello rank %d out of %d\n", rank+1, NUM_PROCS);
    }
    printf("%d/%d\t", rank, NUM_PROCS);
    Communicator2D sender_mpi_all;
    int size[2]={N, N};
    int subsizes[2];
    int starts[2]={0, 0};
    Communicator2D gen_matrix;
    subsizes[0]=N;
    subsizes[1]=N;
    setupCommunicator(&gen_matrix, size, subsizes, starts, subsizes[0]*subsizes[1]);
    DataCommunicate sending, receiving;
    //For create data x for columns y for rows
    if(MODE==MPI_ALL || MODE==SEQ) {
        createData(&sending, 1, NUM_PROCS);
        createData(&receiving, 1, n_y);
    }
    else {
        if(MODE==MPI_BLOCK || MODE==MPI_BLOCK_OPT) {
            createData(&sending, n_x, n_y);
            createData(&receiving, n_x, n_y);
        }
    }
    if(rank==0) {
        if(MODE==MPI_ALL || MODE==SEQ) {
            dataPopulate(&sending, 1, 0, 1, 0);
        }
    }
    //Creation cartesian communicator
    while (count<SAMPLES) {
        //Allocation in memory
        clearAllCache();
        //Setuping
        create2DFloatMatrix(&MGEN, N, N);
        if(rank==0) {
            create2DFloatMatrix(&TGEN, N, N);
            initializeMatrix(MGEN, TESTING, N);
            //printMatrix(MGEN, N, N);
            globalrecvptr=&(TGEN[0][0]);
        }
        globalsendptr=&(MGEN[0][0]);
        subsizes[0]=N;
        subsizes[1]=N;
        setupCommunicator(&gen_matrix, size, subsizes, starts, subsizes[0]*subsizes[1]);
        commitCommunicator(&gen_matrix);
        MPI_Bcast(globalsendptr, 1, gen_matrix.resized_type, 0, actual_comm);
        //printf("===== RANK %d =====\n", rank);
        //printMatrix(MGEN, N, N);
        freeCommunicator(&gen_matrix);
        if(MODE==MPI_ALL || MODE==SEQ) {
            create2DFloatMatrix(&M, rows, N);
            create2DFloatMatrix(&T, N, rows);
            size[0]=N;
            size[1]=N;
            subsizes[0]=rows;
            subsizes[1]=N;
            starts[0]=0;
            starts[1]=0;
            setupCommunicator(&sender_mpi_all, size, subsizes, starts, subsizes[0]*subsizes[1]);
            commitCommunicator(&sender_mpi_all);
        }
        else {
            if(MODE==MPI_BLOCK || MODE==MPI_BLOCK_OPT) {
                create2DFloatMatrix(&M, rows, rows);
                create2DFloatMatrix(&T, rows, rows);
                //commitCommunicator(&sender_mpi_block);
            }
            if(MODE==MPI_BLOCK_OPT) {
                create2DFloatMatrix(&tempM, rows, rows);
            }
        }
        localrecvptr=&(M[0][0]);
        //Starting
        if(rank==0) {
            tw_start=MPI_Wtime();
        }
        bool symmetry=executionProgram(MGEN, M, T, TGEN, tempM, MODE, N, rows, rank, sending, receiving, sender_mpi_all);
        //Ending
        //MPI_Barrier(actual_comm);
        if(rank==0) {
            tw_end=MPI_Wtime();
            time=(tw_end-tw_start);
            if (symmetry) {
                printf("The matrix is symmetric.\n");
            } else {
                printf("The matrix is NOT symmetric.\n");
            }
            printf("Time Elapsed (get time)=%.12f\n", time);
            if(!symmetry) {
                //printMatrix(TGEN, N, N);
                control(MGEN, TGEN, N);
            }
            openFilesResultsPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_PROCS, time);
            results[count]=time;
        }
        free2DMemory(&MGEN);
        free2DMemory(&M);
        free2DMemory(&T);
        if(MODE==MPI_ALL || MODE==SEQ) {
            freeCommunicator(&sender_mpi_all);
        }
        if(MODE==MPI_BLOCK_OPT) {
            free2DMemory(&tempM);
        }
        count++;
        MPI_Barrier(actual_comm);
    }
    freeData(&sending);
    freeData(&receiving);
    if(rank!=0) {
        MPI_Finalize();
        return 0;
    }
    NUM_PROCS+=over;
    //Take the 10 values in the middle
    const int TAKE_SAMPLES=SAMPLES/5*2;//40%
    int start, end;
    //if(NUM_PROCS!=64) {
        start=SAMPLES/2-TAKE_SAMPLES/2;
        end=SAMPLES/2+TAKE_SAMPLES/2;
    /*}
    else {
        start=0;
        end=TAKE_SAMPLES;
    }*/
    double total_time=0.0;
    bubbleSort(results, SAMPLES);
    for (i=start; i<end; i++) {
        total_time+=results[i];
        printf("%.12lf\n", results[i]);
    }
    printf("\n\nFINAL RESULTS WITH:\nMODE %d\nDimension: %d\nTesting: %d\nSamples: %d\nThreads: %d\nAverage Time: %.12lf secs\n\n", MODE, N, TESTING, SAMPLES, NUM_PROCS, total_time/TAKE_SAMPLES);
    openFilesAvgPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_PROCS, total_time/TAKE_SAMPLES);
    free(results);
    MPI_Comm_free(&actual_comm);
    MPI_Finalize();
    return 0;
}
