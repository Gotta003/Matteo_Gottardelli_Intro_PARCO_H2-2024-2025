# Matteo_Gottardelli_Intro_PARCO_H2-2024-2025
Repository for the second partial exam Fundamentals of Parallel Computing at University of Trento

## Table of Contents
- [Introduction](#introduction)
- [Project Layout](#project-layout)
- [Software and Cluster Requirements](#software-and-cluster-requirements)
- [Installation and Configuration](#installation-and-configuration)
  - [Global Protect](#global-protect)
  - [Clone Git](#clone-git)
  - [Cluster](#cluster)
    - [Access Cluster](#access-cluster)
    - [Import Project](#import-project)
    - [Import Libraries](#import-libraries)
  - [Run Project](#run-project)
    - [Warnings Before Running](#warnings-before-running)
  - [Download Results](#download-results)
- [Code Overview](#code-overview)
  - [Input Parameters](#input-parameters)
  - [Flow of the program](#flow-of-the-program)
  - [Performances theorical peak (Optional Task)](#performances-theorical-peak-optional-task)
- [Contact](#contact)
  
---

# Introduction

The purpose of this project is studying various ways to optimize a matrix transposition starting from a sequential code. All the analysis and reflection on the result obtained can be found in the report, but to get a more in-depth of all the simulations done to do this project and the definitive results obtained you can see the folder results. The ways that I explored the Transposition and Symmetry are:<br><br>
(1) **Sequential Code**: code running normaly without any optimization<br>
(2) **Implicit Block**: implicit code (only one thread) using a block-based technique<br>
(3) **Implicit Recursive**: implicit code using a recursive function<br><br>
**OMP**:<br><br>
Symmetry check was done in OMP with two algorithms, one exiting when the condition is respected no more (called Local) and one that does all the iterations without exiting (called global).<br>
Transposition was done in a work-sharing and in a block-based technique.<br>
The combionation of these 4 generated these explored ways:<br><br>
(4) **Local Work-Sharing**<br>
(5) **Global Work-Sharing**<br>
(6) **Local Block-Based**<br>
(7) **Global Block-Based**<br><br>
This is a project done by Matteo Gottardelli which is the owner and responsible of any element in this repository.<br><br>
[Back to top](#table-of-contents)

---

# Project Layout
```
Matteo Gottardelli Project
 ┣ Matrix Transposition         # Project files folder
 ┃ ┣ main.c                     # Main Script with essential code
 ┃ ┣ functions.h                # Headers of file functions.c
 ┃ ┣ functions.c                # Definitions in functions.h
 ┃ ┣ all_simulations.pbs        # PBS file with all the simulations done (37 minutes)
 ┃ ┗ essential_transpose.pbs    # PBS file doing the essential ones (for evaluation - 4 minutes)
 ┣ All Simulations              # All past simulations, data and file Excel
 ┃ ┗ ...
 ┣ Final Results                # Simulation to take as reference for report and the project
 ┃ ┣ Definitive_1               # Folders with all .csv files
 ┃ ┣ ...                  
 ┃ ┗ analysis.xlsx              # File excel summarizing the collected data
 ┣ Matteo_Gottardelli_237749_Report.pdf # Report
 ┗ Latex source                 # Folder with Original files from which the pdf was generated
```

[Back to top](#table-of-contents)

---

# Software and Cluster Requirements
- **Cluster**:<br>
All the simulations you can find in the results folder have been done on a cluster offered by the University of Trento. These were all done in a node that had a minimum of 64 cpus for standard purposes and for OpenMP and for safe I reserved 1 Gb of memory, which is x4 the memory needed for the maximum size.<br>
There are the following properties of the node on which I ran the simulations:<br>
```bash
Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                96
On-line CPU(s) list:   0-95
Thread(s) per core:    1
Core(s) per socket:    24
Socket(s):             4
NUMA node(s):          4
Vendor ID:             GenuineIntel
CPU family:            6
Model:                 85
Model name:            Intel(R) Xeon(R) Gold 6252N CPU @ 2.30GHz
Stepping:              7
CPU MHz:               2300.000
BogoMIPS:              4600.00
Virtualization:        VT-x
L1d cache:             32K
L1i cache:             32K
L2 cache:              1024K
L3 cache:              36608K
NUMA node0 CPU(s):     0-23
NUMA node1 CPU(s):     24-47
NUMA node2 CPU(s):     48-71
NUMA node3 CPU(s):     72-95
```
You can see these information on a Linux system using:
```bash
lscpu
```
The final simulations I've saved as definitive are one on 96 and two on 72. The first one is the more detailed one, so I'll leave you below the description for the on with 72 CPUs, but be careful that the data on the report are based on 96 CPUs architecture.
```bash
Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                72
On-line CPU(s) list:   0-71
Thread(s) per core:    1
Core(s) per socket:    18
Socket(s):             4
NUMA node(s):          4
Vendor ID:             GenuineIntel
CPU family:            6
Model:                 85
Model name:            Intel(R) Xeon(R) Gold 6154 CPU @ 3.00GHz
Stepping:              4
CPU MHz:               3000.000
BogoMIPS:              6000.00
Virtualization:        VT-x
L1d cache:             32K
L1i cache:             32K
L2 cache:              1024K
L3 cache:              25344K
NUMA node0 CPU(s):     0-17
NUMA node1 CPU(s):     18-35
NUMA node2 CPU(s):     36-53
NUMA node3 CPU(s):     54-71
```
- **Native Machine**:<br>
This project was developed with a MacOS machine, the simulation wasn't done on the cluster, but I've said this to evidence that I didn't work with MobaxTerm (an application that gives a GUI interface to the user interfacing with a cluster). The download page for this application is right here an if you have a windows architecture it's highly recommended to use this: [mobaxterm](https://mobaxterm.mobatek.net/download.html)<br><br>
Some operations with Moba aren't necessary to use the instructions in this readMe, like transfering files from and to the cluster.<br><br>

[Back to top](#table-of-contents)

---

# Installation and Configuration

## Global Protect
If you would like to connect to a cluster, the connection has to be protected by a VPN coonection, so to install and setup a secure connection to setup the cluster:<br>
1. Download Global Protect according to version of your architecture: [Global Protect](https://vpn-esterni.icts.unitn.it/global-protect/getsoftwarepage.esp)<br>
2. Open Global Protect App<br>
3. Connection to a vpn portal. If you are part of the Unitn University, you can set the Portal to:
```bash
   vpn-mfa.icts.unitn.it
```
4. Pressing connect you will be redirect to an external login page and putting the credentials the connection will be set
## Clone Git
1. Access to your GitHub account, running the command below in the terminal on your computer:
```bash
cd ~
gh auth login
```
If it doesn't work you may have to download some extra dependencies, like gh.
If you are on MacOS or Linux you can install homebrew. To verify if homebrew is already install run on terminal:
```bash
brew --version
```
If that doesn't put out Homebrew and the version, it does mean you haven't installed it, yet. So, to install it run:
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
Now, doing:
```bash
brew install <missing command>
```
brew will be updated and that command will be integrated in your terminal. So, install with this method gh<br><br>

2. After the command run successfully, you will have to do some choices. Select Github.com then HTTPS, type Y and press Login with a web browser and then press enter. You can use any other type of method, but I'm going to explain it how I have done it<br>
3. In the new open panel, insert the credentials and the one-time code generated on terminal and authorize the connection
4. Insert your passkey and after that you will see on your terminal as last line "Logged in as <Your ID on GitHub><br>
5. Now run this command to clone the above repository:
```bash
gh repo clone Gotta003/Matteo_Gottardelli_Intro_Parco_H1-2024-2025
```
6. To see, if you have successfully clone the repository try to access to it, running:
```bash
cd Matteo_Gottardelli_Intro_Parco_H1-2024-2025
```
Now, your are in the folder.<br><br>

[Back to top](#table-of-contents)

---

## Cluster 
### Access Cluster
1. Now leaving the previous terminal window of the cloned repository, but it has to still be open, open a new terminal window
2. Run the command to access to the cluster (be careful that the VPN is active:
```bash
ssh <username>@hpc.unitn.it
```
If you have another cluster change the obvious parameters.
3. Insert the required password
4. In the University of Trento, I you are inside the cluster you will see your username@<one of two node in the cluster (n1 and n2)>
### Import Project
All the required codes that have to be run are in the "Matrix Transposition" folder, so we have to transfer that folder on the cluster. Now you should have two terminal windows, one in the cloned repository and another connected to the cluster. If so, follow these steps:
1. Run this command in the cloned repository terminal:
```bash
cd Matrix_Transposition
pwd
```
If not present or gives error, install with (if on MacOS or Linux):
```bash
brew install pwd
```
2. Make an alias, copy this instruction, but substitute with <Path From pwd> the path outputed previously
```bash
local_path='<Path From pwd>'
```
If that doesn't work, maybe the '' are indexed different, so change them manually
3. Create another alias with your email of the cluster you have logined with, if you are part of Unitn Cluster it is like:
```bash
cluster_mail='<username>@hpc.unitn.it'
```
4. Knowing that you would like to load all files in a specific directory, save in an alias the destination path of this folder, with you what it to be in the position you are on the cluster terminal, you can run this (ON CLUSTER):
```bash
pwd
```
5. Copy this path and create on the other terminal running this:
```bash
cluster_path='<Path Destination>'
```
6. Now, if you have correctly set the alias, you can directly run this command, in the native terminal (NOT THE CLUSTER ONE):
```bash
scp -r $local_path $cluster_mail:$cluster_path
```
After this you will required to insert the password to access to your cluster account and after that then running this you will see the files:
```bash
cd Matrix_Transposition
ls
```
If you have any problem with scp, try to install it with brew or verify the content of the three variables running:
```bash
echo $local_path
echo $cluster_mail
echo $cluster_path
```
<br>

[Back to top](#table-of-contents)

---

### Import Libraries
To run the simulation as I've run it, you have to include some libraries in the cluster, in specific, but here you these possibility:<br>
1) You want to run the simulations via the pbs files and there the libraries are added, so you can skip right now<br>
2) Do the simulation in an interactive session, because you may want to see the behaviour of a particular simulation of a set of parameters, so do this each time you enter in a new one<br>
3) You can set them as default libraries, so they will be directly included in the interactive session<br><br>
The simulation have been done using gcc91 and numactl library. The first one is the C compiler Version 9.1.0, meanwhile numactl is a library that allow a control of the single sockets, which will help some simulations with an high number of threads.<br>
Remember that you are free to use other libraries, but I you want to emulate the simulations and you chose 3) follow this:<br>
1. We have to modify the .bashrc file, an hidden one. Go to the head node, if you are in Matrix_Transposition, you may do:
```bash
cd ..
ls -la
```
2. Now theoretically, you will be in the folder with .bashrc, run:
```bash
vi .bashrc
```
3. The file will be opened and you have to press I to be able to modify it. So, press I and insert with copy and paste under #User specific aliases and functions, this:
```bash
module load gcc91
alias gcc=gcc-9.1.0
module load numactl
```
4. Now to exit the file type the following in order:
• 'esc' button
• :w in order to save the file
• :q in order to close the file
5. Now try run a try interactive session in order to see if .bashrc works, so run:
```bash
qsub -I -q short_cpuQ
```
```bash
module list
```
You will see all the modules you have loaded, including gcc91 and numactl. After this you can exit from the interactive session running:
```bash
exit
```
<br>

[Back to top](#table-of-contents)

---

## Run Project
### Warnings before Running
Be careful before running of two things:
1. Before runnnig a PBS file you have to change inside it the path to create the files on it, according to the path in your cluster to save it in the directory you like. You can done it directly on the Cluster going in the folder previously exported Matrix_Transposition and performing:
```bash
nano <name .pbs file>.pbs
``` 
This will open you an interactive environment on which you can modify the file.<br>
Both the pbs file have the line to modify at 22. And instead of that you have to modify the username and after it the path to the destination folder.<br>
After doing this to exit from this modality press (control + X), then Y and ENTER. Now you can freerly run your pbs file as is explain in advance.<br><br>
2. My C code in order to run on the cluster with simulation trigger implicitly the cache in order to free it an make it as clear as possible. Do to this in the header file I've put the values of the caches extracted from the command lscpu on the node with 96 CPU. This will guarantee a pretty good clean with all the system with equal or less cache dimensions. But if you run in a smaller node requesting a lower number of CPUs or you are not running on my same cluster as introduced before, you may need to change these costant. To see the properties of the node you want to run on, you can perform:
```bash
lscpu
```
An then change if you want these constants, by doing the command nano on functions.h and modify them at lines according to your system. Obviously, if you have more caches or less you have to manually change the function ClearAllCaches. Mine properties are the following: (CACHEL1D 32KB, CACHEL1I 32KB, CACHEL2 1024KB, CACHEL3 36608KB (less than 36MB but this was to made to make it a perfect power of 2)).<br><br>

[Back to top](#table-of-contents)

---

### Running Time
Now the environment is completly set and you can start running the project. You have two possibilities:<br>
1) PBS execution<br>
In the folder you will find two files .pbs, one transposition_complete.pbs (to run requires almost 37 minutes), which will perform all the simulations at I have performed to develop this project and a transposition_essential.pbs (to run requires 4 minutes), which are the essential like the best implicit algorithm and compilation with flags and the best OMP.<br>
To run it you perform this:
```bash
qsub <pbs file>
```
If you want to monitor the code, you may run:
```bash
qstat -u <username>
```
This pbs will generate a folder results, if already presence, will add an integer after it and generate various files .csv for the results and for the single times one general and one for each mode to see detailed specific for each modes jump to [Code Overview](#code-overview). 
At the end of execution the files will be moved in the first created folder, but not the transpose.o and transpose.e files which are the prints on stdout and stderr, that are created when the pbs file expires its execution. So you have to them with this command:
```bash
mv transpose.o <dest_folder>
mv transpose.e <dest_folder>
```
This isn't mandatory, but if you would like a detailed overview you have to do this, because with the next .pbs execution they will be overwritten.<br><br>
The definitive simulation was performed in a node with 96 CPUs, but to accomodate the conditions above, the system may give you a 72 CPUs node, so some performances may be different because of that.<br>
2) Interactive Session
To run a simulation of a desired combination of parameters, you have to follow these steps:
1. Open an interactive session, with the following parameters:
```bash
qsub -I -l select=1:ncpus=64:ompthreads=64:mem=1Gb -q short_cpuQ
```
2. Go to the folder and check the details of the architecture, if are what you are expecting:
```bash
cd Matrix_Transposition
lscpu
```
3. Compile the code with gcc command the sequential code of the desired size or test_mode (to see the sintax of each parameter jump to [Code Overview](#code-overview))
```bash
gcc -O0 functions.c transpose.c -o transpose -fopenmp -lm
```
The definitive simulation was performed in a node with 96 CPUs, but to accomodate the conditions above, the system may give you a 72 CPUs node, so some performances may be different because of that.<br><br>
4. Then run:
```bash
./transpose SO0 1 <size> <test_mode> 25
```
This will generate 4 files, two dedicated for sequential code (time and average) and two general.<br><br>
5. Now you can perform which compilation you'd like, following the following structure:
```bash
gcc <efficiency flag -O0/-O1/-O2> <other flags> functions.c transpose.c -o transpose -fopenmp -lm
```
The last two flags are always mandatory due to my C code, because to calculate the time I use an omp function, even though I don't use OMP and -lm to explicitly link the two files in compilation, but I you want to run in OMP so from mode 4 to 7, you will have to add -fopenmp-simd<br><br>
(OPTIONAL) Be careful, if you would like to exactly reproduce my results with OpenMP you need to setup the environment variables. So, after the compilation it's important to run the following instructions:
```bash
export OMP_DYNAMIC=FALSE
export OMP_SCHEDULE=STATIC
export OMP_PROC_BIND=spread
export OMP_PLACES=cores
export OMP_WAIT_POLICY=ACTIVE
export OMP_NESTED=TRUE
export OMP_CANCELLATION=TRUE
```
Is not important to set the number of threads, because that is automatically handled by my C code, but if you want for some reason setup that, you can run:
```bash
export OMP_NUM_THREADS=<number of threads>
```
6. The execution in brief:
```bash
<eventually numactl for 64 threads> ./transpose <code_identifier> <mode> <size> <test_mode> <samples> <n° threads (not mandatory and ignored for mode from 1-3)>
```
And example of numactl instruction that I've used is numactl --cpunodebind=0,1 --membind=0,1 for 64 threads.<br><br>
Jump to [Code Overview](#code-overview)), to see a detailed description of each parameter.<br><br>

[Back to top](#table-of-contents)

---

### Download Results
If you want to transfer a file or a folder from cluster to your local machine, you have to perform the previous scp command in reverse. So, create a variable for the destination location:
```bash
dest_path=<dest_path>
```
And then run the following command (if a file remove -r, the example is a folder using the previous data):
```bash
scp -r cluster_mail:cluster_path dest_path
```
Being csv files, I've computed the results and the graphs on excel, importing the file, but you may use python natively in the cluster to elaborate the data. But arrived here, you have obtained your desired data.<br><br>

[Back to top](#table-of-contents)

---

# Code Overview
## Input Parameters
The code takes as for input 6 parameters:<br>
1 & 2. Acronym and modes - The first two parameters to give in input to the system is an acronym and an integer indicating a particolar mode. To allow an easy recognition and collection in Excel each different combination of modes and flags with an identifier was added as the first parameter. These are the flags used in my tests for the implicit modes:<br>
<table>
        <tr>
            <th>Flag</th>
            <th>Purpose</th>
        </tr>
        <tr>
            <td>-fprefetch-loop-arrays</td>
        	  <td>Enabling the prefetching optimizations</td>
        </tr>
        <tr>
            <td>-ftree-vectorize</td>
        	  <td>Enabling auto-vectorization of loops</td>
        </tr>
        <tr>
        	  <td>-funroll-loops</td>  
        	  <td>Unroling loops to reduce overhead</td>
        </tr>
        <tr>
        	  <td>-flto/td>  
        	  <td>Optimizing during linking</td>
        </tr>
        <tr>
        	  <td>-march=native/td>  
        	  <td>Optimizing the code for the host CPU</td>
        </tr>
</table>
Each mode is identified with an integer and if there are any particular configuration with flags is indicated thanks to the acronym and there are the configurations (acronym - mode) that I have used according to my code:<br>
<table>
        <tr>
            <th>Compiler Code</th>
            <th>Mode</th>
            <th>Description</th>
        </tr>
        <tr>
            <td>SO0</td>
        	  <td>1</td>
        	  <td>Sequential Code, with no optimization</td>
        </tr>
        <tr>
            <td>I2</td>
        	  <td>2</td>
        	  <td>Block Implicit Parallelism with optimization 2</td>
        </tr>
        <tr>
        	  <td>I2*</td>  
            <td>2</td>  
        	  <td>Block Implicit Parallelism with optimization 2 and flags summary</td>
        </tr>
        <tr>
          	<td>R2</td>  
            <td>3</td>
        	  <td>Recursive Implicit Parallelism with optimization 2</td>
        </tr>
        <tr>
            <td>R2*</td>
        	  <td>3</td>
        	  <td>Recursive Implicit Parallelism with optimization 2 and flags summary</td>
        </tr>
        <tr>
            <td>OMPLOCWS</td>
        	  <td>4</td> 
        	  <td>Explicit Parallelized Code with OMP with a work-sharing technique for transposition and checking with interruption of the cycle</td>
        </tr>
        <tr>
            <td>OMPGLBWS</td>
        	  <td>5</td>
        	  <td>Explicit Parallelized Code with OMP with a work-sharing technique for transposition and checking with NO interruption of the cycle</td>
        </tr>
        <tr>
             <td>OMPLOCBB</td>
        	  <td>6</td>
        	  <td>Explicit Parallelized Code with OMP with a block-based technique for transposition and checking with interruption of the cycle</td>
        </tr>
        <tr>
           <td>OMPGLBBB</td>
        	  <td>7</td>
        	  <td>Explicit Parallelized Code with OMP with a block-based technique for transposition and checking with NO interruption of the cycle</td>
        </tr>
</table>
You can add other signatures as you wish there is no limit, the most important thing is that in order to have speedup and efficiency in the results, you have to run it mandatory with SO0 1 as the first two parameters with the desired size and test_mode, but how you use the acronyms is your business, because I don't know with which flag you are running. Obviously, you have to pay attention to this, if you are in an interactive session, if you are using my pbs files, don't worry because they are already preimposted.<br><br>
3. Size - The algorithm works only with power of two and sizes between 16 and 4096, so it takes as input the exponential of the power from 4 to 12 (16->4, 32->5, 64->6, 128->7, 256->8, 512->9, 1024->10, 2048->11, 4096->12).<br><br>
4. Test Mode - The project assigned asked only to analyze a standard case, so assign to a matrix random numbers, verify if that is symmetric and make a transposition, but to verify different behaviours I've created 4 test mode, the first for normal usage and the others for testing.<br>
<table>
        <tr>
            <th>Compiler Code</th>
            <th>Scope</th>
            <th>Description</th>
        </tr>
        <tr>
          <th>0</th>
          <th>Random</th>
          <th>General purpose to make a check of symmetry and in case a matrix transposition , with random value, hopefully not repetitives because of unseeded srand</th>
        </tr>
        <tr>
          <th>1</th>
          <th>Static Matrix</th>
          <th>Mode used for testing and to discuss the report results, it uses a matrix with standard values according to an algorithm guaranteeing that each simulation has the exactly same values.</th>
        </tr>
        <tr>
          <th>2</th>
          <th>Symmetric</th>
          <th>Mode that inizializes all the locations of a matrix with an always equal value</th>
        </tr>
        <tr>
          <th>3</th>
          <th>Worst Case</th>
          <th>Mode that iniziates all the locatuibs if a matrix with the same value, but after that changes the one at the bottom right of the matrix, but not on the main diagonal, because my algorithms will run from the high to low, row by row, being row-based algorithms. So, this will lead to a check of symmetry that will be true until the last one which will lead to a matrix transposition.</th>
        </tr>
</table>
The generation of the same values obviously doing simulations will logically be inaccurate if the cache is not free. In my code the problem is not present in the most cases, thanks to a function that indirectly frees the caches, so these static matrices would be a problem.<br><br>
5. Samples - In my code, at each execution will be output directly the average of the times, in order to internally compute the speedup and the efficienct thanks to that algorithm a number of samples can be inputed and all the outputs will be viewable in times*.csv files, but the average time, the speedup and the efficiency will be calculated according to the 40% in the middle of the data. Because of this I've decided to put a minimum of input samples per simulation on 25, in order to take the 10 values in the middle of an ordered array and cutting off the outliers, but there is no above limit, but to it parsimonously, otherwise your simulation can be take an enormous amount of time.<br><br>
6. Number of Threads - This is a not mandatory parameter in the first three modes (sequential and implicit) and the program will work otherwise, because that will be just ignored, but for the other modes (OMP), that is mandatory. The value has to be a power of 2 and the numbers have to be between 1 and 64, so in this specific project could not be run threads other than 1, 2, 4, 8, 16, 32 and 64.<br><br>

[Back to top](#table-of-contents)

---

## Flow of the program
1. Verify that the input parameters inserted are correct (See above [Input Parameters](#input-parameters))<br>
2. Choose of the subblock according to the size and the number of threads. Is always chosen, but it is used only in block-based algorithms. The minimum block is 8 and the maximum according to mine smallest memory cache (32K), considering that I do two operations with floats and doing 2 cycles tells that the maximum block is the (square of 4K)=64. But, if there are threads, this isn't true anymore, so this maximum size is divided by the number of threads. So, is choosen the minimum between the half of the input size and the maximum length divided by the number of threads. Then, is verified that the block size isn't too small, so if lower of 8 it is set to that size.<br>
3. Starting simulations, at each iterations, which will be equal to the number of samples, will happen this:<br>
   - 3.1 The cache is freed with the reasoning told before, which is an implicit call of the cache<br>
   - 3.2 Allocation space of the start matrix and the destination one and the initialization according to the test mode inputed<br>
   - 3.3 Simulation with starting time, end time in which the purpose of this program acts. This is checking if the start matrix is symmetric and then if not true transpose it in the destination matrix.
   - 3.4 The memory is freed and the time obtained is written on a file according to the specific mode and on the general one of the times and then is saved in a local array<br>
4. After the simulations are all done, the array in which the times are saved is reorder with a bubblesort algorithm<br>
5. Then, is took from that reordered array the 40% in the middle and from it is computed the average time and if was done the sequential code with the size and the test mode equal to this simulation, are computed the scaling and the efficiency, too. The obtained resultes are saved in simulation summary files for each mode and general ones, which are different from the one with each single times.
6. Now, the program is ended an ready to get other data as input.
<br><br>
[Back to top](#table-of-contents)

---

## Performances theorical peak (Optional Task)

Because of the lack of space on the four report page, I preferred to write the tasks to do in a good and comprehensive way, so this bonus task, I've decided to put it in the ReadME. Considering the architecture of 96 CPU, to calculate the memory bandwidth, we have to consider the following formula:
$$Memory Bandwidth = Frequency (GHz) * Memory Bus Width (bytes) * Data Rate$$
Considering the information provided above, here there is what we need:
• Base frequency 2,3 GHz
• Bus width 64 bit -> 8 bytes
• 6 memory channels per socket, with 4 sockets, the data rate will be 24 times for fast
$$Memory Bandwidth=2,3*8*24=441,6 \frac{Gb}{s}$$
Considering for example the sequential code, it will be run on only one CPU, so the memory bandwidth per CPU is $`441,6/96=4,6 \frac{Gb}{s}`$. If we are using more threads it will become 4,6*n° threads. From this, we know from the report that my transposition algorithm have $`2N^2`$ operations, all with floats, so the memory to be transfer will be $`8N^2`$. From this we can estimate the times theoretically doing the memory to be transfer/the memory bandwidth per CPU used. With this data here there is the relation between theoretical and sequential time:
<table>
        <tr>
            <th>Size</th>
            <th>Sequential (s)</th>
            <th>Theoretical Time (s)</th>
            <th>Math Relation</th>
        </tr>
        <tr>
          <th>16</th>
          <th>0,00000097</th>
          <th>0,00000045</th>
          <th>46,1%</th>
        </tr>
         <tr>
          <th>32</th>
          <th>0,00000325</th>
          <th>0,00000178</th>
          <th>54,8%</th>
        </tr>
         <tr>
          <th>64</th>
          <th>0,00001085</th>
          <th>0,00000712</th>
          <th>65,7%</th>
        </tr>
         <tr>
          <th>128</th>
          <th>0,00004752</th>
          <th>0,00002849</th>
          <th>60%</th>
        </tr>
        <tr>
          <th>256</th>
          <th>0,00021899</th>
          <th>0,00011398</th>
          <th>52%</th>
        </tr>
         <tr>
           <th>512</th>
            <th>0,00066400</th>
            <th>0,000455903</th>
            <th>68,5%</th>
          </tr>
         <tr>
           <th>1024</th>
            <th>0,00306322</th>
            <th>0,00182361</th>
            <th>59,5%</th>
          </tr>
          <tr>
           <th>2048</th>
            <th>0,02530560</th>
            <th>0,00729444</th>
            <th>28,8%</th>
          </tr>
         <tr>
           <th>4096</th>
            <th>0,12131174</th>
            <th>0,029177767</th>
            <th>24,1%</th>
          </tr>
</table>
All the calculus and the tables can be found in Definitive Results, first table for 96 CPUs. Here below, there is the graph of the relation and is seeable that the values are mostly in the range as expected, because effective bandwidth is typically between 50% and 70% respect from the theoretical one and in the time logic the relation is the same. The downgrade in the last two sizes is caused by the bootlenecks told in the report, leading to collisions, when transfering large amounts of bytes.<br><br>
<div align="center">
  <p>
      <img width="100%" src="Latex_Source/images/Image 1.06.png">
  </p>
</div>
<br><br>
[Back to top](#table-of-contents)

---

# Contact
I you have any doubts or some doesn't work and in order to accreditate this documentation for exam evaluation.<br>This is my university contact:<br><br>
Matteo Gottardelli matteo.gottardelli@studenti.unitn.it

[Back to top](#table-of-contents)

---

