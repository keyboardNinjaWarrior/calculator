#include "lib.h"

long double *parse_digits (char *, unsigned int);
signed short int parse_sign (char *, unsigned int *);
bool check_operator (char *,unsigned int);
bool check_operand (char *, unsigned int);

typedef struct exptree {
	bool is_end;

	union {
		struct {
			struct exptree *lvalue;
			unsigned short int operation;
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

long double *parse_digits (char *exp, unsigned int start) {
	signed short int sign = parse_sign (exp, &start);
	long double *operand;
	if (!(operand = malloc (sizeof (long double)))) {
		exit (3);
	}
	*operand = 0;
	bool decimal = false;
	unsigned int decimal_part = 0;

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
	
	*operand = sign * (*operand / powl (10.0, (long double) decimal_part));
	
	switch (exp[start]) {
		case 'e': case 'E':
			*operand *= powl (10.0, *parse_digits (exp, ++start));
	}

	return operand;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * parse sign:		checks for + and negative signs through recursion 	*	
 * 			and then returns +1 and -1 respectively.		*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

signed short int parse_sign (char *exp, unsigned int *start) {
	if (!(exp[*start] == '-' || exp[*start] == '+')) {
		return 1;
	} else if (exp[*start] == '-') {
		++(*start);
		return -1 * parse_sign(exp, start);
		
	} else {
		++(*start);
		return 1 * parse_sign(exp, start);
	}
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * check operator		returns true and false if it a given operation:	*
 * 				+ - / *						*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

bool check_operator (char *exp, unsigned int index) {
	switch (exp[index]) {
		case '+': case '-': case '/': case '*':
			return true;
		default:
			return false;
	}
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * check operand		returns true and false if the expression	*
 * 				is starting with '+', '-', '.' or is 		*
 * 				between 0 and 9 chars.				*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

bool check_operand (char *exp, unsigned int index) {
	return (exp[index] >= '0' && exp[index] <= '9') || 	\
		exp[index] == '.' || 				\
		exp[index] == '+' || 				\
		exp[index] == '-' ? 				\
		return true : return false;
}
