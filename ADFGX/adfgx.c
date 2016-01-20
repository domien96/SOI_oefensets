#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "adfgx.h"

/*****************
* Help functions *
*****************/

/* Allocates memory for a char*, checks if this allocation was successful and terminates with EOS */
char* allocate_strlen(int strlen) {
	char* allocated = (char*)malloc((strlen + 1)*sizeof(char));
	if (!allocated) {
		printf("ERROR: Allocation failed, insufficient memory?\n");
		exit(1);
	}
	allocated[strlen] = '\0';
	return allocated;
}

/* Alphabetically sorts a given string */
char* sort(const char* s) {
	unsigned int i, j;
	char temp, *s_sorted;
	s_sorted = (char*)allocate_strlen(strlen(s));
	strcpy(s_sorted, s);
	for (i = 1; i<strlen(s_sorted) + 1; i++) {
		for (j = 0; j<strlen(s_sorted) - i; j++) {
			if (s_sorted[j] > s_sorted[j + 1]) {
				temp = s_sorted[j];
				s_sorted[j] = s_sorted[j + 1];
				s_sorted[j + 1] = temp;
			}
		}
	}
	return s_sorted;
}

/* Returns the first index of a character in a given string, -1 if the character cannot be found */
int get_index(const char c, const char* s) {
	unsigned int i;
	for (i = 0; i<strlen(s); i++) {
		if (s[i] == c) {
			return i;
		}
	}
	return -1;
}

/* Returns the to_lower equivalent of a character */
char to_lower(char c) {
	if (c<'A' || c>'Z') {
		return c;
	}
	return c - 'A' + 'a';
}

/* Returns the content of a given file */
char* read_from_file(const char* filename) {
	FILE *ifp;
	unsigned long size;
	char* buffer;
	ifp = fopen(filename, "r");
	if (!ifp) {
		printf("ERROR: FILE equals NULL\n");
		exit(1);
	}
	size = get_file_length(ifp);
	buffer = allocate_strlen(size);
	fread(buffer, 1, size, ifp);
	fclose(ifp);
	return buffer;
}

/* Returns the number of characters in a given file */
unsigned long get_file_length(FILE *ifp) {
	unsigned long size;
	if (!ifp) {
		printf("ERROR: FILE equals NULL\n");
		exit(1);
	}
	fseek(ifp, 0, SEEK_END);
	size = ftell(ifp);
	rewind(ifp);
	return size;
}

/* Writes text to a given file */
void write_to_file(const char* filename, const char* text) {
	FILE *ifp;
	ifp = fopen(filename, "w");
	if (!ifp) {
		printf("ERROR: FILE equals NULL\n");
		exit(1);
	}
	fprintf(ifp, "%s", text);
	fclose(ifp);
}

/* Returns a filtered, lowercase version of the specified message */
char* filter(const char* alphabet, const char* text) {
	unsigned int i, j = 0, k = 0;
	char* filtered_message;
	for (i = 0; i<strlen(text); i++) {
		if (get_index(to_lower(text[i]), alphabet) >= 0) {
			j++;
		}
	}
	filtered_message = allocate_strlen(j);
	for (i = 0; i<strlen(text); i++) {
		if (get_index(to_lower(text[i]), alphabet) >= 0) {
			filtered_message[k++] = to_lower(text[i]);
		}
	}
	return filtered_message;
}

/*****************
* Main functions *
*****************/

/* Creates a new ADFGX machine */
adfgx* create_adfgx(const char* alphabet, const char* symbols, const char* dictionary, const char* transposition) {
	adfgx* machine = (adfgx*)malloc(sizeof(adfgx));
	if (!machine) {
		printf("ERROR: Allocation failed, insufficient memory?\n");
		exit(1);
	}
	machine->alphabet = allocate_strlen(strlen(alphabet));
	machine->symbols = allocate_strlen(strlen(symbols));
	machine->dictionary = allocate_strlen(strlen(dictionary));
	machine->transposition = allocate_strlen(strlen(transposition));
	strcpy(machine->alphabet, alphabet);
	strcpy(machine->symbols, symbols);
	strcpy(machine->dictionary, dictionary);
	strcpy(machine->transposition, transposition);
	return machine;
}

