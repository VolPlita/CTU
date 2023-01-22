#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int unique;
    int val[3];
}dom3;

typedef struct
{
    int unique;
    int val[4];
}dom4;

typedef struct
{
    int unique;
    int val[5];
}dom5;

void readInput(dom3 * set3, dom4 * set4, dom5 * set5, size_t * size3, size_t * size4, size_t * size5);
int areSame(int * c1, int * c2, int n);

int main()
{
    size_t size3 = 5;
    size_t size4 = 5;
    size_t size5 = 5;
    dom3 * set3 = (dom3 *) malloc (sizeof(dom3) * size3);
    dom4 * set4 = (dom4 *) malloc (sizeof(dom4) * size4);
    dom5 * set5 = (dom5 *) malloc (sizeof(dom5) * size5);

    readInput(set3, set4, set5, &size3, &size4, &size5);

    //test
    for (size_t i = 0; i < size3; i++)
    {
        printf("[%d,%d,%d]\n", set3[i].val[0], set3[i].val[1], set3[i].val[2]);
    }
    for (size_t i = 0; i < size4; i++)
    {
        printf("[%d,%d,%d,%d]\n", set4[i].val[0], set4[i].val[1], set4[i].val[2], set4[i].val[3]);
    }
    for (size_t i = 0; i < size5; i++)
    {
        printf("[%d,%d,%d,%d,%d]\n", set5[i].val[0], set5[i].val[1], set5[i].val[2], set5[i].val[3], set5[i].val[4]);
    }
    //test

    // Check 3's
    for (size_t i = 0; i < size3; i++)
    {
        for (size_t j = i + 1; j < size3; j++)
        {
            if (areSame(set3[i].val, set3[j].val, 3) == 1)
            {
                set3[j].unique = 0;
            }
        }
    }
    // Check 4's
    for (size_t i = 0; i < size4; i++)
    {
        for (size_t j = i + 1; j < size4; j++)
        {
            if (areSame(set4[i].val, set4[j].val, 4) == 1)
            {
                set4[j].unique = 0;
            }
        }
    }
    // Check 5's
    for (size_t i = 0; i < size5; i++)
    {
        for (size_t j = i + 1; j < size5; j++)
        {
            if (areSame(set5[i].val, set5[j].val, 5) == 1)
            {
                set5[j].unique = 0;
            }
        }
    }

    int unique = 0;
    for (size_t i = 0; i < size3; i++)
        if(set3[i].unique == 1) unique++;
    for (size_t i = 0; i < size4; i++)
        if(set4[i].unique == 1) unique++;
    for (size_t i = 0; i < size5; i++)
        if(set5[i].unique == 1) unique++;

    for (size_t i = 0; i < size3; i++)
    {
        printf("[%d,%d,%d], unique = %d\n", set3[i].val[0], set3[i].val[1], set3[i].val[2], set3[i].unique );
    }

    printf("Unique: %d\n", unique);

    free(set3);
    free(set4);
    free(set5);
    return 0;
}

void readInput(dom3 * set3, dom4 * set4, dom5 * set5, size_t * size3, size_t * size4, size_t * size5)
{
    char * buffer;
    size_t buffer_size = 100;
    buffer = (char * ) malloc ( buffer_size * sizeof(char) );

    size_t i3 = 0;
    size_t i4 = 0; 
    size_t i5 = 0;
    while(!feof(stdin))
    {
        if (getline(&buffer, &buffer_size, stdin) < 0) break;
        int i = 0;
        int k = 1;
        while (buffer[i] != '\n') 
        {
            if (buffer[i] == ',') k++;
            i++;
        }

        if ( k == 3 ) //Three-sided domino
        {
            if(i3 >= (*size3))
            {
                (*size3) *= 2;
                set3 = (dom3 *) realloc (set3, sizeof(dom3) * (*size3));
            }

            sscanf(buffer, "[ %d , %d , %d ]", &set3[i3].val[0], &set3[i3].val[1], &set3[i3].val[2]);
            set3[i3].unique = 1;

            i3++;
        }
        else if ( k == 4 ) //Four-sided domino
        {
            if(i4 >= (*size4))
            {
                (*size4) *= 2;
                set4 = (dom4 *) realloc (set4, sizeof(dom4) * (*size4));
            }

            sscanf(buffer, "[ %d , %d , %d , %d ]", &set4[i4].val[0], &set4[i4].val[1], &set4[i4].val[2], &set4[i4].val[3]);
            set4[i4].unique = 1;

            i4++;
        }
        else if(k == 5)// Five sided domino
        {
            if(i5 >= (*size5))
            {
                (*size5) *= 2;
                set5 = (dom5 *) realloc (set5, sizeof(dom5) * (*size5));
            }

            sscanf(buffer, "[ %d , %d , %d , %d , %d ]", &set5[i5].val[0], &set5[i5].val[1], &set5[i5].val[2], &set5[i5].val[3], &set5[i5].val[4]);
            set5[i5].unique = 1;

            i5++;
        }
    }
    //test
    // printf("size3 = %ld | i3 = %ld\n", (*size3), i3);
    // printf("size4 = %ld | i4 = %ld\n", (*size4), i4);
    // printf("size5 = %ld | i5 = %ld\n", (*size5), i5);
    //test

    (*size3) = i3;
    (*size4) = i4;
    (*size5) = i5;

    //test
    // printf("size3 = %ld | i3 = %ld\n", (*size3), i3);
    // printf("size4 = %ld | i4 = %ld\n", (*size4), i4);
    // printf("size5 = %ld | i5 = %ld\n", (*size5), i5);
    //test

    free(buffer);
}

int areSame(int * c1, int * c2, int n)
{
    //Check sum
    int sum1 = 0;
    int sum2 = 0;
    for (int i = 0; i < n; i++)
    {
        sum1 += c1[i];
        sum2 += c2[i];
    }
    if (sum1 != sum2) return 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (c1[i] == c2[j])
            {
                int old_i = i;
                int old_j = j;
                int found = 1;
                for (int k = 0, p = n - 1; k < p; k++)
                {
                    i = (i + 1) % n;
                    j = (j + 1) % n;
                    if (c1[i] != c2[j])
                    {
                        found = 0;
                        break;
                    }
                }
                i = old_i;
                j = old_j;
                    
                if (found == 1) return 1;
            }
        }
    }
    return 0;
}