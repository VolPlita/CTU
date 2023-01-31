#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name1[31];
    char name2[31];
    char name3[31];
    int in;
}student;

int areSame(student s1, student s2)
{
    if ( (strcasecmp(s1.name1, s2.name1) == 0 && strcasecmp(s1.name2, s2.name2) == 0 && strcasecmp(s1.name3, s2.name3) == 0) ||
         (strcasecmp(s1.name1, s2.name1) == 0 && strcasecmp(s1.name2, s2.name3) == 0 && strcasecmp(s1.name3, s2.name2) == 0) ||
         (strcasecmp(s1.name1, s2.name2) == 0 && strcasecmp(s1.name2, s2.name1) == 0 && strcasecmp(s1.name3, s2.name3) == 0) ||
         (strcasecmp(s1.name1, s2.name2) == 0 && strcasecmp(s1.name2, s2.name3) == 0 && strcasecmp(s1.name3, s2.name1) == 0) ||
         (strcasecmp(s1.name1, s2.name3) == 0 && strcasecmp(s1.name2, s2.name2) == 0 && strcasecmp(s1.name3, s2.name1) == 0) ||
         (strcasecmp(s1.name1, s2.name3) == 0 && strcasecmp(s1.name2, s2.name1) == 0 && strcasecmp(s1.name3, s2.name2) == 0) )
    {
        return 1;
    }
    return 0;
}

int main()
{
    int reg_size = 0;
    int reg_cap = 10;
    student * reg = (student *) calloc (reg_cap, sizeof(student));

    int came_size = 0;
    int came_cap = 10;
    student * came = (student *) calloc (came_cap, sizeof(student));

    int buffer_size = 200;
    char * buffer = (char *) malloc (sizeof(char) * buffer_size);

    puts("Students:");

    char who;
    int filled = 0;
    while(fgets(buffer, 200, stdin) != NULL)
    {
        who = buffer[0];
        if (who == '\n') continue;
        if (buffer[1] != ':' || (who != 'R' && who != 'P') || (filled == 1 && who != 'P'))
        {
            puts("Invalid input.");
            free(reg);
            free(came);
            free(buffer);
            exit(1);
        }
        else if (who == 'R')
        {
            buffer += 3;
            if (reg_size >= reg_cap)
            {
                reg_cap *= 2;
                reg = (student *) realloc (reg, sizeof(student) * reg_cap);
            }
            sscanf(buffer, "%s %s %s", reg[reg_size].name1, reg[reg_size].name2, reg[reg_size].name3);
            reg[reg_size].in = 1;
            reg_size++;
            buffer -= 3;
        }
        else if (who == 'P')
        {
            filled = 1;
            buffer += 3;
            if (came_size >= came_cap)
            {
                came_cap *= 2;
                came = (student *) realloc (came, sizeof(student) * came_cap);
            }
            sscanf(buffer, "%s %s %s", came[came_size].name1, came[came_size].name2, came[came_size].name3);
            came[came_size].in = 1;
            came_size++;
            buffer -= 3;
        }
    }

//    for (int i = 0; i < reg_size; ++i)
//        printf("R: %s %s %s\n", reg[i].name1, reg[i].name2, reg[i].name3);
//
//    for (int i = 0; i < came_size; ++i)
//        printf("P: %s %s %s\n", came[i].name1, came[i].name2, came[i].name3);

    puts("Didn't come:");
    int c;
    for (int i = 0; i < reg_size; ++i) {
        c = 0;
        for (int j = 0; j < came_size; ++j) {
            if (came[j].in == 1 && areSame(reg[i], came[j]) == 1)
            {
                came[j].in = 0;
                c = 1;
                break;
            }
        }
        if (c == 0)
        {
            printf("* %s %s %s\n", reg[i].name1, reg[i].name2, reg[i].name3);
        }
    }

    puts("Didn't register:");
    int r = 0;
    for (int i = 0; i < came_size; ++i) {
        r = 0;
        for (int j = 0; j < reg_size; ++j) {
            if (reg[j].in == 1 && areSame(reg[j], came[i]) == 1)
            {
                reg[j].in = 0;
                r = 1;
                break;
            }
        }
        if (r == 0)
        {
            printf("* %s %s %s\n", came[i].name1, came[i].name2, came[i].name3);
        }
    }

    free(reg);
    free(came);
    free(buffer);
    return 0;
}
