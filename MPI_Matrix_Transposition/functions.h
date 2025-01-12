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
#define FILENAMETMPIROWM "timesMPIRowMajor.csv"
#define FILENAMEMPIROWM "resultsMPIRowMajor.csv"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
//Using the floating number in order to be sure preciser equality I'm going to use the absolute value of the difference of the two higher than a low error
#define ERROR 1e-6
#define ABS_DIFF(X, Y) (((X) > (Y)) ? (X)-(Y) : (Y)-(X))
//A problem because we don't know how many threads there are in a node with qsb, so this maximum is tared on the maximum in UNITN system in a single node
#define MAX_THREADS 96
#define MIN_SIZE pow(2, 4)
#define MAX_SIZE pow(2, 12)
#define MIN_SAMPLES 1
typedef struct Communicator2D {
    MPI_Datatype submatrix_type;
    MPI_Datatype resized_type;
    int sizes[2];
    int subsizes[2];
    int starts[2];
} Communicator2D;
typedef struct DataCommunicate {
    int* counts;
    int* displacements;
    int size;
    int nprocs;
}DataCommunicate;
extern float* globalsendptr;
extern float* localrecvptr;
extern float* globalrecvptr;
extern float* localsendptr;
typedef enum {START, SEQ, MPI_ALL, MPI_ROW_M, END} Mode;
//OMP_LOC_TB, OMP_GLB_TB, END} Mode;
typedef enum {RANDOM, STATIC, SYM, WORST} Test;

void createData(DataCommunicate* data, int size, int n_procs);
void freeData(DataCommunicate* data);
void setupCommunicator(Communicator2D* comm, int size[2], int subsizes[2], int start[2]);
void dataPopulate(DataCommunicate* comm, int count, int disp_row, int disp_col);
//Number generation
float random_float2 (int min, int max);
//Input Management
void inputParameters(int argc);
int valueInputed(int argc, const char* argv, int value);
//Generation and deleting
float** createFloatMatrix(int x, int y);
void create2DFloatMatrix(float*** m, int x, int y);
void initializeMatrix(float** M, Test test, int n);
void freeMemory(float** M, int size);
void free2DMemory(float*** M);
//Execution
bool executionProgram(float** MGEN, float** M, float** T, float** TGEN, Mode mode, int N, int rows, int rank, DataCommunicate sending, DataCommunicate receiving, Communicator2D sender);
//Check Symmetry Algorithms
bool checkSym (float** M, int size);
//bool checkSymMPIAllGather (float** M, int N, int rank, int rows);
bool checkSymMPI (float** M, int N, int rank, int rows);
//Transposition Algorithms
void matTranspose (float** M, float** T, int size);
void matTransposeMPIAllGather (float** MGEN, float** M, float** T, float** TGEN, int rank, int N, int rows, DataCommunicate sending, DataCommunicate receiving, Communicator2D sender);
void matTransposeMPIRowMajor (float** LM, float** TLM, float** T, int rank, int N, int rows);
//Control Results
void printMatrix(float** M, int x, int y);
void control(float** M, float** T, int N);
void matrixCheckPerRank(float** M, int rank, int x, int y);
void bubbleSort(double* a, int size);
//Cache Management
void clearCache(long long int dimCache);
void clearAllCache(void);
//Files csv Management
double getSequential(const int dim, const int test);
void openFile(const char* filename, const char* code, const int mode, const int dim, const int test, const int samples, const int num_threads, double avg_time, int type);
void openFilesAvgPerMode(const char* code, const int mode, const int n, const int test, const int samples, const int num_threads, const double avg_time);
void openFilesResultsPerMode(const char* code, const int mode, const int n, const int test, const int samples, const int num_threads, const double time);
#endif /* functions_h */
