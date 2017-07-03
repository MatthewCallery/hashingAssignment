/* Hash functions - binary search tree used instead of 
   separate chaining */

#include "extnd.h"

void init_hash_table(hash_table *h)
{
   h->array = (elem *)malloc(INIT_ARRAY_SIZE * sizeof(elem));

   if(h->array == NULL){
      fprintf(stderr, "Error - can't allocate initial array\n");
      exit(EXIT_FAILURE);
   }

   h->size = INIT_ARRAY_SIZE;
   h->num_words = 0;
   h->num_chained = 0;

   set_to_null(h);
}


void set_to_null(hash_table *h)
{
   int i;

   for(i = 0; i < h->size; i++){
      h->array[i].str = NULL;
      h->array[i].left = NULL;
      h->array[i].right = NULL;      
   }
}


void insert(hash_table *h, char *line)
{
   unsigned int pos;
   elem *p;

   pos = hash_all(line) % h->size;

   p = &h->array[pos];

   /* Initial collision? */
   if(p->str != NULL){
      p = insert_in_tree(p, line); 
      if(p == NULL){
         return;
      }     
      h->num_chained += 1;
   }

   insert_word(p, line);
   h->num_words += 1;

   if(is_max_occupancy(h)){
      *h = resize(h);
   }
}


/* Inserts new node in the tree and returns that node */
elem *insert_in_tree(elem *p, char *line)
{
   /* Don't insert same word twice */
   if(p == NULL || strcmp(p->str, line) == 0){
      return NULL;
   }

   if(strcmp(line, p->str) < 0){
      if(p->left == NULL){
         p->left = create_element();
         p = p->left;
      }
      else{
         p = insert_in_tree(p->left, line);
      }
   }
   else{
      if(p->right == NULL){
         p->right = create_element();
         p = p->right;
      }
      else{
         p = insert_in_tree(p->right, line);
      }
   }   

   return p;
}


elem *create_element(void)
{
   elem *t;

   t = (elem *)malloc(sizeof(elem));

   if(t == NULL){
      fprintf(stderr, "Error - can't allocate element\n");
      exit(EXIT_FAILURE);
   }

   t->left = NULL;
   t->right = NULL;
   t->str = NULL;

   return t;
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
   unsigned int lookup = 1, pos;
   elem *p;

   pos = hash_all(line) % h->size;
   p = &h->array[pos];

   if(p->str == NULL){
      return -1;
   }

   lookup = (unsigned int) search_tree(p, line, lookup);

   /* Word not found in tree? */
   if(lookup == 0){
      return -1;
   }

   return (int)lookup;
}


/* Returns 0 if word not found. Otherwise returns num lookups 
   required */
int search_tree(elem *p, char *line, int lookup)
{
   if(p == NULL || p->str == NULL){
      return 0;  
   }

   if(strcmp(line, p->str) == 0){
      return lookup;
   }

   if(strcmp(line, p->str) < 0){
      return search_tree(p->left, line, lookup + 1);
   }
   else{
      return search_tree(p->right, line, lookup + 1);
   }
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
   t.num_chained = 0;

   set_to_null(&t);

   rehash(&t, h);

   return t;
}


void rehash(hash_table *t, hash_table *h)
{
   int i;
   elem *p;

   for(i = 0; i < h->size; i++){
      p = &h->array[i];
      rehash_tree(t, p);
   }

   free_hash_table(h);
}


void rehash_tree(hash_table *t, elem *p)
{
   if(p == NULL || p->str == NULL){
      return;
   }

   insert(t, p->str);

   rehash_tree(t, p->left);
   rehash_tree(t, p->right);
}


int is_max_occupancy(hash_table *h)
{
   double occupancy;

   occupancy = (double)h->num_chained / (double)h->num_words;

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


void free_hash_table(hash_table *h)
{
   int i;
   elem *p;

   for(i = 0; i < h->size; i++){
      p = &h->array[i];
      free(p->str);
      free_tree(p->left);
      free_tree(p->right);
   }

   free(h->array);
}


void free_tree(elem *p)
{
   if(p == NULL){
      return;
   }

   free(p->str);
   free_tree(p->left);
   free_tree(p->right);
   free(p);
}
