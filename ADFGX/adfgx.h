#ifndef _ADFGX_HEADER_
#define _ADFGX_HEADER_

/*****************
*  ADFGX struct  *
*****************/
typedef struct {
	char* alphabet;         /* Defines the supported characters by the encryption algorithm     */
	char* symbols;          /* Defines the symbols used to encrypt a message                    */
	char* dictionary;       /* Defines the translation for all characters defined in alphabet   */
	char* transposition;    /* Defines the transposition key used in the second encryption step */
} adfgx;

/*****************
* Help functions *
*****************/

/* Allocates memory for a char*, checks if this allocation was successful and terminates with EOS */
char* allocate_strlen(int strlen);

/* Alphabetically sorts a given string */
char* sort(const char* s);

/* Returns the number of characters in a given file */
unsigned long get_file_length(FILE *ifp);

/* Returns the index of a character in a given string, -1 if the character cannot be found */
int get_index(const char c, const char* s);

/* Returns the content of a given file */
char* read_from_file(const char* filename);

/* Writes text to a given file */
void write_to_file(const char* filename, const char* text);

/* Returns a filtered, lowercase version of the specified message */
char* filter(const char* alphabet, const char* text);

/*****************
* Main functions *
*****************/

/* Creates a new ADFGX machine */
adfgx* create_adfgx(const char* alphabet, const char* symbols, const char* dictionary, const char* transposition);

/* Sets the transposition for a given ADFGX machine */
void set_transposition(adfgx* machine, const char* transposition);

/* Encodes a message using a given ADFGX machine */
char* encode_simple(adfgx* machine, const char* text);

/* Decodes a message using a given ADFGX machine */
char* decode_simple(adfgx* machine, const char* encoded);

/* Encodes and transposes a message using a given ADFGX machine */
char* encode_hard(adfgx* machine, const char* text);

/* Transposes and decodes a message using a given ADFGX machine */
char* decode_hard(adfgx* machine, const char* encoded);

/* Finds the transposition for a given ADFGX machine */
char* brute_force(adfgx* machine, const char* encoded, const char* start);

/* Frees allocated memory of a given ADFGX machine */
void free_adfgx(adfgx* machine);

#endif