/* CS261 - Assignment2 */
/* Name: Rebecca L. Taylor
 * Date: 23 April 2018
 * Solution description: Stack Application:
   Functions to return the next character in a string and
   check for whether the parentheses of a string are balanced
   using a dynamic array stack. */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dynArray.h"

/* ***************************************************************
Using stack to check for unbalanced parentheses.
***************************************************************** */

/* Returns the next character of the string, once reaches end return '0' (zero)
	param: 	s pointer to a string
	pre: s is not null
*/
char nextChar(char* s)
{
    static int i = -1;
    char c;
    ++i;
    c = *(s+i);
    if ( c == '\0' )
        return '\0';
    else
        return c;
}

/* Checks whether the (), {}, and [] are balanced or not
	param: 	s pointer to a string
	pre: s is not null
	post:
*/
int isBalanced(char* s)
{
    /* FIXME: You will write this function */
    /* Check that s is not null */
    assert(s != 0);

    /* Allocate a stack used to store closing parentheses: ), }, and ]*/
    DynArr *charStack = newDynArr(5);

    char temp = nextChar(s);           //Variable for next character in the string
    int balanced = 1;                  //Initial state of balance set to 1 (true)

    /* Read string until reached null character */
    while (temp != '\0')
    {
        /* If reach open parenthesis, push matching closing paren onto stack */
        if (temp == '(')
        {
            pushDynArr(charStack, ')');
        }

        if (temp == '{')
        {
            pushDynArr(charStack, '}');
        }

        if (temp == '[')
        {
            pushDynArr(charStack, ']');
        }

        /* When reach closing parenthesis */
        if (temp == ')' || temp == '}' || temp == ']')
        {
            /* If matching closing paren */
            if (temp == topDynArr(charStack))
            {
                popDynArr(charStack);  //Pop from stack
            }

            /* If top of stack does not match closing paren */
            else if (temp != topDynArr(charStack))
            {
                balanced = 0;          //Set balanced to 0 (false)
            }

            /* If stack is empty (no matching closing paren) */
            else if (isEmptyDynArr(charStack))
            {
                balanced = 0;          //Set balanced to 0 (false)
            }
        }

        /* Get next character */
        temp = nextChar(s);
    }

    /* If stack is not empty by end of string */
    if (!isEmptyDynArr(charStack))
    {
        balanced = 0;                  //Set balanced to 0 (false)
    }

    /* Free character stack and return value of balanced */
    deleteDynArr(charStack);
    return balanced;
}

int main(int argc, char* argv[]){

    char* s=argv[1];
    int res;

    printf("Assignment 2\n");

    res = isBalanced(s);

    if (res)
        printf("The string %s is balanced\n",s);
    else
        printf("The string %s is not balanced\n",s);

    return 0;
}

