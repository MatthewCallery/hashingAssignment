/* Program reads in a dictionary file from argv[1] and stores
   the words in a hash table. It then reads another list of words
   from argv[2] and calculates the average number of look-ups 
   required in the hash table */

#include "dhash.h"

#define MAX_LINE 200
#define VALID_ARGC 3

void read_first_file(hash_table *h, char *file_name);
double find_av_lookup(hash_table *h, char *file_name);
void check_file(FILE *file, char *file_name);
void check_line(char *line, char *file_name);

int main(int argc, char **argv)
{
   hash_table h;
   double av_lookup;

   if(argc != VALID_ARGC){
      fprintf(stderr, "ERROR: Incorrect usage, try e.g. %s file1.txt file2.txt\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   init_hash_table(&h);
   read_first_file(&h, argv[1]);
   av_lookup = find_av_lookup(&h, argv[VALID_ARGC - 1]);

   if(av_lookup > 0){
      printf("%f\n", av_lookup);
   }

   free_hash_table(&h);

   exit(EXIT_SUCCESS);
}


void read_first_file(hash_table *h, char *file_name)
{
   FILE *file;
   char line[MAX_LINE];

   file = fopen(file_name, "r");
   check_file(file, file_name);

   while(fgets(line, sizeof(line), file)){
      check_line(line, file_name);
      insert(h, line);
   }

   fclose(file);
}


double find_av_lookup(hash_table *h, char *file_name)
{
   int num_lookups, total_lookups = 0, words_matched = 0;
   char line[MAX_LINE];
   double av_lookup;
   FILE *file;

   file = fopen(file_name, "r");
   check_file(file, file_name);

   while(fgets(line, sizeof(line), file)){
      check_line(line, file_name);
      num_lookups = search(h, line);

      /* Has word been matched? Ignore otherwise */
      if(num_lookups != -1){
         total_lookups += num_lookups;
         words_matched++;
      }
   }

   if(words_matched == 0){
      printf("No words matched\n");
      return 0;
   }

   av_lookup = (double)total_lookups / (double)words_matched;

   fclose(file);

   return av_lookup;  
}


void check_file(FILE *file, char *file_name)
{
   if(file == NULL){
      fprintf(stderr, "Failed to open %s\n", file_name);
      exit(EXIT_FAILURE);
   } 
}


void check_line(char *line, char *file_name)
{
   int i = 0;

   if(strlen(line) >= (MAX_LINE - 1)){
      fprintf(stderr, "ERROR: Line in %s too long\n", file_name);
      exit(EXIT_FAILURE);
   }

   while(line[i] != '\0'){
      /* Remove newline chars which fgets scans in */
      if(line[i] == '\n' || line[i] == '\r'){
         line[i] = '\0';
      }
      else if(!isalpha(line[i])){
         fprintf(stderr, "%s contains invalid character(s)\n", file_name);
         exit(EXIT_FAILURE); 
      }
      /* Avoid problems with capitalised words */
      else{
         line[i] = tolower(line[i]);
      }
      i++;
   }
}
