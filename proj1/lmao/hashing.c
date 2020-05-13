#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "hashing.h"
#include "md5.h"




unsigned char* md5(unsigned char *data, int size){
   unsigned char *result = malloc(16 * sizeof(char));

   strcpy(result, "");

   MD5_CTX md5;
   MD5_Init(&md5);
   MD5_Update(&md5, data, size);
   MD5_Final(result, &md5);

   //strcpy((char*)data, result);
   memcpy(&data[0], result, HASH_SIZE);

   free(result);

   return (unsigned char*)data;
}

void hex(unsigned char* digest, int len)
{
  char buf[100];
  for (int i=0; i<len; i++)
  {
    sprintf(buf+i*2, "%02x", digest[i]);
  }

  for(int i = 0; i < (len*2)-1; i+=2)
  {
    printf("%c%c:", buf[i], buf[i + 1]);
  }
  printf("\n\n");
  buf[100]=0;
}

//Takes in string with binary representation of password length
//Processes binary string and returns string to be appended to intermZero
//per step 3(5) of initialization
//Parameters: binary (char pointer to binary rep)
//            firstChar (first character of password)
//
//Returns:    char pointer to string to append (saved in temp variable)
//            count (number of bytes appended)
void create_string(unsigned char* binary, char firstChar, unsigned char* temp,
  int* append_len)
{
  const int BINARY_LEN = strlen(binary);
  int len = 0;
  int pos = 0;
  
  //find position of first set bit (left to right / index 0 to (len -1))
  for(int i = 0; i < BINARY_LEN; i++)
  {
    if(binary[i] == '1')
    {
      pos = i;
      break;
    }
  }

  //append null byte or first char of password accordingly
  //do so only from BINARY_LEN -1 of binary to pos
  for(int i = BINARY_LEN - 1; i >= pos; i--, len++)
  {
    switch(binary[i])
    {
      case '1':
        //append null byte
        temp[len] = '\0';
        break;
      case '0':
        //append first char of password
        temp[len] = firstChar;
        break;
      default:
        printf("ERROR! ABORT MISSION! (unexpected case in create_string() switch)");
    }
  }
  *append_len = len;
}

//called by two functions
//one function requires 4-bit binary representation
//other function requires 8-bit binary representation
//must define wich case you want (1 or 2)
void dec_to_bin(const int NUM, unsigned char* binary, int option)
{
  int i = 0;

  switch(option){
    case FOUR_BIT_REP:
      //NUM_BITS is the number of bits that will be used to represent our password length
      for(i = NUM_BITS_1 - 1; i >= 0; i--)
      {
        if((NUM & (1 << i)) != 0)
           strcat(binary, "1");
        else
           strcat(binary, "0");
      }
      break;

    case EIGHT_BIT_REP:

      //NUM_BITS is the number of bits that will be used to represent our password length
      for(i = NUM_BITS_2 - 1; i >= 0; i--)
      {
        if((NUM & (1 << i)) != 0)
           strcat(binary, "1");
        else
           strcat(binary, "0");
      }
      break;

    default:
      printf("ERROR: invalid binary option chosen\n");
  }
}

int bin_to_dec(unsigned char* binNum)
{
  int result = 0;
  int binSize = strlen(binNum);
  int i = 0, j = binSize - 1;
  for(i = 0; i < binSize; i++, j--)
  {
    if(binNum[i] == '1')
    {
       result += pow(2, j);
    }
  }

  return result;
}


//=========================================================================
//Initialization step of hashing
//Parameters: password, salt, magic
//Returns: altsum and intermZero
void initialize(unsigned char* password, unsigned char* salt,
   unsigned char* magic, unsigned char* altsum, unsigned char* intermZero)
{
  const int PSWD_LEN = strlen(password);
  const int SALT_LEN = strlen(salt);
  unsigned char* temp = malloc(sizeof(*temp));
  unsigned char* binary = malloc((sizeof(char) * 4) + 1);
  int currSize = 0;
  int append_len; //var used to append string to intermZero

  strcpy(temp, "");
  strcpy(binary, "");

  //compute the hash for alternate sum
  strcat(altsum, password);
  strcat(altsum, salt);
  strcat(altsum, password);

  currSize += (2 * PSWD_LEN) + SALT_LEN;

  memcpy(altsum, md5(altsum, currSize), HASH_SIZE);

  //compute the hash for intermediate zero
  currSize = 0;
  strcat(intermZero, password);
  strcat(intermZero, magic);
  strcat(intermZero, salt);

  currSize += strlen(intermZero);

  memcpy(&intermZero[currSize], altsum, PSWD_LEN); //we know that the password
                                                   //will never be longer than
                                                   //altsum (16 bytes) (step 3(4))
  currSize += PSWD_LEN;

  //convert password length number to binary and get string to append to intermZero
  dec_to_bin(PSWD_LEN, binary, FOUR_BIT_REP);
  create_string(binary, password[0], temp, &append_len);


  memcpy(&intermZero[currSize], temp, append_len);
  currSize += append_len;

  memcpy(intermZero, md5(intermZero, currSize), HASH_SIZE);

  free(temp);
  free(binary);
}

