#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "hashing.h"
#include "md5.h"


void* no_threading(unsigned char* find)
{
  struct hash_arg_struct args;

  strcpy(args.password, "aaaaaa");//initialize password field
  strcpy(args.salt, "4fTgjp6q");  //initialize salt field
  
  unsigned char chars[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 
  'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int k = 0;
  unsigned int l = 0;
  unsigned int m = 0;
  unsigned int n = 0;

  unsigned char found = 0x00;
  unsigned char performanceMode = 0x00;

  unsigned int counterI = 0;
  unsigned int counterJ = 0;
  unsigned int counterK = 0;
  unsigned int counterL = 0;
  unsigned int counterM = 0;
  unsigned int counterN = 0;

  unsigned int pswds_checked = 0;

  //Timing code
  clock_t begin = clock();
  clock_t end;
  double elapsed_time;

  //Threading code
  pthread_t thread_ids[NUM_THREADS];

  while(pswds_checked < AMOUNT_PSWDS)
  {
    counterJ++;
    counterK++;
    counterL++;
    counterM++;
    counterN++;
    pswds_checked++;

    Hash(&args);

    //hash compare section
    if(strcmp(args.encoding, find) == 0)
    {
      found = 0x01;
      break;
    }
    
    //password incrementer section

    i = (i + 1) % 26;
    args.password[5] = chars[i];

    if((counterJ % 26) == 0)
    {
      j = (j + 1) % 26;
      args.password[4] = chars[j];
      counterJ = 0;
    }
    if((counterK % 676) == 0)
    {
      k = (k + 1) % 26;
      args.password[3] = chars[k];
      counterK = 0;
    }
    if((counterL % 17576) == 0)
    {
      l = (l + 1) % 26;
      args.password[2] = chars[l];
      counterL = 0;
    }
    if((counterM % 456976) == 0)
    {
      m = (m + 1) % 26;
      args.password[1] = chars[m];
      counterM = 0;
    }
    if((counterN % 11881376) == 0)
    {
      n = (n + 1) % 26;
      args.password[0] = chars[n];
      counterN = 0;
    }

    //performance code
    // if(((double)(clock() - begin)/CLOCKS_PER_SEC) > 1.0){
    //   performanceMode = 0x01;
    //   break;
    // }
  }

  elapsed_time = (double)(clock() - begin)/CLOCKS_PER_SEC;

  if(found)
  {
    printf("suceess!\n");
    printf("passwords checked: %d\n", pswds_checked);
    printf("time elapsed: %f seconds\n", elapsed_time);
    printf("password is: %s\n", args.password);
  }
  else if(performanceMode)
  {
    printf("passwords checked: %d\n", pswds_checked);
    printf("time elapsed: %f seconds\n", elapsed_time);
  }
  else
  {
    printf("password not found\n");
  }
}

/*
* In 4 thread config:
* will check range -> {aaaaaa, gggaaa}
*/
void* thread_one(void* info)
{
	struct thread_struct* tone_s = (struct thread_struct*)info;

	//init thread struct;
	tone_s->found = 0x00;
	tone_s->num_checked = 0;

  struct hash_arg_struct args;

  strcpy(args.password, "aaaaaa");//initialize password field(this is where
  																//the search will begin)
  strcpy(args.salt, "4fTgjp6q");  //initialize salt field
  
  unsigned char chars[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 
  'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

  //controls how each char in password gets updated
  //     [n][m][l][k][j][i]
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int k = 0;
  unsigned int l = 0;
  unsigned int m = 0;
  unsigned int n = 0;

  unsigned char found = 0x00;
  unsigned char performanceMode = 0x00;

  unsigned int counterI = 0;
  unsigned int counterJ = 0;
  unsigned int counterK = 0;
  unsigned int counterL = 0;
  unsigned int counterM = 0;
  unsigned int counterN = 0;

  //Timing code
  clock_t begin = clock();
  clock_t end;
  double elapsed_time;

  while(strcmp(args.password, "ggmaaa") != 0 && !global_found) //only checks upto gggaaa
  {
    //printf("password is: %s\n", args.password);
    // usleep(1000);
    counterJ++;
    counterK++;
    counterL++;
    counterM++;
    counterN++;
    tone_s->num_checked++;
    

    Hash(&args);

    //hash compare section
    if(strcmp(args.encoding, tone_s->find) == 0)
    {
      tone_s->found = 0x01;
      break;
    }

    //performance check
    // if(((clock() - begin) / (CLOCKS_PER_SEC * NUM_THREADS)) >= 1.0){
    // 	performanceMode = 0x01;
    // 	printf("entered\n");
    // 	break;
    // }
    
    //password incrementer section

    i = (i + 1) % 26;
    args.password[5] = chars[i];

    if((counterJ % 26) == 0)
    {
      j = (j + 1) % 26;
      args.password[4] = chars[j];
      counterJ = 0;
    }
    if((counterK % 676) == 0)
    {
      k = (k + 1) % 26;
      args.password[3] = chars[k];
      counterK = 0;
    }
    if((counterL % 17576) == 0)
    {
      l = (l + 1) % 26;
      args.password[2] = chars[l];
      counterL = 0;
    }
    if((counterM % 456976) == 0)
    {
      m = (m + 1) % 26;
      args.password[1] = chars[m];
      counterM = 0;
    }
    if((counterN % 11881376) == 0)
    {
      n = (n + 1) % 26;
      args.password[0] = chars[n];
      counterN = 0;
    }
  }

  elapsed_time = (double)(clock() - begin)/(CLOCKS_PER_SEC * NUM_THREADS);

  if(tone_s->found)
  {
    printf("suceess!\n");
    printf("passwords checked: %d\n", tone_s->num_checked);
    printf("time elapsed: %f seconds\n", elapsed_time);
    printf("password is: %s\n", args.password);
    global_found = 0x01;
  }
  else if(performanceMode)
  {
  	global_found = 0x01; //not actually found but setting this will stop other threads
    printf("passwords checked: %d\n", tone_s->num_checked);
    printf("time elapsed: %f seconds\n", elapsed_time);
  }
  else if(global_found)
  {
    pthread_exit(0);
  }
  else
  {
  	printf("password not found\n");
  }
  pthread_exit(0);
}

/*
* In 4 thread config:
* will check range -> {gghaaa, nnnaaa}
*/
void* thread_two(void* info)
{
	struct thread_struct* ttwo_s = (struct thread_struct*)info;

	ttwo_s->found = 0x00;
	ttwo_s->num_checked = 0;

  struct hash_arg_struct args;

  strcpy(args.password, "gghaaa");//initialize password field(this is where
  																//the search will begin)
  strcpy(args.salt, "4fTgjp6q");  //initialize salt field
  
  unsigned char chars[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 
  'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

  //controls how each char in password gets updated
  //     [n][m][l][k][j][i]
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int k = 0;
  unsigned int l = 7;
  unsigned int m = 6;
  unsigned int n = 6;

  unsigned char performanceMode = 0x00;

  unsigned int counterI = 0;
  unsigned int counterJ = 0;
  unsigned int counterK = 0;
  unsigned int counterL = 7;
  unsigned int counterM = 6;
  unsigned int counterN = 6;


  //Timing code
  clock_t begin = clock();
  clock_t end;
  double elapsed_time;


  while(strcmp(args.password, "nnoaaa") != 0 && !global_found)//only checks upto nnnaaa
  {
    //printf("password is: %s\n", args.password);
    // usleep(1000);
    counterJ++;
    counterK++;
    counterL++;
    counterM++;
    counterN++;
    ttwo_s->num_checked++;

    Hash(&args);

    //hash compare section
    if(strcmp(args.encoding, ttwo_s->find) == 0)
    {
      ttwo_s->found = 0x01;
      break;
    }
    
    //password incrementer section

    i = (i + 1) % 26;
    args.password[5] = chars[i];

    if((counterJ % 26) == 0)
    {
      j = (j + 1) % 26;
      args.password[4] = chars[j];
      counterJ = 0;
    }
    if((counterK % 676) == 0)
    {
      k = (k + 1) % 26;
      args.password[3] = chars[k];
      counterK = 0;
    }
    if((counterL % 17576) == 0)
    {
      l = (l + 1) % 26;
      args.password[2] = chars[l];
      counterL = 0;
    }
    if((counterM % 456976) == 0)
    {
      m = (m + 1) % 26;
      args.password[1] = chars[m];
      counterM = 0;
    }
    if((counterN % 11881376) == 0)
    {
      n = (n + 1) % 26;
      args.password[0] = chars[n];
      counterN = 0;
    }
  }

  elapsed_time = (double)(clock() - begin)/(CLOCKS_PER_SEC * NUM_THREADS);

  if(ttwo_s->found)
  {
    printf("suceess!\n");
    printf("passwords checked: %d\n", ttwo_s->num_checked);
    printf("time elapsed: %f seconds\n", elapsed_time);
    printf("password is: %s\n", args.password);
    global_found = 0x01;
  }
  else if(performanceMode)
  {
    printf("passwords checked: %d\n", ttwo_s->num_checked);
    printf("time elapsed: %f seconds\n", elapsed_time);
  }
  else if(global_found)
  {
    pthread_exit(0);
  }
  else
  {
  	printf("password not found\n");
  }
  pthread_exit(0);
}

/*
* In 4 thread config:
* will check range -> {nnoaaa, tttaaa}
*/
void* thread_three(void* info)
{
	struct thread_struct* tthree_s = (struct thread_struct*)info;

	tthree_s->found = 0x00;
	tthree_s->num_checked = 0;

  struct hash_arg_struct args;

  strcpy(args.password, "nnoaaa");//initialize password field(this is where
  																//the search will begin)
  strcpy(args.salt, "4fTgjp6q");  //initialize salt field
  
  unsigned char chars[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 
  'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

  //controls how each char in password gets updated
  //     [n][m][l][k][j][i]
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int k = 0;
  unsigned int l = 14;
  unsigned int m = 13;
  unsigned int n = 13;

  unsigned char performanceMode = 0x00;

  unsigned int counterI = 0;
  unsigned int counterJ = 0;
  unsigned int counterK = 0;
  unsigned int counterL = 14;
  unsigned int counterM = 13;
  unsigned int counterN = 13;


  //Timing code
  clock_t begin = clock();
  clock_t end;
  double elapsed_time;


  while(strcmp(args.password, "ttuaaa") != 0 && !global_found)//only checks upto tttaaa
  {
    //printf("password is: %s\n", args.password);
    // usleep(1000);
    counterJ++;
    counterK++;
    counterL++;
    counterM++;
    counterN++;
    tthree_s->num_checked++;

    Hash(&args);

    //hash compare section
    if(strcmp(args.encoding, tthree_s->find) == 0)
    {
      tthree_s->found = 0x01;
      break;
    }
    
    //password incrementer section

    i = (i + 1) % 26;
    args.password[5] = chars[i];

    if((counterJ % 26) == 0)
    {
      j = (j + 1) % 26;
      args.password[4] = chars[j];
      counterJ = 0;
    }
    if((counterK % 676) == 0)
    {
      k = (k + 1) % 26;
      args.password[3] = chars[k];
      counterK = 0;
    }
    if((counterL % 17576) == 0)
    {
      l = (l + 1) % 26;
      args.password[2] = chars[l];
      counterL = 0;
    }
    if((counterM % 456976) == 0)
    {
      m = (m + 1) % 26;
      args.password[1] = chars[m];
      counterM = 0;
    }
    if((counterN % 11881376) == 0)
    {
      n = (n + 1) % 26;
      args.password[0] = chars[n];
      counterN = 0;
    }
  }

  elapsed_time = (double)(clock() - begin)/(CLOCKS_PER_SEC * NUM_THREADS);

  if(tthree_s->found)
  {
    printf("suceess!\n");
    printf("passwords checked: %d\n", tthree_s->num_checked);
    printf("time elapsed: %f seconds\n", elapsed_time);
    printf("password is: %s\n", args.password);
    global_found = 0x01;
  }
  else if(performanceMode)
  {
    printf("passwords checked: %d\n", tthree_s->num_checked);
    printf("time elapsed: %f seconds\n", elapsed_time);
  }
  else if(global_found)
  {
    pthread_exit(0);
  }
  else
  {
  	printf("password not found\n");
  }
  pthread_exit(0);
}


/*
* In 4 thread config:
* will check range -> {ttuaaa, zzzzzz}
*/
void* thread_four(void* info)
{
	struct thread_struct* tfour_s = (struct thread_struct*)info;

	tfour_s->found = 0x00;
	tfour_s->num_checked = 0;

  struct hash_arg_struct args;

  strcpy(args.password, "ttuaaa");//initialize password field(this is where
  																//the search will begin)
  strcpy(args.salt, "4fTgjp6q");  //initialize salt field
  
  unsigned char chars[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 
  'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

  //controls how each char in password gets updated
  //     [n][m][l][k][j][i]
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int k = 0;
  unsigned int l = 20;
  unsigned int m = 19;
  unsigned int n = 19;

  unsigned char performanceMode = 0x00;

  unsigned int counterI = 0;
  unsigned int counterJ = 0;
  unsigned int counterK = 0;
  unsigned int counterL = 20;
  unsigned int counterM = 19;
  unsigned int counterN = 19;


  //Timing code
  clock_t begin = clock();
  clock_t end;
  double elapsed_time;


  while(strcmp(args.password, "zzzzzz") != 0 && !global_found)//only checks upto 
  {
    //printf("password is: %s\n", args.password);
    // usleep(1000);
    counterJ++;
    counterK++;
    counterL++;
    counterM++;
    counterN++;
    tfour_s->num_checked++;

    Hash(&args);

    //hash compare section
    if(strcmp(args.encoding, tfour_s->find) == 0)
    {
      tfour_s->found = 0x01;
      break;
    }
    
    //password incrementer section

    i = (i + 1) % 26;
    args.password[5] = chars[i];

    if((counterJ % 26) == 0)
    {
      j = (j + 1) % 26;
      args.password[4] = chars[j];
      counterJ = 0;
    }
    if((counterK % 676) == 0)
    {
      k = (k + 1) % 26;
      args.password[3] = chars[k];
      counterK = 0;
    }
    if((counterL % 17576) == 0)
    {
      l = (l + 1) % 26;
      args.password[2] = chars[l];
      counterL = 0;
    }
    if((counterM % 456976) == 0)
    {
      m = (m + 1) % 26;
      args.password[1] = chars[m];
      counterM = 0;
    }
    if((counterN % 11881376) == 0)
    {
      n = (n + 1) % 26;
      args.password[0] = chars[n];
      counterN = 0;
    }
  }

  elapsed_time = (double)(clock() - begin)/(CLOCKS_PER_SEC * NUM_THREADS);

  if(tfour_s->found)
  {
    printf("suceess!\n");
    printf("passwords checked by thread: %d\n", tfour_s->num_checked);
    printf("time elapsed: %f seconds\n", elapsed_time);
    printf("password is: %s\n", args.password);
    global_found = 0x01;
  }
  else if(performanceMode)
  {
    printf("passwords checked: %d\n", tfour_s->num_checked);
    printf("time elapsed: %f seconds\n", elapsed_time);
  }
  else if(global_found)
  {
    pthread_exit(0);
  }
  else
  {
  	printf("password not found\n");
  }
  pthread_exit(0);
}