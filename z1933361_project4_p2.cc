/**
 * @file z1933361_project4_p2.cc
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
#include <string.h>
#include <unistd.h>
#include <ctype.h>


//global variables:
char sharedstr[] = "All work and no play makes Jack a dull boy.";     //shared string
sem_t sem1;   //reader and writer
sem_t sem2;   //protects readers' crit section
int readcount = 0;
int numreaders, numwriters;

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
        return false;

    for (int i = 0; args[i] != 0; i++)
    {
        if (!isdigit(args[i]))
            return false;
    }
    return true;
}

void *writer(void *param)
{
    //loop until string is empty
    while (sharedstr[0] != '\0')
    {
        sem_wait(&sem1);

        int size = strlen(sharedstr);
        if (size > 0)
            sharedstr[size - 1] = '\0';
        
        //notify reader about writing completion
        sem_post(&sem2);    

        //sleep for 1 second
        sleep(1);
    } 

    pthread_exit(NULL);
}


void *reader(void *param)
{
    //enter critical section
    while (1)
    {
        sem_wait(&sem2);
        //readcount++;

    //    printf("Readcount: %d\n", readcount);

        //do reading ----> print out the shared string
        printf("%s\n", sharedstr);

        //signal the writers to proceed
        sem_post(&sem1);

        int waitingwriters = numwriters - numreaders;
        for (int i = 0; i < waitingwriters; i++)
            sem_post(&sem2);

        sleep(1);

        if (sharedstr[0] == '\0') 
            break;
    }
    pthread_exit(NULL);
    
}


//takes 2 cmmd line args - # of readers, # of writers
int main (int argc, char *argv[])
{
    
    //check for correct number of arguments
    if (argc != 3)
    {
        printf("Incorrent number of arguments");
        exit(EXIT_FAILURE);
    }
    //check for valid command line arguments:
    if (isNum(argv[1]))
    {
        if (atoi(argv[1]) == 1)
            numreaders = atoi(argv[1]);
        else
        {
            printf("Only 1 reader allowed.\n");
            exit(EXIT_FAILURE);
        }
    }
    if (isNum(argv[2]))
        numwriters = atoi(argv[2]);



    //innitialize semaphore 1 to 1
    if (sem_init(&sem1, 0, 1) == -1) 
    {
        perror("semaphore initialization");
        exit(EXIT_FAILURE);
    }
    //initialize semaphore 2 to 0
    if (sem_init(&sem2, 0, 0) == -1)
    {
        perror("semaphore initialization");
        exit(EXIT_FAILURE);
    }
   

    //create reader and writer threads
    //pthread
    pthread_t rthreads[numreaders]; //will hold reader thread id's
    pthread_t wthreads[numwriters]; //will hold writer thread ids
   
   //create reader threads
   for (int i = 0; i < numreaders; i++)
   {
        if (pthread_create(&rthreads[i], NULL, reader, NULL) != 0) 
        {
            printf("ERROR; return code from pthread_create() is %d\n", i);
            exit(EXIT_FAILURE);
        }
   }
   
    //create writer threads
    for (int i = 0; i < numwriters; i++)
    {
        if (pthread_create(&wthreads[i], NULL, writer, NULL) != 0) 
        {
            printf("ERROR; return code from pthread_create() is %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    //wait for reader threads to finish
    for (int i = 0; i < numreaders; i++)
    {
        if (pthread_join(rthreads[i], NULL) != 0)
        {
            printf("Error joining reader thread%d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    //wait for writer threads to finish
    for (int i = 0; i < numwriters; i++)
    {
        if (pthread_join(wthreads[i], NULL) != 0)
        {
            printf("Error joining reader thread%d\n", i);
            exit(EXIT_FAILURE);
        }
    }



   /* Last thing that main() should do */
   

    //cleanup and exit
    sem_destroy(&sem1);
    sem_destroy(&sem2);


    return 0;

}