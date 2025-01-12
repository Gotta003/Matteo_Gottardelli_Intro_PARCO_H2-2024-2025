#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

struct global_index {
    int x;
    int y;
};

struct local_index {
    int procX;
    int procY;
    int arrayX;
    int arrayY;
};

// Converts local array index to global index in the original matrix
struct global_index local_to_global(int procX, int procY, int arrayX, int arrayY, int cols, int rows) {
    struct global_index result;
    result.x = arrayX + (procX * cols);
    result.y = arrayY + (procY * rows);
    return result;
}

// Converts global index to local array index
struct local_index global_to_local(int x, int y, int cols, int rows) {
    struct local_index result;
    result.arrayX = x % cols;
    result.procX = x / cols;
    result.arrayY = y % rows;
    result.procY = y / rows;
    return result;
}

// Allocate a 2D array of doubles
double** alloc_2d_double(int x, int y) {
    double **temp = (double **)malloc(sizeof(double*) * x);
    if (temp == NULL) {
        return NULL; // Check for allocation failure
    }
    for (int i = 0; i < x; i++) {
        temp[i] = (double*)malloc(sizeof(double) * y);
        if (temp[i] == NULL) {
            // Free previously allocated memory if there's an allocation failure
            for (int j = 0; j < i; j++) {
                free(temp[j]);
            }
            free(temp);
            return NULL;
        }
    }
    return temp;
}

// Free a 2D array of doubles
void free_2d_double(double** m, int size) {
    for (int i = 0; i < size; i++) {
        free(m[i]);
    }
    free(m);
}

int main(int argc, char** argv) {
    int n;
    int nprocs, rank;
    int dim_size[2], periods[2];
    int npx, npy;
    int x_offset, y_offset;
    int std_rows_in_core, std_cols_in_core;
    int coords[2];
    int rows_in_core, cols_in_core;
    double **array, **new_array;
    MPI_Status status;
    int i, j;
    MPI_Comm cart_comm;
    int coords2[2], rank2;
    int global_x1, global_x2, global_y1, global_y2;
    MPI_Request send_request, recv_request;
    int tag;
    MPI_Datatype send_type, recv_type;
    int sizes[2], subsizes[2], starts[2];

    n = atoi(argv[1]);
    printf("!!!! N = %d\n", n);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    npx = (int)sqrt(nprocs);
    npy = npx;

    dim_size[0] = npy;
    dim_size[1] = npx;
    periods[0] = 1;
    periods[1] = 1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dim_size, periods, 1, &cart_comm);

    if (rank >= (npx * npx)) {
        MPI_Finalize();
        return 0;
    }

    MPI_Cart_coords(cart_comm, rank, 2, coords);

    rows_in_core = ceil(n / (float)npx);
    cols_in_core = ceil(n / (float)npy);

    std_rows_in_core = rows_in_core;
    std_cols_in_core = cols_in_core;

    if (coords[0] == (npy - 1)) {
        cols_in_core = n - (cols_in_core * (npy - 1));
    }
    if (coords[1] == (npx - 1)) {
        rows_in_core = n - (rows_in_core * (npx - 1));
    }

    printf("Rank: %d, X: %d, Y: %d, RpC: %d, CpC: %d\n", rank, coords[0], coords[1], rows_in_core, cols_in_core);

    // Initialize matrices
    array = alloc_2d_double(rows_in_core, cols_in_core);
    new_array = alloc_2d_double(rows_in_core, cols_in_core);
    if (array == NULL || new_array == NULL) {
        printf("Problem with array allocation.\nExiting\n");
        MPI_Finalize();
        return 1;
    }

    for (i = 0; i < rows_in_core; i++) {
        for (j = 0; j < cols_in_core; j++) {
            new_array[i][j] = 0.0;
        }
    }

    // Calculate offsets
    x_offset = coords[0] * std_cols_in_core;
    y_offset = coords[1] * std_rows_in_core;

    for (i = 0; i < rows_in_core; i++) {
        for (j = 0; j < cols_in_core; j++) {
            array[i][j] = (i + y_offset) * n + (j + x_offset) + 1;
        }
    }

    // Do transpose (send data to opposite process)
    coords2[0] = coords[1];
    coords2[1] = coords[0];

    MPI_Cart_rank(cart_comm, coords2, &rank2);

    tag = (coords[0] + 1) * (coords[1] + 1);

    // Create new derived type for sending and receiving the subarray
    sizes[0] = rows_in_core;
    sizes[1] = cols_in_core;
    subsizes[0] = rows_in_core;
    subsizes[1] = cols_in_core;
    starts[0] = 0;
    starts[1] = 0;

    MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_FORTRAN, MPI_DOUBLE, &send_type);
    MPI_Type_commit(&send_type);

    MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_C, MPI_DOUBLE, &recv_type);
    MPI_Type_commit(&recv_type);

    // Send and receive data
    MPI_Send(&array[0][0], 1, send_type, rank2, tag, cart_comm);
    MPI_Recv(&new_array[0][0], 1, recv_type, rank2, tag, cart_comm, &status);

    // Print result
    for (i = 0; i < rows_in_core; i++) {
        printf("Result Process (%d, %d): ", coords[0], coords[1]);
        for (j = 0; j < cols_in_core; j++) {
            printf("%f ", new_array[i][j]);
        }
        printf("\n");
    }

    // Free memory
    free_2d_double(array, rows_in_core);
    free_2d_double(new_array, rows_in_core);

    MPI_Finalize();
    return EXIT_SUCCESS;
}
