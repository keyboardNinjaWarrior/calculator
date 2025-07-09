#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define SYNTX_ERR 1

#define NUMBER 	long double
#define LONGINT uint64_t
#define SMALINT uint8_t

char *expression;
LONGINT I = 0;

enum OPER_VAL {SUM, SUB, MUL, DIV};
enum PRECEDENCE {SECOND, FIRST};

typedef struct
{
	bool is_binary;
	SMALINT oper_val;
	SMALINT precedence;
} Operation;

bool is_digit (void) 
{
	return (expression[I] >= '0' && expression[I] <= '9');
}

NUMBER parse_number (void) 
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
				fprintf (stderr, "Syntax Error: More than one decimal.\n");
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
				number = number * pow (10, parse_number ());
			}
			else
			{
				fprintf (stderr, "Syntax Error: More than one exponent.\n");
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

Operation *parse_binary_operation (void) 
{
	Operation * x = (Operation *) malloc (sizeof (Operation));
	switch (expression[I])
	{
		case '+':
			x->is_binary = true;
			x->oper_val = SUM;
			x->precedence= SECOND;
			break;
		case '-':
			x->is_binary = true;
			x->oper_val = SUB;
			x->precedence = SECOND;
			break;
		case '*':
			x->is_binary = true;
			x->oper_val = MUL;
			x->precedence = FIRST;
			break;
		case '/':
			x->is_binary = true;
			x->oper_val = DIV;
			x->precedence = FIRST; 
			break;
		default:
			free (x);
			fprintf (stderr, "Unkown Operation.");
			exit (SYNTX_ERR);
			break;
	}

	return x;
}

int main (int argc, char *argv[]) {
	expression = argv[1];
	
	printf ("%Lf\n", parse_number ());	

	return 0;
}
