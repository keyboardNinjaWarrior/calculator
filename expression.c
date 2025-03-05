#include "lib.h"

void expression (char *exp) {
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
}
