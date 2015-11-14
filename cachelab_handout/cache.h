#ifndef CACHE_H
#define CACHE_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>


typedef struct {
    int B;
    char valid;
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
            cache.set[i].line[j].B = pow(2, b);
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

void cache_add_value(cache_set * cache_ptr, uint64_t tag, int set, int block){
    //if(cache_ptr->set[set].E > 1){
    //  
    //    
    //}else{
    if (!(cache_ptr->set[set].line[0].tag == -1))
        cache_inc_eviction(cache_ptr);
    cache_ptr->set[set].line[0].tag = tag;
    //}
}

void check_cache(cache_set * cache_ptr, char op, uint64_t tag, int set, int block, int size){
    int i;
    for(i=0; i<cache_ptr->set[set].E; ++i){
        if (cache_ptr->set[set].line[i].tag == tag){
            switch(op){
                case 'L':
                    cache_inc_hit(cache_ptr);
                    return;
                    break;
                case 'S':
                    cache_inc_hit(cache_ptr);
                    return;
                    break;
                case 'M':
                    cache_inc_hit(cache_ptr);
                    cache_inc_hit(cache_ptr);
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
        check_cache(cache_ptr, op, tag, set + 1, 0, (block+size)-max_block);
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

#endif
