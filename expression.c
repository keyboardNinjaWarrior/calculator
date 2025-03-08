#include "lib.h"

long double *parse_digits (char *, int);

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
	printf("%llf", *parse_digits (exp, 0));
}

long double *parse_digits (char *exp, int start) {	
	long double *operand;
	if (!(operand = malloc (sizeof (long double)))) {
		exit (3);
	}

	*operand = 0;
	for(; exp[start] >= '0' && exp[start] <= '9'; start++) {
		*operand *= 10;
		*operand += exp[start] - '0';
	}
	return operand;
}
