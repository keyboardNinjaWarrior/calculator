#include "lib.h"

long double *parse_digits (char *, unsigned int *);
signed short int parse_sign (char *, unsigned int *);
unsigned short int check_operator (char *,unsigned int *);
bool check_digits (char *, unsigned int *);
struct exptree *obtain_operation (char *, int *);
struct exptree *obtain_operand (char *, int *);
struct exptree *expression_tree (char *, int *);	

typedef struct exptree {
	bool is_child;
	struct exptree *above;
	union {
		struct {
			struct exptree *lvalue;
			unsigned short int precedence;
			unsigned short int operation;
			struct exptree *rvalue;
		} parent;

		struct {
			long double *operand;
		} child;	
	} node;

} exptree;

void print_node (exptree *tree) {
	if (tree->is_child) {
		printf ("%Lf\n", *tree->node.child.operand);
	} else {
		print_node (tree->node.parent.lvalue);
		print_node (tree->node.parent.rvalue);
	}
}

void expression (char *exp) {
	unsigned int exp_index = 0;
	exptree *tree = expression_tree (exp, &exp_index);		
	print_node (tree);
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * expression tree:	This is the core of function of this program as it generates 	*
 * 			the tree. Two pair of nodes having an operation and right 	*
 * 			operand is created and stored in node array of three elements. 	*
 * 			The first contain initial, second current and third points 	*
 * 			to the top most element. The insertion is done comparing the 	*
 * 			precedences on the nodes with each other. The lower precedences *	
 * 			nodes are inserted above and higher precedences nodes are 	*
 * 			inserted bellow. It's better to understand to insertion through *	
 * 			diagram.							*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct exptree *expression_tree (char *exp, int *index) {	
	exptree *lop = obtain_operand (exp, index), *rop, *tree[3];

	for (unsigned short int i = 0; exp[*index] != '\0'; i = 1) {
	tree[i] = obtain_operation (exp, index);
	rop = obtain_operand (exp, index);
	
	tree[i]->node.parent.rvalue = rop;
	rop->above = tree[i];

	if (i == 1) {
		if (tree[i]->node.parent.precedence > tree[i - 1]->node.parent.precedence) {
			// adjusting the tree
			tree[i]->node.parent.lvalue = tree[i - 1]->node.parent.rvalue;
			tree[i - 1]->node.parent.rvalue->above = tree[i];
			tree[i - 1]->node.parent.rvalue = tree[i];
			tree[i]->above = tree[i - 1];
			
		} else {
			while ((tree[i - 1]->above) && tree[i]->node.parent.precedence <= tree[i - 1]->above->node.parent.precedence)
				tree[i - 1] = tree[i - 1]->above;
			// adjusting the tree
			tree[i]->node.parent.lvalue = tree[i - 1];
			tree[i]->above = tree[i - 1]->above;
			if (tree[i - 1]->above)
				tree[i - 1]->above->node.parent.rvalue = tree[i];
			tree[i - 1]->above = tree[i];
		}

		tree[i - 1] = tree[i];
		}
	}

	// adjusting the tree array
	tree[2] = tree[0];
	while (tree[2]->above)
		tree[2] = tree[2]->above;

	tree[1] = tree[2];
	while (tree[1]->node.parent.lvalue)
		tree[1] = tree[1]->node.parent.lvalue;
	tree[1]->node.parent.lvalue = lop;

	return tree[2];
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * obtain operand &	Both first check if the syntax is valid and the 	*
 * obtain operation	suitable characters exist at their positions. Then 	*
 * 			they parse them and stor them in exptree node at 	*
 * 			its relative positions.					*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
		//Associates precedence depending on the value of operator
		switch (operator->node.parent.operation) {
			case 1: case 2:
				operator->node.parent.precedence = 1;
				break;
			case 3: case 4:
				operator->node.parent.precedence = 2;
				break;
		}
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

unsigned short int check_operator (char *exp, unsigned int *index) {
	switch (exp[*index]) {
		case '+':
			*index = *index + 1;
			return 1;
		case '-':
			*index = *index + 1;
			return 2;
		case '/':
			*index = *index + 1;
			return 3;	
		case '*':
			*index = *index + 1;
			return 4;
		default:
			return 0;
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
