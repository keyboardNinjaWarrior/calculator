#include "header.h"

int main (char argc, char *argv[]) {
	
         /* * * * * * * * * * * * * * * *
	 * '-':	tags intitialized	*
	 * 'e': expression		*
	 * * * * * * * * * * * * * * * */
	if (argv[1][0] == '-') {
		if (argv[1][1] == 'e') {
			expression (argv[2]);
		}

	} else {
		printf("calc: This command is not recognized.\n");
	}

	return 0;
}
