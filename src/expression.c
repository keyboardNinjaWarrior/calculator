#include "header.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define NUMBER 	long double
#define LONGINT uint64_t

LONGINT I = 0;

bool is_digit (void) 
{
	return (expression[I] >= '0' && expression[I] <= '9');
}

LONGINT power (LONGINT i)
{
	LONGINT power = 10;
	while (i != 0) {
		power *= 10;
		--i;

	}

	return power;
}

void parse_number (void) 
{
	NUMBER number = 0;
	bool decimal = false;
	LONGINT decimalPosition;

	while (true) 
	{
		if (is_digit ()) 
		{
			if (decimal) 
			{
				number += (expression[I++] - '0') / (NUMBER) (power (decimalPosition - 1));
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
				fprintf (stderr, "Syntax Error: More than one decimal.");
				exit (SYNTX_ERR);
			}
			else
			{
				decimal = true;
				++I;
				decimalPosition = 1;
			}
		} 
		else
		{
			break;
		}
	}
	
	printf("parse_number: %Lf\n", number);
}
