#include "lib.h"

long double *parse_digits (char *, unsigned int *);
signed short int parse_sign (char *, unsigned int *);
bool check_operator (char *,unsigned int *);
bool check_digits (char *, unsigned int *);
struct exptree *obtain_operation (char *, int *);
struct exptree *obtain_operand (char *, int *);

typedef struct exptree {
	bool is_child;

	union {
		struct {
			struct exptree *above;
			struct exptree *lvalue;
			unsigned short int operation;
			struct exptree *rvalue;
		} parent;

		struct {
			struct exptree *above;
			long double *operand;
		} child;	
	} node;

} exptree;

void expression (char *exp) {
	unsigned int exp_index = 0;
	exptree *num1 = obtain_operand(exp, &exp_index);
	printf("%llf\n", *num1->node.child.above);
}

struct exptree *obtain_operand (char *exp, int *index) {	
	exptree *operand = malloc (sizeof (exptree));
	if (check_digits (exp, index)) {
		operand->is_child = true;
		operand->node.child.operand = parse_digits (exp, index);
	} else {
		exit (3);
	}
	return operand;
} 

struct exptree *obtain_operation (char *exp, int *index) {
	struct exptree *operator;
	if ((operator = malloc (sizeof (exptree))) && (operator->node.parent.operation = check_operator (exp, index))) {
		operator->is_child = false;
	} else {
		exit (4);
	}
	return operator;
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

long double *parse_digits (char *exp, unsigned int *start) {
	signed short int sign = parse_sign (exp, start);
	long double *operand;
	if (!(operand = malloc (sizeof (long double))))
		exit (2);
	*operand = 0;
	bool decimal = false;
	unsigned int decimal_part = 0;

	for (; (exp[*start] >= '0' && exp[*start] <= '9') || exp[*start] == '.'; *start = *start + 1) {
		if (exp[*start] != '.') {
			*operand *= 10;
			*operand += exp[*start] - '0';
			if (decimal) {
				++decimal_part;
			}
		} else if (!decimal) {
			decimal = true;
		} else {
			exit (3);
		}
	}
	
	*operand = sign * (*operand / powl (10.0, (long double) decimal_part));
	
	switch (exp[*start]) {
		case 'e': case 'E':
			*start = *start + 1;
			*operand *= powl (10.0, *parse_digits (exp, start));
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
 * check operator	returns true and false if it a given operation:		*
 * 			+ - / *							*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

bool check_operator (char *exp, unsigned int *index) {
	switch (exp[*index]) {
		case '+': case '-': case '/': case '*':
			return true;
		default:
			return false;
	}
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * check operand	returns true and false if the expression is starting 	*
 * 			with '+', '-', '.' or is between 0 and 9 		*
 * 			chars.							*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

bool check_digits (char *exp, unsigned int *index) {
	return (exp[*index] >= '0' && exp[*index] <= '9') || 	\
		exp[*index] == '.' || 				\
		exp[*index] == '+' || 				\
		exp[*index] == '-' ? 				\
		true : false;
}
