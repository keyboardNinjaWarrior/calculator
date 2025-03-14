#include "lib.h"


unsigned int position = 0;

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * the structure of tree consists of a boolian which tells if its end node or not	*  
 * and an above pointer to  itself for  pointing nodes that is its parent. It the 	*
 * consists of union of end and the two types nodes. one is used to store inform-	*
 * ation of  the operation and  the other store  the opointer to the operand. the 	*
 * operation  is either uniary  and binary for binary operations  the pointers to 	*
 * its childern are stored in lvalue and rvalue and for unary, only in its lvalue. 	*
 * i wonder if its more effecient?							*
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

struct exptree *top_node (exptree *);
long double calculate (exptree *);

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * check operand	returns  true if the expression  is starting with a number. It 	*
 * 			also checks if the number comes right after enclosing bracket.	*
 * 			returns false in that case. it also checks the sign and either	*
 * 			is a operator or not in its second condition			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

bool check_digits (char *exp) {
	if (((exp[position] >= '0' && exp[position] <= '9') || exp[position] == '.') && (position != 0 ? exp[position - 1] != ')' : true))							// also see if the number is fol-
		return true;																					//lowed by a bracket 


	else if ((exp[position] == '+'|| exp[position] == '-') && !(position != 0 && ((exp[position - 1] >= '0' && exp[position - 1] <= '9') || exp[position - 1] == ')'))) 			// is it + or - and is the number
																								// coming  before  it is either a
		return true;																					// a number or or a closing brac-
	else																							// ket

	       return false;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * parse sign:		if at given position, there is + or - it calls itself recursiv- *
 * 			ely and multiplies itself with 1 or -1 				*
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
 * parse digits:	it takes an expression. then it allocates memory for storing 	*
 * 			the long double. it loops over  the characters and adds them 	*
 * 			to the  variable  and multiplies  it with ten.  meanwhile it  	*
 * 			looks for the decimal and if its there, it starts adding the 	*
 * 			digits  after the decimal. at the end it divides  the number 	*
 * 			with  it. it also  evaluates expression given in  scientific 	*
 * 			notation through recursion. if there comes bracket it calles	*
 * 			the expression recursively.					*
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
 * 			exptree is passed the changes happen in real time. it been	*
 * 			called again and again and hence deserves a seperate fuct-	*
 * 			ion.								*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void set_operation_value (exptree *operator, const unsigned short int steps, const bool uniary,	const unsigned short int precedence, int value) {

	position = position + steps;
	operator->node.parent.precedence = precedence;
	operator->node.parent.operation = value;
	operator->node.parent.is_unary = uniary;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
  * compare	this function is used to compare strings at a given position to the 	*
  * 		'end'th terms. takes position as I.  return true if same and  false 	*
  * 		if not									*
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool compare (char *exp, char *str, unsigned short int I, unsigned short int end) {
	for(int i = 0; I < end; I++, i++)
		if (exp[I] != str[i])
			return false;
	return true;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * obtain operation		it returns a pointer to exptree containing relevant	*
 * 				information  to it. the operations implemented  are	*
 * 				addition, subtraction, division, multiplication, p-	*
 * 				ower, root,  trignomatic functions, their inverses,	*
 * 				natural and common log.					*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
			if (compare (exp, "sqrt", position, 4)) 
				set_operation_value (operator, 4, true, 4, 7);
			else if (compare (exp, "sin", position, 3))
				set_operation_value (operator, 3, true, 4, 8);
			else if (compare (exp, "sec", position, 3))
				set_operation_value (operator, 3, true, 4, 9);
			else
				exit (4);
			break;
		case 'c':
			if (compare (exp, "cos", position, 3))
				set_operation_value (operator, 3, true, 4, 10);
			else if (compare (exp, "csc", position, 3))
				set_operation_value (operator, 3, true, 4, 11);
			else if (compare (exp, "cot", position, 3))
				set_operation_value (operator, 3, true, 4, 16);
			else 
				exit (4);
			break;
		case 'a':
			if (compare (exp, "acos", position, 4))
				set_operation_value (operator, 4, true, 4, 12);
			else if (compare (exp, "asin", position, 4))
				set_operation_value (operator, 4, true, 4, 13);
			else if (compare (exp, "atan", position, 4))
				set_operation_value (operator, 4, true, 4, 14);
			else
				exit (4);
			break;
		case 't':
			if (compare (exp, "tan", position, 3))
				set_operation_value (operator, 3, true, 4, 15);
			else
				exit (4);
			break;
		case 'e':
			if (compare (exp, "exp", position, 3))
				set_operation_value (operator, 3, true, 4, 17);
			else
				exit (4);	
			break;
		case 'l':
			if (compare (exp, "ln", position, 2))
				set_operation_value (operator, 2, true, 4, 18);
			else if (compare (exp, "log", position, 3))
				set_operation_value (operator, 3, true, 4, 19);
			else
				exit (4);
			break;
		default:
			exit (4);
	}
	
	return operator;
}


 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * obtain operand		it returns a pointer to exptree containing operand node	*
 * 				by calling parse digits function.			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
 * 			or an operand or \0 or a bracket.				*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        
struct exptree *node (char *exp) {

	if (exp[position] == '\0') 
		return NULL;
	else if (exp[position] == ')') {								// checks if the bracket is followed by
		if (exp[position + 1] >= '0' && exp[position + 1] <= '9') {				// a number
	       		exit (7);
		}	
		return NULL;
	} else if (check_digits (exp) || (exp[position] == '('))
		return obtain_operand (exp);
	else
		return obtain_operation (exp);
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *	*
 * node pair		this  function  makes a pointer and  then compare two nodes	*
 * 			which are now made inside the  function if the operation is 	*
 * 			unary  than  it  cojoines the  node b with a and if its not  	*
 * 			unary then  it  cojoins a with b  on their left side. or if	* 
 * 			both are numbers then they are multiplied togather. it also	*
 * 			handles  if  the  number  has  come  first  and  then unary 	*
 * 			operator							*
 * 											*
 * uniary operation 	this function is a recursive function to join multiple unia-	*
 * list			ry operators togather and is used in conjuction with node pair	*
 * 											*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 


struct exptree *uni_list (exptree *a, exptree *b, char *exp) {
	if (b->is_child) {
		a->node.parent.rvalue = NULL;
		a->node.parent.lvalue = b;
		b->above = a;
		a = top_node (a);

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
		exptree *c, *d; 
		if (!((c = malloc (sizeof (exptree))) && (d = malloc (sizeof (exptree)))))
			exit (2);

		c->is_child = false;
		c->node.parent.operation = 3;
		
		c->node.parent.lvalue = a;
		a->above = c;
		c->node.parent.rvalue = b;
		b->above = c;

		d->is_child = true;
		if (!(d->node.child.operand = malloc (sizeof (long double))))
			exit (2);
		
		*d->node.child.operand = calculate (c);	
		
		return d;

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
		b = uni_list (b, node (exp), exp);
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

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *	*
  * insert rvalue	inserts the value at the end the tree to the very right		*

  * make tree		it compares the opertions togather and then inserts the		*
  * 			nodes accordingly. the description are given by the co-		*
  * 			ditions								*
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *	*/

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
		recent->above = old;

		return make_tree (exp, node_pair (exp), old);
	
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
			
			if (!old->node.parent.rvalue->node.parent.is_unary && (recent->node.parent.lvalue)) {				// when an operation is placed betw-
				insert_rvalue (recent->node.parent.lvalue, old);
				recent->node.parent.lvalue = old->node.parent.rvalue;							// een unary and lower precedence t-
				recent->node.parent.lvalue->above = recent;								// he unary must come on left
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


 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *	*
  * top node		goes to the top of the tree and returns the top node to it 	*
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */	

struct exptree *top_node (exptree *tree) {
	if (!tree->above)
		return tree;
	else
		return top_node (tree->above);
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *	
  *  calculate 	recursively evaluate the tree... 					*
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
	else if (tree->node.parent.operation == 8)
		return sinl (calculate (tree->node.parent.lvalue));
	else if (tree->node.parent.operation == 9)
		return 1 / (cosl (calculate (tree->node.parent.lvalue)));
	else if (tree->node.parent.operation == 10)
		return cosl (calculate (tree->node.parent.lvalue));
	else if (tree->node.parent.operation == 11)
		return 1 / (sinl (calculate (tree->node.parent.lvalue)));
	else if (tree->node.parent.operation == 12)
		return acosl (calculate (tree->node.parent.lvalue));
	else if (tree->node.parent.operation == 13)
		return asinl (calculate (tree->node.parent.lvalue));
	else if (tree->node.parent.operation == 14)
		return atanl (calculate (tree->node.parent.lvalue));
	else if (tree->node.parent.operation == 15)
		return tanl (calculate (tree->node.parent.lvalue));
	else if (tree->node.parent.operation == 16)
		return 1 / (tanl (calculate (tree->node.parent.lvalue)));
	else if (tree->node.parent.operation == 17)
		return expl (calculate (tree->node.parent.lvalue));
	else if (tree->node.parent.operation == 18)
		return logl (calculate (tree->node.parent.lvalue));
	else if (tree->node.parent.operation == 19)
		return log10l (calculate (tree->node.parent.lvalue));
	else
		exit (8);
}

long double expression (char *exp) {
	exptree *tree = top_node (make_tree (exp, node_pair (exp), node_pair (exp)));
	if (exp[position] == ')')
		++position;
	
	return calculate (tree);
}
