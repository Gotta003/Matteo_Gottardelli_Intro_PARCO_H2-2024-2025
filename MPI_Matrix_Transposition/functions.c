//
//  functions.c
//  Matrix Transposition
//
//  Created by Matteo Gottardelli on 16/11/24.
//

#include "functions.h"

/*
 * Name: createData
 * Allocates memory for the counts and displacements arrays in the DataCommunicate struct
 * and sets the nprocs_x and nprocs_y fields.
 *
 * Input:
 *      data (DataCommunicate*) - Pointer to the `DataCommunicate` structure where data will be stored
 *      n_procs_x (int) - The number of processors in the x-direction
 *      n_procs_y (int) - The number of processors in the y-direction
 *
 * Output: none
 */
void createData(DataCommunicate* data, int n_procs_x, int n_procs_y) {
    data->counts=malloc(sizeof(int)*n_procs_x*n_procs_y);
    if(data->counts==NULL) {
        printf("Memory allocation failed\n");
        MPI_Finalize();
        exit(1);
    }
    data->displacements=malloc(sizeof(int)*n_procs_x*n_procs_y);
    if(data->displacements==NULL) {
        printf("Memory allocation failed\n");
        MPI_Finalize();
        exit(1);
    }
    data->nprocs_x=n_procs_x;
    data->nprocs_y=n_procs_y;
    if(n_procs_x==0 || n_procs_y==0) {
        printf("Be careful, if you initialize the inputs in createData equal to 0, you won't allocate space!!!");
    }
}
/*
 * Name: freeData
 * Frees the dynamically allocated memory for the counts and displacements arrays in the DataCommunicate structure.
 *
 * Input:
 *      data (DataCommunicate*) - Pointer to the DataCommunicate structure containing the arrays to be freed
 *
 * Output: none
 */
void freeData(DataCommunicate* data) {
    free(data->counts);
    free(data->displacements);
}
/*
 * Name: setupCommunicator
 * Initializes the properties of a 2D communicator and creates necessary MPI data types
 * for subarrays and resized subarrays based on the input parameters.
 *
 * Input:
 *      comm (Communicator2D*) - Pointer to the Communicator2D structure to store communicator data
 *      size (int[2]) - The global dimensions of the matrix (total size in each direction)
 *      subsizes (int[2]) - The dimensions of the local subarray (size of the subarray in each direction)
 *      start (int[2]) - The starting indices of the local subarray (position within the global matrix)
 *      resize (int) - The resize factor for the data type, used for memory allocation size adjustment
 *
 * Output: none
 */
void setupCommunicator(Communicator2D* comm, int size[2], int subsizes[2], int start[2], int resize) {
    comm->sizes[0]=size[0];
    comm->sizes[1]=size[1];
    comm->subsizes[0]=subsizes[0];
    comm->subsizes[1]=subsizes[1];
    comm->starts[0]=start[0];
    comm->starts[1]=start[1];
    MPI_Type_create_subarray(2, comm->sizes, comm->subsizes, comm->starts, MPI_ORDER_C, MPI_FLOAT, &comm->submatrix_type);
    MPI_Type_create_resized(comm->submatrix_type, 0, resize*sizeof(float), &comm->resized_type);
}
/*
 * Name: commitCommunicator
 * Commits the resized MPI data type for the 2D communicator.
 * This function must be called after the MPI data types have been created.
 *
 * Input:
 *      comm (Communicator2D*) - Pointer to the `Communicator2D` structure containing the resized MPI data type.
 *
 * Output: none
 */
void commitCommunicator(Communicator2D* comm) {
    MPI_Type_commit(&comm->resized_type);
}
/*
 * Name: freeCommunicator
 * Frees the MPI data types associated with the 2D communicator.
 * This function should be called to release any resources associated with the MPI data types after their use.
 *
 * Input:
 *      comm (Communicator2D*) - Pointer to the Communicator2D structure containing the resized MPI data type.
 *
 * Output: none
 */

void freeCommunicator(Communicator2D* comm) {
    MPI_Type_free(&comm->resized_type);
}
/*
 * Name: dataPopulate
 * Populates the counts and displacements arrays of the DataCommunicate structure
 * with values based on the input parameters.
 *
 * The counts array stores the number of elements assigned to each process,
 * while the displacements array stores the displacement offsets for the data distribution
 * across a 2D grid of processes.
 *
 * Input:
 *      comm (DataCommunicate*) - Pointer to the DataCommunicate structure that holds the
 *   counts and displacements arrays.
 *      count (int) - The value to be assigned to the counts array.
 *      delay (int) - A starting value for the displacement calculations.
 *      disp_row (int) - Increment for displacement in the row direction.
 *      disp_col (int) - Increment for displacement in the column direction.
 *
 * Output: none
 */
void dataPopulate(DataCommunicate* comm, int count, int delay, int disp_row, int disp_col) {
    int i, j;
    for (i=0; i<comm->nprocs_x; i++) {//1
        for (j=0; j<comm->nprocs_y; j++) {//N
            comm->counts[i*comm->nprocs_x+j] = count;
            comm->displacements[i*comm->nprocs_x+j]=delay+i*disp_col+j*disp_row;//0 2 8 10
        }
    }
    /*printf("Counts:\t");
    for(i=0; i<comm->nprocs_x*comm->nprocs_y; i++) {
        printf("%d\t", comm->counts[i]);
    }
    printf("\t\t\tDisp:\t");
    for (i=0; i<comm->nprocs_x*comm->nprocs_y; i++) {
        printf("%d\t", comm->displacements[i]);
    }
    printf("\n");*/
}
/*
 * Name: random_float2
 * Generates a random float number in a range with 2 decimals
 * Input:
 *      min (int) - Minimum value of the range
 *      max (int) - Maximum value of the range
 * Output: float - Randomize value with 2 decimals
 */

