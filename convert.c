#include "convert.h"

/**
 * Converts a given integer to a char* array of characters containing the binary
 * conversion.
 * @param input
 * @return char* of size 32
 */
char* convertIntToBinary(int input)
{
    //initialize variables
    int x;
    int remainder;
    int digit = 2;                  //represents which digit the function is calculating
    char* ptr = (char*) malloc(32); //allocation of array
    
    //algorithm utilizes the modulus function and remainder to compute the binary number
    for( x = MAX-32; input != 0; x--)
    {
        remainder = input%digit;
        if( remainder != 0 )
        {
            input = input - remainder;
            *(ptr+x) = '1';
        }
        else
        {
            *(ptr+x) = '0';
        }
        digit = digit*2;
    }
    return ptr;
}

/**
 * Converts a given binary char* array to a hex char* array.
 * @param binary
 * @return char* of size 8
 */
char* convertBinaryToHex(char* binary)
{
    //initialize variables
    int x;
    int counter;
    int hex;
    char* ptr = malloc(8);
    
    for(x = MAX-1;x >= 0;x = x - 4)
    {
        counter = ((x+1)/4)-1;
        //essentially adds up the binary number
        hex = atoi(binary+x-3)*8 + atoi(binary+x-2)*4 + atoi(binary+x-1)*2 + atoi(binary+x);
        
        //switch statement for the other hex values, could use array;
        switch( hex )
        {
            case 15:
                *(ptr+counter) = 'F';
                break;
            case 14:
                *(ptr+counter) = 'E';
                break;
            case 13:
                *(ptr+counter) = 'D';
                break;
            case 12:
                *(ptr+counter) = 'C';
                break;
            case 11:
                *(ptr+counter) = 'B';
                break;
            case 10:
                *(ptr+counter) = 'A';
                break;
            default:
                *(ptr+counter) = (char)hex;
        }
    }
    return ptr;
}

/**
 * Converts a given integer to a hex char* array using the given functions above.
 * @param input
 * @return char* of size 8
 */
char* convertIntToHex(int input)
{
    return convertBinaryToHex(convertIntToBinary(input));
}