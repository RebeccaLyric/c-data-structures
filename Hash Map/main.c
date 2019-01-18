/* CS261 - Assignment 5: Part 2 */
/* Name: Rebecca L. Taylor
 *       tayloreb@oregonstate.edu
 * Date: 4 June 2018
 * Solution description: Concordance
 * Checks how many times a word (case insensitive) appears in a .txt file */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);

    //Fix me: Do the necessary change to make the implementation //case-insensitive
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||       //if char is a digit
            (c >= 'A' && c <= 'Z') ||       //if char is uppercase
            (c >= 'a' && c <= 'z') ||       //if char is lowercase
            c == '\'')  
        {
            if (length + 1 >= maxLength)    //resize if necessary
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }

            /* Make case insensitive */
            c = (char)tolower(c);

            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    const char* fileName = "input1.txt";
    if (argc > 1)
    {
        fileName = argv[1];
    }
    printf("Opening file: %s\n", fileName);
    
    clock_t timer = clock();
    
    HashMap* map = hashMapNew(10);
    
    // --- Concordance code begins here ---
    // Be sure to free the word after you are done with it here.

    /* Open file and check not null */
    FILE *readFile = fopen(fileName, "r");
    assert (readFile != 0);

    /* 1: Get each word */
    char* currentWord = nextWord(readFile);
    while (currentWord != 0)
    {
        /* 2: If already in map, increment number of occurrences */
        if (hashMapContainsKey(map, currentWord))
        {
            /* Resource for code to change value:
             * https://github.com/sreil/CS-261/blob/master/Week%207/Program%206/main.c */
            (*hashMapGet(map, currentWord))++;
        }

        /* 3: Else, add to hash map with count 1 */
        else
        {
            hashMapPut(map, currentWord, 1);
        }

        /* 4: Free word and get next */
        free(currentWord);
        currentWord = nextWord(readFile);
    }

    /* Close file */
    fclose(readFile);

    // --- Concordance code ends here ---

    hashMapPrint(map);
    
    timer = clock() - timer;
    printf("\nRan in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    printf("Empty buckets: %d\n", hashMapEmptyBuckets(map));
    printf("Number of links: %d\n", hashMapSize(map));
    printf("Number of buckets: %d\n", hashMapCapacity(map));
    printf("Table load: %f\n", hashMapTableLoad(map));
    
    hashMapDelete(map);
    return 0;
}
