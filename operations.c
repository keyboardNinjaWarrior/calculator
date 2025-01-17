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
	if (power == 0) {
		return 1;
	} else if (power == 1) {
		return a;
	} else if (power > 1) {
		a = a * pow_ (a, power - 1);
	} else if (power < 1) {
		a = 1/a;
		pow_ (a, -power);
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

float bin_ (float a) {
       float integer = 0;
       float decimal = 0;
       for (int x = (int) a, i = 0; x > 0; x /= 2, i++) {
               integer += (x % 2) * pow_ (10, i);      
       }

       for (float x = a - (int) a, i = 1; i < 5; i++) {
               x = x * 2;
               decimal += (int) x * pow_ (10, -i);
               x -= (int) x;
       }

       return (integer + decimal);
}

int main (void) {
	float a;
	scanf("%f", &a); 
	printf("%f\n", bin_ (a));
	return 0;
}
