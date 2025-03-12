#include "lib.h"

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * the structure of tree consists of a boolian which tells if its end node or not	*  
 * and an above pointer to itself for pointing nodes that are up. It the consists	*   
 * of union of end and the rest of the nodes. the other operational nodes have an 	*
 * rvalue and an lvalue, the operational integer, its precedence and boolian det-	*
 * ermining if its a unary operator or not.						*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

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

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * check operand	returns true and false if the expression is starting with 	*	
 * 			'+', '-', '.' or is between 0 and 9 chars			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

bool check_digits (char *exp, unsigned int *index) {
	if ((exp[*index] >= '0' && exp[*index] <= '9') || exp[*index] == '.') 

		return true;

	else if ((exp[*index] == '+'|| exp[*index] == '-') && !(*index - 1 >= 0 && exp[*index - 1] >= '0' && exp[*index - 1] <= '9'))

		return true;

	else
	       return false;	
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * parse sign:		checks for + and negative signs through recursion and then	* 
 * 			returns +1 and -1 respectively.					*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */  

signed short int parse_sign (char *exp, unsigned int *start) {
	if (!(exp[*start] == '-' || exp[*start] == '+'))
		return 1;

	else if (exp[*start] == '-') {
		++(*start);
		return -1 * parse_sign(exp, start);	
	} else {
		++(*start);
		return 1 * parse_sign(exp, start);
	}
}


 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * parse digits:	It takes an expression and starting point. Then it allocates 	*
 * 			memory for storing the long double. It loops over the chara-	*
 * 			cters and adds them to the  variable and multiplies  it with 	*
 * 			ten.  Meanwhile it  looks for the  decimal and if its there, 	*
 * 			it starts  adding the digits  after the decimal. At the  end	*
 * 			it divides the number with  it. It also evaluates expression	*
 * 			given in scientific notation through recursion.			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

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
			if (decimal)
				++decimal_part;
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

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * set operation value	this function takes the parameters that needed to assigned	* 
 * 			to operator and then assign to it.  since a pointer to the 	*
 * 			exptree is passed the changes happen in real time.		*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void set_operation_value (exptree *operator, 				\
			  unsigned int *index, 				\
			  const unsigned short int steps, 		\
			  const bool uniary,	 			\
			  const unsigned short int precedence,		\
			  int value) {

	*index = *index + steps;
	operator->node.parent.precedence = precedence;
	operator->node.parent.operation = value;
	operator->node.parent.is_unary = uniary;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
  * compare	this function is used to compare strings  at a given index to the 	*
  * 		nth terms 								*
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool compare (char *exp, char *str, unsigned int index, unsigned short int end) {
	for(int i = 0; index < end; index++, i++)
		if (exp[index] != str[i])
			return false;
	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * obtain operand &	Both first check if the syntax is valid and the 	*
 * obtain operation	suitable characters exist at their positions. Then 	*
 * 			they parse them and stor them in exptree node at 	*
 * 			its relative positions.					*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct exptree *obtain_operation (char *exp, unsigned int *index) {
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

// see above comment
struct exptree *obtain_operand (char *exp, unsigned int *index) {	
	exptree *operand; 
	if (!(operand = malloc (sizeof (exptree))))
		exit (2);
	
	if (exp[*index] == '(') {
		*index = *index + 1;
		// working...
	}

	operand->is_child = true;
	operand->node.child.operand = parse_digits (exp, index);

	return operand;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * node			as the name sugguests it checks if the node is an digit		*
 * 			or an operand or \0 and hence returns it accordingly		*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct exptree *node (char *exp, unsigned int *index) {
	exptree *n;
	if (exp[*index] == '\0' || exp[*index] == ')') 
		return NULL;
	else if (check_digits (exp, index) || exp[*index] == '(')
		return 	n = obtain_operand (exp, index);
	else
		return n = obtain_operation (exp, index);
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *	*
 * node pair		this  function  makes a pointer and  then compare two nodes	*
 * 			which are now made inside the  function if the operation is 	*
 * 			unary  than  it  cojoines the  node b with a and if its not  	*
 * 			unary then  it cojoins  a with b on their left side.		*
 * 											*
 * uniary operation 	this function is a recursive function to join multiple unia-	*
 * list			ry operators togather and is used in conjuction with node pair	*
 * 											*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct exptree *uni_list (exptree *a, exptree *b, char *exp, unsigned int *index) {
	if (b->is_child) {
		a->node.parent.rvalue = NULL;
		a->node.parent.lvalue = b;	

		return a;
	}

	a->node.parent.lvalue = b;
	a->node.parent.rvalue = NULL;

	return uni_list (b, node (exp, index), exp, index);

}

struct exptree *node_pair (char *exp, unsigned int *index) {
	exptree *a, *b;	
	
	if(!(a = node (exp, index))) {
	
		return NULL;

	} else if ((!a->is_child && !a->node.parent.is_unary) || (!(b = node (exp, index)))) {		// if operation is obtain first 
													// and is not unary or the next	
		return a;										// there doesn't anything next
	
	} else if (a->is_child && !b->node.parent.is_unary) {						// in case of binary operation
		b->node.parent.lvalue = a;
		a->above = b;

		return b;
	
	} else if (b->is_child && a->node.parent.is_unary) {						// in case of a unarary operation
		a->node.parent.lvalue = b;
		a->node.parent.rvalue = NULL;
		b->above = a;

		return a;
	
	} else if (b->node.parent.is_unary && a->node.parent.is_unary) {				// when the unary operation comes

		return uni_list (a, b, exp, index); 							// right after unary operation
	
	} else {

		exit (6);
	}
}

void insert_rvalue (exptree *value, exptree *tree) {
	if (!(tree->node.parent.rvalue) && !tree->node.parent.is_unary) {
		tree->node.parent.rvalue = value;
		value->above = tree;
	} else {
		insert_rvalue (value, tree->node.parent.rvalue);
	}
}

struct exptree *make_tree (char *exp, unsigned int *index, exptree *recent, exptree *old) {
	// base casses 
	
	if (!old && !recent) {						// when both a and b are not existing in case of "" input
		exit (6);						// this one is erro
	
	} else if (old && !recent) {					// when there is no next joined node
		return old;
	
	} else if (old && recent->is_child) {				// when the tree already exist and the last element is operand
		old->node.parent.rvalue = recent;
		return old;
	
	} else if (old && !recent) {					// in case of the last term being unary operator
		return old;

	} else if (old->node.parent.precedence < recent->node.parent.precedence) {			// when the precendence of incoming 
		if (old->node.parent.rvalue) {								// node is greater than the node at 
													// the present	
			// not child condition is checked because 
			// if  not then  it may cause  unexpected 
			// error like node.parent not existing

			if (!old->node.parent.rvalue->is_child && 							\
			   (old->node.parent.rvalue->node.parent.precedence < recent->node.parent.precedence)) 
				return make_tree (exp, index, recent, old->node.parent.rvalue);
			
			if (!old->node.parent.rvalue->node.parent.is_unary) {				// when an operation is placed betw-
				recent->node.parent.rvalue = old->node.parent.rvalue;			// een unary and lower precedence t-
				recent->node.parent.rvalue->above = recent;				// he unary must come on left
			} else {
				recent->node.parent.lvalue = old->node.parent.rvalue;
				recent->node.parent.lvalue->above = recent;
			}
		}	

		recent->above = old;
		old->node.parent.rvalue = recent;
	
		return make_tree(exp, index, node_pair (exp, index), recent);
	
	} else if (old->node.parent.precedence > recent->node.parent.precedence && (old->above))  {		// in case of precedence being
														// lower	
		return make_tree (exp, index, recent, old->above);
	
	} else if (old->node.parent.precedence == recent->node.parent.precedence || !(old->above)) {		// for precdence  being equall
		if (recent->node.parent.lvalue) {								// or reaching the heighest
			insert_rvalue (recent->node.parent.lvalue, old);					// node
		}
		
		if (old->above) {
			recent->above = old->above;
			old->above->node.parent.rvalue = recent;
		}

		recent->node.parent.lvalue = old;
		old->above = recent;

		return make_tree (exp, index, node_pair (exp, index), recent);

	}
}


// made on the run to test the tree


void expression (char *exp) {
	unsigned int index = 0;
	exptree *tree;
	print_nodes (make_tree(exp, &index, node_pair (exp, &index), node_pair (exp, &index)));
}
