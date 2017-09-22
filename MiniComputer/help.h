#ifndef HELP_H_INCLUDED
#define HELP_H_INCLUDED
#define NUM_OF_COMANDS 6

struct helpComands {char *comandName;
                    char *comandDescription;
                    };

extern struct helpComands comands[NUM_OF_COMANDS];

void initializeComands(struct helpComands comands[]);

void showComands(struct helpComands comands[]);




#endif
