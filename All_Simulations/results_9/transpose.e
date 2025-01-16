functions.c: In function ‘matTransposeMPIAllGather’:
functions.c:294:16: warning: implicit declaration of function ‘MPI_WTIME’ [-Wimplicit-function-declaration]
  294 |         time_s=MPI_WTIME();
      |                ^~~~~~~~~
transpose.c: In function ‘main’:
transpose.c:167:17: error: lvalue required as left operand of assignment
  167 |             time=(tw_end-tw_start);
      |                 ^
transpose.c:168:17: error: lvalue required as left operand of assignment
  168 |             time-=time_extra_check;
      |                 ^~
transpose.c:169:17: error: lvalue required as left operand of assignment
  169 |             time-=time_extra_transpose;
      |                 ^~
transpose.c:180:83: error: incompatible type for argument 7 of ‘openFilesResultsPerMode’
  180 |             openFilesResultsPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_THREADS, time);
      |                                                                                   ^~~~
      |                                                                                   |
      |                                                                                   time_t (*)(time_t *) {aka long int (*)(long int *)}
In file included from transpose.c:15:
functions.h:112:148: note: expected ‘double’ but argument is of type ‘time_t (*)(time_t *)’ {aka ‘long int (*)(long int *)’}
  112 | void openFilesResultsPerMode(const char* code, const int mode, const int n, const int test, const int samples, const int num_threads, const double time);
      |                                                                                                                                       ~~~~~~~~~~~~~^~~~
transpose.c:181:28: error: incompatible types when assigning to type ‘double’ from type ‘time_t (*)(time_t *)’ {aka ‘long int (*)(long int *)’}
  181 |             results[count]=time;
      |                            ^~~~
functions.c: In function ‘matTransposeMPIAllGather’:
functions.c:294:16: warning: implicit declaration of function ‘MPI_WTIME’ [-Wimplicit-function-declaration]
  294 |         time_s=MPI_WTIME();
      |                ^~~~~~~~~
transpose.c: In function ‘main’:
transpose.c:167:17: error: lvalue required as left operand of assignment
  167 |             time=(tw_end-tw_start);
      |                 ^
transpose.c:168:17: error: lvalue required as left operand of assignment
  168 |             time-=time_extra_check;
      |                 ^~
transpose.c:169:17: error: lvalue required as left operand of assignment
  169 |             time-=time_extra_transpose;
      |                 ^~
transpose.c:180:83: error: incompatible type for argument 7 of ‘openFilesResultsPerMode’
  180 |             openFilesResultsPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_THREADS, time);
      |                                                                                   ^~~~
      |                                                                                   |
      |                                                                                   time_t (*)(time_t *) {aka long int (*)(long int *)}
In file included from transpose.c:15:
functions.h:112:148: note: expected ‘double’ but argument is of type ‘time_t (*)(time_t *)’ {aka ‘long int (*)(long int *)’}
  112 | void openFilesResultsPerMode(const char* code, const int mode, const int n, const int test, const int samples, const int num_threads, const double time);
      |                                                                                                                                       ~~~~~~~~~~~~~^~~~
transpose.c:181:28: error: incompatible types when assigning to type ‘double’ from type ‘time_t (*)(time_t *)’ {aka ‘long int (*)(long int *)’}
  181 |             results[count]=time;
      |                            ^~~~
functions.c: In function ‘matTransposeMPIAllGather’:
functions.c:294:16: warning: implicit declaration of function ‘MPI_WTIME’ [-Wimplicit-function-declaration]
  294 |         time_s=MPI_WTIME();
      |                ^~~~~~~~~
transpose.c: In function ‘main’:
transpose.c:167:17: error: lvalue required as left operand of assignment
  167 |             time=(tw_end-tw_start);
      |                 ^
transpose.c:168:17: error: lvalue required as left operand of assignment
  168 |             time-=time_extra_check;
      |                 ^~
transpose.c:169:17: error: lvalue required as left operand of assignment
  169 |             time-=time_extra_transpose;
      |                 ^~
transpose.c:180:83: error: incompatible type for argument 7 of ‘openFilesResultsPerMode’
  180 |             openFilesResultsPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_THREADS, time);
      |                                                                                   ^~~~
      |                                                                                   |
      |                                                                                   time_t (*)(time_t *) {aka long int (*)(long int *)}
