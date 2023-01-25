#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int dict_i = 0;
    int dict_size = 10;
    char ** dictionary = (char **) malloc (sizeof (char *) * dict_size);

    puts("Dictionary:");
    int end = 0;

    while (1)
    {
        char buffer[31];
        scanf(" %s", buffer);
        if ( strcmp(buffer, "*****") == 0 ) // Terminating condition
        {
            end = 1;
            break;
        }
        if (dict_i >= dict_size) // Reallocation part
        {
            dict_size *= 2;
            dictionary = (char **) realloc (dictionary, sizeof (char *) * dict_size);
        }

        char * word = (char *) malloc (sizeof(char) * 31);
        strcpy(word, buffer);
        dictionary[dict_i] = word;
        dict_i++;

        if (feof(stdin) && end != 1)
        {
            puts("Invalid input.");
            exit(1);
        }
    } // End of reading dictionary

    puts("Text:");

    int text_i = 0;
    int text_size = 10;
    char ** text = (char **) malloc (sizeof (char *) * text_size);

    while (!feof(stdin))
    {
        char buffer[31];
        scanf(" %s", buffer);

        if (text_i >= text_size) // Reallocation part
        {
            text_size *= 2;
            text = (char **) realloc (text, sizeof (char *) * text_size);
        }

        char * word = (char *) malloc (sizeof(char) * 31);
        strcpy(word, buffer);
        text[text_i] = word;
        text_i++;
    }

    int found = 0;
    for(int i = 0; i < text_i; i++)
    {
        found = 0;
        for (int j = 0; j < dict_i; j++)
        {
            if ( strcasecmp(dictionary[j], text[i]) == 0 )
            {
                printf("%s ", text[i]);
                found = 1;
                break;
            }
        }
        if (found == 0)
        {
            printf("<err>%s</err> ", text[i]);
        }
    }

    for (int i = 0; i < dict_i; i++) free(dictionary[i]);
    free(dictionary);
    for (int i = 0; i < text_i; i++) free(text[i]);
    free(text);
    return 0;
}
