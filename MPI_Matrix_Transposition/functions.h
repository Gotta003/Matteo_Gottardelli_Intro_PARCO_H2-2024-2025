//
//  functions.h
//  Matrix Transposition
//
//  Created by Matteo Gottardelli on 16/11/24.
//

#ifndef functions_h
#define functions_h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/time.h>
#include <mpi.h>
#include <time.h>
#include <string.h>
#include <math.h> //-lm
#define KB 1024
#define CACHESIZEL1D (32*KB)
#define CACHESIZEL1I (32*KB)
#define CACHESIZEL2 (1024*KB)
#define CACHESIZEL3 (36*KB*KB)
#define FILENAMETGEN "timesGeneral.csv"
#define FILENAMEGEN "resultsGeneral.csv"
#define FILENAMETSEQ "timesSequential.csv"
#define FILENAMESEQ "resultsSequential.csv"
#define FILENAMETMPIALL "timesMPIAllGather.csv"
#define FILENAMEMPIALL "resultsMPIAllGather.csv"
#define FILENAMETMPIBLOCK "timesMPIBlock.csv"
#define FILENAMEMPIBLOCK "resultsMPIBlock.csv"
#define FILENAMETMPIBLOCKOPT "timesMPIBLOCKOpt.csv"
#define FILENAMEMPIBLOCKOPT "resultsMPIBLOCKOpt.csv"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
//Using the floating number in order to be sure preciser equality I'm going to use the absolute value of the difference of the two higher than a low error
#define ERROR 1e-6
#define ABS_DIFF(X, Y) (((X) > (Y)) ? (X)-(Y) : (Y)-(X))
//A problem because we don't know how many threads there are in a node with qsb, so this maximum is tared on the maximum in UNITN system in a single node
#define MAX_THREADS 96
#define MIN_SIZE pow(2, 4)
#define MAX_SIZE pow(2, 12)
#define MIN_SAMPLES 25
//Struct for Setup a Custom 2D Type
typedef struct Communicator2D {
    MPI_Datatype submatrix_type;
    MPI_Datatype resized_type;
    int sizes[2];
    int subsizes[2];
    int starts[2];
} Communicator2D;
//Struct for Communication using 2D matrices
typedef struct DataCommunicate {
    int* counts;
    int* displacements;
    int nprocs_x;
    int nprocs_y;
}DataCommunicate;
//Struct for Communication with Blocks
typedef struct Transposer {
    int rank_start;
    int rank_dest;
    int coords_start[2];
    int coords_dest[2];
} Transposer;
//Global variables accessible from main and functions.c
extern float* globalsendptr;
extern float* localrecvptr;
extern float* globalrecvptr;
extern float* localsendptr;
extern MPI_Comm actual_comm;
extern Transposer transposer;
//Enum to classify the execution_modes and test_modes
typedef enum {START, SEQ, MPI_ALL, MPI_BLOCK, MPI_BLOCK_OPT, END} Mode;
//OMP_LOC_TB, OMP_GLB_TB, END} Mode;
typedef enum {RANDOM, STATIC, SYM, WORST} Test;
//Functions
void createData(DataCommunicate* data, int n_procs_x, int n_procs_y);
void freeData(DataCommunicate* data);
void setupCommunicator(Communicator2D* comm, int size[2], int subsizes[2], int start[2], int resize);
void commitCommunicator(Communicator2D* comm);
void freeCommunicator(Communicator2D* comm);
void dataPopulate(DataCommunicate* comm, int count, int delay, int disp_row, int disp_col);
//Number generation
float random_float2 (int min, int max);
//Input Management
void inputParameters(int argc);
int valueInputed(int argc, const char* argv, int value);
//Space Management - Allocation and Deallocation
float** createFloatMatrix(int x, int y);
void create2DFloatMatrix(float*** m, int x, int y);
void initializeMatrix(float** M, Test test, int x, int y);
void freeMemory(float** M, int size);
void free2DMemory(float*** M);
//Execution
bool executionProgram(float** MGEN, float** M, float** T, float** TGEN, float** tempM, Mode mode, int N, int rows, int rank, int scaling, DataCommunicate sending, DataCommunicate receiving, Communicator2D sender_mpi_all);
//Check Symmetry Algorithms
bool checkSym (float** M, int size);
//bool checkSymMPIAllGather (float** M, int N, int rank, int rows);
bool checkSymMPI (float** M, int N, int rank, int rows, int scaling);
//Transposition Algorithms
void matTranspose (float** M, float** T, int x, int y);
void matTransposeMPIAllGather (float** MGEN, float** M, float** T, float** TGEN, int rank, int N, int rows, int scaling, DataCommunicate sending, DataCommunicate receiving, Communicator2D sender);
void matTransposeMPIBlock (float** MGEN, float** M, float** T, float** TGEN, int rank, int N, int rows, DataCommunicate sending, DataCommunicate receiving);
void matTransposeMPIBlockOPT (float** MGEN, float** M, float** T, float** TGEN, float** tempM, int rank, int N, int rows, DataCommunicate sending, DataCommunicate receiving);
//Control Results
void printMatrix(float** M, int x, int y);
void control(float** M, float** T, int N);
void matrixCheckPerRank(float** M, int rank, int x, int y);
void bubbleSort(double* a, int size);
//Cache Management
void clearCache(long long int dimCache);
void clearAllCache(void);
//Files csv Management
double getSequential(const int dim, const char* code, const int mode, const int test, const int scaling);
void openFile(const char* filename, const char* code, const int mode, const int dim, const int test, const int samples, const int num_threads, const int scaling, double avg_time, int type);
void openFilesAvgPerMode(const char* code, const int mode, int n, const int test, const int samples, const int num_threads, const int scaling, const double avg_time);
void openFilesResultsPerMode(const char* code, const int mode, int n, const int test, const int samples, const int num_threads, const int scaling, const double time);
#endif /* functions_h */
