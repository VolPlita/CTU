#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct
{
    int * tokens = NULL;
    int len = 0;
    char name;
}arm;

int pickToken(arm * arms, int move, int *a, int *b);
void takeTail(int move, int tail, int *a, int *b);
void freef(arm * arms);

int main()
{
    arm arms[4];
    arms[0].name = 'N';
    arms[1].name = 'E';
    arms[2].name = 'W';
    arms[3].name = 'S';

    char L;   // Variable for arm N/E/W/S
    int n;    // Variable for scanned number
    char del; // Variable for delimiter , or }
    char del1;

    printf("Tokens:\n");

    for (int i = 0; i < 4; ++i) // Scanning for arms
    {
        if ( scanf(" %c : %c", &L, &del1) != 2 || del1 != '{' )
        {
            printf("Invalid input.\n");
            freef(arms);
            return 1;
        }

        int len = 0;
        int max = 10;
        int * buffer = (int * ) malloc (sizeof(int) * max);

        if (n >= max)
        {
            max *= 2;                    
            buffer = (int * ) realloc (buffer, sizeof(int) * max);
        }

        while ( scanf(" %d %c", &n, &del) == 2)
        {
            if (del != ',' && del != '}')
            {
                printf("Invalid input.\n");
                freef(arms);
                free(buffer);
                return 1;
            }
            else
            {
                buffer[len] = n;
                ++len;
                if (del == '}') break;
            }
            if (len >= 32 || len < 1)
            {
                printf("Invalid input.\n");
                freef(arms);
                free(buffer);
                return 1;
            }
        }

        switch (L)
        {
            case 'N':
                if (arms[0].tokens == NULL)
                {
                    arms[0].tokens = buffer;
                    arms[0].len = len;
                    break;
                }
                else
                {
                    printf("Invalid input.\n");
                    freef(arms);
                    free(buffer);
                    return 1;
                }

            case 'E':
                if (arms[1].tokens == NULL)
                {
                    arms[1].tokens = buffer;
                    arms[1].len = len;
                    break;
                }
                else
                {
                    printf("Invalid input.\n");
                    freef(arms);
                    free(buffer);
                    return 1;
                }

            case 'W':
                if (arms[2].tokens == NULL)
                {
                    arms[2].tokens = buffer;
                    arms[2].len = len;
                    break;
                }
                else
                {
                    printf("Invalid input.\n");
                    freef(arms);
                    free(buffer);
                    return 1;
                }
            case 'S':
                if (arms[3].tokens == NULL)
                {
                    arms[3].tokens = buffer;
                    arms[3].len = len;
                    break;
                }
                else
                {
                    printf("Invalid input.\n");
                    freef(arms);
                    free(buffer);
                    return 1;
                }
            default:
                printf("Invalid input.\n");
                freef(arms);
                free(buffer);
                return 1;
        } // End of switch
    }

    if ( getchar() != '\n' && !feof(stdin) )
    {
        printf("Invalid input.\n");
        freef(arms);
        return 1;
    }

    // Checks for invalid input not fulfilled

    /************************************************************************/
    // printf("N:");
    // for (int i = 0; i < arms[0].len; ++i)
    // {
    //     printf(" %d", arms[0].tokens[i]);
    // }
    // printf("\n");
    // printf("E:");
    // for (int i = 0; i < arms[1].len; ++i)
    // {
    //     printf(" %d", arms[1].tokens[i]);
    // }
    // printf("\n");
    // printf("W:");
    // for (int i = 0; i < arms[2].len; ++i)
    // {
    //     printf(" %d", arms[2].tokens[i]);
    // }
    // printf("\n");
    // printf("S:");
    // for (int i = 0; i < arms[3].len; ++i)
    // {
    //     printf(" %d", arms[3].tokens[i]);
    // }
    // printf("\n\n");
    /*************************************************************************/

    int total_A = 0; 
    int total_B = 0;

    int armsLen[4] = {arms[0].len, arms[1].len, arms[2].len, arms[3].len};
    int countElem = arms[0].len + arms[1].len + arms[2].len + arms[3].len;

    int move = 0;
    while (move < countElem) 
    {
        int a = 0;
        int b = 0;
        int best = pickToken(arms, move, &a, &b);

        if (move % 2 == 0) 
            printf("A: %c", arms[best].name);
        else 
            printf("B: %c", arms[best].name);
        
        printf("[%d] (%d)\n", armsLen[best] - arms[best].len, arms[best].tokens[0]);

        takeTail(move, arms[best].tokens[0], &total_A, &total_B);

        arms[best].len--;
        arms[best].tokens++;
        move++;
    }

    printf("Total A/B: %d/%d\n", total_A, total_B);

    for (int i = 0; i < 4; i++) free(arms[i].tokens - armsLen[i]);

    return 0;

    // freef(arms);
    // return 0;
}

int pickToken(arm * arms, int move, int *a, int *b) 
{
    int best = 0;
    int atot = INT_MIN;
    int btot = INT_MIN;
    for (int i = 0; i < 4; i++) 
    {
        if (arms[i].len == 0) continue;

        int ap = 0;
        int bp = 0;
        takeTail( move, arms[i].tokens[0], &ap, &bp);

        if (arms[i].len >= 1) 
        {
            arms[i].len--;
            arms[i].tokens++;
            pickToken(arms, move + 1, &ap, &bp); 
            arms[i].tokens--;
            arms[i].len++;
        }

        if ( (atot == INT_MIN && btot == INT_MIN) || 
             ((move % 2 == 0) ? ap > atot : bp > btot ) )
        {
            best = i;
            atot = ap;
            btot = bp;
        }
    }

    if (atot != INT_MIN) *a += atot;
    if (btot != INT_MIN) *b += btot;

    return best;
}

void takeTail(int move, int tail, int *a, int *b) 
{
    if (move % 2 == 0) 
        (*a) += tail;
    else 
        (*b) += tail;
}

void freef(arm * arms)
{
    free(arms[0].tokens);
    free(arms[1].tokens);
    free(arms[2].tokens);
    free(arms[3].tokens);
}