float random_float2 (int min, int max) {
    return (rand()%(max-min+1)+min)*1.0/100;
}

/*
 * Name: createFloatSquareMatrix
 * Create in the heap a square matrix
 * Input: n (int) - Side dimension of the matrix
 * Output: float** - Matrix address, it's a heap array long n, having
 * in each position an heap array of float long n
 */

float** createFloatMatrix(int x, int y) {
    float** temp=(float**)malloc(sizeof(float*)*x);
    if(temp==NULL) {
        printf("Memory allocation failed\n");
        MPI_Finalize();
        exit(1);
    }
    int i;
    for (i=0; i<x; i++) {
        temp[i]=(float*)malloc(sizeof(float)*y);
        if(temp[i]==NULL) {
            printf("Memory allocation failed\n");
            MPI_Finalize();
            free(temp);
            exit(1);
        }
    }
    return temp;
}
/*
 * Name: create2DFloatMatrix
 * Allocates memory for a 2D matrix (array of floats) with dimensions x by y, where
 * the matrix is stored in a single contiguous block of memory, but accessed as a
 * 2D array (matrix) for convenience.
 *
 * Input:
 *      m (float***) - Pointer to a pointer to a 2D float array (matrix) that will
 *                     be allocated.
 *      x (int)      - Number of rows of the matrix.
 *      y (int)      - Number of columns of the matrix.
 *
 * Output: none
 *
 * Notes:
 *      The memory for the matrix is allocated as a contiguous block of memory for
 *      efficiency, and each row is accessed as a pointer to its corresponding part
 *      of the block.
 */

void create2DFloatMatrix(float*** m, int x, int y) {
    int i;
    float* temp=malloc(x*y*sizeof(float));
    if(temp==NULL) {
        printf("Memory allocation failed\n");
        MPI_Finalize();
        exit(1);
    }
    (*m)=malloc(x*sizeof(float*));
    if((*m)==NULL) {
        printf("Memory allocation failed\n");
        free(temp);
        MPI_Finalize();
        exit(1);
    }
    for(i=0; i<x; i++) {
        (*m)[i]=&(temp[i*y]);
    }
}
/*
 * Name: free2DMemory
 * Frees the memory allocated for a 2D matrix that was previously allocated using
 * the `create2DFloatMatrix` function. It first frees the memory block where the
 * data for the matrix is stored and then frees the array of row pointers.
 *
 * Input:
 *      M (float***) - Pointer to the 2D matrix to be freed, which was allocated
 *                     using `create2DFloatMatrix`.
 *
 * Output: none
 */

void free2DMemory(float*** M) {
    free(&((*M)[0][0]));
    free(*M);
}

/*
 * Name: freeMemory
 * Free the memory allocated in heap, being careful of properly deallocating
 * array of pointers
 * Input:
 *      M (float**) - The allocated in heap matrix
 *      size (int) - Dimension of the side of the squared matrix
 * Output: none
 */

void freeMemory(float** M, int size) {
    if(M!=NULL) {
        int i;
        for (i=0; i<size; i++) {
            free(M[i]);
        }
        //free(M[0]);
        free(M);
    }
}

/*
 * Name: checkSym
 * Verifies if the square matrix in input is symmetric, so the matrix and its
 * transpose are equal (M[i][j]=M[j][i] - i, j counters of rows and columns
 * respectively)
 * Input:
 *      M (float**) - The allocated in heap matrix
 *      size (int) - Dimension of the side of the squared matrix
 * Output: bool (true - Matrix is symmetric; false - The matrix doesn't concide
 * with its transpose
 */
/*  j
  i - 0 1 2 .. N-2 N-1
    0
    1 x
    2 x x
   ..
  N-2 x x x ..
  N-1 x x x ..  x

 */
/* When one combination doesn't concide we can istantaneously
determine that the matrix is not symmetric */
bool checkSym (float** M, int size) {
    bool returnBool=true;
    int i, j;
    for (i=1; i<size && returnBool; i++) {
        for (j=0; j<i && returnBool; j++) {
            if(ABS_DIFF(M[i][j],M[j][i])>ERROR) {
                returnBool=false;
            }
        }
    }
    return returnBool;
}
//CAN'T DO THIS, THE ROOT RANK HAS TO BE KNOWN

/*bool checkSymMPIAllGather (float** MGEN, int N, int rank, int rows) {
    int int_sim=1;
    int i, j;
    for (i=rank*rows; i<(rank+1)*rows && (int_sim==1); i++) {
        for (j=0; j<i && (int_sim==1); j++) {
            if(MGEN[i*N+j]!=MGEN[j*N+i]) {
                int_sim=0;
                MPI_Bcast(&int_sim, 1, MPI_INT, rank, actual_comm);
            }
        }
    }
    return (int_sim==1);
}*/
/*
 * Name: checkSymMPI
 * Verifies if a distributed square matrix is symmetric using MPI across multiple processes.
 * Each process checks a portion of the matrix and the results are reduced using MPI_Allreduce
 * to determine if the entire matrix is symmetric.
 *
 * Input:
 *      MGEN (float**) - The 2D matrix to be checked for symmetry (allocated in memory).
 *      N (int) - The dimension of the matrix (number of rows and columns, N x N).
 *      rank (int) - The rank of the current process in the MPI communicator.
 *      rows (int) - The number of rows of the matrix assigned to the current process.
 *      scaling (int) - A flag to indicate whether the check should be performed (scaling=0) or not (scaling!=0).
 *
 * Output:
 *      bool - Returns `true` if the matrix is symmetric across all processes, otherwise returns `false`.
 */

