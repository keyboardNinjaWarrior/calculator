#include "header.h"

int count_operations (char *);
int expression_length (char *);
void numify (char *exp, int start, int end);


typedef struct {
	int* operand1;
	char operation;
	int* operand2;
} operations;

 /* * * * * * * * * * * * * * * * * * * * * * * *
 * expression:	It evaluates the expression.	*
 * 		Medod used is parsing.		*
 * * * * * * * * * * * * * * * * * * * * * * * */

void expression (char *exp) {
	
	int operation_count = count_operations(exp)
	numify ("123456789", 0, 9);
	printf ("\n");
	printf ("%d\n", expression_length (exp));
	printf ("%d\n", count_operations (exp));

}

long double numify (char *exp, int start, int end) {
	long double number = 0;
	for (int i = start; i < end; i++) {
		number += ((long double)(exp[i] - '0') * powl (10, (end - (i + 1)) ));
	}

	return number;
}

int count_operations (char *exp) {
	int count = 0;
	for (int i = 0; exp[i] != '\0'; i++) {
		switch (exp[i]) {
			case '-': case '+': case '*': case '/':
				++count;
				break;
			default:
				break;
		}
	}

	return count;
}

int expression_length (char *exp) {
	int count = 0;
	for (count; exp[count] != '\0'; ++count)
		;
	return count;
}

operations* operation_array(int number, char *exp) {
	operations array[number];
	for (int i = 0, j = 0; exp[i] != '\0'; i++) {
		switch (exp[i]) {
			case '+': case '*': case '/':
				array[j].operation = exp[i];

		}
	}
}
