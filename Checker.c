// C Libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <fcntl.h>
#include <string.h>

//System Libraries
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/resource.h>
#include <sys/mman.h>

int checker(int divisor, int numerator, int PID){ //checker method pretty simple, like in HW#2
    if ((numerator % divisor) == 0 ){
        printf("Checker process [%d]: %d *IS* divisable by %d. \n", PID, numerator, divisor);
        return 1;
    }
    else {
        printf("Checker process [%d]: %d *IS NOT* divisable by %d. \n",PID, numerator, divisor);
        return 0;
    }
}
int main (int argc, char** argv){
    int PID = getpid(); // get process ID
    printf("Checker process [%d]: Starting. \n", PID);
    int* shared_mem; //Pointer declaration to be used on shared memory
    int SMID;
    int fd = (atoi(argv[0]));
    read(fd, &SMID, sizeof(int)); // read from pipe
    printf("Checker process [%d]: read 4 bytes containing shm ID: %d\n",PID, SMID);
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int result = checker(a,b,PID); //run checker 
    shared_mem = shmat(SMID, NULL, 0); // attach shared memory
    *shared_mem = result; //write to shared memory
    printf("Checker process [%d]: Wrote result (%d) to shared memory.\n", PID, result);
    return *shared_mem; // not really sure if this line is even relevent. 
}
