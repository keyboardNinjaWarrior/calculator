#include "header.h"

char *expression;

int main (int argc, char *argv[]) {
	expression = argv[1];
	
	parse_number ();	

	return 0;
}
