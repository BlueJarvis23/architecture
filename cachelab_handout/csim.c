// Team: A01073629 -- DMarshall
//       A01107224 -- AWallentine
#define _GNU_SOURCE

#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <getopt.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>


typedef struct {
    int B;
    unsigned int valid;
    uint64_t tag;
    char * data;
} cache_block;

typedef struct {
    int E;
    cache_block * line;
} cache_line;

typedef struct {
    unsigned int line_num;
    int s, E, b;
    int S;
    int hit_count, miss_count, eviction_count;
    cache_line * set;
} cache_set;

int two_to_the_x(int x);
cache_set init_cache(int s, int E, int b);
void cache_inc_hit(cache_set * cache_ptr);
void cache_inc_miss(cache_set * cache_ptr);
void cache_inc_eviction(cache_set * cache_ptr);
cache_block * get_last_used(cache_line * cache_line);
void cache_add_value(cache_set * cache_ptr, uint64_t tag, int set, int block);
void check_cache(cache_set * cache_ptr, char op, uint64_t tag, int set, int block, int size);
void add_cache_entry(cache_set * cache_ptr, char op, uint64_t address, int size);
void add_line_cache(cache_set * cache_ptr, char * line);


int main(int argc, char **argv)
{
    int opt;
    short s, E, b;
    char * t = NULL;

    s = 0;
    E = 0;
    b = 0;
    while ((opt = getopt(argc, argv, "s:E:b:t:")) != -1) {
        switch(opt){
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                t = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -s sets -E associativity -b blocks -t trace_path\n", argv[0]);
                exit(0);
        }
    }

    //printf("s: %d, E: %d, b: %d, t: %s\n", s,E,b,t);

    cache_set cache = init_cache(s, E, b);
    cache_set * cache_ptr = & cache;

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(t, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    while((read = getline(&line, &len, fp)) != -1) {
        add_line_cache(cache_ptr, line);
        //printSummary(cache.hit_count, cache.miss_count, cache.eviction_count);
    }

    fclose(fp);

    printSummary(cache.hit_count, cache.miss_count, cache.eviction_count);

    return 0;
}

/*
 *******************************************
 * cache_sim --> functions
 *******************************************
 */

cache_set init_cache(int s, int E, int b){
    int i,j;
    cache_set cache;
    cache.line_num = 0;
    cache.s = s;
    cache.E = E;
    cache.b = b;
    cache.S = pow(2,s);
    cache.hit_count = 0;
    cache.miss_count = 0;
    cache.eviction_count = 0;
    cache.set = (cache_line *) malloc(sizeof(cache_line) * cache.S);

    for(i=0;i<cache.S;++i){
        cache.set[i].E = E;
        cache.set[i].line = (cache_block *) malloc(sizeof(cache_block) * cache.set[i].E);
        for(j=0;j<cache.set[i].E;++j){
            cache.set[i].line[j].B = pow(2,b);
            cache.set[i].line[j].valid = 0;
            cache.set[i].line[j].tag = -1;
            cache.set[i].line[j].data = NULL;
        }
    }
    return cache;
}

void cache_inc_hit(cache_set * cache_ptr){
    cache_ptr->hit_count++;
}

void cache_inc_miss(cache_set * cache_ptr){
    cache_ptr->miss_count++;
}

void cache_inc_eviction(cache_set * cache_ptr){
    cache_ptr->eviction_count++;
}

cache_block * get_last_used(cache_line * cache_line){
    int i;
    unsigned int v_temp = ~0x0;
    cache_block * lru = NULL;
    for(i=0;i<cache_line->E;++i){
        if(cache_line->line[i].valid < v_temp){
            v_temp = cache_line->line[i].valid;
            lru = &cache_line->line[i];
        }
    }
    return lru;
}

void cache_add_value(cache_set * cache_ptr, uint64_t tag, int set, int block){
    cache_block * cache_lru = get_last_used(&cache_ptr->set[set]);
    if(!(cache_lru->tag == -1))
        cache_inc_eviction(cache_ptr);
    cache_lru->tag = tag;
    cache_lru->valid = cache_ptr->line_num;
}

void check_cache(cache_set * cache_ptr, char op, uint64_t tag, int set, int block, int size){
    int i;
    for(i=0; i<cache_ptr->set[set].E; ++i){
        if (cache_ptr->set[set].line[i].tag == tag){
            switch(op){
                case 'L':
                    cache_inc_hit(cache_ptr);
                    cache_ptr->set[set].line[i].valid = cache_ptr->line_num;
                    return;
                    break;
                case 'S':
                    cache_inc_hit(cache_ptr);
                    cache_ptr->set[set].line[i].valid = cache_ptr->line_num;
                    return;
                    break;
                case 'M':
                    cache_inc_hit(cache_ptr);
                    cache_inc_hit(cache_ptr);
                    cache_ptr->set[set].line[i].valid = cache_ptr->line_num;
                    return;
                    break;
            }
        }
    }
    switch(op){
        case 'L':
            cache_inc_miss(cache_ptr);
            cache_add_value(cache_ptr, tag, set, block);
            break;
        case 'S':
            cache_inc_miss(cache_ptr);
            cache_add_value(cache_ptr, tag, set, block);
            break;
        case 'M':
            cache_inc_miss(cache_ptr);
            cache_add_value(cache_ptr, tag, set, block);
            cache_inc_hit(cache_ptr);
            break;
    }
}

void add_cache_entry(cache_set * cache_ptr, char op, uint64_t address, int size){
    uint64_t tag = address >> (cache_ptr->s + cache_ptr->b);
    int set = (address << (64 - (cache_ptr->s + cache_ptr->b))) >> (64 - (cache_ptr->s));
    int block = (address << (64 - (cache_ptr->b))) >> (64 - (cache_ptr->b));
    //printf("tag: %" PRIx64 ", set: %d, block: %d\n", tag, set, block);
    
    unsigned int j = ~0x0;
    unsigned int max_block = ((j << (unsigned int)(32 - cache_ptr->b)) >> (unsigned int)(32 - cache_ptr->b));
    if((block + size - 1) > max_block){
        if (set+1 > cache_ptr->S - 1){
            check_cache(cache_ptr, op, tag + 1, 0, 0, (block+size)-max_block);
             
        }else{
            check_cache(cache_ptr, op, tag, set + 1, 0, (block+size)-max_block);
        }
    }

    check_cache(cache_ptr, op, tag, set, block, size);
}

void add_line_cache(cache_set * cache_ptr, char * line){
    if ( line[0] == 'I' && ( line[1] == 'L' || line[1] == 'M' || line[1] == 'S'))
        return;

    cache_ptr->line_num++;

    char op;
    uint64_t address;
    int size;
    
    char delim[] = ", ";
    char * token, *cp;
    cp = strdup((const char *) line);
    token = strtok(cp, delim);
    op = *token;

    token = strtok(NULL, delim);
    address = strtol(token, NULL, 16);

    token = strtok(NULL, delim);
    size = atoi(token);
    
    //printf("op: %c, address: %" PRIx64 ", size: %d\n", op, address, size);

    add_cache_entry(cache_ptr, op, address, size);
}

// TODO: Should free dynamic memory
//void free_cache_mem(cache_set * cache){
//
//}

