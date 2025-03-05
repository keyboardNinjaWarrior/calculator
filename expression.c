#include "lib.h"

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
	exptree *tree = malloc (sizeof (exptree));
}

long double parse_operand (char *exp) {


}
