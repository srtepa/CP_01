#ifndef UTILS_H
#define UTILS_H

#include <string>

const int MAX_TOPICS = 5;
const int MAX_LINES = 1000;
const int MAX_NAME_LENGTH = 1000;

extern char selectedTopic[MAX_NAME_LENGTH];
extern int questionCount;
extern int currentTicketNumber;

struct Topic {
    char name[MAX_NAME_LENGTH];
};

extern Topic topics[MAX_TOPICS];

void waitForReturn();
void clearConsole();

#endif // UTILS_H 