#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int val[3];
}dom3;

typedef struct
{
    int val[4];
}dom4;

typedef struct
{
    int val[5];
}dom5;

int readInput(dom3 * set3, dom4 * set4, dom5 * set5);

int main()
{
    int size3 = 5;
    int size4 = 5;
    int size5 = 5;
    dom3 * set3 = (dom3 *) malloc (sizeof(dom3) * size3);
    dom4 * set4 = (dom4 *) malloc (sizeof(dom4) * size4);
    dom5 * set5 = (dom5 *) malloc (sizeof(dom5) * size5);

    readInput(set3, set4, set5, &size3, &size4, &size5);

    
}

void readInput(dom3 * set3, dom4 * set4, dom5 * set5, int * size3, int * size4, int * size5)
{
    char * buffer;
    size_t buffer_size = 100;
    buffer = (char * ) malloc ( buffer_size * sizeof(char) );

    int i3 = 0;
    int i4 = 0; 
    int i5 = 0;
    while(!feof(stdin))
    {
        if ( getline(&buffer, &buffer_size, stdin) == 8 ) //Three-sided domino
        {
            if(i3 >= (*size3))
            {
                (*size3) *= 2;
                set3 = (dom3 *) realloc (sizeof(dom3) * size3);
            }

            sscanf(buffer, "[ %d , %d , %d ]", set3[i3]->val[0], set3[i3]->val[1], set3[i3]->val[2]);

            i3++;
        }
        else if ( getline(&buffer, &buffer_size, stdin) == 10 ) //Four-sided domino
        {
            if(i4 >= (*size4))
            {
                (*size4) *= 2;
                set4 = (dom4 *) realloc (sizeof(dom4) * size4);
            }

            sscanf(buffer, "[ %d , %d , %d , %d ]", set4[i4]->val[0], set4[i4]->val[1], set4[i4]->val[2], set4[i4]->val[3]);

            i4++;
        }
        else // Five sided domino
        {
            if(i5 >= (*size5))
            {
                (*size5) *= 2;
                set5 = (dom5 *) realloc (sizeof(dom5) * size5);
            }

            sscanf(buffer, "[ %d , %d , %d , %d , %d ]", set5[i5]->val[0], set5[i5]->val[1], set5[i5]->val[2], set5[i5]->val[3], set5[i5]->val[4]);

            i4++;
        }
    }

    (*size3) = i3;
    (*size4) = i4;
    (*size5) = i5;
}