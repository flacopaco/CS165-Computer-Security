#ifndef HASHING_H
#define HASHING_H

#define NUM_HASH_ITERATIONS 1000
#define HASH_SIZE 16
#define ENCODING_IT 21 //22 groups needed must start at last index (22 - 1)
#define NUM_BITS_1 4
#define NUM_BITS_2 8
#define FOUR_BIT_REP 1
#define EIGHT_BIT_REP 2
#define AMOUNT_PSWDS (26*26*26*26*26*26) + 1

#define NUM_THREADS 4


//global variables
unsigned char global_found;

unsigned char* md5(unsigned char *data, int size);

void hex(unsigned char* digest, int len);

void create_string(unsigned char* binary, char firstChar, unsigned char* temp,
  int* append_len);

void dec_to_bin(const int NUM, unsigned char* binary, int option);
int bin_to_dec(unsigned char* binNum);

void initialize(unsigned char* password, unsigned char* salt,
   unsigned char* magic, unsigned char* altsum, unsigned char* intermZero);

void hash_loop(unsigned char* password, unsigned char* salt, 
  unsigned char* intermZero, unsigned char* finalHash);

void encode(unsigned char* finalHash, unsigned char* encoding);

void *Hash(void *args);

void* no_threading(unsigned char* find);

void* thread_one(void* find);

void* thread_two(void* find);

void* thread_three();

void* thread_four();

struct hash_arg_struct {
	//we know the expected size for each of the fields
	//+1 for the NULL byte at the end
	unsigned char password[7]; 
	unsigned char salt[9];
	unsigned char encoding[23];
};

struct thread_struct {
	unsigned char find[23];
	unsigned char found;
	int num_checked;
};

#endif