bool checkSymMPI (float** MGEN, int N, int rank, int rows, int scaling) {
    if(scaling==0) {
        int num_procs=N/rows;
        int rows_per_proc=(N+num_procs-1)/num_procs;
        int start=rank*rows;
        int end=MIN(start+rows_per_proc,N);
        //BOOLEAN SLOW
        int symmetric=1;
        int i, j;
        for (i = start; i<end && (symmetric==1); i++) {
            for (j = 0; j<i && (symmetric==1); j++) {
                if (ABS_DIFF(MGEN[i][j], MGEN[j][i])>ERROR) {
                    symmetric=0;
                }
            }
        }
        //printf("%d\n", int_sim);
        int global;
        //MPI_Reduce(&int_sim, &global_sim, 1, MPI_INT, MPI_MIN, 0, actual_comm);
        //MPI_Bcast(&global_sim, 1, MPI_INT, 0, actual_comm);
        MPI_Allreduce(&symmetric, &global, 1, MPI_INT, MPI_MIN, actual_comm);
        return (global==1);
    }
    else {
        return false;
    }
}
/*
 * Name: matTranspose
 * Does a standard transposition. Even if the one with a temporal variable is more
 * efficient considering the cycle will be on half of that matrix, this is a more
 * general one, which allows us to use more elements to optimize this code.
 * Input:
 *      M (float**) - The allocated in heap matrix
 *      size (int) - Dimension of the side of the squared matrix
 * Output: float** - Resulting matrix, after transposition
 */
void matTranspose (float** M, float** T, int x, int y) {
    int i, j;
    for (i=0; i<x; i++) {
        for (j=0; j<y; j++) {
            T[j][i]=M[i][j];
        }
    }
}
/*
 * Name: matTransposeMPIAllGather
 * Performs a matrix transpose operation in a distributed setting using MPI, where the original matrix is divided
 * across multiple processes. After transposing the local portion of the matrix, it gathers the transposed data from
 * all processes and combines them into a global matrix.
 *
 * Input:
 *      MGEN (float**) - The original matrix (before transpose), distributed across processes.
 *      M (float**) - The local portion of the matrix assigned to the current process for transpose.
 *      T (float**) - The local transposed portion of the matrix.
 *      TGEN (float**) - The globally transposed matrix (after all processes perform transpose).
 *      rank (int) - The rank of the current MPI process.
 *      N (int) - The size of the matrix (N x N), a square matrix.
 *      rows (int) - The number of rows assigned to the current process.
 *      scaling (int) - A flag indicating whether the matrix transpose operation should be scaled.
 *      sending (DataCommunicate) - Contains information about the communication size and displacements for sending data.
 *      receiving (DataCommunicate) - Contains information about the communication size and displacements for receiving data.
 *      sender (Communicator2D) - The MPI communicator object containing details about the matrix distribution.
 *
 * Output: none
 */

void matTransposeMPIAllGather (float** MGEN, float** M, float** T, float** TGEN, int rank, int N, int rows, int scaling, DataCommunicate sending, DataCommunicate receiving, Communicator2D sender) {
    int i, j;
    MPI_Scatterv(globalsendptr, sending.counts, sending.displacements, sender.resized_type, localrecvptr, rows*N, MPI_FLOAT, 0, actual_comm);
    //matrixCheckPerRank(M, rank, rows, N);
    //printf("======RANK %d ======\n", rank);
    //printMatrix(M, rows, N);
    matTranspose(M, T, rows, N);
    //printMatrix(T, N, rows);
    if(scaling==1){
        for (i=0; i<N; i++) {
            dataPopulate(&receiving, rows, i*N*sending.nprocs_y, rows, 1);
            localsendptr=&(T[i][0]);
            MPI_Gatherv(localsendptr, rows, MPI_FLOAT, globalrecvptr, receiving.counts, receiving.displacements, MPI_FLOAT, 0, actual_comm);
        }
    }
    else {
        if(scaling==0) {
            for (i=0; i<N; i++) {
                dataPopulate(&receiving, rows, i*N, rows, 1);
                localsendptr=&(T[i][0]);
                MPI_Gatherv(localsendptr, rows, MPI_FLOAT, globalrecvptr, receiving.counts, receiving.displacements, MPI_FLOAT, 0, actual_comm);
            }
        }
    }
}
/*
 * Name: matTransposeMPIBlock
 * Performs a block-wise matrix transpose operation in a distributed setting using MPI, where the matrix is divided
 * into blocks distributed across multiple processes. Each process handles a block of the matrix and exchanges
 * data with other processes to ensure the correct transpose operation.
 *
 * Input:
 *      MGEN (float**) - The original matrix (before transpose), distributed across processes.
 *      M (float**) - The local portion of the matrix assigned to the current process for transpose.
 *      T (float**) - The local transposed portion of the matrix.
 *      TGEN (float**) - The globally transposed matrix (after all processes perform transpose).
 *      rank (int) - The rank of the current MPI process.
 *      N (int) - The size of the matrix (N x N), a square matrix.
 *      rows (int) - The number of rows assigned to the current process.
 *      sending (DataCommunicate) - Contains information about the communication size and displacements for sending data.
 *      receiving (DataCommunicate) - Contains information about the communication size and displacements for receiving data.
 *
 * Output: none
 */

