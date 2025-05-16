#include "Utils.h"
#include <iostream>
#include <limits>
using namespace std;

char selectedTopic[MAX_NAME_LENGTH] = "";
int questionCount = 0;
int currentTicketNumber = -1;

Topic topics[MAX_TOPICS] = {
    {"Понятие алгоритма и системы программирования"},
    {"Основные элементы языка"},
    {"Структуры данных"},
    {"Файлы. Подпрограммы"},
    {"Динамические структуры данных"}
};

void waitForReturn() {
    cout << "\nНажмите Enter, чтобы вернуться в меню...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
} 