In file included from transpose.c:15:
functions.h:112:148: note: expected ‘double’ but argument is of type ‘time_t (*)(time_t *)’ {aka ‘long int (*)(long int *)’}
  112 | void openFilesResultsPerMode(const char* code, const int mode, const int n, const int test, const int samples, const int num_threads, const double time);
      |                                                                                                                                       ~~~~~~~~~~~~~^~~~
transpose.c:181:28: error: incompatible types when assigning to type ‘double’ from type ‘time_t (*)(time_t *)’ {aka ‘long int (*)(long int *)’}
  181 |             results[count]=time;
      |                            ^~~~
functions.c: In function ‘matTransposeMPIAllGather’:
functions.c:294:16: warning: implicit declaration of function ‘MPI_WTIME’ [-Wimplicit-function-declaration]
  294 |         time_s=MPI_WTIME();
      |                ^~~~~~~~~
transpose.c: In function ‘main’:
transpose.c:167:17: error: lvalue required as left operand of assignment
  167 |             time=(tw_end-tw_start);
      |                 ^
transpose.c:168:17: error: lvalue required as left operand of assignment
  168 |             time-=time_extra_check;
      |                 ^~
transpose.c:169:17: error: lvalue required as left operand of assignment
  169 |             time-=time_extra_transpose;
      |                 ^~
transpose.c:180:83: error: incompatible type for argument 7 of ‘openFilesResultsPerMode’
  180 |             openFilesResultsPerMode(CODE, MODE, N, TESTING, SAMPLES, NUM_THREADS, time);
      |                                                                                   ^~~~
      |                                                                                   |
      |                                                                                   time_t (*)(time_t *) {aka long int (*)(long int *)}
In file included from transpose.c:15:
functions.h:112:148: note: expected ‘double’ but argument is of type ‘time_t (*)(time_t *)’ {aka ‘long int (*)(long int *)’}
  112 | void openFilesResultsPerMode(const char* code, const int mode, const int n, const int test, const int samples, const int num_threads, const double time);
      |                                                                                                                                       ~~~~~~~~~~~~~^~~~
transpose.c:181:28: error: incompatible types when assigning to type ‘double’ from type ‘time_t (*)(time_t *)’ {aka ‘long int (*)(long int *)’}
  181 |             results[count]=time;
      |                            ^~~~
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0xcfa490, scnts=0xcf7660, displs=0xcfa060, MPI_FLOAT, rbuf=0xd5ac90, rcount=16, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 44188 bytes received but buffer size is 64
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x1ab34c0, scnts=0x1ab3090, displs=0x1ab2f20, MPI_FLOAT, rbuf=0x1b13cc0, rcount=8, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 128 bytes received but buffer size is 32
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0xc4c4c0, scnts=0xc4c090, displs=0xc4bf20, MPI_FLOAT, rbuf=0xcaccc0, rcount=8, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 196 bytes received but buffer size is 32
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x1d8b490, scnts=0x1d88660, displs=0x1d8b060, MPI_FLOAT, rbuf=0x1dec910, rcount=32, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 43692 bytes received but buffer size is 128
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x21664c0, scnts=0x2166090, displs=0x2165f20, MPI_FLOAT, rbuf=0x21c7940, rcount=16, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 128 bytes received but buffer size is 64
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x25d64c0, scnts=0x25d6090, displs=0x25d5f20, MPI_FLOAT, rbuf=0x2637940, rcount=16, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 196 bytes received but buffer size is 64
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x9e6490, scnts=0x9e3660, displs=0x9e6060, MPI_FLOAT, rbuf=0xa4aa10, rcount=64, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 43848 bytes received but buffer size is 256
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x14714c0, scnts=0x1471090, displs=0x1470f20, MPI_FLOAT, rbuf=0x14d5a40, rcount=32, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 196 bytes received but buffer size is 128
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x1932490, scnts=0x192f660, displs=0x1932060, MPI_FLOAT, rbuf=0x19a2cf0, rcount=128, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 44076 bytes received but buffer size is 512