void matTransposeMPIBlock (float** MGEN, float** M, float** T, float** TGEN, int rank, int N, int rows, DataCommunicate sending, DataCommunicate receiving) {
    int i, j;
    for (i=0; i<rows; i++) {
        dataPopulate(&sending, rows, i*N, rows, rows*N);
        localrecvptr=&(M[i][0]);
        //MPI_Barrier(actual_comm);
        MPI_Scatterv(globalsendptr, sending.counts, sending.displacements, MPI_FLOAT, localrecvptr, rows, MPI_FLOAT, 0, actual_comm);
    }
    //matrixCheckPerRank(M, rank, rows, N);
    //printMatrix(M, rows, rows);
    int col=rank%(N/rows);
    int row=rank/(N/rows);
    //printf("Rank %d (%d %d)", rank, row, col);
    //MPI_Barrier(actual_comm);
    //printf("Rank %d (%d %d)\n", rank, row, col);
    if(row!=col) {//5 [1][2] -> 7 [2][1]
        int rank_dest=col*(N/rows)+row;
        int tag;
        MPI_Status status;
        //No deadlock logic
        if(row<col) {
            tag=rank*sending.nprocs_x*sending.nprocs_y + rank_dest;
            for (i = 0; i < rows; i++) {
                for (j = 0; j < rows; j++) {
                    MPI_Send(&M[i][j], 1, MPI_FLOAT, rank_dest, tag, actual_comm);
                    //printf("Send value %.2f", M[i][j]);
                }
            }
            for (i = 0; i < rows; i++) {
                for (j = 0; j < rows; j++) {
                    MPI_Recv(&T[j][i], 1, MPI_FLOAT, rank_dest, tag, actual_comm, &status);
                    //printf("Received value %.2f", T[j][i]);
                }
            }
        }
        else {
            tag=rank_dest*sending.nprocs_x*sending.nprocs_y + rank;
            for (i = 0; i < rows; i++) {
                for (j = 0; j < rows; j++) {
                    MPI_Recv(&T[j][i], 1, MPI_FLOAT, rank_dest, tag, actual_comm, &status);
                    //printf("Received value %.2f", T[j][i]);
                }
            }
            for (i = 0; i < rows; i++) {
                for (j = 0; j < rows; j++) {
                    MPI_Send(&M[i][j], 1, MPI_FLOAT, rank_dest, tag, actual_comm);
                    //printf("Send value %.2f", M[i][j]);
                }
            }
        }
    }
    else {
        matTranspose(M, T, rows, rows);
    }
    //printf("======RANK %d ======\n", rank);
    //printMatrix(T, rows, rows);
    //MPI_Barrier(actual_comm);
    for (i=0; i<rows; i++) {
        if(rank==0){
            dataPopulate(&receiving, rows, i*N, rows, rows*N);
        }
        localsendptr=&(T[i][0]);
        MPI_Gatherv(localsendptr, rows, MPI_FLOAT, globalrecvptr, receiving.counts, receiving.displacements, MPI_FLOAT, 0, actual_comm);
    }
    //MPI_Barrier(actual_comm);
}
/*
 * Name: matTransposeMPIBlockOPT
 * Performs an optimized block-wise matrix transpose operation in a distributed setting using MPI. Each process
 * handles a local block of the matrix and exchanges the data with other processes to ensure the correct transpose
 * operation. This version improves upon the previous version by using `MPI_Sendrecv` for communication, which
 * optimizes the data exchange by combining send and receive operations into a single call.
 *
 * Input:
 *      MGEN (float**) - The original matrix (before transpose), distributed across processes.
 *      M (float**) - The local portion of the matrix assigned to the current process for transpose.
 *      T (float**) - The local transposed portion of the matrix.
 *      TGEN (float**) - The globally transposed matrix (after all processes perform transpose).
 *      tempM (float**) - Temporary matrix used for communication between processes.
 *      rank (int) - The rank of the current MPI process.
 *      N (int) - The size of the matrix (N x N), a square matrix.
 *      rows (int) - The number of rows assigned to the current process.
 *      sending (DataCommunicate) - Contains information about the communication size and displacements for sending data.
 *      receiving (DataCommunicate) - Contains information about the communication size and displacements for receiving data.
 *
 * Output: none
 */

void matTransposeMPIBlockOPT (float** MGEN, float** M, float** T, float** TGEN, float** tempM, int rank, int N, int rows, DataCommunicate sending, DataCommunicate receiving) {
    int i, j, target_rank;
    double time_s, time_e;
    //if(rank==0) {
    //    printMatrix(MGEN, N, N);
    //}
    for (i = 0; i < rows; i++) {
        dataPopulate(&sending, rows, i*N, rows, rows*N);
        localrecvptr = &(M[i][0]);
        MPI_Barrier(actual_comm);
        MPI_Scatterv(globalsendptr, sending.counts, sending.displacements, MPI_FLOAT, localrecvptr, rows, MPI_FLOAT, 0, actual_comm);
    }
    //printf("===== M - RANK %d ====\n", rank);
    //printMatrix(M, rows, rows);
    //MPI_Barrier(actual_comm);
    localrecvptr=&(M[0][0]);
    float* tempptr=&(tempM[0][0]);
    MPI_Sendrecv(localrecvptr, rows*rows, MPI_FLOAT, transposer.rank_dest, 0,
                 tempptr, rows*rows, MPI_FLOAT, transposer.rank_dest, 0,
                     actual_comm, MPI_STATUS_IGNORE);
    //printf("===== tempM - RANK %d ====\n", rank);
    //printMatrix(tempM, rows, rows);
    //MPI_Barrier(actual_comm);
    matTranspose(tempM, T, rows, rows);
    //printf("===== T - RANK %d ====\n", rank);
    //printMatrix(T, rows, rows);
    //MPI_Barrier(actual_comm);
    for (i = 0; i < rows; i++) {
        if(rank==0){
            dataPopulate(&receiving, rows, i*N, rows, rows*N);
        }
        localsendptr = &(T[i][0]);
        MPI_Gatherv(localsendptr, rows, MPI_FLOAT, globalrecvptr, receiving.counts, receiving.displacements, MPI_FLOAT, 0, actual_comm);
    }
    //if(rank==0) {
    //    printMatrix(TGEN, N, N);
    //}
}

