#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INIT_ARRAY_SIZE 31
#define MAX_OCCUPANCY 0.5
#define PRIME_INCR 2
#define PRIME_DENOM 3

struct elem{
   char *str;
   struct elem *left;
   struct elem *right;
};
typedef struct elem elem;

struct hash_table{
   elem *array;
   int size;
   int num_words;
   int num_chained;
};
typedef struct hash_table hash_table;

void init_hash_table(hash_table *h);
void set_to_null(hash_table *h);
void insert(hash_table *h, char *line);
elem *insert_in_tree(elem *p, char *line);
elem *create_element(void);
void insert_word(elem *p, char *line);
int search(hash_table *h, char *line);
int search_tree(elem *p, char *line, int lookup);
hash_table resize(hash_table *h);
void rehash(hash_table *t, hash_table *h);
void rehash_tree(hash_table *t, elem *p);
int is_max_occupancy(hash_table *h);
int find_next_prime(int current_prime);
int is_prime(int prime);
unsigned int hash_all(char *line);
void free_hash_table(hash_table *h);
void free_tree(elem *p);
