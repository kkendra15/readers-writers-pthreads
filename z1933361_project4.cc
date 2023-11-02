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

//global variables:
//shared string

void *writer(void *param)
{
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
    //get command line args

    if (argc != 2)
    {
        printf("Incorrent number of arguments");
        exit(EXIT_FAILURE);
    }

    //intialize shared string - global variable

    //initialize semaphores:
    //semaphore mutex=1, wrt=1
    sem_t rw_sem;
    sem_t cs_sem;

    //innitialize semaphores to 1
    sem_init(&rw_sem, 0, 1);
    sem_init(&cs_sem, 0, 1);
    //check return value for success

    //readcount=0

    //create reader and writer threads

    //wait for reader threads to finish
    // pthread_joing
    //wait for writer threads to finish

    //cleanup and exit



}