#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define SYNTX_ERR       1
#define MEM_FAIL        2
#define UNCOND_IF_ERR   3

#define NUMBER 	    long double
#define LONGINT     uint64_t
#define SMALINT     uint8_t

char *expression;
LONGINT I = 0;

// OPER_VAL defines what operations are being used and PRE-
// CEDENCE defines when the operation is going to be execu- 
// ted. priorities are reversed.
enum OPER_VAL {SUM, SUB, MUL, DIV, POSITIVE, NEGATIVE};
enum PRECEDENCE {FIRST, SECOND};
enum NODE_TYPE {LEFT, RIGHT};

typedef struct
{
	bool is_binary;
	SMALINT oper_val;
	SMALINT precedence;
} Operation;

typedef struct Node
{
    struct Node *Reffer_Node[2];
    bool is_operand;
    union
    {
        Operation *Operation_Node;
        NUMBER *Operand_Node;
    } Branch;
} Node;

// functions
bool is_digit (void); 
NUMBER initial_parse_number (void);
NUMBER *parse_number (void);
bool is_binary_operation (void);
Operation *parse_binary_operation (void); 
bool is_unary_operation (void);
Operation *parse_unary_operation (void);
Node *make_tree (void);
static inline void insert_binary_node (Node *node, Node **top_node);
static inline void insert_end_node (Node *node, Node **top_node);

bool is_digit (void) 
{
	return (expression[I] >= '0' && expression[I] <= '9');
}

// checks if it is a digit or not and then sees if the deci-
// mal point has been passed or not and run two different a-
// lgorithms in each case. It also supports 'e' and 'E' and
// recurses the function once if it's found.

// parse_number should be used as it return the pointer to 
// the number
NUMBER initial_parse_number (void) 
{
	NUMBER number = 0;
	bool decimal = false;
	LONGINT decimalPosition;
	static bool exponent = false;

	while (true) 
	{
		if (is_digit ()) 
		{
			if (decimal) 
			{
				number += (expression[I++] - '0') / powl (10, decimalPosition);
				++decimalPosition;
			}
			else
			{
				number = (number * 10) + (expression[I++] - '0');
			}
		}
		else if (expression[I] == '.')
		{
			if (decimal)
			{
				fprintf (stderr, "initial_parse_number:\nSyntax Error: More than one decimal.\n");
				exit (SYNTX_ERR);
			}
			else
			{
				decimal = true;
				++I;
				decimalPosition = 1;
			}
		}
		else if (expression[I] == 'e' || expression[I] == 'E')
		{
			if (! exponent)
			{
				exponent = true;
				++I;
				number = number * pow (10, initial_parse_number ());
			}
			else
			{
				fprintf (stderr, "intial_parse_number:\nSyntax Error: More than one exponent.\n");
				exit (SYNTX_ERR);
			}
		}
		else
		{
			break;
		}
	}

	return number;
}

NUMBER *parse_number (void)
{
    NUMBER *number = (NUMBER *) malloc (sizeof (NUMBER));
    if (! number)
    {
        fprintf (stderr, "parse_number: number:\nMemory allocation failed.\n");
        exit (MEM_FAIL);
    }
    *number = initial_parse_number ();
    return number;
}

// allocates Operation and assign respective values for each
// respective operations. there are two types of operations:
// binary and unary operations
bool is_binary_operation (void)
{
    switch (expression [I])
    {
        case '+': case '-': case '*': case '/':
            return true;
        default:
            return false;
    }
}

Operation *parse_binary_operation (void) 
{
	Operation *x = (Operation *) malloc (sizeof (Operation));
    if (! x)
    {
        fprintf (stderr, "parse_binary_operation: x:\nMemory allocation failed.\n");
        exit (MEM_FAIL);
    }

	switch (expression[I])
	{
		case '+':
			x->is_binary = true;
			x->oper_val = SUM;
			x->precedence= FIRST;
			break;
		case '-':
			x->is_binary = true;
			x->oper_val = SUB;
			x->precedence = FIRST;
			break;
		case '*':
			x->is_binary = true;
			x->oper_val = MUL;
			x->precedence = SECOND;
			break;
		case '/':
			x->is_binary = true;
			x->oper_val = DIV;
			x->precedence = SECOND; 
			break;
	}

	++I;
	return x;
}

bool is_unary_operation (void)
{
    switch (expression [I])
    {
        case '+': case '-':
            return true;
        default:
            return false;
    }
}

Operation *parse_unary_operation (void)
{
	Operation *x = (Operation *) malloc (sizeof (Operation)); 
    if (! x)
    {
        fprintf (stderr, "parse_unary_operation: x:\nMemory allocation failed.\n");
        exit (MEM_FAIL);
    }
	
    switch (expression[I])
	{
		case '+':
            x->is_binary = false;
            x->oper_val = POSITIVE;
            break;
        case '-':
            x->is_binary = false;
            x->oper_val = NEGATIVE;
            break; 
	}

    ++I;
    return x;
}


