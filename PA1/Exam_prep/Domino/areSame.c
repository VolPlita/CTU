#include <stdio.h>

int areSame(int * c1, int * c2);

int main()
{
    int d1[5] = {2, 5, 7, 2, 5};
    int d2[5] = {2, 5, 2, 5, 7};
    int d3[5] = {7, 2, 5, 2, 5};
    int d4[5] = {4, 6, 7, 8, 9};
    int d5[5] = {2, 7, 5, 2, 5};

    areSame(d1, d2, 5) == 1 ? printf("1 | 2 - same\n") : printf("1 | 2 - different\n");
    areSame(d1, d3, 5) == 1 ? printf("1 | 3 - same\n") : printf("1 | 3 - different\n");
    areSame(d1, d4, 5) == 1 ? printf("1 | 4 - same\n") : printf("1 | 4 - different\n");
    areSame(d1, d5, 5) == 1 ? printf("1 | 5 - same\n") : printf("1 | 5 - different\n");

    areSame(d2, d3, 5) == 1 ? printf("2 | 3 - same\n") : printf("2 | 3 - different\n");
    areSame(d2, d4, 5) == 1 ? printf("2 | 4 - same\n") : printf("2 | 4- different\n");
    areSame(d2, d5, 5) == 1 ? printf("2 | 5 - same\n") : printf("2 | 5 - different\n");

    areSame(d3, d4, 5) == 1 ? printf("3 | 4 - same\n") : printf("3 | 4 - different\n");
    areSame(d3, d5, 5) == 1 ? printf("3 | 5 - same\n") : printf("3 | 5 - different\n");

    areSame(d4, d5, 5) == 1 ? printf("4 | 5 - same\n") : printf("4 | 5 - different\n");

    return 0;
}

int areSame(int * c1, int * c2, int n)
{
    //Check sum
    int sum1 = 0;
    int sum2 = 0;
    for (int i = 0; i < n; i++)
    {
        sum1 += c1[i];
        sum2 += c2[j];
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