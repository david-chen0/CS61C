/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

extern void checkCases(char* str);

extern char* changeToLower(char* str);

extern void reallocate(int* curSize, int* maxSize, char** str);

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv) {

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();
  return 0;
}

/*
 * This should hash a string to a bucket index.  void *s can be safely cast
 * to a char * (null terminated string)
 */
unsigned int stringHash(void *s) {
  // -- TODO --

  //We are using the hash function djb2 by Dan Bernstein
  //No clue why using 33 is optimal, but tests show it is
  unsigned long hash = 5381;
  for (int c = *(char *) s; c != '\0'; s++, c = *(char *) s) {
    hash = (hash * 33) + c;
  }
  return hash;
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  // -- TODO --
  if (strcmp((char *) s1, (char *) s2) == 0) {
    return 1;
  }
  return 0;
}


void checkAlloc(void *location) { //Checks if allocation was successful
  if (location == NULL) {
    fprintf(stderr, "Malloc failed\n");
    exit(1);
  }
}

/* Reallocate str if it exceeds maxSize */
void reallocate(int* curSize, int* maxSize, char** str) {
  if (*curSize >= *maxSize) { // Resizes the string if needed
      *maxSize *= 2;
      *str = realloc(*str, (*maxSize + 1) * sizeof(char));
      checkAlloc(*str);
  }
}

char *getWord(FILE** f) { //Gets a word starting at pointer fp
  FILE *fp = *f;
  char c = fgetc(fp);
  while (c == ' ' || c == '\n' || c == '\t') { //Gets rid of the whitespace
    c = fgetc(fp);
  }

  int curSize = 0;
  int maxSize = 60;
  char *str = malloc((maxSize + 1) * sizeof(char));
  checkAlloc(str);
  for (; c != ' ' && c != '\n' && c != '\t' && !feof(fp); curSize++) { //For loop to read one word
    reallocate(&curSize, &maxSize, &str);
    str[curSize] = c;
    c = fgetc(fp);
  }
  str[curSize] = '\0';
  if (curSize < maxSize) { //Resizes to save space
    str = realloc(str, (curSize + 1) * sizeof(char));
    checkAlloc(str);
  }
  return str;
}

/*
 * This function should read in every word and replacement from the dictionary
 * and store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final bit of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(61)
 * to cleanly exit the program.
 */
void readDictionary(char *dictName) {
  // -- TODO --
  //Referenced from StackOverflow
  FILE *fp = fopen(dictName, "r");
  if (fp == NULL) { //Checks if the file exists
    fprintf(stderr, "Invalid dictionary file\n");
    exit(61);
  }

  while (1) { //Each loop is one line; first word is key, second is data
    char *key = getWord(&fp);
    char *data = getWord(&fp);
    insertData(dictionary, key, data);

    if (feof(fp)) { //Ends function if it is the end of the file
      return;
    }
  }
}


/*
 * This should process standard input (stdin) and perform replacements as 
 * described by the replacement set then print either the original text or 
 * the replacement to standard output (stdout) as specified in the spec (e.g., 
 * if a replacement set of `taest test\n` was used and the string "this is 
 * a taest of  this-proGram" was given to stdin, the output to stdout should be 
 * "this is a test of  this-proGram").  All words should be checked
 * against the replacement set as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the replacement set should 
 * it report the original word.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final bit of your grade, you cannot assume words have a bounded length.
 */
void processInput() {
  // -- TODO --
  // Referenced from StackExchange on how to while loop through stdin
  int curSize = 0;
  int maxSize = 60;
  char* str = malloc((maxSize + 1) * sizeof(char));
  checkAlloc(str);

  int stdin_int;
  while((stdin_int = getchar()) != EOF) {  // Loop through characters of stdin
    char stdin_ch = (char) stdin_int;
    if (!isalnum(stdin_ch)) {
      str[curSize] = '\0';
      checkCases(str);
      putchar(stdin_ch);  // Place space & punct to stdout
      curSize = 0;
      maxSize = 60;
    } else {
      reallocate(&curSize, &maxSize, &str);  // Reallocate str if it exceeds maxSize
      str[curSize] = stdin_ch;
      curSize++;
    }
  }
  str[curSize] = '\0';
  checkCases(str);
}

/* Check all 3 cases to find the replacement from dict */
void checkCases(char* str) {
  // Case #1: The exact word
  char* replacement = (char *) findData(dictionary, str);
  if (replacement) {
    fprintf(stdout, "%s", (char *) replacement);
    return;
  }
  // Case #2: All but the first character converted to lowercase
  char* lower = changeToLower(str);
  replacement = (char *) findData(dictionary, lower);
  if (replacement) {
    fprintf(stdout, "%s", (char *) replacement);
    free(lower);
    return;
  }
  // Case #3: Converted completely to lower case
  lower[0] = tolower(str[0]);
  replacement = (char *) findData(dictionary, lower);
  if(replacement) {
    fprintf(stdout, "%s", (char *) replacement);
    free(lower);
    return;
  }
  free(lower);
  // None of the above cases (no replacement)
  fprintf(stdout, "%s", str);
}

/* Make every character char lower case expect the first one */
char* changeToLower(char* str) {
  int len = strlen(str);
  char* copyStr = malloc((len + 1) * sizeof(char));
  copyStr[0] = str[0];
  for (int i = 1; i < len; ++i) {
    copyStr[i] = tolower(str[i]);
  }
  copyStr[len] = '\0';
  return copyStr;
}