/*
 * Name: PrintMatrix
 * Prints out on stdout all the Matrix indexed. This was used for control to verify that the program worked for small matrices. It's not present in the running code
 *   Input:
 *      M (float**) - The start allocated in heap matrix
 *      size(int) - Dimension of the matrix
 * Output: none
 */
void printMatrix(float** M, int x, int y) {
    int i, j;
    for (i=0; i<x; i++) {
        for(j=0; j<y; j++) {
            printf("%.2f\t", M[i][j]);
        }
        printf("\n");
    }
}
/*
 * Name: matrixCheckPerRank
 * Prints the matrix data that has been assigned to the current MPI rank for debugging purposes.
 * This function is useful to verify the data distribution across processes.
 *
 * Input:
 *      M (float**) - The matrix to be printed. This is the local portion of the matrix assigned to the current rank.
 *      rank (int) - The rank of the current MPI process.
 *      x (int) - The number of rows assigned to the current process.
 *      y (int) - The number of columns assigned to the current process.
 *
 * Output: none
 */

void matrixCheckPerRank(float** M, int rank, int x, int y) {
    int i, j;
    printf("Rank %d scatter\n\n", rank);
    for (i=0; i<x; i++) {
        for (j=0; j<y; j++) {
            printf("Rank: %d %d %d - %.2f\n", rank, i+1, j+1, M[i*x+j]);
        }
        printf("\n");
    }
}
/*
 * Name: control
 * Another control algorithm for verifying that the transposition happened correctly. Happened a transposition of
 *   Input:
 *      M (float**) - The start allocated in heap matrix
 *      T (float**) - The destination allocated in heap matrix
 *      size(int) - Dimension of the matrix
 * Output: none
 */
void control(float** M, float** T, int N) {
    printf("[%d][%d] M: %.2f T: %.2f\n", 1, 2, M[1][2], T[2][1]);
    printf("[%d][%d] M: %.2f T: %.2f\n", N/2-4, N/2+2, M[N/2-4][N/2+2], T[N/2+2][N/2-4]);
    printf("[%d][%d] M: %.2f T: %.2f\n", N-5, N-3, M[N-5][N-3], T[N-3][N-5]);
}
/*
 *  Name: clearCache
 *  Function that tries to free memory for the various chunk of memory in order to minimize the number of more efficient simulation. This isn't guarantee to always work, but it brought less outliers.
 *  Input:
 *      dimCache (long long int) - Dimension of the cache we would like to clear
 *  Output: none
 */
void clearAllCache(void) {
    float* cacheL1d, *cacheL1i, *cacheL2, *cacheL3;
    if (posix_memalign((void**)&cacheL1d, 64, CACHESIZEL1D) != 0 ||
            posix_memalign((void**)&cacheL1i, 64, CACHESIZEL1I) != 0 ||
            posix_memalign((void**)&cacheL2, 64, CACHESIZEL2) != 0 ||
            posix_memalign((void**)&cacheL3, 64, CACHESIZEL3) != 0) {
            fprintf(stderr, "Failed to allocate aligned memory\n");
            exit(1);
        }
    int i;
    for (i = 0; i < CACHESIZEL1D / sizeof(float); ++i) {
        cacheL1d[i] = 1.0f;
    }
    for (i = 0; i < CACHESIZEL1I / sizeof(float); ++i) {
        cacheL1i[i] = 1.0f;
    }
    for (i = 0; i < CACHESIZEL2 / sizeof(float); ++i) {
        cacheL2[i] = 1.0f;
    }
    for (i = 0; i < CACHESIZEL3 / sizeof(float); ++i) {
        cacheL3[i] = 1.0f;
    }
    free(cacheL1d);
    free(cacheL1i);
    free(cacheL2);
    free(cacheL3);
}
/*
 *  Name: getSequential
 *  Function that retrieves the sequential execution time for a given dimension and test from a file.
 *  Input:
 *      dim (int) - Dimension for which we are retrieving the sequential time
 *      test (int) - Test identifier to filter the relevant entry
 *      scaling (int) - Scaling mode defining 0. Strong 1. Weak
 *  Output:
 *      double - Returns the average time for the sequential execution. If not found in the file, it returns 0.00
 */
double getSequential(const int dim, const char* code, const int mode, const int test, const int scaling) {
    char line[256];
    int findex, fmode, fdimension, ftest, fsamples, fthreads, fscaling;
    char fcompile[20];
    double favg_time=0.0, fseq_time, fspeedup, fefficiency;
    FILE* file=fopen(FILENAMESEQ, "a+");
    if(file==NULL) {
        fprintf(stderr, "Couldn't open or create file %s\n", FILENAMESEQ);
        exit(1);
    }
    bool found=false;
    fgets(line, sizeof(line), file);
    while(!found && (fscanf(file, "%d %s %d %d %d %d %d %d %lf %lf %lf %lf%%", &findex, fcompile, &fmode, &fdimension, &ftest,
          &fsamples, &fthreads, &fscaling, &favg_time, &fseq_time,
          &fspeedup, &fefficiency)==12)) {
        if(fmode==mode && (strcmp(fcompile, code)==0) && fdimension==dim && ftest==test && fscaling==scaling) {
            found=true;
        }
    }
    fclose(file);
    if(found) {
        return favg_time;
    }
    else {
        return 0.00;
    }
}
/*
 *  Name: openFile
 *  Function that opens a file and appends performance data. Depending on the mode and type, it computes speedup and efficiency, then writes on the file the input parameters and calculated information. Writes performance data to the file. If the file is empty, it writes a header before appending data.
 *  Input:
 *      filename (const char*) - The name of the file to open.
 *      code (const char*) - The code or label to be recorded in the file
 *      mode (int) - The mode of the computation (SO0 for sequential - see readMe for others)
 *      dim (int) - Dimension Size
 *      test (int) - The test case identifier
 *      samples (int) - The number of samples
 *      num_procs (int) - The number of threads used
 *      scaling (int) - Scaling mode defining 0. Strong 1. Weak
 *      avg_time (double) - The average time taken for the computation
 *      type (int) - Determines the type of output file (0 - times, 1- average)
 *  Output: none
 */
