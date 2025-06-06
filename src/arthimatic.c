#include "header.h"

void increment (void) {
		FILE *numFile = openFile ("number", "rwb");
		FILE *signFile = openFile ("sign", "rwb");
		CHAR sign = getc (signFile);
		BYTE digit;
		bool round = false;

		//fseek (numFile, -1, SEEK_END);
		digit = getc (numFile);
		printf ("%d\n", digit);
		++digit;
		printf ("%d\n", digit);
		//fseek (numFile, 1, SEEK_SET);
		fputc (digit, numFile);
			
		closeFiles (2, numFile, signFile);
}

int main (void) {
		increment ();

		return 0;
}
