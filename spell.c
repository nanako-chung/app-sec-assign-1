#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool trim(char* word) {
	while (!isalnum(word[strlen(word) - 1]) || !isalnum(word[0])) {
		if (strlen(word) == 0) break;
		if (!isalnum(word[0])) memmove(word, word + 1, strlen(word)); 
		if (!isalnum(word[strlen(word) - 1])) word[strlen(word) - 1] = 0;
	}
	
	// faulty detection
	if (strlen(word) == 0) return false;
	if (strcmp(word, "'") == 0) return false;
	return true;
}

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {

	//check if file exists
	if (fp == NULL || hashtable == NULL || misspelled == NULL) {
		return 0;
	}

	char** ptr = misspelled;
	int count = 0;
	char word[LENGTH + 1]; 

	// read word one by one
	while ((fscanf(fp, "%45[\"!-~Ç-■]%*[^\"!-~Ç-■]", word)) != EOF) {
		if (strlen(word) == 0) continue;

		char delimiter[] = "/-";
		char *individual_word = strtok(word, delimiter); 
		while (individual_word != NULL) {
			if (trim(individual_word)) {
				if (!check_word(individual_word, hashtable)) {
					ptr[count] = (char*) malloc(sizeof(individual_word));
					strcpy(ptr[count], individual_word);
					count += 1;
				}
			}

			individual_word = strtok(NULL, delimiter);
		}

		// prevent overflow
		if (count == MAX_MISSPELLED) break;
	}

	//close file
	fclose(fp);
	return count;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {

	// open the file for reading
	FILE * in_file = fopen(dictionary_file, "r");

	//check if file exists
	if (in_file == NULL || hashtable == NULL) {
		return false;
	}

	// initialize hashtable with nulls
	for (int i = 0; i < HASH_SIZE; i++) {
		hashtable[i] = NULL;
	}
	
	// maximum length for a word is 45 * 1 byte
	char line[LENGTH + 1];
	int count = 0;

	// read word one by one
	while (fgets(line, sizeof(line), in_file) != NULL) {
		node * newNode = malloc(sizeof(node));
		if (newNode == NULL) {
			return false;
		}
		
		for (int i = 0; i < strlen(line); i++) {
			if (line[i] != '\n' && line[i]) {
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
		
		count++;

		// prevent overflow
		if (sizeof(node) * count == HASH_SIZE) break;
	}

	//close file
	fclose(in_file);
	return true;
}

bool check_word(const char* word, hashmap_t hashtable[]) {
	char individual_word[LENGTH + 1];
	memcpy(individual_word, word, LENGTH + 1);
	if (trim(individual_word)) {
		int numeric_flag = 0;
		int num_commas = 0;
		int num_decimals = 0;
		for (int i = 0; i < strlen(word); i++) {
			
			// lower case the word because everything in dictionary is lower case
			individual_word[i] = tolower(word[i]);

			if (isdigit(individual_word[i])) numeric_flag++;
			if (individual_word[i] == ',') num_commas++;
			if (individual_word[i] == '.') num_commas++;
		}

		// all numeric with commas, don't add to misspelled - consider th,st,nd,rd and fractions (e.g. 1/10)
		if (numeric_flag == (strlen(individual_word) - num_commas) || numeric_flag == (strlen(individual_word) - num_decimals)) return true;

		int index = hash_function(individual_word);
		node * current = hashtable[index];
		while (current != NULL) {	
			if (strcmp(current->word, individual_word) == 0) {
				return true;
			}

			current = current->next;
		}
	}

	return false;
}