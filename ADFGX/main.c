#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#include <math.h>
#include "adfgx.h"

int main(int argc, char* argv []) {
	unsigned int i, j, k;
	adfgx *machine;
	char *adfgx, *alphabet, *symbols, *dictionary, *transposition, *text, *encoded_simple,
		 *decoded_simple, *encoded_hard, *decoded_hard, *encoded, *encoded_start,
		 *encoded_message, *decoded_message;

	// _CrtSetBreakAlloc(96);

	/* Read ADFGX information */
	adfgx = read_from_file("adfgx.txt");

	/* Define the alphabet */
	alphabet = strtok(adfgx,"\n");

	/* Define the code symbols */
	symbols = strtok(NULL,"\n");

	/* Define the dictionary */
	dictionary = strtok(NULL,"\n");

	/* Define the transposition */
	transposition = strtok(NULL,"\n");

	/* Create adfgx machine without dictionary */
	machine = create_adfgx(alphabet,symbols,dictionary,transposition);

	/* Print translation for every letter in the alphabet */
	printf("\nWoordenboek:\n\n");
	for (i=0; i<strlen(machine->symbols); i++) {
		for (j=0; j<strlen(machine->symbols); j++) {
			k = i*strlen(machine->symbols)+j;
			printf("   %c = %c%c",machine->alphabet[k],machine->dictionary[2*k],machine->dictionary[2*k+1]);
		}
		printf("\n");
	}

	/* Define the plain text */
	text = read_from_file("text.txt");

	/* Encode the plain text */
	encoded_simple = encode_simple(machine,text);
	printf("\nEenvoudig gecodeerde boodschap:\n\n");
	printf("   %s\n\n",encoded_simple);

	/* Decode the encoded message */
	decoded_simple = decode_simple(machine,encoded_simple);
	printf("Eenvoudig gedecodeerde boodschap:\n\n");
	printf("   %s\n\n",decoded_simple);

	/* Encode the plain text */
	encoded_hard = encode_hard(machine,text);
	printf("Complex gecodeerde boodschap:\n\n");
	printf("   %s\n\n",encoded_hard);

	/* Decode the encoded message */
	decoded_hard = decode_hard(machine,encoded_hard);
	printf("Complex gedecodeerde boodschap:\n\n");
	printf("   %s\n\n",decoded_hard);

	/* Read the encoded file */
	encoded = read_from_file("encoded.txt");

	/* Define the plain text start of the encoded message */
	encoded_start = strtok(encoded,"\n");

	/* Define the encoded message */
	encoded_message = strtok(NULL,"\n");

	/* Brute force decode the encoded message */
	printf("\nBrute force decodering... ");
	decoded_message = brute_force(machine,encoded_message,encoded_start);

	if (decoded_message != NULL) {
		/* Print decoded message */
		printf("Het volgende Duitse bericht werd ontcijferd met transpositiesleutel \"%s\":\n\n",machine->transposition);
		printf("   %s\n\n",decoded_message);
		/* Write solution to file */
		write_to_file("decoded.txt",decoded_message);
		/* Free memory */
		free(decoded_message);
	} else {
		printf("Het bericht kon niet ontcijferd worden. De implementatie is wellicht niet correct.\n\n");
	}

	/* Free memory */
	free_adfgx(machine);
	free(adfgx);
	free(text);
	free(encoded);
	free(encoded_simple);
	free(decoded_simple);
	free(encoded_hard);
	free(decoded_hard);

	/* Allow to read the console when debugging */
	getchar();

	/* Test for memory leaks */
	#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
	#endif
	return 0;
}