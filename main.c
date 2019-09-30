#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// test main - DOES NOT NEED TO BE SUBMITTED
int main(int argc, char ** argv) {
	if (argc < 3) exit(1);
	node* hashtable[HASH_SIZE];
	if (load_dictionary(argv[2], hashtable)) {
		char* misspelled[MAX_MISSPELLED];
		int num_wrong = check_words(fopen(argv[1], "r, ccs=UTF-8"), hashtable, misspelled);
		for (int i = 0; i < num_wrong; i++) {
			printf("%s\n", misspelled[i]);
		}
	}
	return 0;
}