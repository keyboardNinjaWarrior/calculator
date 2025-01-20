 /* * * * * * * * * * * * * * * * * * * *
  * Name:	Nadeem Anwar		*
  * Language:	C			*
  * Date:	08 January, 2025	*
  * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#define	PRECISION	0.0001f

float pow_ (float a, int power) {
	if (power == 0) {
		return 1;
	} else if (power == 1) {
		return a;
	} else if (power > 1) {
		a = a * pow_ (a, power - 1);
	} else if (power < 1) {
		pow_ (1/a, -power);
	}
}

/* This root function is created by Newton-Ralphson method */

/* I wanted to create a recursive function but that would
 * require intialization in parameters that C language doesn't
 * support. It would become very complicated */  
 
float root_ (float a, int x) {
	float num = a, r = 0;
	do {
		r = ( (pow_ (a, x) - num) / (x * pow_ (a, x - 1)) );
		a = a - r;
	} while (r > PRECISION);
	return a;
}

int fact_ (int a) {
	if (a == 0) {
		return 1;
	} else if (a == 1) {
		return a;
	} else if (a > 1) {
		a = a * fact_ (a - 1);
	}
}

double bin_ (float a) {
       double integer = 0, decimal = 0;
       for (int x = (int) a, i = 0; x > 0; x /= 2, i++) {
               integer += (x % 2) * pow_ (10, i);      
       }

       for (float x = a - (int) a, i = 1; i < 5; i++) {
               x *= 2;
               decimal += (int) x * pow_ (10, -i);
               x -= (int) x;
       }

       return (integer + decimal);
}

/* The trignomatric fuctions take input in radians.
 * They are also acurate upto 3 decimal places.*/

double sin_ (float x) {
	int term = 0;
        double	r = 0, sine = 0;
	for (int n = 0; (1 / fact_ (term)) > PRECISION; n++) {
		term = (n * 2) + 1;
		r = pow_ (-1, n) / fact_ (term);
		sine += pow_ (x, term) * r;
	}
	return sine;

}

double cos_ (float x) {
	return root_ ((1 - pow_( sin_ (x) , 2 )) , 2 );
}

double tan_ (float x) {
	return sin_ (x) / cos_ (x);
}

int main (void) {
	float a;
	scanf("%f", &a); 
	printf("%f\n", tan_ (a));
	return 0;
}
