 /* * * * * * * * * * * * * * * * * * * *
  * Name:	Nadeem Anwar		*
  * Language:	C			*
  * Date:	08 January, 2025	*
  * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>

float sum_ (float a, float b) {
	return a + b;
}

float sub_ (float a, float b) {
	return a - b;
}

float mul_ (float a, float b) {
	return a * b;
}

float div_ (float a, float b) {
	return a / b;
}

float pow_ (float a, int power) {
	if (power == 1) {
		return a;
	} else {
		a = a * pow_ (a, power - 1);
	}
}

/* This root function is created by Newton-Ralphson method */

/* * * * * *  * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * I wanted to create a recursive function but that would	*
 * require intialization in parameters that C language doesn't	*
 * support. It would become very complicated 			*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */  
 
float root_ (float a, int x) {
	float num = a;
	float r = 0;
	do {
		r = ( (pow_ (a, x) - num) / (x * pow_ (a, x - 1)) );
		a = a - r;
	} while (r > 0.0001f);
	return a;
}

int fact_ (int a) {
	if (a == 1) {
		return a;
	} else {
		a = a * fact_ (a - 1);
	}
}

int main (void) {
	printf("%d\n", fact_ (10));
	return 0;
}
