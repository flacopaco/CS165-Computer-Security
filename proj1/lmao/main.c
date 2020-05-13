/*
Steps for program taken from exlplanation in the following blog:
 https://www.vidarholen.net/contents/blog/?p=32
*/
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "hashing.h"


int main(void)
{
  //init global var
  global_found = 0x00;

  //pthread ids
  pthread_t threadOne_id;
  pthread_t threadTwo_id;
  pthread_t threadThree_id;
  pthread_t threadFour_id;


  //pthread structs
  struct thread_struct tone_s;
  struct thread_struct ttwo_s;
  struct thread_struct tthree_s;
  struct thread_struct tfour_s;

  strcpy(tone_s.find, "v.2IIvLpQ.L5X5OPxZhMB/");
  strcpy(ttwo_s.find, "v.2IIvLpQ.L5X5OPxZhMB/");
  strcpy(tthree_s.find, "v.2IIvLpQ.L5X5OPxZhMB/");
  strcpy(tfour_s.find, "v.2IIvLpQ.L5X5OPxZhMB/");

  pthread_create(&threadOne_id, NULL, thread_one, &tone_s);
  pthread_create(&threadTwo_id, NULL, thread_two, &ttwo_s);
  pthread_create(&threadThree_id, NULL, thread_three, &tthree_s);
  pthread_create(&threadFour_id, NULL, thread_four, &tfour_s);

  pthread_join(threadOne_id, NULL);
  pthread_join(threadTwo_id, NULL);
  pthread_join(threadThree_id, NULL);
  pthread_join(threadFour_id, NULL);

  printf("total passwords checked: %d\n", tone_s.num_checked + ttwo_s.num_checked
                                        + tthree_s.num_checked + tfour_s.num_checked);

  return 0;
}