void openFile(const char* filename, const char* code, const int mode, const int dim, const int test, const int samples, const int num_procs, const int scaling, double avg_time, int type) {
    FILE* file=fopen(filename, "a+"); //read and append
    if(file==NULL) {
        fprintf(stderr, "Couldn't open or create %s\n", filename);
        exit(1);
    }
    //int lines=countFile(file);
    FILE* wc=NULL;
    if(type==1) {
        wc=popen("wc -l < resultsGeneral.csv", "r");
    }
    else{
        if (type==0) {
            wc=popen("wc -l < timesGeneral.csv", "r");
        }
    }
    if (wc==NULL) {
        fprintf(stderr, "Error running wc -l\n");
        fclose(file);
        exit(1);
    }
    int lines;
    fscanf(wc, "%d", &lines);
    fclose(wc);
    if(lines==0){
        lines=1;
    }
    double seq_time = 0.0;
    double speedup=0.0;
    double efficiency=0.0;
    if(num_procs!=1 && type==1) {
        fclose(file);
        seq_time=getSequential(dim, code, mode, test, scaling);
        file=fopen(filename, "a+"); //read and append
        if(file==NULL) {
            fprintf(stderr, "Couldn't open or create %s\n", filename);
            exit(1);
        }
        speedup=seq_time/avg_time;
        if(scaling==0) {
            efficiency=speedup/num_procs*100;
        }
        else {
            efficiency=speedup*100;
        }
    }
    else {
        seq_time=avg_time;
        speedup=seq_time/avg_time;
        if(scaling==0) {
            efficiency=speedup/num_procs*100;
        }
        else {
            efficiency=speedup*100;
        }
    }
    fseek(file, 0, SEEK_END);
    long long file_size=ftell(file);
    if(file_size==0 && type==1) {
        fprintf(file, "%-10s %-15s %-5s %-10s %-10s %-10s %-10s %-10s %-15s %-15s %-10s %-10s\n", "N°", "Compile", "Mode", "Dimension", "Test_Mode", "Samples", "N Procs", "Scaling", "Avg_Time(s)", "Seq_time(s)", "Speedup", "Efficiency");
    }
    else {
        if(file_size==0 && type==0) {
            fprintf(file, "%-10s %-15s %-5s %-10s %-10s %-10s %-10s %-10s %-15s\n", "N°", "Compile", "Mode", "Dimension", "Test_Mode", "Samples", "N Procs", "Scaling", "Time(s)");
        }
    }
    char void_element='-';
    if(type==0) {
        fprintf(file, "%-10d %-15s %-5d %-10d %-10d %-10d %-10d %-10d %-15.12lf\n", lines, code, mode, dim, test, samples, scaling, num_procs, avg_time);
    }
    else {
        if(seq_time<1e-9) {
            fprintf(file, "%-10d %-15s %-5d %-10d %-10d %-10d %-10d %-10d %-15.12f %-15c %-10c %-10c%%\n", lines, code, mode, dim, test, samples, num_procs, scaling, avg_time, void_element, void_element, void_element);
        }
        else {
            fprintf(file, "%-10d %-15s %-5d %-10d %-10d %-10d %-10d %-10d %-15.12lf %-15.12lf %-10.2lf %-10.2lf%%\n", lines, code, mode, dim, test, samples, num_procs, scaling, avg_time, seq_time, speedup, efficiency);
        }
    }
    fclose(file);
}
/*
 *  Name: inputParameters
 *  Function that checks the number of arguments passed to the program (7 ok, less exits or more).
 *  Input:
 *      argc (int) - The number of arguments passed to the program.
 *  Output: none
 */
void inputParameters(int argc) {
    printf("%d\n\n\n", argc);
    if(argc!=7) {
        fprintf(stderr, "Invalid parameters. They are:\n\n(1) Code identifying the compilation (everything is acceptable, default one is SO0, is used as reference for sequential code -O0 and will be used as comparison for speedup and efficiency)\n\n(2)Integer for mode. The modes are:\n1. Sequential Code\n2. MPI Parallelism All Gather\n3. MPI Parallelism using Row Major\n\n(3) Exponential of 2 from 4 to 12 (16->4, 64->6, 1024->10, 4096->12)\n\n(4) Testing:\n0. Matrix is generated randomly\n1. A default test matrix is generated according to an algorithm (used for testing, with one interaction in checkSym and Transposition)\n2. A default test symmetric matrix (only checkSym works)\n3. The worst case scenario with a matrix, with only one element different not on main diagonal\n\n(5) Number of samples (>=25)\n\n(6) Scaling Type:\n0. Strong Scaling (Size inputed partitioned of job among processes)\n1. Weak Scaling (Size inputed assigned to every process)\n\n");
        exit(1);
    }
}
/*
 *  Name: valueInputed
 *  Function that processes and validates the input values based on the argument provided. It checks if the input values are within valid ranges for different parameters (mode, dimension, test, samples, and number of threads). If any value is invalid, it prints an error message and exits the program.
 *  Input:
 *      argc (int) - The number of command-line arguments passed to the program
 *      argv (const char*) - A string representing the argument value to be process
 *      value (int) - The specific value identifying which input parameter is being validated (2. mode, 3. dimension, 4. test, 5. samples, 6. scaling)
 *  Output:
 *      returnValue (int) - The validated integer value for the corresponding parameter
 */
