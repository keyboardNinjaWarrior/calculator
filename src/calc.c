#include "header.h"

#include <stdbool.h>
#include <stdlib.h>

static CHAR *expression;	//unglobal it later
static VLINT index = 0;

bool isDigit (void) {
	return (expression[index] >= '0' && expression[index] <= '9');
}

void digitStr (void) {
	FILE *numFile = openFile ("number", "ab");

	while (isDigit () || expression[index] == '0') {
		bool decimal = false;
		
		if (isDigit ()) {
			fputc (expression[index++] - '0', numFile);

			// if decimal == True then do something
		} else if (expression[index++] == '.') {
			if (! decimal) {
				decimal = true;
			} else {
				fprintf (stderr, "Invalid Syntax: Only one decimal point is expected.");
				exit (INVALID_SYNTX);
			}
		}
	}
	
	closeFile (numFile);
}

int8_t obtainSign (void) {
	if (expression[index] == '+')
		return 1;
	else if (expression[index] == '-')
		return -1;
	else
		return 0;
}

// a recursive function to parse sign
int8_t parseSign (int8_t sign) {
	if (! isDigit ()) {
		sign *= obtainSign ();
		++index;
		parseSign (sign);
	} else {
		return sign;
	}
}

void saveSign (void) {
	FILE *signFile = openFile ("sign", "wb");

	fputc (parseSign (1), signFile);
	
	closeFile (signFile);
}

int main (int argc, char *argv[static argc + 1]) {
	expression = argv[1];

	saveSign ();
	digitStr ();
	
	return 0;
}
