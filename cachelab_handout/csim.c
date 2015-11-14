// Team: A01073629 -- DMarshall
//       A01107224 -- AWallentine


#include "cachelab.h"
#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <getopt.h>

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

    //printSummary(cache.hit_count, cache.miss_count, cache.eviction_count);

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(t, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    while((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length: %zu\n", read);
        //printf("%s", line);
        add_line_cache(cache_ptr, line);
        //printSummary(cache.hit_count, cache.miss_count, cache.eviction_count);
    }

    fclose(fp);

    printSummary(cache.hit_count, cache.miss_count, cache.eviction_count);

    return 0;
}