int valueInputed(int argc, const char* argv, int value) {
    int returnValue=-1;
    if(value>=2 && value<=6) {
        switch (value) {
            case 2: {
               returnValue=atoi(argv);
                if (returnValue<=START || returnValue>=END) {
                    fprintf(stderr, "1. Sequential Code\n2. MPI Parallelism All Gather\n3. MPI Parallelism using Row Major\n\n");
                    exit(1);
                }
            }
            break;
            case 3: {
                returnValue=pow(2, atoi(argv));
                if (returnValue<MIN_SIZE || returnValue>MAX_SIZE) {
                    fprintf(stderr, "Invalid dimension, it has to be a power of 2 between 4 and 12. Insert the exponential\n\n");
                    exit(1);
                }
            }
            break;
            case 4: {
                returnValue=atoi(argv);
                if(returnValue>2 || returnValue<0) {
                    fprintf(stderr, "Invalid testing, it has to be:\n\n0. (Random)\n1. (Standard Test No Symmetric)\n2. (Standard Test Symmetric)\n\n");
                    exit(1);
                }
            }
            break;
            case 5: {
                returnValue=atoi(argv);
                if(returnValue<MIN_SAMPLES) {
                    fprintf(stderr, "Invalid samples number, it has to be at least of 25\n\n");
                    exit(1);
                }
            }
            break;
            case 6:
                returnValue=atoi(argv);
                if(returnValue!=0 && returnValue!=1) {
                    fprintf(stderr, "Scaling Type:\n0. Strong Scaling (Size inputed partitioned of job among processes)\n1. Weak Scaling (Size inputed assigned to every process)\n\n");
                    exit(1);
                }
            break;
            default:
                exit(1);
                break;
        }
    }
    return returnValue;
}
/*
 *  Name: initializeMatrix
 *  Function that initializes a matrix based on test. Elements are populated with either random values, static values, or predefined values for symmetric or worst-case test scenarios.
 *  Input:
 *      M (float**) - The matrix to be initialized. This matrix will be filled with values based on the selected test type
 *      test (Test) - The test type that determines how the matrix will be initialized. It can be RANDOM, STATIC, SYM, or WORST (0, 1, 2, 3)
 *      n (int) - Size of Matrix
 *  Output: none
 */
void initializeMatrix(float** M, Test test, int x, int y) {
    if(test==STATIC) {
        srand(38);
    }
    else {
        if (test==RANDOM) {
            srand((unsigned)time(NULL));
        }
    }
    int i, j;
    for (i=0; i<x; i++) {
        for (j=0; j<y; j++) {
            switch (test) {
                case RANDOM: M[i][j]=random_float2(0, 9999); break;
                case STATIC:
                    M[i][j]=random_float2(0, 9999); break;
                    //M[i][j]=i*1.0/200000*n+j*1.0/100+1; break;
                case SYM: M[i][j]=5.0; break;
                default: fprintf(stderr, "Unexpected Error\n"); exit(1);
            }
        }
    }
}
/*
 * Name: executionProgram
 * Executes the program based on the given mode, and performs matrix transposition using different MPI strategies.
 * This function checks if the matrix is symmetric and, if not, performs the transposition using different strategies.
 *
 * Input:
 *      MGEN (float**) - The generated matrix, typically the global matrix to check symmetry against.
 *      M (float**) - The local matrix for the current MPI rank.
 *      T (float**) - The matrix where the transposed result is stored.
 *      TGEN (float**) - The matrix to store the generated transposed result.
 *      tempM (float**) - A temporary matrix used for optimized block transposition.
 *      mode (Mode) - The execution mode (SEQ, MPI_ALL, MPI_BLOCK, or MPI_BLOCK_OPT).
 *      N (int) - The size of the matrix (N x N).
 *      rows (int) - The number of rows assigned to the current rank.
 *      rank (int) - The MPI rank of the current process.
 *      scaling (int) - Scaling mode defining 0. Strong 1. Weak.
 *      sending (DataCommunicate) - Structure containing the sending data for MPI operations.
 *      receiving (DataCommunicate) - Structure containing the receiving data for MPI operations.
 *      sender_mpi_all (Communicator2D) - The MPI communicator for the 2D process grid.
 *
 * Output:
 *      bool - True if the matrix is symmetric and no transposition was needed, false if the matrix was not symmetric
 *   and the transposition was performed.
 */

