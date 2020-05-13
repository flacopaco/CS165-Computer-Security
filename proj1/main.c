#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "hash.h"

int main(void) {
  global_found = 0x00;

  pthread_t threadOne_id;
  pthread_t threadTwo_id;
  pthread_t threadThree_id;
  pthread_t threadFour_id;

  struct thread_struct tone_s;
  struct thread_struct ttwo_s;
  struct thread_struct tthree_s;
  struct thread_struct tfour_s;

  strcpy(tone_s.find, "r07.glxwJV9.elvf5FoCm/");
  strcpy(ttwo_s.find, "r07.glxwJV9.elvf5FoCm/");
  strcpy(tthree_s.find, "r07.glxwJV9.elvf5FoCm/");
  strcpy(tfour_s.find, "r07.glxwJV9.elvf5FoCm/");

  pthread_create(&threadOne_id, NULL, thread_one, &tone_s);
  pthread_create(&threadTwo_id, NULL, thread_two, &ttwo_s);
  pthread_create(&threadThree_id, NULL, thread_three, &tthree_s);
  pthread_create(&threadFour_id, NULL, thread_four, &tfour_s);

  pthread_join(threadOne_id, NULL);
  pthread_join(threadTwo_id, NULL);
  pthread_join(threadThree_id, NULL);
  pthread_join(threadFour_id, NULL);

  printf("total number of passwords checked: %d\n", tone_s.num_checked + ttwo_s.num_checked + tthree_s.num_checked + tfour_s.num_checked);

  return 0;
}