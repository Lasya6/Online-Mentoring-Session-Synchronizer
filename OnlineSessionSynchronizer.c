
/**
* ** ONLINE MENTORING SESSION SYNCHRONIZER **
*
**/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STUDENTS 20

// Function prototypes
void *student(void *num);
void *mentor(void *);
void randwait(int secs);

// Declaration of required semaphores
sem_t studyRoom;
sem_t mentorchair;
sem_t mentorlaptop;
sem_t sign;

// A variable to keep track of a student who has attended session.
int completed = 0;

/** DRIVER CODE **/
int main(int argc, char *argv[]) {

/*pthread_t is the data type used to uniquely identify a thread*/
pthread_t m1;
pthread_t s[MAX_STUDENTS];

// Declaration of required Variables
long RandSeed;
int i, nStudents;
int Number[MAX_STUDENTS];

printf("Enter the number of Students : ");
scanf("%d",&nStudents) ;

if (nStudents > MAX_STUDENTS) {
printf("SORRY, NO MENTORING IS PROVIDED!\n.");
printf("Maximum number of Students we can provide mentoring is %d.\n",
MAX_STUDENTS);
exit(-1);
}

// Initializing Number array with index
for (i=0; i<MAX_STUDENTS; i++) {
Number[i] = i;
}

// Initializing required semaphores
sem_init(&studyRoom, 0, MAX_STUDENTS); //Counting semaphore
sem_init(&mentorchair, 0, 1);
sem_init(&mentorlaptop, 0, 0);
sem_init(&sign, 0, 0);

// Creating a mentor thread
pthread_create (&m1, NULL, mentor, NULL);

// Creating student threads.
for (i=0; i<nStudents; i++) {
pthread_create(&s[i], NULL, student , (void *)&Number[i]);
sleep(1);
}

for (i=0; i<nStudents; i++) {
pthread_join(s[i],NULL);
sleep(1);
}

completed = 1;
// Initially mentor is working on his laptop.
sem_post(&mentorlaptop);
pthread_join(m1,NULL);
}
/** END OF MAIN FUNCTION **/

/** STUDENT THREAD IMPLEMENTATION **/
void *student(void *number) {
int num = *(int *)number;
printf("Student %d arrived! \n", num);

// A signal to indicate that student entered room.
sem_wait(&studyRoom);
printf("Student %d entered study room.\n", num);

// A signal that mentor started teaching.
sem_wait(&mentorchair);
sem_post(&studyRoom);//Count- No of students in studyroom.

printf("Student %d disturbing the mentor.\n", num);
// A signal that he is keept laptop aside and ready to teach.
sem_post(&mentorlaptop);

// Wait for the mentor to finish teaching.

//Finished teaching.
// After teaching to keep track of student, using variable sign.
sem_wait(&sign);
printf("Student %d leaving studyroom.\n", num);
sem_post(&mentorchair);

}

/** MENTOR IMPLEMENTATION **/
void *mentor(void *temp) {

while (!completed) {

// Work until student arrives and disturbs mentor.

printf("Mentor is working on his laptop\n");
sem_wait(&mentorlaptop);

if (!completed) {

//Random amount of time to teach!

printf("The mentor is teaching\n");
randwait(2);

printf("The mentor finished teaching\n");

sem_post(&sign);
}

else {
printf("After completion of teaching of all students, mentor is leaving for the
day!!\n");
/* Producing sound*/
for(int i=0;i<50;i++){
printf("\a");
}
}

}
}

/** FUNCTION TO GENERATE RANDOM NUMBER **/
void randwait(int secs) {
int timer;
// Random number generation
timer = (int) (secs + 1);
sleep(timer);
}