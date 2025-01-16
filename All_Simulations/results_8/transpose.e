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
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x1f5e490, scnts=0x1f5b660, displs=0x1f5e060, MPI_FLOAT, rbuf=0x1fbec90, rcount=16, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 44388 bytes received but buffer size is 64
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x14b64c0, scnts=0x14b6090, displs=0x14b5f20, MPI_FLOAT, rbuf=0x1516cc0, rcount=8, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 128 bytes received but buffer size is 32
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x17b44c0, scnts=0x17b4090, displs=0x17b3f20, MPI_FLOAT, rbuf=0x1814cc0, rcount=8, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 196 bytes received but buffer size is 32
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0xfe5490, scnts=0xfe2660, displs=0xfe5060, MPI_FLOAT, rbuf=0x1046910, rcount=32, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 44016 bytes received but buffer size is 128
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x8964c0, scnts=0x896090, displs=0x895f20, MPI_FLOAT, rbuf=0x8f7940, rcount=16, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 128 bytes received but buffer size is 64
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x1e154c0, scnts=0x1e15090, displs=0x1e14f20, MPI_FLOAT, rbuf=0x1e76940, rcount=16, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 196 bytes received but buffer size is 64
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x1be9490, scnts=0x1be6660, displs=0x1be9060, MPI_FLOAT, rbuf=0x1c4da10, rcount=64, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 44148 bytes received but buffer size is 256
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0x186a4c0, scnts=0x186a090, displs=0x1869f20, MPI_FLOAT, rbuf=0x18cea40, rcount=32, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 196 bytes received but buffer size is 128
Fatal error in PMPI_Scatterv: Message truncated, error stack:
PMPI_Scatterv(386)................: MPI_Scatterv(sbuf=0xe1d490, scnts=0xe1a660, displs=0xe1d060, MPI_FLOAT, rbuf=0xe8dcf0, rcount=128, MPI_FLOAT, root=0, MPI_COMM_WORLD) failed
MPIR_Scatterv_impl(184)...........: 
MPIR_Scatterv(139)................: 
MPIDI_CH3U_Receive_data_found(131): Message from rank 0 and tag 6 truncated; 44556 bytes received but buffer size is 512
=>> PBS: job killed: walltime 1220 exceeded limit 1200
