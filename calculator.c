#include "lib.h"

char *remove_spaces (char *);

// -	tag init
// e	expression

int main (char argc, char *argv[]) {
	if (argv[1][0] == '-') {
		if (argv[1][1] == 'e') {
			expression (remove_spaces (argv[2]));
		}

	} else {
		printf ("calc: this command is not recognized.\n");
		exit (1);
	}

	return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * remove spaces: It takes the string as input, calculates its length and 	*
 *	 	  removes the space by placing the next non-space character 	*
 *	 	  at its position and then add \0 on remaining locations.	*
 * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * * * * * * * */

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

// Errors
// 1		missing tag initilization
// 2		memory allocation failled
// 3		error in obtaining digit
// 4		error in obtailing operation
// 5		error in tree calculation
// 6		error making operational node
// 7		empty string
