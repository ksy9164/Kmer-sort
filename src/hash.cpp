#include "hash.h"

uint64_t jshash(const char* str, uint32_t len) {
    uint64_t hash = 1315423911;
    uint64_t i = 0;

    for ( i = 0; i < len; i++) {
        hash ^= ((hash<<5) + str[i] + (hash>>2));
    }
    return hash;
}

uint64_t elfhash(const char* str, uint32_t length)
{
    uint64_t hash = 0;
    uint64_t x    = 0;
    uint64_t i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash << 4) + (*str);

        //if ((x = hash & 0xF0000000L) != 0)
        if ((x = hash & 0xF000000000000000L) != 0)
        {
            hash ^= (x >> 24);
        }

        hash &= ~x;
    }

    return hash;
}
uint64_t sdbmhash(const char* str, uint64_t length)
{
    uint64_t hash = 0;
    uint64_t i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}
uint64_t djbhash(const char* str, uint64_t length)
{
    uint64_t hash = 5381;
    uint64_t i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = ((hash << 5) + hash) + (*str);
    }

    return hash;
}
uint64_t dekhash(const char* str, uint64_t length)
{
    uint64_t hash = length;
    uint64_t i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
    }

    return hash;
}
unsigned int rshash(const char* str, unsigned int length)
{
   unsigned int b    = 378551;
   unsigned int a    = 63689;
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = hash * a + (*str);
      a    = a * b;
   }

   return hash;
}
unsigned int pjwhash(const char* str, unsigned int length)
{
   const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
   const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
   const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
   const unsigned int HighBits          =
                      (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
   unsigned int hash = 0;
   unsigned int test = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (hash << OneEighth) + (*str);

      if ((test = hash & HighBits) != 0)
      {
         hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
      }
   }

   return hash;
}
unsigned int bkdrhash(const char* str, unsigned int length)
{
   unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = (hash * seed) + (*str);
   }

   return hash;
}
uint64_t hashfunction(const char* str, uint32_t len, int functionid) {
    switch (functionid) {
        case 0: return jshash(str, len);
        case 1: return elfhash(str,len);
        case 2: return sdbmhash(str,len);
        case 3: return djbhash(str,len);
        case 4: return dekhash(str,len);
        case 5: return rshash(str,len);
        case 6: return pjwhash(str,len);
        case 7: return bkdrhash(str,len);
        default:
                printf( "Hash function %d not defined. Using 0...\n", functionid );
                return jshash(str, len);
    }
}
