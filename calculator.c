#include "lib.h"

char *remove_spaces (char *);

int main (char argc, char *argv[]) {
	// checks for tags
	if (argv[1][0] == '-') {
		// check for tag type
		
		// tag = expression
		if (argv[1][1] == 'e') {
			printf ("%s\n", remove_spaces (argv[2]));
		}

	} else {
		printf("calc: This command is not recognized.\n");
	}

	return 0;
}

char *remove_spaces (char *exp) {
	int end = 0, position = 0;
	for (; exp[end] != '\0'; end++)
		;

	for (int i = 0; exp[i] != '\0'; i++) {
		if (exp[i] != ' ') {
			exp[position++] = exp[i];
		}
	}

	for (; position != end; ++position) {
		exp[position] = '\0';
	}

	return exp;
}
