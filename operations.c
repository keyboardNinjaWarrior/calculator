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

float sqr_ (float a) {
	return a * a;
}

float pow_ (float a, int power) {
	if (power == 1) {
		return a;
	} else {
		a = a * pow_ (a, power - 1);
	}
}

// This square root is calculated by Babolanion method

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
	//	printf("%f\n", a);
	
	} while (r > 0.00001);
		
	return a;
}

int main (void) {
	printf("%f\n", root_ (27000, 3));
	return 0;
}
