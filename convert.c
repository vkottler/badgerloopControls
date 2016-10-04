#include "convert.h"

/**
 * Converts a given integer to a char* array of characters containing the binary
 * conversion.
 * @param input
 * @return 
 */
char* convertIntToBinary(int input)
{
    int x;
    int remainder;
    int digit = 2;
    char* ptr = (char*) malloc(32);
    
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

char* convertBinaryToHex(char* binary)
{
    int x;
    int counter = 8;
    int hex;
    char* ptr = malloc(8);
    
    for(x=MAX-1;x>=0;x=x-4)
    {
        hex = atoi(binary+x-3)*8 + atoi(binary+x-2)*4 + atoi(binary+x-1)*2 + atoi(binary+x);
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
        counter --;
    }
    return ptr;
}

char* convertIntToHex(int input)
{
    return convertBinaryToHex(convertIntToBinary(input));
}