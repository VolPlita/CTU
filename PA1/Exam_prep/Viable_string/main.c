#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void findViable(char * s, int len);

int main() {

    int cap = 10;
    int word_len = 0;
    char * word = (char *) malloc (sizeof(char) * cap);

    char cur;
    while(1)
    {
        if ( word_len >= cap )
        {
            cap *= 2;
            word = (char *) realloc (word, sizeof(char) * cap);
        }

        int x = scanf("%c", &cur);

        if (cur == '\n' || cur == ' ' || cur == EOF)
        {
            word[word_len] = '\0';
            break;
        }

        if ( x != 1 || (cur != '?' && (cur > 122 || cur < 97)) )
        {
            puts("Invalid input.");
            free(word);
            exit(1);
        }

        word[word_len] = cur;
        word_len++;
    }

    findViable(word, strlen(word));

    free(word);
    return 0;
}

void findViable(char * s, int len)
{
    //!!!One step at a time
    int found = 0;
    for (int i = 0; i < len; ++i)
    {
        if (s[i] == '?')
        {
            found = 1;
            if ( (i == 0 || s[i - 1] == '?') && s[i + 1] != '?')
            {
                // Consider only +1
                for (int j = -2; j < 3; j++)
                {
                    if (s[i + 1] + j < 123 && s[i + 1] + j > 96)
                    {
                        char * new_s = strdup(s);
                        new_s[i] = (new_s[i + 1] + j);
                        findViable(new_s, len);
                        free(new_s);
                    }
                }
            }
            else if ( (i == (len - 1) || s[i + 1] == '?') && s[i - 1] != '?')
            {
                // Consider only -1
                for (int j = -2; j < 3; j++)
                {
                    if (s[i - 1] + j < 123 && s[i - 1] + j > 96)
                    {
                        char * new_s = strdup(s);
                        new_s[i] = (new_s[i - 1] + j);
                        findViable(new_s, len);
                        free(new_s);
                    }
                }
            }
            else if (s[i - 1] != '?' && s[i + 1] != '?')
            {
                // Consider both
                for (int j = -2; j < 3; j++)
                {
                    for (int k = -2; k < 3; k++)
                    {
                        if ( ((s[i+1] + j) == (s[i-1] + k)) &&
                             s[i - 1] + k < 123 && s[i - 1] + k > 96 &&
                             s[i + 1] + j < 123 && s[i + 1] + j > 96)
                        {
                            char * new_s = strdup(s);
                            new_s[i] = (new_s[i + 1] + j);
                            findViable(new_s, len);
                            free(new_s);
                        }
                    }
                }

            }
            break;
        }
    }
    if (found == 0)
    {
        puts(s);
        return;
    }
}
