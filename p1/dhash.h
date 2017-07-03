#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INIT_ARRAY_SIZE 31
#define PRIME_INCR 2
#define PRIME_DENOM 3
#define H_END 2
#define MAX_OCCUPANCY 0.6

struct elem{
   char *str;
};
typedef struct elem elem;

struct hash_table{
   elem *array;
   int size;
   int num_words;
};
typedef struct hash_table hash_table;

void init_hash_table(hash_table *h);
void set_to_null(hash_table *h);
void insert(hash_table *h, char *line);
void insert_word(elem *p, char *line);
int search(hash_table *h, char *line);
hash_table resize(hash_table *h);
void rehash(hash_table *t, hash_table *h);
int is_max_occupancy(hash_table *h);
int find_next_prime(int current_prime);
int is_prime(int prime);
unsigned int hash_all(char *line);
unsigned int hash_ends(char *line);
void free_hash_table(hash_table *h);
