#include "Student.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void studentMenu() {
    while (true) {
        int choice;
        cout << "---------------------------------\n";
        cout << "Меню студента:\n";
        cout << "1. Прохождение тестирования\n";
        cout << "2. Выход из программы\n";
        cout << "0. Вернуться в главное меню\n";
        cout << "\nВаш выбор: ";
        cin >> choice;
        while (choice<0 || choice>2) {
            cout << "\nВаш выбор:";
            cin >> choice;
        }
        if (choice == 0) return;
        switch (choice) {
            case 1: startTesting(); break;
            case 2: cout << "\nВыход из программы...\n"; exit(0);
            default:cout << "\nНеверный выбор. Повторите попытку.\n";
        }
        waitForReturn();
    }
}

void startTesting() {
    ifstream file("data/examcard.txt");
    if (!file) {//проверка на открытие файла
        cout << "Файл examcard.txt не найден!\n";
        return;
    }

    const int MAX_LINES = 1000;
    string lines[MAX_LINES];
    int lineCount = 0;
    string line;

    while (getline(file, line) && lineCount < MAX_LINES) {//счетчик строк
        if (!line.empty()) {
            lines[lineCount++] = line;
        }
    }

    file.close();

    //доступные билеты
    cout << "\nДоступные билеты:\n";
    int ticketStartIndexes[100]; //индексы начала билетов
    int ticketCount = 0;

    for (int i = 0; i < lineCount; i++) {
        if (lines[i].find("Билет") != string::npos || lines[i].find("Счастливый билет") != string::npos) {
            ticketStartIndexes[ticketCount++] = i;
        }
    }

    for (int i = 0; i < ticketCount; i++) {//вывод доступных билетов
        cout << i + 1 << ". Билет #" << i + 1 << endl;
    }

    int choice = 1;
    bool redrawUsed = false;
    while (true) {
        cout << "\nВыберите номер билета: ";
        cin >> choice;
        while (choice < 1 || choice > ticketCount) {
            cout << "Неверный выбор. Повторите ввод: ";
            cin >> choice;
        }
        // Показываем вопросы выбранного билета
        int start = ticketStartIndexes[choice - 1];
        int end = (choice < ticketCount) ? ticketStartIndexes[choice] : lineCount;
        cout << "\n--- Вопросы выбранного билета ---\n";
        for (int i = start; i < end; i++) {
            if (lines[i] == "==================================" || lines[i] == "Основы алгоритмизации и программирования") continue;
            cout << lines[i] << endl;
        }
        if (!redrawUsed) {
            int redraw;
            cout << "\nХотите перетянуть билет? (1 — да, 0 — нет): ";
            cin >> redraw;
            while (redraw != 0 && redraw != 1) {
                cout << "Введите 1 (да) или 0 (нет): ";
                cin >> redraw;
            }
            if (redraw == 1) {
                redrawUsed = true;
                continue; // позволяем выбрать билет ещё раз
            }
        }
        break; // окончательный выбор
    }

    cin.ignore();

    int start = ticketStartIndexes[choice - 1];
    int end = (choice < ticketCount) ? ticketStartIndexes[choice] : lineCount;

    ofstream out("data/solutioncard.txt", ios::app);
    out << "Ответы на билет #" << choice << ":\n";

    cout << "\n--- Вопросы билета ---\n";//основной блок тестирования
    for (int i = start; i < end; i++) {
        if (lines[i] == "==================================" || lines[i] == "Основы алгоритмизации и программирования") continue;
        if (lines[i].find("Вопрос") != string::npos && i + 1 < lineCount) { //!= string::npos - подстрока найдена
            cout << lines[i] << endl;
            cout << lines[i + 1] << endl;
            cout << "Введите ваш ответ: ";
            string answer;
            getline(cin, answer);
            out << lines[i] << "\n" << lines[i + 1] << "\nОтвет: " << answer << "\n\n";
            i++;
        } else {
            cout << lines[i] << endl;
        }
    }

    out << "==============================\n";
    out.close();

    cout << "\nОтветы сохранены в файл solutioncard.txt\n";
    waitForReturn();
}