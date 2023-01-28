#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    long int amount;
    int reg_len;
    int reg_cap;
    char ** regions;
    int met;
}salary;

int main() {
    int base_cap = 10;
    int base_len = 0;
    salary * base = (salary *) malloc (sizeof(salary) * base_cap);

    char c;
    while (1)
    {
        if (scanf(" %c", &c) != 1 || (c != '+' && c != '?'))
        {
            puts("Invalid input.");
            for (int i = 0; i < base_len; i++)
            {
                for (int j = 0; j < base[i].reg_len; j++)
                {
                    free(base[i].regions[j]);
                }
                free(base[i].regions);
            }
            free(base);
            exit(1);
        }
        else
        {
            if (c == '?')
            {
                char search[10];
                int k = scanf("%s", search);
                int f = 0;
                for (size_t i = 0; i < strlen(search); i++)
                {
                    if (isalpha(search[i]))
                    {
                        f = 1;
                        break;
                    }
                }
                if (k != 1 || f == 1)
                {
                    puts("Invalid input.");
                    for (int i = 0; i < base_len; i++)
                    {
                        for (int j = 0; j < base[i].reg_len; j++)
                        {
                            free(base[i].regions[j]);
                        }
                        free(base[i].regions);
                    }
                    free(base);
                    exit(1);
                }
                if (strncmp(search, "*", 10) == 0)
                {
                    //Search max
                    for (int i = 0; i < base_len; ++i) {
                        for (int j = 0; j < base[i].reg_len; ++j) {
                            base[i].met++;
                        }
                    }

                }
                else
                {
                    //Search some region
                    for (int i = 0; i < base_len; ++i) {
                        for (int j = 0; j < base[i].reg_len; ++j) {
                            if (strncmp(base[i].regions[j], search, strlen(search)) == 0) {
                                base[i].met++;
                                //printf("%s = %s for %ld\n", base[i].regions[j], search, strlen(search));
                            }
                        }
                    }

                }

                //test
//        for (int i = 0; i < base_len; i++)
//        {
//            printf("%ld - %d\n", base[i].amount, base[i].met);
//        }
                //test

                // Look for met max
                int met_max = 0;
                int m_amount = 1;
                for (int i = 0; i < base_len; ++i) {
                    if (base[i].met > met_max) met_max = base[i].met;
                    else if (base[i].met == met_max) m_amount++;
                }

                //printf("%d\n", m_amount);

                //What if met max == 0?
                if (met_max == 0)
                    printf("Modus: N/A\n");
                else
                {
                    // Print maximal met salary
                    printf("Modus: ");
                    for (int i = 0; i < base_len; ++i) {
                        if (base[i].met == met_max)
                        {
                            if (m_amount > 1)
                            {
                                printf("%ld, ", base[i].amount);
                            }
                            else
                            {
                                printf("%ld ", base[i].amount);
                                break;
                            }
                            m_amount--;
                        }
                    }
                    printf("[%d]\n", met_max);
                }

                //Set met back to 0
                for (int i = 0; i < base_len; ++i)
                    base[i].met = 0;

                break;
            }
            else
            {
                char region[10];
                long int cur_sal = 0;
                if (scanf(" %s %ld", region, &cur_sal) != 2)
                {
                    puts("Invalid input.");
                    for (int i = 0; i < base_len; i++)
                    {
                        for (int j = 0; j < base[i].reg_len; j++)
                        {
                            free(base[i].regions[j]);
                        }
                        free(base[i].regions);
                    }
                    free(base);
                    exit(1);
                }
                else
                {
                    //Look if we already have such salary
                    int nw = 1;
                    for(int i = 0; i < base_len; i++)
                    {
                        if(base[i].amount == cur_sal)
                        {
                            nw = 0;
                            //Add reallocation region
                            if (base[i].reg_len >= base[i].reg_cap)
                            {
                                base[i].reg_cap *= 2;
                                base[i].regions = (char **) realloc (base[i].regions, sizeof(char *) * base[i].reg_cap);
                            }
                            //Add new region
                            base[i].regions[base[i].reg_len] = strdup(region);
                            base[i].reg_len++;
                            break;
                        }
                    }

                    // If it is new
                    if (nw)
                    {
                        if (base_len >= base_cap)
                        {
                            base_cap *= 2;
                            base = (salary *) realloc (base, sizeof(salary) * base_cap);
                        }

                        salary cur;
                        cur.met = 0;
                        cur.reg_cap = 5;
                        cur.reg_len = 0;
                        cur.regions = (char **) malloc (sizeof(char *) * cur.reg_cap);

                        cur.amount = cur_sal;
                        cur.regions[cur.reg_len] = strdup(region);

                        cur.reg_len++;

                        base[base_len] = cur;
                        base_len++;
                    }
                }
            }

        }
    }

//    for (int i = 0; i < base_len; ++i) {
//        printf("$%ld\n", base[i].amount);
//        for (int j = 0; j < base[i].reg_len; j++)
//        {
//            printf("    %s\n", base[i].regions[j]);
//        }
//    }

    while(1)
    {
        char search[10];
        int k = scanf(" %c %s", &c, search);
        int f = 0;
        for (size_t i = 0; i < strlen(search); i++)
        {
            if (isalpha(search[i]))
            {
                f = 1;
                break;
            }
        }

        if ( k != 2 || c != '?' || f == 1)
        {
            if(feof(stdin)) break;
//            printf("%d\n", k);
//            printf("%c %s", c, search);
            puts("Invalid input.");
            for (int i = 0; i < base_len; i++)
            {
                for (int j = 0; j < base[i].reg_len; j++)
                {
                    free(base[i].regions[j]);
                }
                free(base[i].regions);
            }
            free(base);
            exit(1);
        }

        //test
//        for (int i = 0; i < base_len; i++)
//        {
//            printf("%ld - %d\n", base[i].amount, base[i].met);
//        }
        //test

        if (strncmp(search, "*", 10) == 0)
        {
            //Search max
            for (int i = 0; i < base_len; ++i) {
                for (int j = 0; j < base[i].reg_len; ++j) {
                    base[i].met++;
                }
            }

        }
        else
        {
            //Search some region
            for (int i = 0; i < base_len; ++i) {
                for (int j = 0; j < base[i].reg_len; ++j) {
                    if (strncmp(base[i].regions[j], search, strlen(search)) == 0) {
                        base[i].met++;
                        //printf("%s = %s for %ld\n", base[i].regions[j], search, strlen(search));
                    }
                }
            }

        }

        //test
//        for (int i = 0; i < base_len; i++)
//        {
//            printf("%ld - %d\n", base[i].amount, base[i].met);
//        }
        //test

        // Look for met max
        int met_max = 0;
        int m_amount = 1;
        for (int i = 0; i < base_len; ++i) {
            if (base[i].met > met_max) met_max = base[i].met;
            else if (base[i].met == met_max && base[i].met != 0) m_amount++;
        }

        //printf("%d\n", m_amount);

        //What if met max == 0?
        if (met_max == 0)
            printf("Modus: N/A\n");
        else
        {
            // Print maximal met salary
            //printf("Amount = %d\n", m_amount);
            printf("Modus: ");
            for (int i = 0; i < base_len; ++i) {
                if (base[i].met == met_max)
                {
                    if (m_amount == 1)
                    {
                        printf("%ld ", base[i].amount);
                        break;
                    }
                    else
                    {
                        printf("%ld, ", base[i].amount);
                    }
                    m_amount--;
                }
            }
            printf("[%d]\n", met_max);
        }

        //Set met back to 0
        for (int i = 0; i < base_len; ++i)
            base[i].met = 0;
    }

    for (int i = 0; i < base_len; i++)
    {
        for (int j = 0; j < base[i].reg_len; j++)
        {
            free(base[i].regions[j]);
        }
        free(base[i].regions);
    }

    free(base);
    return 0;
}