// this functions works by switching states:
//      1.  if it's the fist parse or parse after binary o-
//          peration then parse unary operation or number
//      2.  if it's a unary operation expect either and un-
//          ary operation or a number
//      3.  if it's a number then parse a binary operation.
Node *make_tree (void)
{
    static SMALINT state = 0;
    Node *node, *top_node = NULL;

    while (expression [I] != '\0')
    {
    	node = (Node *) malloc (sizeof (Node));
        if (! node)
        {
            fprintf (stderr, "make_tree:node:\nMemory allocation failed.\n");
            exit (MEM_FAIL);
        }

    	switch (state)
    	{
    	    case 0:
    	        if (is_digit ())
    	        {
    	            node->is_operand = true;
    	            node->Branch.Operand_Node = parse_number ();

    	            state = 1;
    	        }
    	        else if (is_unary_operation ())
    	        {
    	            node->is_operand = false;
    	            node->Branch.Operation_Node = parse_unary_operation ();
    	        }
    	        else if (expression [I] == ' ')
    	        {
    	            ++I;
                    break;
    	        }
    	        else
    	        {
    	            fprintf (stderr, "make_tree: case 0:\nSyntax error. The string cannot be parsed.\n");
    	            exit (SYNTX_ERR);
    	        }

                if (top_node == NULL)
                {
                    top_node = node;
                }
                else
                {
                    insert_end_node (node, &top_node);
                }

    	        break;
            
            case 1:
                if (is_binary_operation ())
                {
                    node->is_operand = false;
                    node->Branch.Operation_Node = parse_binary_operation ();
                    insert_binary_node (node, &top_node);
                    state = 0;
                }
    	        else if (expression [I] == ' ')
    	        {
    	            ++I;
                    break;
    	        }
    	        else
    	        {
    	            fprintf (stderr, "make_tree: case 0:\nSyntax error. The string cannot be parsed.\n");
    	            exit (SYNTX_ERR);
    	        }
                    
                break;
        }
    }

    free (node);
    return top_node;
}

static inline void insert_binary_node (Node *node, Node **top_node)
{
    Node *index_node = (*top_node), *previous_node = NULL;
    
    while (true)
    {
        // checks if either:
        //      1. node is operand;
        //      2. it's a unary operation; or
        //      3. its precedence exceeds the precedence of
        //         node
        // and then inserts it. previous node is required 
        // to attach it to the node and establish linkage
        if (index_node->is_operand || (! index_node->Branch.Operation_Node->is_binary) ||               \
            node->Branch.Operation_Node->precedence <= index_node->Branch.Operation_Node->precedence    )
        {
            node->Reffer_Node[LEFT] = index_node;
            if (previous_node != NULL)
            {
                previous_node->Reffer_Node[RIGHT] = node;
            }
            break;
        }
        else if (node->Branch.Operation_Node->precedence > index_node->Branch.Operation_Node->precedence)
        {
            previous_node = index_node;
            index_node = index_node->Reffer_Node[RIGHT];
        }
        else
        {
            fprintf (stderr, "insert_binary_mode:\nUnspecified codition met.\n");
            exit (UNCOND_IF_ERR);
        }
    }

    // checks if the insertion was at front of the node
    if (index_node == *top_node)
    {
        (*top_node) = node;
    }
    
}

static inline void insert_end_node (Node *node, Node **top_node)
{
    Node *index_node = (*top_node);
    while (index_node->Reffer_Node[RIGHT])
    {
        index_node = index_node->Reffer_Node[RIGHT];
    }
    index_node->Reffer_Node[RIGHT] = node;
}

NUMBER evaluate_tree (Node *node)
{
    NUMBER result;

    if (node->is_operand)
    {
        return *(node->Branch.Operand_Node);
    } else
    {
        switch (node->Branch.Operation_Node->oper_val)
        {
            case SUM:
                return evaluate_tree (node->Reffer_Node[LEFT]) + evaluate_tree (node->Reffer_Node[RIGHT]); 
                break;
            case SUB:
                return evaluate_tree (node->Reffer_Node[LEFT]) - evaluate_tree (node->Reffer_Node[RIGHT]); 
                break;
            case MUL:
                return evaluate_tree (node->Reffer_Node[LEFT]) * evaluate_tree (node->Reffer_Node[RIGHT]); 
                break;
            case DIV:
                return evaluate_tree (node->Reffer_Node[LEFT]) / evaluate_tree (node->Reffer_Node[RIGHT]); 
                break;
            case POSITIVE:
                return 1 * evaluate_tree (node->Reffer_Node[RIGHT]); 
                break;
            case NEGATIVE:
                return -1 * evaluate_tree (node->Reffer_Node[RIGHT]); 
                break;
        }
    }

}

int main (int argc, char *argv[]) 
{
	expression = argv[1];

	printf ("%Lf\n", evaluate_tree(make_tree ()));

	return 0;
}
