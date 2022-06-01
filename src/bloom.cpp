#include "hash.h"
#include <inttypes.h>
using namespace std;

#define BUCKET_NUM 256
#define DATA_SIZE 1024 * 1024 * 1024
#define DATA_NUM DATA_SIZE / 8
#define TABLE_SIZE 1024 * 1024 * 256
#define F_NAME "./data/data.dat"

int h_cnt[BUCKET_NUM];

void direct_set(uint64_t* table, uint64_t nh)
{
    uint64_t internalmask = (1<<6)-1;
    uint64_t wordoff = nh >> 6;
    uint64_t internaloff = nh & internalmask;

    table[wordoff] |= ( 1<<internaloff);
}

int main(int argc, char** argv) {
    int kcnt = 31;
    int hashcnt = 8;
    int i, j;
    uint64_t tablebits = 31;
    uint64_t hashmask = (1L<<tablebits)-1;

    uint64_t *data = (uint64_t*)malloc(sizeof(uint64_t) * DATA_NUM);
    uint64_t *table = (uint64_t*)malloc(TABLE_SIZE);

    FILE* fp = fopen(F_NAME, "r");
    for (int i = 0; i < DATA_NUM; ++i) {
        fscanf(fp, "%"SCNu64 , &data[i]);
    }
    struct timespec start, mid, finish;
    double elapsed;
    uint64_t kmer = 0;
    uint64_t kmask = (1<<kcnt)-1;
    int thread_cnt = atoi(argv[1]);
    int mode = atoi(argv[2dd]);

    if (mode == 0) {
        printf("Normal version ! \n");
        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int i = 0; i < DATA_NUM; ++i) {
            uint64_t temp = data[i];
            for (int j = 0; j < hashcnt; ++j) {
                char* wordstr = (char*)&data[i];
                uint64_t nh = hashfunction(wordstr, (kcnt+7)/8, j); // little endian, so this works
                nh = nh & hashmask;

                direct_set(table, nh);
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Original consumes %f\n", elapsed);
        fflush(stdout);
    }
    else {
        uint64_t **hashbuffer = (uint64_t**)malloc(sizeof(uint64_t*) * BUCKET_NUM);

        for (int i = 0; i < BUCKET_NUM; ++i)
            hashbuffer[i] = (uint64_t*)malloc(sizeof(uint64_t) * DATA_NUM / 12);

        printf("New version %d ! \n", thread_cnt);
        clock_gettime(CLOCK_MONOTONIC, &start);

#pragma omp parallel for num_threads(thread_cnt)
        for (int i = 0; i < DATA_NUM; ++i) {
            uint64_t temp = data[i];
            for (int j = 0; j < hashcnt; ++j) {
                char* wordstr = (char*)&data[i];
                uint64_t nh = hashfunction(wordstr, (kcnt+7)/8, j); // little endian, so this works
                nh = nh & hashmask;
                int idx = (int)nh % BUCKET_NUM;

                hashbuffer[idx][h_cnt[idx]++] = nh;
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &mid);
        elapsed = (mid.tv_sec - start.tv_sec);
        elapsed += (mid.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Hash consumes %f\n", elapsed);
        fflush(stdout);

        for (i = 0; i < BUCKET_NUM; ++i) {
#pragma omp parallel for num_threads(thread_cnt)
            for (j = 0; j < h_cnt[i]; ++j) {
                direct_set(table, hashbuffer[i][j]);
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - mid.tv_sec);
        elapsed += (finish.tv_nsec - mid.tv_nsec) / 1000000000.0;
        printf("New consumes %f\n\n", elapsed);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Total consumes %f\n\n", elapsed);
        fflush(stdout);
    }
}