bool executionProgram(float** MGEN, float** M, float** T, float** TGEN, float** tempM, Mode mode, int N, int rows, int rank, int scaling, DataCommunicate sending, DataCommunicate receiving, Communicator2D sender_mpi_all) {
    switch (mode) {
        case SEQ:
        case MPI_ALL: {
            if(!checkSymMPI(MGEN, N, rank, rows, scaling)) {
                matTransposeMPIAllGather(MGEN, M, T, TGEN, rank, N, rows, scaling, sending, receiving, sender_mpi_all);
                return false;
            }
        }
        break;
        case MPI_BLOCK: {
            if(!checkSymMPI(MGEN, N, rank, N/(sending.nprocs_x*sending.nprocs_y), scaling)) {
                matTransposeMPIBlock(MGEN, M, T, TGEN, rank, N, rows, sending, receiving);
                return false;
            }
        }
        case MPI_BLOCK_OPT: {
            if(!checkSymMPI(MGEN, N, rank, N/(sending.nprocs_x*sending.nprocs_y), scaling)) {
                matTransposeMPIBlockOPT(MGEN, M, T, TGEN, tempM, rank, N, rows, sending, receiving);
                return false;
            }
        }
        break;
        default:
            MPI_Finalize();
            exit(1);
            break;
    }
    //freeMemory(T, N);
    //freeMemory(TLM, rows);
    TGEN=MGEN;
    return true;
}
/*
 *  Name: openFilesAvgPerMode
 *  Function that opens specific files based on the mode and writes the average time for a particular mode of matrix operation in a particular file
 *  Input:
 *      code (const char*) - A string representing the code identifier
 *      mode (const int) - The execution mode that determines which file will be opened and written to
 *      n (const int) - The dimension of the matrix (number of rows/columns)
 *      test (const int) - Test for data in matrix
 *      samples (const int) - The number of samples.
 *      num_procs (const int) - The number of threads
 *      scaling (int) - Scaling mode defining 0. Strong 1. Weak
 *      avg_time (const double) - The average time taken
 */
void openFilesAvgPerMode(const char* code, const int mode, int n, const int test, const int samples, const int num_procs, const int scaling, const double avg_time) {
    /*if(scaling==1) {
        n=n/num_procs;
    }*/
    openFile(FILENAMEGEN, code, mode, n, test, samples, num_procs, scaling, avg_time, 1);
    switch (mode) {
        case SEQ:
            openFile(FILENAMESEQ, code, mode, n, test, samples, num_procs, scaling, avg_time, 1);
            break;
        case MPI_ALL:
            if (num_procs!=1) {
                openFile(FILENAMEMPIALL, code, mode, n, test, samples, num_procs, scaling,  avg_time, 1);
            }
            else {
                openFile(FILENAMESEQ, code, mode, n, test, samples, num_procs,  scaling, avg_time, 1);
            }
            break;
        case MPI_BLOCK:
            if (num_procs!=1) {
                openFile(FILENAMEMPIBLOCK, code, mode, n, test, samples, num_procs, scaling, avg_time, 1);
            }
            else {
                openFile(FILENAMESEQ, code, mode, n, test, samples, num_procs,scaling,  avg_time, 1);
            }
            break;
        case MPI_BLOCK_OPT:
            if (num_procs!=1) {
                openFile(FILENAMEMPIBLOCKOPT, code, mode, n, test, samples, num_procs, scaling, avg_time, 1);
            }
            else {
                openFile(FILENAMESEQ, code, mode, n, test, samples, num_procs, scaling, avg_time, 1);
            }
            break;
        default:
            exit(1);
            break;
    }
}
/*
 *  Name: openFilesResultsPerMode
 *  Function that opens specific result files based on the mode and writes the execution time for a particular mode of matrix operation.
 *  Input:
 *      code (const char*) - A string representing the code identifier
 *      mode (const int) - The execution mode that determines which file will be opened and written to
 *      n (const int) - The dimension of the matrix (number of rows/columns)
 *      test (const int) - The test type used for matrix generation.
 *      samples (const int) - The number of samples for benchmarking.
 *      num_procs (const int) - The number of threads used in parallelism
 *      scaling (int) - Scaling mode defining 0. Strong 1. Weak
 *      time (const double) - The execution time taken for the operation
 */
void openFilesResultsPerMode(const char* code, const int mode, int n, const int test, const int samples, const int num_procs, const int scaling, const double time) {
    if(scaling==1) {
        n=n/num_procs;
    }
    openFile(FILENAMETGEN, code, mode, n, test, samples, num_procs, scaling, time, 0);
    switch (mode) {
        case SEQ:
            openFile(FILENAMETSEQ, code, mode, n, test, samples, num_procs, scaling, time, 0);
            break;
        case MPI_ALL:
            if (num_procs!=1) {
                openFile(FILENAMETMPIALL, code, mode, n, test, samples, num_procs, scaling, time, 0);
            }
            else {
                openFile(FILENAMETSEQ, code, mode, n, test, samples, num_procs, scaling, time, 0);
            }
            break;
        case MPI_BLOCK:
            if (num_procs!=1) {
                openFile(FILENAMETMPIBLOCK, code, mode, n, test, samples, num_procs, scaling, time, 0);
            }
            else {
                openFile(FILENAMETSEQ, code, mode, n, test, samples, num_procs, scaling, time, 0);
            }
            break;
        case MPI_BLOCK_OPT:
            if (num_procs!=1) {
                openFile(FILENAMETMPIBLOCKOPT, code, mode, n, test, samples, num_procs, scaling, time, 0);
            }
            else {
                openFile(FILENAMETSEQ, code, mode, n, test, samples, num_procs, scaling, time, 0);
            }
            break;
        default:
            exit(1);
            break;
    }
}
/*
 * Name: bubbleSort
 * Description: Sorts an array of doubles in ascending order using the bubble sort algorithm.
 * Input:
 *      a (double*) - A pointer to the array of doubles to be sorted.
 *      size (int) - The number of elements in the array.
 * Output: none
 */
void bubbleSort(double* a, int size);

void bubbleSort(double* a, int size) {
    int i, j;
    for (i=0; i<(size-1); i++) {
        for (j=0; j<(size-1-i); j++) {
            if(a[j]>a[j+1]) {
                double temp=a[j+1];
                a[j+1]=a[j];
                a[j]=temp;
            }
        }
    }
}
