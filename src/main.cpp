#include "Auth.h"
#include "Teacher.h"
#include "Student.h"
#include "Utils.h"
#include <iostream>

using namespace std;

int mainMenu() {
    int choice;
    cout << "Вход в программу ''Конструктор экзаменационных билетов''" << endl;
    cout << "---------------------------------" << endl;
    cout << "1. Авторизация" << endl;
    cout << "2. Выход" << endl;
    cout << "\nВаш выбор: ";
    cin >> choice;
    while (choice < 1 || choice > 2) {
        cout << "Неверный выбор. Повторите ввод: ";
        cin >> choice;
    }
    return choice;
}

int main() {
    setlocale(LC_ALL, "RU");

    while (true) {
        int mainChoice = mainMenu();
        if (mainChoice == 2) {
            cout << "\nВыход из программы...\n";
            break;
        }
        int role = loginSystem();
        if (role == 1) {
            teacherMenu();
        }
        else if (role == 0) {
            studentMenu();
        }
    }
    return 0;
} 