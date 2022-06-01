#ifndef _HASH_
#define _HASH_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include <pthread.h>
#include <chrono>
#include <unistd.h>
#include <vector>
#include <iostream>

uint64_t jshash(const char* str, uint32_t len);
uint64_t elfhash(const char* str, uint32_t length);
uint64_t sdbmhash(const char* str, uint64_t length);
uint64_t djbhash(const char* str, uint64_t length);
uint64_t dekhash(const char* str, uint64_t length);
unsigned int rshash(const char* str, unsigned int length);
unsigned int pjwhash(const char* str, unsigned int length);
unsigned int bkdrhash(const char* str, unsigned int length);
uint64_t hashfunction(const char* str, uint32_t len, int functionid);

#endif
