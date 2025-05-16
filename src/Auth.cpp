#include "Auth.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int authorizeUser(string login, string password) {
    ifstream file("data/users.txt");
    if (!file) {
        cout << "Ошибка: файл users.txt не найден!" << endl;
        return -1;
    }

    string fileLogin, fileHashedPassword;
    int fileRole;
    string inputHashedPassword = funcHash(password);

    while (file >> fileLogin >> fileHashedPassword >> fileRole) {
        if (fileLogin == login && inputHashedPassword == fileHashedPassword) {
            return fileRole;
        }
    }
    return -1;
}

int loginSystem() {
    string login, password;
    int currentRole = -1;
    do {
        cout << "Введите логин: ";
        cin >> login;
        cout << "Введите пароль: ";
        cin >> password;
        currentRole = authorizeUser(login, password);
        if (currentRole == -1) {
            cout << "Ошибка: Неверный логин или пароль!" << endl;
        }
    } while (currentRole == -1);

    if (currentRole == 1) {
        cout << "---------------------------------" << endl;
        cout << "Добро пожаловать, преподаватель!" << endl;
    } else if (currentRole == 0) {
        cout << "---------------------------------" << endl;
        cout << "Добро пожаловать, студент!" << endl;
    }
    return currentRole;
}

string funcHash(const string& password) {
    string hash = "";
    for (size_t i = 0; i < password.length(); ++i) {
        char c = password[i];
        char shifted = c + 3 + (i % 4);
        hash += shifted;
    }
    return hash;
} 