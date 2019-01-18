/* CS261 - Assignment 5: Part 3 */
/* Name: Rebecca L. Taylor
 *       tayloreb@oregonstate.edu
 * Date: 4 June 2018
 * Solution description: Spell Checker
 * Spell checker prompts user for word(s) and uses a hash map to check for
 * dictionary match and provide suggestions for misspelled words */

#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_SUGGEST 5                   //Number of spell checker suggestions

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
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }

            //Make case insensitive
            c = tolower(c);

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
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    // FIXME: implement
    char* currentWord = nextWord(file);
    while (currentWord != 0)
    {  
        hashMapPut(map, currentWord, 0);    //Last param Levenshtein distance
        free(currentWord);        
        currentWord = nextWord(file);
    }
}

/**
 * Additional function to convert the input buffer to lowercase.
 * @param inputBuffer string
 * Resource: Piazza posts @651, @721
 */
int validateInput(char *inputBuffer)
{
    /* Check for upper- and lowercase letters */
    for (int i = 0; i < strlen(inputBuffer); i++)
    {
        char c = inputBuffer[i];

        /* If char is not upper- or lowercase letter */
        if (!isalpha(c))
        {
            return 0;
        }

        /* Make case insensitive and change buffer */
        else
        {
            c = tolower(c);
            inputBuffer[i] = c;
        }
    }

    return 1;
}

/**
 * Additional function to compute the Levenshtein distance.
 * @param string incorrectly spelled word
 * @param string word from dictionary
 * @return calculation of Levenshtein distance
 * Resources: https://en.wikipedia.org/wiki/Levenshtein_distance#Computing_Levenshtein_distance
 *            https://www.lemoda.net/c/levenshtein/ (from Piazza post @645)
 */
int calcLevenshtein(char *wrongWord, char *dictWord)
{
    int wrongWordLength = strlen(wrongWord);
    int dictWordLength = strlen(dictWord);
    int i, j;

    /* Create matrix to store distances between each character */
    int matrix[wrongWordLength + 1][dictWordLength + 1];

    /* Populate rows with row numbers */
    for (i = 0; i <= wrongWordLength; i++)
    {
        matrix[i][0] = i;
    }

    /* Populate columns with column numbers */
    for (i = 0; i <= dictWordLength; i++)
    {
        matrix[0][i] = i;
    }

    /* Compare characters at each position */
    for (i = 1; i <= wrongWordLength; i++)
    {
        char char1 = wrongWord[i-1];             //Store character of wrong word

        for (j = 1; j <= dictWordLength; j++)
        {
            char c2 = dictWord[j-1];             //Store character of dict word

            if (char1 == c2)                     //If characters are equal
            {
                matrix[i][j] = matrix[i-1][j-1]; //Store val of prev position
            }

            else                                 //If characters are not equal
            {
                int delete = matrix[i-1][j] + 1;
                int insert = matrix[i][j-1] + 1;
                int subCost = matrix[i-1][j-1] + 1;
                int min = delete;

                if (insert < min)
                {
                    min = insert;
                }

                if (subCost < min)
                {
                    min = subCost;
                }

                matrix[i][j] = min;              //Assign position min cost to change
            }
        }
    }

    return matrix[wrongWordLength][dictWordLength];
}

/**
 * Additional function to get the Levenshtein distance for each word in map.
 * @param map
 * @param string input buffer
 * Resources: https://en.wikipedia.org/wiki/Levenshtein_distance#Computing_Levenshtein_distance
 *            https://www.lemoda.net/c/levenshtein/ (from Piazza post @645)
 */
void getLevenshtein(struct HashMap *map, char* inputBuffer)
{
    for (int i = 0; i < hashMapCapacity(map); i++)     //Search each bucket
    {
        struct HashLink *current = map->table[i];
        if (current != 0)                              //If link not null
        {
            while (current != 0)                       //Traverse list
            {
                /* Get Levenshtein distance */
                int levDist = calcLevenshtein(inputBuffer, current->key);

                /* Store distance as key value */
                /* Resource for code to change value:
                 * https://github.com/sreil/CS-261/blob/master/Week%207/Program%206/main.c */
                (*hashMapGet(map, current->key)) = levDist;

                current = current->next;
            }
        }
    }
}

/**
 * Additional function to find five closest suggestions to non-matching word
 * and store in array
 * @param map
 * @param suggestions array
 * Resource: https://www.reddit.com/r/javahelp/comments/49h606/finding_smallest_value_of_hashmap_and_returning/
 */
void getSuggestions(struct HashMap *map, char **suggestions)
{
    int levDist = 1;                   //Start looking for words with distance 1
    int maxLevDist = 100;
    int suggestIndex = 0;              //Position in suggestions array

    while (levDist < maxLevDist)
    {
        /* Check dictionary for matching Levenshtein distance */
        for (int i = 0; i < hashMapCapacity(map); i++)
        {
            if (map->table[i] != 0)
            {
                HashLink *current = map->table[i];
                while (current != 0)
                {
                    /* If matching value for Levenshtein distance */
                    if (*hashMapGet(map, current->key) == levDist)
                    {
                        /* Copy dictionary word to suggestions array */
                        char *addWord = current->key;
                        suggestions[suggestIndex] = malloc(sizeof(char) * strlen(addWord)+1);
                        strcpy(suggestions[suggestIndex], addWord);

                        /* Increment suggestions index */
                        suggestIndex++;
                        if (suggestIndex >= NUM_SUGGEST)
                        {
                            return;
                        }
                    }
                    current = current->next;
                }
            }
        }

        levDist++;                     //Increment if no match found
    }

}

/**
 * Additional function to print suggestions for misspelled words.
 * @param map
 */
void printSuggestions(char **suggestions)
{
    for (int i = 0; i < NUM_SUGGEST; i++)
    {
        printf("%i: %s\n", i+1, suggestions[i]);
    }
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
    HashMap* map = hashMapNew(1000);
    
    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);
    
    char inputBuffer[256];
    int quit = 0;
    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);
        
        // Implement the spell checker code here..
        /* Validate input */
        int valid = validateInput(inputBuffer);
        while (!valid)
        {
            printf("Oops, invalid input.\n");
            printf("Enter a word (upper- and lowercase only) or \"quit\" to quit: ");

            scanf("%s", inputBuffer);
            valid = validateInput(inputBuffer);
        }

        /* Check for correct spelling */
        if (hashMapContainsKey(map, inputBuffer))
        {
            printf("The inputted word \"%s\" is spelled correctly\n", inputBuffer);
        }

        else /* If word not found in dictionary */
        {
            /* Add Levenshtein distance as value in map */
            getLevenshtein(map, inputBuffer);

            /* Get and suggest five closest matches */
            char **suggestions = (char **)malloc(NUM_SUGGEST * sizeof(char*));
            assert (suggestions != 0);

            getSuggestions(map, suggestions);

            printf("Word \"%s\" spelled incorrectly. Did you mean...?\n", inputBuffer);
            printSuggestions(suggestions);

            /* Free suggestions array */
            for (int i = 0; i < NUM_SUGGEST; i++)
            {
                free(suggestions[i]);
            }
            free(suggestions);
        }

        /* 0: Quit if user specifies */
        if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
        }
    }

    hashMapDelete(map);
    return 0;
}
