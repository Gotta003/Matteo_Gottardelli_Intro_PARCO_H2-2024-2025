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

int main(int argc, char * argv[]) {
    //Initialization
    int i, j;
    int count=0;
    double time=0.0;
    float** M=NULL;
    float** MGEN=NULL;
    float** T=NULL;
    float** TGEN=NULL;
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
    int rank, NUM_THREADS;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_THREADS);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n_x=1, n_y=1;
    int rows=1;
    if (NUM_THREADS > N) {
        printf("Number of threads %d exceeds N=%d\n", NUM_THREADS, rank);
        MPI_Finalize();
        return 1;
    }
    else {
        if(MODE==MPI_BLOCK) {
            n_x=(int)sqrt(NUM_THREADS);
            n_y=(int)sqrt(NUM_THREADS);
            rows=N/n_y;
            if(N%(n_x*n_y)!=0) {
                printf("Number of threads %d is not the square root of N=%d\n", NUM_THREADS, rank);
                MPI_Finalize();
                return 0;
            }
        }
        else {
            if (N % NUM_THREADS == 0) {
                rows=N/NUM_THREADS;
            } else {
                rows=N/NUM_THREADS+(rank<N%NUM_THREADS ? 1 : 0);
            }
            n_x=1;
            n_y=NUM_THREADS;
            printf("Hello rank %d out of %d\n", rank+1, NUM_THREADS);
        }
    }
    NUM_THREADS=MIN(NUM_THREADS, N);
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
        createData(&sending, 1, NUM_THREADS);
        createData(&receiving, 1, n_y);
    }
    else {
        if(MODE==MPI_BLOCK) {
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
        MPI_Bcast(globalsendptr, 1, gen_matrix.resized_type, 0, MPI_COMM_WORLD);
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
            if(MODE==MPI_BLOCK) {
                create2DFloatMatrix(&M, rows, rows);
                create2DFloatMatrix(&T, rows, rows);
                //commitCommunicator(&sender_mpi_block);
            }
        }
        localrecvptr=&(M[0][0]);
        //Starting
        if(rank==0) {
            tw_start=MPI_Wtime();
        }
        bool symmetry=executionProgram(MGEN, M, T, TGEN, MODE, N, rows, rank, sending, receiving, sender_mpi_all);
        //Ending
        //MPI_Barrier(MPI_COMM_WORLD);
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
            openFilesResultsPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_THREADS, time);
            results[count]=time;
        }
        free2DMemory(&MGEN);
        free2DMemory(&M);
        free2DMemory(&T);
        if(MODE==MPI_ALL || MODE==SEQ) {
            freeCommunicator(&sender_mpi_all);
        }
        count++;
        MPI_Barrier(MPI_COMM_WORLD);
    }
    freeData(&sending);
    freeData(&receiving);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank!=0) {
        MPI_Finalize();
        return 0;
    }
    //Take the 10 values in the middle
    const int TAKE_SAMPLES=SAMPLES/5*2;//40%
    int start=SAMPLES/2-TAKE_SAMPLES/2;
    int end=SAMPLES/2+TAKE_SAMPLES/2;
    double total_time=0.0;
    bubbleSort(results, SAMPLES);
    for (i=start; i<end; i++) {
        total_time+=results[i];
        printf("%.12lf\n", results[i]);
    }
    printf("\n\nFINAL RESULTS WITH:\nMODE %d\nDimension: %d\nTesting: %d\nSamples: %d\nThreads: %d\nAverage Time: %.12lf secs\n\n", MODE, N, TESTING, SAMPLES, NUM_THREADS, total_time/TAKE_SAMPLES);
    openFilesAvgPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_THREADS, total_time/TAKE_SAMPLES);
    free(results);
    MPI_Finalize();
    return 0;
}
//Trying performing Block Transposition
/*
 MPI_Status status;
 MPI_Comm cart_comm;
 int num_x_threads=(int)sqrt(NUM_THREADS);
 int num_y_threads=(int)sqrt(NUM_THREADS);
int dim_size[2]={num_x_threads, num_y_threads};
int times[2]={1, 1};
MPI_Comm cartesian_c;
MPI_Cart_create(MPI_COMM_WORLD, 2, dim_size, times, 1, &cartesian_c);
if(rank>=num_x_threads*num_y_threads) {
    printf("Rank %d out of %d processes not used\n", rank, NUM_THREADS);
    MPI_Finalize();
}
else {
    printf("Hello from rank %d out of %d processes\n", rank, NUM_THREADS);
}
int coordinates[2];
int opposite_coordinates[2];
MPI_Cart_coords(cartesian_c, rank, 2, coordinates);
int rows_per_thread=ceil(N/(float)num_x_threads);
int cols_per_thread=ceil(N/(float)num_x_threads);
int normal_rows=rows_per_thread;
int normal_cols=cols_per_thread;
if (coordinates[0] == (num_x_threads - 1)) {
    rows_per_thread=N-(cols_per_thread*(num_x_threads- 1));
}
if (coordinates[1] == (num_y_threads - 1))
{
    cols_per_thread=N-(rows_per_thread*(num_y_threads-1));
}
printf("Rank: %d, X: %d, Y: %d, RpC: %d, CpC: %d\n", rank, coordinates[0], coordinates[1], rows_per_thread, cols_per_thread);
opposite_coordinates[0]=coordinates[1];
opposite_coordinates[1]=coordinates[0];
int rank_dest;
MPI_Cart_rank(cartesian_c, opposite_coordinates, &rank_dest);
int target=(coordinates[0]+1)*(coordinates[1]+1);
int sizes[2]={rows_per_thread, cols_per_thread};
int subsizes[2]={rows_per_thread, cols_per_thread};
int starts[2]={0, 0};
MPI_Request send_request;
MPI_Request recv_request;
MPI_Datatype send_type;
MPI_Datatype recv_type;
MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_FORTRAN, MPI_DOUBLE, &send_type);
MPI_Type_commit(&send_type);
MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_C, MPI_DOUBLE, &recv_type);
MPI_Type_commit(&recv_type);*/
/*//printMatrix(M, N);
 if(rank==0) {
     MGEN=createFloatMatrix(N, N);
     initializeMatrix(MGEN, TESTING, N);
     //Sendv
     printMatrix(MGEN, N);
     for (int p = 0; p < NUM_THREADS; ++p) {
             int coords[2];
             MPI_Cart_coords(cartesian_c, p, 2, coords);

             int start_row = coords[0] * normal_rows;
             int start_col = coords[1] * normal_cols;
             int current_rows = (coords[0] == (num_x_threads - 1)) ? rows_per_thread : normal_rows;
             int current_cols = (coords[1] == (num_y_threads - 1)) ? cols_per_thread : normal_cols;

             // Create subarray type for the specific sub-block
             int sizes_gen[2] = {N, N};               // Global matrix dimensions
             int subsizes_gen[2] = {current_rows, current_cols}; // Sub-block dimensions
             int starts_gen[2] = {start_row, start_col};         // Starting position of the sub-block
             MPI_Datatype subarray_type;
             MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_C, MPI_FLOAT, &subarray_type);
             MPI_Type_commit(&subarray_type);

             if (p == 0) {
                 // Copy sub-block for rank 0
                 for (int i = 0; i < current_rows; ++i) {
                     for (int j = 0; j < current_cols; ++j) {
                         M[i][j] = MGEN[start_row + i][start_col + j];
                     }
                 }
                 matrixCheckPerRank(M, rank, rows_per_thread, cols_per_thread);
             } else {
                 // Send sub-block to process p
                 MPI_Send(&(MGEN[0][0]), 1, subarray_type, p, 0, MPI_COMM_WORLD);
             }

             MPI_Type_free(&subarray_type); // Free the subarray type
         }
 }
 else {
     MPI_Recv(&(M[0][0]), rows_per_thread * cols_per_thread, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
     printf("Current Rank %d\n\n", rank);
     matrixCheckPerRank(M, rank, rows_per_thread, cols_per_thread);
     //printMatrix(M, rows_per_thread);
 }
 //Modalities
 MPI_Send(&M[0][0], 1, send_type, rank_dest, target, cartesian_c);
 MPI_Recv(&T[0][0], 1, recv_type, rank_dest, target, cartesian_c, &status);
 printMatrix(T, N);
 */
