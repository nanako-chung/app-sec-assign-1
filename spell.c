#include "dictionary.h"
#include "dictionary.c"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {
	int count = 0;

	//check if file exists
	if (fp != NULL && hashtable != NULL) {

		// maximum length for a word is 45 * 1 byte
		char line[LENGTH + 1];

		char delimiter[] = " \t\r\n\v\f.,";

		// change size?
		char * line_data[256];
		int i = 0;
		
		// read word one by one
		while (fgets(line, sizeof(line), fp) != NULL) {
			
			line_data[i] = strtok(line, delimiter); 
			while (line_data[i] != NULL) {

				for (int j = 0; j < strlen(line_data[i]); j++) {
					line_data[i][j] = tolower(line_data[i][j]);
				}

				if (!check_word(line_data[i], hashtable)) {
					misspelled[count] = line_data[i];
					count += 1;
				}

				i++;
				line_data[i] = strtok(NULL, delimiter);
			}
		}
		
		//close file
		fclose(fp);

		// since count kept track of index we have to add 1 to get the length
		return count + 1;
	} 

	return count;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
	// initialize hashtable with nulls
	for (int i = 0; i < HASH_SIZE; i++) {
		hashtable[i] = NULL;
	}

	// open the file for reading
	FILE * in_file = fopen(dictionary_file, "r" );

	//check if file exists
	if (in_file != NULL && hashtable != NULL) {

		// maximum length for a word is 45 * 1 byte
		char line[LENGTH + 1];

		// read word one by one
		while (fgets(line, sizeof(line), in_file) != NULL) {
			node * newNode = malloc(sizeof(node));
			if (newNode == NULL) {
				return false;
			}
			
			for (int i = 0; i < strlen(line); i++) {
				if (line[i] != '\n') {
					newNode->word[i] = tolower(line[i]);
				}
			}

			int index = hash_function(newNode->word);

			newNode->next = NULL;

			// if there is nothing stored at that index yet, add it
			if (hashtable[index] == NULL) {
				hashtable[index] = newNode;
			} else {
				node * current = hashtable[index];
				while (current->next != NULL) {
					current = current->next;
				}
				current->next = newNode;
			}
		}

		//close file
		fclose(in_file);
		return true;
	} 

	return false;
}

bool check_word(const char* word, hashmap_t hashtable[]) {
	int index = hash_function(word);
	node * current = hashtable[index];
	while (current != NULL) {
		if (strcmp(current->word, word) == 0) {
			return true;
		}

		current = current->next;
	}

	return false;
}

// test main - DOES NOT NEED TO BE SUBMITTED
int main() {
	if (load_dictionary("wordlist.txt", hashtable)) {
		char * misspelled[MAX_MISSPELLED];
		printf("%i\n", check_words(fopen("test1.txt", "r"), hashtable, misspelled));
	}
	return 0;
}