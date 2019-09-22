#include "dictionary.h"
#include "dictionary.c"
#include <string.h>

// checks if a word is correctly spelled
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {
    return 0;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    
    // open the file for reading
    FILE * in_file = fopen(dictionary_file, "r" );

    //check if file exists
    if (in_file != NULL && hashtable != NULL) {

        // maximum length for a word is 45 * 1 byte
        char line[45];

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

// ties above funcs together
// word param is file to pointer containing lines of words separated by spaces, punctuation, etc.
bool check_word(const char* word, hashmap_t hashtable[]) {
    return false;
}

// test main - DOES NOT NEED TO BE SUBMITTED
int main() {
    // printf(check_words());
    load_dictionary("wordlist.txt", hashtable);
    // printf(check_word());
    return 0;
}