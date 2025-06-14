#include "header.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define LONGINT uint64_t

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
				fprintf (stderr, "Syntax Error: More than one exponents.\n");
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
