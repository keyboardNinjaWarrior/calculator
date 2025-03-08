#include "lib.h"

long double *parse_digits (char *, int);
int parse_sign (char *, int);

typedef struct exptree {
	bool is_end;

	union {
		struct {
			struct exptree *lvalue;
			char operation;
			struct exptree *rvalue;
		} parent;

		struct {
			long double operand;
		} end;	
	} node;
} exptree;

void expression (char *exp) {
//	exptree *tree = malloc (sizeof (exptree));
	printf ("%llf\n", *parse_digits (exp, 0));
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * parse digits:	It takes an expression and starting point. Then it 	*
 * 			allocates memory for storing the long double. It loops 	*
 * 			over the characters and adds them to the variable and 	*
 * 			multiplies it with ten. Meanwhile it looks for the 	*
 * 			decimal and if its there, it starts adding the digits 	*
 * 			after the decimal. At the end it divides the number 	*
 * 			with it. It also evaluates expression given in 		*
 * 			scientific notation through recursion.			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

long double *parse_digits (char *exp, int start) {
	int sign = parse_sign (exp, &start);
	long double *operand;
	if (!(operand = malloc (sizeof (long double)))) {
		exit (3);
	}
	*operand = 0;
	bool decimal = false;
	int decimal_part = 0;

	for (; (exp[start] >= '0' && exp[start] <= '9') || exp[start] == '.'; start++) {
		if (exp[start] != '.') {
			*operand *= 10;
			*operand += exp[start] - '0';
			if (decimal) {
				++decimal_part;
			}
		} else if (!decimal) {
			decimal = true;
		} else {
			exit (4);
		}
	}
	
	*operand /= powl (10.0, (long double) decimal_part);
	
	switch (exp[start]) {
		case 'e': case 'E':
			*operand *= powl (10.0, *parse_digits (exp, ++start));
	}

	return operand;
}

int parse_sign (char *exp, int *start) {
	if (exp[*start] != '+' && exp[*start] != '-') {
		return 1;
	} else if (exp[*start] == '+') {
		return 1 * parse_sign(exp, ++(*start));
		
	} else {
		return -1 * parse_sign(exp, ++(*start));
	}
}