//Hashing step
//Parameters: password, salt, intermZero
//Returns: finalHash
void hash_loop(unsigned char* password, unsigned char* salt, 
  unsigned char* intermZero, unsigned char* finalHash)
{
  const int PSWD_LEN = strlen(password);
  const int SALT_LEN = strlen(salt);
  unsigned char* intermI = malloc(128);
  unsigned char* temp    = malloc(128);
  int currSize = 0;

  strcpy(intermI, "");
  strcpy(temp, "");

  for(int i = 0; i < NUM_HASH_ITERATIONS; i++)
  {
    //Reset variables for this iteration
    currSize = 0;

    //concat phase
    if((i % 2) == 0) //if i is ever, concat intermI
    {
      if(i == 0)
      {
        memcpy(&temp[currSize], intermZero, HASH_SIZE);
        currSize += HASH_SIZE;
      }
      else
      {
        memcpy(&temp[currSize], intermI, HASH_SIZE);
        currSize += HASH_SIZE;
      }
    }
    if((i % 2) != 0) //if i is odd, concat password
    {
      memcpy(&temp[currSize], password, PSWD_LEN);
      currSize += PSWD_LEN;
   }
    if((i % 3) != 0) //if i is not divisible by, concat salt
    {
      memcpy(&temp[currSize], salt, SALT_LEN);
      currSize += SALT_LEN;
    }
    if((i % 7) != 0) //if i is not divisible by 7, concat password
    {
      memcpy(&temp[currSize], password, PSWD_LEN);
      currSize += PSWD_LEN;
    }
    if((i % 2) == 0) //if i is even, concat password
    {
      if(i == 0)
      {
        memcpy(&temp[currSize], password, PSWD_LEN);
        currSize += PSWD_LEN;
      }
      else{
        memcpy(&temp[currSize], password, PSWD_LEN);
        currSize += PSWD_LEN; 
      }
    }
    if((i % 2) != 0) //if is odd, concat intermI
    {
      memcpy(&temp[currSize], intermI, HASH_SIZE);
      currSize += HASH_SIZE;
    }

    //hash phase
    memcpy(intermI, md5(temp, currSize), HASH_SIZE); //hash and store into intermI
  }

  memcpy(finalHash, intermI, HASH_SIZE);

  free(intermI);
  free(temp);
}

//Encode finalHash
//Paramater: finalHash, 
//Returns: encoding
void encode(unsigned char* finalHash, unsigned char* encoding)
{
  unsigned char* temp = malloc(128);
  unsigned char* binary = malloc((sizeof(char) * 8) + 1);
  unsigned char* binString = malloc((sizeof(char) * 128) + 1);
  int binStringLen = 128;
  unsigned char cryptEncoding[64] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  int z;

  strcpy(temp, "");
  strcpy(binary, "");
  strcpy(binString, "");
  strcpy(encoding, "");

  //first re-arrange bytes in finalHash
  int order[] = {11, 4, 10, 5, 3, 9, 15, 2, 8, 14, 1, 7, 13, 0, 6, 12};
  for(int i = 0; i < HASH_SIZE; ++i)
  {
    temp[i] = finalHash[order[i]];
  }
  memcpy(finalHash, temp, HASH_SIZE);
  temp[0] = '\0';

  //convert characters in final hash to their binary form and
  //save into binString
  for(int i = 0; i < HASH_SIZE; i++){
    dec_to_bin(finalHash[i], binary, EIGHT_BIT_REP);
    strcat(binString, binary);
    binary[0] = '\0';
  }

  //get encoding for the first two bits
  for(int i = 0; i < 2; i++)
  {
    temp[i] = binString[i];
  }
  temp[2] = '\0';
  memcpy(binString, &binString[2], binStringLen - 2); //deletes first two chars (bytes)
  encoding[ENCODING_IT] = cryptEncoding[bin_to_dec(temp)];


  //get encoding for the remaining 21 groups
  z = ENCODING_IT - 1;
  for(int i = 0; i < ENCODING_IT; i++, z--)
  {
    temp[0] = '\0';
    for(int j = 0; j < 6; j++)
    {
      temp[j] = binString[j];
    }
    temp[6] = '\0';
    memcpy(binString, &binString[6], binStringLen - 6); //deletes first 6 chars (bytes 0-5)
    encoding[z] = cryptEncoding[bin_to_dec(temp)];
  }
  encoding[22] = '\0';

  free(temp);
  free(binary);
  free(binString);
}

//=================================================================

void *Hash(void* arguments)
{
  struct hash_arg_struct *args = (struct hash_arg_struct*)arguments;

//printf("this is the password: %s\n", args->password);
  unsigned char* altsum     = malloc((sizeof(char) * 15) + 1);
  unsigned char* intermZero = malloc(sizeof(char) * 30);
  unsigned char* finalHash  = malloc((sizeof(char) * 22 + 1));
  unsigned char* magic      = malloc((sizeof(char) * 3) + 1);//"$1$";
  strcpy(magic, "$1$");

  // strcpy(password, args->password);
  // strcpy(salt, )

  strcpy(altsum, "");
  strcpy(intermZero, "");
  strcpy(finalHash, "");
  strcpy(args->encoding, "");

  //Init
  initialize(args->password, args->salt, magic, altsum, intermZero);
  //Hashing
  hash_loop(args->password, args->salt, intermZero, finalHash);
  //Encoding
  encode(finalHash, args->encoding);
}