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
		a = a * pow_ (a, power -1);
	}
}
