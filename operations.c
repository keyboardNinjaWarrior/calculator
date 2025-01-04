#include <stdio.h>

float sum (float a, float b) {
	return a + b;
}

float sub (float a, float b) {
	return a - b;
}

float mul (float a, float b) {
	return a * b;
}

float div (float a, float b) {
	return a / b;
}

float sqr (float a) {
	return a * a;
}

float pov (float a, int power) {
	if (power == 1) {
		return a;
	} else {
		a = a * pov (a, power -1);
	}
}

int main (void) {
	printf("%f\n", pov(5.0, 5.0));
	return 0;
}



