#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int domino[3];
    char * buffer;
    size_t buffer_size = 100;
    buffer = (char * ) malloc ( buffer_size * sizeof(char) );
    if (getline(&buffer, &buffer_size, stdin))
    {
        int i = 0;
        char * cur;
        cur = strtok(buffer, "[, ");
        while (cur != NULL)
        {
            printf("i = %d\n", i);
            domino[i] = atoi(cur);
            puts(cur);
            printf("%d\n", domino[i]);
            cur = strtok(NULL, "[, ");
            i++;
        }
        printf("[%d, %d, %d]\n", domino[0], domino[1], domino[2]);
    }
    else
    {
        puts("Error.\n");
        free(buffer);
        return 1;
    }
    free(buffer);
    return 0;
}