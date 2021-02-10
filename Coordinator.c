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

int main(int argc, char** argv) {

    int ProcessID[4]; // initialize array for ProcessIDs
    int ShmID[4]; // initialize array for shared memory
    int status;
    int result;

    for (int i = 2; i<6; i++){ //for loop for process creation 

        int fd[2]; // Create Pipe
        if (pipe(fd) < 0){
            printf("Coordinator: Pipe creation failed\n");
            exit(1);
        }

        ShmID[i-2] = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666); //create shared memory

        ProcessID[i-2] = fork(); //fork
        if (ProcessID[i-2] > 0){ // parent
            printf("Coordinator: forked process with ID %d.\n", ProcessID[i-2]);
            close(fd[0]); // close Read end
            write(fd[1], &ShmID[i-2], sizeof(int)); //write shared memory to pipe
            printf("Coordinator: wrote shm ID %d to pipe (4 bytes)\n", ShmID[i-2]);
            close(fd[1]); // close write end

        } else {// child
            close(fd[1]); // Close Write end
            char str[8];
            sprintf(str, "%d", fd[0]); 
            execlp("./checker", str, argv[1], argv[i], NULL); // new image in in child
        }
    }

    for (int i = 2; i<6; i++){ // for loop to recieve
        printf("Coordinator: waiting on child process ID %d...\n", ProcessID[i-2]);

        // This was an attempt to receive concurently without a double for loop (O(N^2) time)
        //-----------------------------Skip-----------------------------
        // int First_Back = wait(NULL);

        // switch (First_Back){
        // case first[0]:
        //     printf("Hello");
        //     break;
        
        // case ProcessID[1]:
        //     printf("Hello");
        //     break;
        
        // case (ProcessID[2]):
        //     printf("Hello");
        //     break;
                    
        // case ProcessID[3]:
        //     printf("Hello");
        //     break;
        // default:
        //     break;
        // }

        int* shared_mem; // initialize pointer
        wait(&status); // wait
        shared_mem = shmat(ShmID[i-2], NULL, 0); // attach shared memory to pointer
        if (*shared_mem == 1){ //check if shared memory is 1
        printf("Coordinator: result %d read from shared memory: %s is divisible by %s.\n", *shared_mem, argv[i], argv[1]);
        } else if (*shared_mem == 0){
        printf("Coordinator: result %d read from shared memory: %s is not divisible by %s.\n", *shared_mem, argv[i], argv[1]);
        }else {
            printf("Coordinator: ERROR, Shared memory content not 1 or 0");// error
        }
        result = shmctl(ShmID[i-2], IPC_RMID, 0);
        //delete shared memory
    }
    printf("Coordinator: exiting.\n");
    return 0;
}