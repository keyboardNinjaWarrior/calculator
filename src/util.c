#include "header.h"
#include <stdarg.h>

typedef char * string;

FILE *openFile (char* filename, char* mode) {
	FILE *file;
	
	if (! (file = fopen (filename, mode))) {
		fprintf (stderr, "Error in opening the file.\n");
		exit (FILE_ERR);
	}
	
	return file;
}

void closeFile (FILE *file) {
	if (ferror (file))
		fprintf (stderr, "Error during I/O of file.\n");

	fclose (file);
}

void closeFiles (VLINT arglen, ...) {
	FILE *file;
	va_list fileNames;
	va_start (fileNames, arglen);
	
	for (VLINT i = 1; i <= arglen; i++)
		closeFile (va_arg (fileNames, FILE *));
}

// includes '\0' as well
int strlength (string x) {
	int i;
	for (i = 1; x[i - 1] != '\0'; i++)
		;
	return i;
}

string cat (string a, string b) {
	int length_A = strlength (a);
	int length_B = strlength (b);
	int lengthTotal = length_A + length_B;
	string cated = malloc (lengthTotal + 1);
	
	int i = 0;
	--length_A;
	for (; i < length_A; i++)
		cated[i] = a[i];
	
	int j = i;
	--length_B;
	for (i = 0; i < length_B; i++)
		cated[j++] = b[i];

	cated[lengthTotal] = '\0';
	
	return cated;
}
