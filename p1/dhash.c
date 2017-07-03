/* Hash functions - double hashing used */

#include "dhash.h"

void init_hash_table(hash_table *h)
{
   h->array = (elem *)malloc(INIT_ARRAY_SIZE * sizeof(elem));

   if(h->array == NULL){
      fprintf(stderr, "Error - can't allocate initial array\n");
      exit(EXIT_FAILURE);
   }

   h->size = INIT_ARRAY_SIZE;
   h->num_words = 0;

   set_to_null(h);
}


void set_to_null(hash_table *h)
{
   int i;

   for(i = 0; i < h->size; i++){
      h->array[i].str = NULL;
   }
}


void insert(hash_table *h, char *line)
{
   unsigned int pos, step = 0;

   pos = hash_all(line) % h->size;

   /* Initial collision? */
   if(h->array[pos].str != NULL){
      step = (hash_ends(line) % (h->size - 1)) + 1;
      /* Step through array until no collision */
      while(h->array[pos].str != NULL){
         pos = (pos + step) % h->size;
      }
   }

   insert_word(&h->array[pos], line);

   h->num_words += 1;

   if(is_max_occupancy(h)){
      *h = resize(h);
   }
}


void insert_word(elem *p, char *line)
{
   int line_len;

   line_len = strlen(line) + 1;
   p->str = (char *)malloc(sizeof(char) * line_len);

   if(p->str == NULL){
      fprintf(stderr, "Error - can't allocate string\n");
      exit(EXIT_FAILURE);
   }

   strcpy(p->str, line);
}


/* Returns -1 if word not found in hash table */
int search(hash_table *h, char *line)
{
   unsigned int lookup = 1, pos, step, first_hash;

   first_hash = hash_all(line) % h->size;
   pos = first_hash;

   /* Not in array[first_hash]? */
   if(h->array[pos].str == NULL){
      return -1;
   }

   step = (hash_ends(line) % (h->size - 1)) + 1;

   while(strcmp(line, h->array[pos].str) != 0){
      pos = (pos + step) % h->size;
      lookup++;
      /* Not in array? */
      if(h->array[pos].str == NULL || pos == first_hash){
         return -1;
      }
   }

   return (int)lookup;
}


hash_table resize(hash_table *h)
{
   hash_table t;
   int new_prime;

   new_prime = find_next_prime(h->size);

   t.array = (elem *)malloc(new_prime * sizeof(elem));

   if(t.array == NULL){
      fprintf(stderr, "Error - can't allocate array\n");
      exit(EXIT_FAILURE);
   }

   t.size = new_prime;
   t.num_words = 0;

   set_to_null(&t);

   rehash(&t, h);

   return t;
}


/* Rehash words from old array into new */
void rehash(hash_table *t, hash_table *h)
{
   int i;

   for(i = 0; i < h->size; i++){
      if(h->array[i].str != NULL){
         insert(t, h->array[i].str);
      }
   }

   free_hash_table(h);
}


int is_max_occupancy(hash_table *h)
{
   double occupancy;

   occupancy = (double)h->num_words / (double)h->size;

   if(occupancy > MAX_OCCUPANCY){
      return 1;
   }
   else{
      return 0;
   }
}


int find_next_prime(int current_prime)
{
   int new_prime;

   /* New prime must be greater than twice the size of current.
      ((n * 2) + 1) will always be odd, if n is an integer */
   new_prime = (current_prime * PRIME_INCR) + 1;

   while(!is_prime(new_prime)){
      new_prime += PRIME_INCR;
   }

   return new_prime;
}


int is_prime(int prime)
{
   int i;

   if(prime % PRIME_INCR == 0){
      return 0;
   }

   /* If number is odd, factors must be between 3 and (number/3).
      If no factors in this range then number is a prime */
   for(i = PRIME_DENOM; i < (prime / PRIME_DENOM); i += PRIME_INCR){
      if(prime % i == 0){
         return 0;
      }
   }

   return 1;
}


unsigned int hash_all(char *line)
{
   int i;
   int n = strlen(line);
   unsigned int hash = 0;

   for(i = 0; i < n; i++){
      hash = line[i] + (INIT_ARRAY_SIZE * hash);
   }

   return hash;
}


unsigned int hash_ends(char *line)
{
   unsigned int c1, c2, cn1, cn2;
   unsigned int hash;
   unsigned int n;

   n = (unsigned int)strlen(line);
   
   c1 = line[0];
   c2 = line[1];
   cn1 = line[n - 1];
   cn2 = line[n - H_END];

   hash = ((c1 * c2) + (cn1 * cn2)) * n;

   return hash;
}


void free_hash_table(hash_table *h)
{
   int i;

   for(i = 0; i < h->size; i++){
      if(h->array[i].str != NULL){
         free(h->array[i].str);
      }
   }

   free(h->array);
}
