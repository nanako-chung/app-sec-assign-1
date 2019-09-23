#include "dictionary.h"
#include "dictionary.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {
	int count = 0;

	//check if file exists
	if (fp != NULL && hashtable != NULL) {

		// maximum length for a word is 45 * 1 byte
		char line[LENGTH + 1];

		// read word one by one
		while (fgets(line, sizeof(line), fp) != NULL) {

            // use check_word function
            if (!check_word(line, hashtable)) {
                misspelled[count] = line;
                count += 1;
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
	
	// open the file for reading
	FILE * in_file = fopen(dictionary_file, "r" );

	//check if file exists
	if (in_file != NULL && hashtable != NULL) {

		// maximum length for a word is 45 * 1 byte
		char line[LENGTH + 1];

		// read word one by one
		while (fgets(line, sizeof(line), in_file) != NULL) {
			int index = hash_function(line);
			node * head = hashtable[index];
			node * newNode = malloc(sizeof(node));
			if (newNode == NULL) {
				return false;
			}
			
			for (int i = 0; i < strlen(line); i++) {
				newNode->word[i] = line[i];
			}
			
			newNode->next = NULL;

			// if there is nothing stored at that index yet, add it
			if (head == NULL) {
				head = newNode;
			} else {
				node * current = head;
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
    node * head = hashtable[index];

    if (head != NULL) {
        node * current = head;
        while (current != NULL) {
            if (strcmp(current->word, word) == 0) {
                return true;
            }
        }
    }

	return false;
}

// test main - DOES NOT NEED TO BE SUBMITTED
// int main() {
// 	char * misspelled[MAX_MISSPELLED];
// 	printf("%i\n", check_words(fopen("test_worlist.txt", "r"), hashtable, misspelled));
// 	load_dictionary("wordlist.txt", hashtable);
// 	return 0;
// }