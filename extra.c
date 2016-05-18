/*
 Author: Dafu Ai 766586
*/
#include "extra.h"
#include "hash.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

#define MAXSTRLEN 256 /* Same as the one defined in hash.c */
#define HASH_TARGET 0 /* We want strings to be hashed to this number */
#define TRIAL_LEN 10 /* Length of trial key for collide_dumb */

/*
Acknowledgement: 
isprime is a modified from the program
orginally written by Alistair Moffat 
http://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/isprime.c

Returns whether the input number is a prime. 
*/
bool isprime(unsigned int n) {
	unsigned int isprime, divisor;
	isprime = 1;
	for (divisor=2; divisor*divisor<=n; divisor++) {
		if (n % divisor == 0) {
			isprime = 0;
			break;
		}
	}
	if (isprime == 1) {
		return true;
	} else {
		return false;
	}
}

/* Determine appropriate size of a hash table given input size n */
unsigned int determine_size(unsigned int n) {
	unsigned int size = 2*n+1;

	while (size <= UINT_MAX){
		if(isprime(size) == true){
		/* Found first prime after 2n+1 */
			return(size);
		}
		size++;
	}
	
	return UINT_MAX; /* Max possible size of the table */
}

/* Print n strings that are hashed to 0 by universal_hash seeded with seed */
void collide_dumb(unsigned int size, unsigned int seed, int n) {
	int i;
	int progress = 0;
	unsigned char uchar;
	unsigned char trial[TRIAL_LEN];
	unsigned int hash_result = HASH_TARGET + 1;
	static int r[MAXSTRLEN];
	static int flag = 1;

	/* number of r[i] values to appear */
	fprintf(stdout, "%d\n", TRIAL_LEN);
	
	/* Do the same staff with r as in universal hash*/
	if (flag == 1){
		for (i=0; i<TRIAL_LEN; i++) {
			r[i] = rand()%size;
			fprintf(stdout, "%d\n", r[i]);
		}
		flag++;
	}
	int a = 0;
	while (progress < n) {
	/* Not enough strings */
		while (hash_result != HASH_TARGET) {
			/* Generate random strings of fixed length*/
			for (i=0; i<TRIAL_LEN; i++) {
				uchar = rand()%UCHAR_MAX;
				trial[i] = uchar;
			}
			a++;
			/* Get hashed value */
			hash_result = universal_hash(trial, size);
		}
		/* Reset result to a non target number*/
		hash_result = HASH_TARGET + 1;
		/* A string which hases to 0 has been generated */
		progress++;
		
		if (progress == n){
		/* Last line */
			fprintf(stdout, "%s", trial);
		} else {
			fprintf(stdout, "%s\n", trial);
		}	
	}
}


/* Print n strings that are hashed to 0 by universal_hash seeded with seed */
void collide_clever(unsigned int size, unsigned int seed, int n) {
	static int r[MAXSTRLEN];
	static int flag = 1;
	int i;
	int progress = 0;
	/* Length of the key to be generated */
	int key_len = 1; 
	int multiple = 1;
	/* Value of the uchar to be checked */
	unsigned int val;
	/* List of keys to be out put*/
	unsigned char **keys;
	keys = malloc(sizeof(unsigned char*)*n);
	assert(keys);
	
	/* Do the same staff with r as in universal hash*/
	if (flag == 1){
		for (i=0; i<MAXSTRLEN; i++) {
			r[i] = rand()%size;
		}
		flag++;
	}

	while (progress < n && key_len < MAXSTRLEN) {
	/* Not enough strings */
		val = HASH_TARGET + size*multiple;
		if (val <= UCHAR_MAX) {
		/* uchar value in correct range */
			keys[progress] = malloc(sizeof(unsigned char)*(key_len+1));
			for (i=0; i<key_len-1; i++) {
				/* Set all uchars (except for the last one) to be size */
				keys[progress][i] = size;
			}	
			keys[progress][key_len-1] = val;
			/* A string which hashes to 0 has been generated */
			progress++;
			/* Check for next multiple */
			multiple++;
			
		} else {
		    /* uchar not in range. increase the length of the key. */	
			key_len++;
			/* Reset multiple */
			multiple = 1;
		}
	}
	
	/* Output */
	/*fprintf(stdout, "%d\n", key_len);
	for (i=0; i<key_len; i++) {
		fprintf(stdout, "%d\n", r[i]);
	}
	*/
	for (i=0; i<n-1; i++) {
		fprintf(stdout, "%s\n", keys[i]);
	}
	/* Last line */
	fprintf(stdout, "%s", keys[n-1]);
}