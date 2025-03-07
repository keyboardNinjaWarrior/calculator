#include "lib.h"

long double parse_operand (char *, int);

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
	printf("%llf", parse_operand (exp, 0));
}

long double parse_operand (char *exp, int start) {	
	long double operand = 0;
	if (exp[start] >= '0' && exp[start] <= '9') {
		for(; exp[start] >= '0' && exp[start] <= '9'; start++) {
			operand *= 10;
			operand += exp[start] - '0';
		}
	}
	return operand;
}