/* Sets the transposition for a given ADFGX machine */
void set_transposition(adfgx* machine, const char* transposition) {
	machine->transposition = (char*)realloc(machine->transposition, (strlen(transposition) + 1)*sizeof(char));
	if (!machine->transposition) {
		printf("ERROR: Allocation failed, insufficient memory?\n");
		exit(1);
	}
	strcpy(machine->transposition, transposition);
}

/* Encodes a message using a given ADFGX machine */
char* encode_simple(adfgx* machine, const char* text) {
	unsigned int i;
	int j;
	char *text_filtered, *encoded;
	text_filtered = filter(machine->alphabet, text);
	encoded = allocate_strlen(2 * strlen(text_filtered));
	for (i = 0; i<strlen(text_filtered); i++) {
		j = get_index(text_filtered[i], machine->alphabet);
		encoded[2 * i] = machine->dictionary[2 * j];
		encoded[2 * i + 1] = machine->dictionary[2 * j + 1];
	}
	free(text_filtered);
	return encoded;
}

/* Decodes a message using a given ADFGX machine */
char* decode_simple(adfgx* machine, const char* encoded) {
	unsigned int i, j;
	char *decoded = allocate_strlen(strlen(encoded) / 2);
	for (i = 0; i<strlen(encoded) / 2; i++) {
		for (j = 0; j<strlen(machine->alphabet); j++) {
			if (encoded[2 * i] == machine->dictionary[2 * j] && encoded[2 * i + 1] == machine->dictionary[2 * j + 1]) {
				decoded[i] = machine->alphabet[j];
			}
		}
	}
	return decoded;
}

/* Encodes and transposes a message using a given ADFGX machine */
char* encode_hard(adfgx* machine, const char* text) {
	unsigned int i, j, k, l = 0;
	char *transposition_sorted, *encoded_simple, *encoded_hard;
	encoded_simple = encode_simple(machine, text);
	encoded_hard = allocate_strlen(strlen(encoded_simple));
	transposition_sorted = sort(machine->transposition);
	for (i = 0; i<strlen(transposition_sorted); i++) {
		k = get_index(transposition_sorted[i], machine->transposition);
		for (j = k; j<strlen(encoded_simple); j += strlen(machine->transposition)) {
			encoded_hard[l++] = encoded_simple[j];
		}
	}
	free(transposition_sorted);
	free(encoded_simple);
	return encoded_hard;
}

/* Transposes and decodes a message using a given ADFGX machine */
char* decode_hard(adfgx* machine, const char* encoded) {
	unsigned int i, j, k, l = 0;
	char *transposition_sorted, *encoded_simple, *decoded_simple;
	encoded_simple = allocate_strlen(strlen(encoded));
	transposition_sorted = sort(machine->transposition);
	for (i = 0; i<strlen(transposition_sorted); i++) {
		k = get_index(transposition_sorted[i], machine->transposition);
		for (j = k; j<strlen(encoded); j += strlen(machine->transposition)) {
			encoded_simple[j] = encoded[l++];
		}
	}
	decoded_simple = decode_simple(machine, encoded_simple);
	free(transposition_sorted);
	free(encoded_simple);
	return decoded_simple;
}

/* Finds the transposition for a given ADFGX machine */
char* brute_force(adfgx* machine, const char* encoded, const char* start) {
	char *decoded, *header, *permutation, *permutation_array;
	permutation_array = read_from_file("permutations.txt");
	header = strtok(permutation_array, "\n");
	while ((permutation = strtok(NULL, "\n")) != NULL) {
		set_transposition(machine, permutation);
		decoded = decode_hard(machine, encoded);
		if (strncmp(decoded, start, strlen(start)) == 0) {
			free(permutation_array);
			return decoded;
		}
		free(decoded);
	}
	free(permutation_array);
	return NULL;
}

/* Frees allocated memory of a given ADFGX machine */
void free_adfgx(adfgx* machine) {
	free(machine->alphabet);
	free(machine->symbols);
	free(machine->dictionary);
	free(machine->transposition);
	free(machine);
}