#include "lib.h"

typedef struct exptree {
	bool is_child;
	struct exptree *above;
	union {
		struct {
			bool is_unary;
			unsigned short int precedence;
			unsigned short int operation;
			struct exptree *rvalue;
			struct exptree *lvalue;
		} parent;

		struct {
			long double *operand;
		} child;	
	} node;

} exptree;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * check operand	returns true and false if the expression is starting 	*
 * 			with '+', '-', '.' or is between 0 and 9 		*
 * 			chars.							*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

bool check_digits (char *exp, unsigned int *index) {
	if ((exp[*index] >= '0' && exp[*index] <= '9') || exp[*index] == '.') 
		return true;
	else if ((exp[*index] == '+'|| exp[*index] == '-') && !(exp[*index - 1] >= '0' && exp[*index - 1] <= '9'))
		return true;
	else
	       return false;	
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

// This function is used with obtain operation
void set_operation_value (exptree *operator, 				\
			  int *index, 					\
			  const unsigned short int steps, 		\
			  const bool uniary,	 			\
			  const unsigned short int precedence,		\
			  int value) {

	*index = *index + steps;
	operator->node.parent.precedence = precedence;
	operator->node.parent.operation = value;
	operator->node.parent.is_unary = uniary;
}

bool compare (char *exp, char *str, unsigned int index, unsigned short int end) {
	for(int i = 0; index < end; index++, i++)
		if (exp[index] != str[i])
			return false;
	return true;
}

struct exptree *obtain_operation (char *exp, int *index) {
	struct exptree *operator;
	bool child = false;

	if (!(operator = malloc (sizeof (exptree))))
		exit (2);

	operator->is_child = false;	
	switch (exp[*index]) {
		case '+': 
			set_operation_value (operator, index, 1, false, 1, 1);
			break;
		case '-':
			set_operation_value (operator, index, 1, false, 1, 2);
			break;
		case '*':
			set_operation_value (operator, index, 1, false, 2, 3);
			break;	
		case '/':
			set_operation_value (operator, index, 1, false, 2, 4);
			break;
		case '^':
			set_operation_value (operator, index, 1, false, 3, 5);
			break;
		case 'v':
			set_operation_value (operator, index, 1, false, 3, 6);
			break;
		case 's':
			if (compare (exp, "sqrt", *index, 4)) {
				set_operation_value (operator, index, 4, true, 4, 7);
				break;
			}
		default:
			exit (4);
	}
	return operator;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * obtain operand &	Both first check if the syntax is valid and the 	*
 * obtain operation	suitable characters exist at their positions. Then 	*
 * 			they parse them and stor them in exptree node at 	*
 * 			its relative positions.					*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct exptree *obtain_operand (char *exp, int *index) {	
	exptree *operand; 
	if (!(operand = malloc (sizeof (exptree))))
		exit (2);
	
	operand->is_child = true;
	operand->node.child.operand = parse_digits (exp, index);

	return operand;
}

// makes a node and return it
struct exptree *make_node (char *exp, int *index) {
	exptree *n;
	if (exp[*index] == '\0')
		return NULL;
	else if (check_digits (exp, index))
		return 	n = obtain_operand (exp, index);
	else
		return n = obtain_operation (exp, index);
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * expression tree:	This is the core of function of this program as it 	*
 * 			generates the tree. Two pair of nodes having an 	*
 * 			operation and right operand is created and stored in 	*
 * 			node array of three elements. The first contain initial,* 	
 * 			second current and third points to the top most element.*	 
 * 			The insertion is done comparing the precedences on the	*
 * 			nodes with each other. The lower precedences nodes are 	*
 * 			inserted above and higher precedences nodes are 	*
 * 			inserted bellow. It's better to understand to insertion	*
 * 			through diagram.					*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct exptree *expression_tree (char *exp, int *index, exptree *nodeB, exptree *nodeA) {
	exptree *cojoined_node;
	if (!(cojoined_node = malloc (sizeof (exptree))))
		exit (2);

	if (!nodeB) {
		return nodeA;
	} else if (nodeA->is_child && !nodeB->node.parent.is_unary) {
		cojoined_node = nodeB;
		cojoined_node->node.parent.lvalue = nodeA;
		return cojoined_node;
	} else if (nodeB->is_child && nodeA->node.parent.is_unary) {
		cojoined_node = nodeA;
		cojoined_node->node.parent.lvalue = nodeB;
		return cojoined_node;
	}
	
}

long double evaltree (exptree *tree) {
	if (tree->is_child)
		return *tree->node.child.operand;
	else 
		switch (tree->node.parent.operation) {
			case 1:
				return evaltree (tree->node.parent.lvalue) + evaltree (tree->node.parent.rvalue);
				break;
			case 2:
				return evaltree (tree->node.parent.lvalue) - evaltree (tree->node.parent.rvalue);
				break;
			case 3:
				return evaltree (tree->node.parent.lvalue) / evaltree (tree->node.parent.rvalue);
				break;
			case 4:
				return evaltree (tree->node.parent.lvalue) * evaltree (tree->node.parent.rvalue);
				break;
			default:
				exit (5);
		}
}

void print_nodes (exptree *tree) {
	if (tree->is_child) {
		printf ("%Lf\n", *tree->node.child.operand);
	} else {
		print_nodes (tree->node.parent.lvalue);
		print_nodes (tree->node.parent.rvalue);
	}
}

void expression (char *exp) {
	unsigned int exp_index = 0;
	exptree *tree;
	if (!(tree = malloc (sizeof (exptree))))
		exit (2);
	tree = expression_tree (exp, &exp_index, make_node(exp, &exp_index), make_node(exp, &exp_index));
	print_nodes (tree);
}
