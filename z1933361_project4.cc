/**
 * @file z1933361_project4.cc
 * @author Kendra Ferguson
 * @version 0.1
 * @date 2023-11-02
 * 
 * 
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include <iostream> //remove

//global variables:
char sharedstring[] = "Cats are better than dogs.";     //shared string

/**
 * Checks if arguments are valid non-negative integers
 * 
 * @param args - A command line argument
 * @return true if argument is a valid positive number
 * @return false if argument is not a valid positive number
 */
bool isNum(char args[])
{
    //check for negative symbol
    if (args[0] == '-')
    {
        return false;
    }

    for (int i = 0; args[i] != 0; i++)
    {
        if (!isdigit(args[i]))
            return false;
    }
    return true;
}

void *writer(void *param)
{


    long tid;
   tid = (long)param;
   printf("Hello World! It's me, writer thread #%ld!\n", tid);
   pthread_exit(NULL);
    //variables

    //wait(wrt)

    //loop until string is empty
    // {
        //print message stating it is writing
        //remove last char of string

        //sleep for 1 second
    // }

    //signal(wrt)
}


void *reader(void *param)
{

    long tid;
   tid = (long)param;
   printf("Hello World! It's me, reader thread #%ld!\n", tid);
   pthread_exit(NULL);
    //enter crit section
    //wait(mutex)
    //readcount++
    //if readcount ==1
        //wait(wrt)
    //signal(mutex) ---- exit crit section

    //do reading ----> print out

    //wait(mutex) ----- enter crit sec
    //readcount--
    //if readount==0
        //signal(wrt)
    //signal(mutex) ------ exit crit sec
}


//takes 2 cmmd line args - # of readers, # of writers
int main (int argc, char *argv[])
{
    int numreaders, numwriters;
    //check for correct number of arguments
    if (argc != 3)
    {
        printf("Incorrent number of arguments");
        exit(EXIT_FAILURE);
    }
    //check for valid command line arguments:
    if (isNum(argv[1]))
        numreaders = atoi(argv[1]);
    if (isNum(argv[2]))
        numwriters = atoi(argv[2]);


    //initialize semaphores:
    //semaphore mutex=1, wrt=1
    sem_t rw_sem;   //reader and writer
    sem_t cs_sem;   //critical section
    int readcount = 0;

    //innitialize semaphores to 1
    if (sem_init(&rw_sem, 0, 1) == -1) 
    {
        perror("semaphore initialization");
        exit(EXIT_FAILURE);
    }
    
    if (sem_init(&cs_sem, 0, 1) == -1)
    {
        perror("semaphore initialization");
        exit(EXIT_FAILURE);
    }
   

    //create reader and writer threads
    //pthread
    pthread_t rthreads[numreaders];
    pthread_t wthreads[numwriters];
    int rc;
    long t;

    for(t=0; t< numreaders; t++)
    {
      printf("In main: creating reader thread %ld\n", t);
      rc = pthread_create(&rthreads[t], NULL, reader, (void*)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   for (t=0; t< numwriters; t++)
   {   
        pthread_join(wthreads[t], NULL);

        for(t=0; t< numwriters; t++)
        {
            printf("In main: creating writer thread %ld\n", t);
            rc = pthread_create(&wthreads[t], NULL, writer, (void*)t);

            if (rc)
            {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
            }
        }
    }

   /* Last thing that main() should do */
  // pthread_exit(NULL);


    //wait for reader threads to finish
    // pthread_join

    //wait for writer threads to finish

    //cleanup and exit


    return 0;

}