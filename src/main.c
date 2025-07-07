#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define SYNTX_ERR 1

#define NUMBER 	long double
#define LONGINT uint64_t

char *expression;
NUMBER parse_number (void);
LONGINT I = 0;

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

void parse_operation (void) 
{
	switch (expression[I])
	{
		case '+':
		case '-':
		case '*':
		case '/':
	}	
}

int main (int argc, char *argv[]) {
	expression = argv[1];
	
	printf ("%Lf\n", parse_number ());	

	return 0;
}
