#include "lib.h"


unsigned int position = 0;

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

bool check_digits (char *exp) {
	if (((exp[position] >= '0' && exp[position] <= '9') || exp[position] == '.') && (position != 0 ? exp[position - 1] != ')' : true)) 

		return true;

	else if ((exp[position] == '+'|| exp[position] == '-') && 	\
		!(position != 0 && ((exp[position - 1] >= '0' && exp[position - 1] <= '9') || exp[position - 1] == ')')))

		return true;

	else
	       return false;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * parse sign:		checks for + and negative signs through recursion and then	* 
 * 			returns +1 and -1 respectively.					*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */  

signed short int parse_sign (char *exp) {
	if (!(exp[position] == '-' || exp[position] == '+')) {
		return 1;
	} else if (exp[position] == '-') {
		++position;
		return -1 * parse_sign(exp);	
	} else {
		++position;	
		return 1 * parse_sign(exp);
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

long double *parse_digits (char *exp) {
	signed short int sign = parse_sign (exp);
	long double *operand;

	if (!(operand = malloc (sizeof (long double))))
		exit (2);

	*operand = 0;
	
	if (exp[position] == '(') {
		++position;
		*operand = sign * expression (exp);
		return operand;
	}

	bool decimal = false;
	unsigned int decimal_part = 0;

	for (; (exp[position] >= '0' && exp[position] <= '9') || exp[position] == '.'; position++) {
		if (exp[position] != '.') {
			*operand *= 10;
			*operand += exp[position] - '0';	
			if (decimal)
				++decimal_part;
		} else if (!decimal) {
			decimal = true;
		} else { 
			exit (3);
		}
	}
	
	*operand = sign * (*operand / powl (10.0, (long double) decimal_part));
	
	switch (exp[position]) {
		case 'e': case 'E':
			++position;
			*operand *= powl (10.0, *parse_digits (exp));
	}

	return operand;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * set operation value	this function takes the parameters that needed to assigned	* 
 * 			to operator and then assign to it.  since a pointer to the 	*
 * 			exptree is passed the changes happen in real time.		*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void set_operation_value (exptree *operator, 				\
			  const unsigned short int steps, 		\
			  const bool uniary,	 			\
			  const unsigned short int precedence,		\
			  int value) {

	position = position + steps;
	operator->node.parent.precedence = precedence;
	operator->node.parent.operation = value;
	operator->node.parent.is_unary = uniary;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
  * compare	this function is used to compare strings  at a given position to the 	*
  * 		nth terms 								*
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool compare (char *exp, char *str, unsigned short int I, unsigned short int end) {
	for(int i = 0; I < end; I++, i++)
		if (exp[I] != str[i])
			return false;
	return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * obtain operand &	Both first check if the syntax is valid and the 	*
 * obtain operation	suitable characters exist at their positions. Then 	*
 * 			they parse them and stor them in exptree node at 	*
 * 			its relative positions.					*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct exptree *obtain_operation (char *exp) {
	struct exptree *operator;
	bool child = false;

	if (!(operator = malloc (sizeof (exptree))))
		exit (2);

	operator->is_child = false;	
	switch (exp[position]) {
		case '+': 
			set_operation_value (operator, 1, false, 1, 1);
			break;
		case '-':
			set_operation_value (operator, 1, false, 1, 2);
			break;
		case '*':
			set_operation_value (operator, 1, false, 2, 3);
			break;	
		case '/':
			set_operation_value (operator, 1, false, 2, 4);
			break;
		case '^':
			set_operation_value (operator, 1, false, 3, 5);
			break;
		case 'v':
			set_operation_value (operator, 1, false, 3, 6);
			break;
		case 's':
			if (compare (exp, "sqrt", position, 4)) {
				set_operation_value (operator, 4, true, 4, 7);
				break;
			}
		case '(':
			set_operation_value (operator, 1, false, 2, 3);
			break;
		default:
			exit (4);
	}
	
	return operator;
}

// see above comment
struct exptree *obtain_operand (char *exp) {	
	exptree *operand; 
	if (!(operand = malloc (sizeof (exptree))))
		exit (2);

	operand->is_child = true;
	operand->node.child.operand = parse_digits (exp);

	return operand;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * node			as the name sugguests it checks if the node is an digit		*
 * 			or an operand or \0 and hence returns it accordingly		*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        
struct exptree *node (char *exp) {
	exptree *n;
	if (exp[position] == '\0') 
		return NULL;
	else if (exp[position] == ')') {
		if (exp[position + 1] >= '0' && exp[position + 1] <= '9') {
	       		exit (7);
		}	

		return NULL;
	} else if (check_digits (exp) ||											\
		  (exp[position] == '(' && !(position != 0 && exp[position - 1] >= '0' && exp[position - 1] <= '9')))

		return 	n = obtain_operand (exp);
	else
		return n = obtain_operation (exp);
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

struct exptree *top_node (exptree *);

struct exptree *uni_list (exptree *a, exptree *b, char *exp) {
	if (b->is_child) {
		a->node.parent.rvalue = NULL;
		a->node.parent.lvalue = b;
		b->above = a;
		top_node (a);

		return a;
	}

	a->node.parent.lvalue = b;
	b->above = a;
	a->node.parent.rvalue = NULL;

	return uni_list (b, node (exp), exp);
}

struct exptree *node_pair (char *exp) {
	exptree *a, *b;

	if(!(a = node (exp))) {
		return NULL;
	
	} else if ((!a->is_child && !a->node.parent.is_unary) || !(b = node (exp))) {			// if operation is obtain first 
		return a;										// there doesn't anything next	
	} else if (a->is_child && b->is_child) {
		exptree *c; 
		if (!(c = malloc (sizeof (exptree))))
			exit (2);

		c->is_child = false;
		c->node.parent.operation = 3;
		c->node.parent.precedence = 2;
		
		c->node.parent.lvalue = a;
		a->above = c;
		c->node.parent.rvalue = b;
		b->above = c;

		return c;
	} else if (a->is_child && !b->node.parent.is_unary) {						// in case of binary operation
		b->node.parent.lvalue = a;
		a->above = b;

		return b;
	
	} else if (a->is_child && b->node.parent.is_unary) {						// in case of <num> <uniary>
		exptree *c; 
		if (!(c = malloc (sizeof (exptree))))
			exit (2);		
		c->is_child = false;
		c->node.parent.operation = 3;
		c->node.parent.precedence = 2;
		
		c->node.parent.lvalue = a;
		a->above = c;
		b->node.parent.lvalue = uni_list (b, node (exp), exp);
		b->node.parent.lvalue->above = b;		
		c->node.parent.rvalue = b;
		b->above = c;
		
		return c;

	} else if (b->is_child && a->node.parent.is_unary) {						// in case of a unarary operation
		a->node.parent.lvalue = b;
		a->node.parent.rvalue = NULL;
		b->above = a;
		
		return a;
	
	} else if (b->node.parent.is_unary && a->node.parent.is_unary) {				// when the unary operation comes
		return uni_list (a, b, exp);	 							// right after unary operation
	
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

struct exptree *make_tree (char *exp, exptree *recent, exptree *old) {
	// base casses 
	
	if (!old && !recent) {						// when both a and b are not existing in case of "" input
		exit (6);						// this one is error
	
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
				return make_tree (exp, recent, old->node.parent.rvalue);
			
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
	
		return make_tree(exp, node_pair (exp), recent);
	
	} else if (old->node.parent.precedence > recent->node.parent.precedence && (old->above))  {		// in case of precedence being
														// lower	
		return make_tree (exp, recent, old->above);
	
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

		return make_tree (exp, node_pair (exp), recent);

	}
}

struct exptree *top_node (exptree *tree) {
	if (!tree->above)
		return tree;
	else
		return top_node (tree->above);
}

long double calculate (exptree *tree) {
	if (tree->is_child) 
		return *tree->node.child.operand;
	else if (tree->node.parent.operation == 1)
		return calculate (tree->node.parent.lvalue) + calculate (tree->node.parent.rvalue);
	else if (tree->node.parent.operation == 2)
		return calculate (tree->node.parent.lvalue) - calculate (tree->node.parent.rvalue);
	else if (tree->node.parent.operation == 3)
		return calculate (tree->node.parent.lvalue) * calculate (tree->node.parent.rvalue);
	else if (tree->node.parent.operation == 4)
		return calculate (tree->node.parent.lvalue) / calculate (tree->node.parent.rvalue);
	else if (tree->node.parent.operation == 5)
		return powl (calculate (tree->node.parent.lvalue), calculate (tree->node.parent.rvalue));
	else if (tree->node.parent.operation == 6)
		return powl (calculate (tree->node.parent.lvalue), 1 / calculate (tree->node.parent.rvalue));
	else if (tree->node.parent.operation == 7)
		return sqrtl (calculate (tree->node.parent.lvalue));
}

long double expression (char *exp) {
	exptree *tree = top_node (make_tree (exp, node_pair (exp), node_pair (exp)));
	++position;
	return calculate (tree);
}
