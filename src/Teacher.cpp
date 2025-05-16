#include "Teacher.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstring>
using namespace std;

struct SolvedTicket {
    int number;
    int startLine;
    int endLine;
};

void teacherMenu() {
    while (true) {
        int choice;
        cout << "---------------------------------";
        cout << endl << "Меню преподавателя:" << endl;
        cout << endl;
        cout << "1. Выбор темы для экзаменационного билета" << endl;
        cout << "2. Выбор количества вопросов в билете" << endl;
        cout << "3. Генерация экзаменационного билета" << endl;
        cout << "4. Сортировка вопросов в банке заданий по теме" << endl;
        cout << "5. Генерация 'Счастливого билета'" << endl;
        cout << "6. Просмотр билетов по разделу" << endl;
        cout << "7. Добавление вопроса в банк заданий" << endl;
        cout << "8. Удаление вопроса из банка заданий" << endl;
        cout << "9. Поиск решенного билета" << endl;
        cout << "10. Выход из программы" << endl;
        cout << "0. Вернуться в главное меню" << endl;
        cout << "\nПримечание: Для корректной генерации билетов сначала выберите" << endl;
        cout << "тему (1), затем количество вопросов (2) и только потом" << endl;
        cout << "генерируйте билеты (3)." << endl;
        cout << "\nВаш выбор:";
        cin >> choice;
        while (choice<0 || choice>10) {
            cout << "\nВаш выбор:";
            cin >> choice;
        }
        if (choice == 0) return;
        cout << "\n[Инфо] Выбрана тема: ";
        if (selectedTopic[0] != '\0') cout << selectedTopic;
        else cout << "не выбрана";
        cout << "\n[Инфо] Количество вопросов в билете: ";
        if (questionCount > 0) cout << questionCount;
        else cout << "не выбрано";
        cout << "\n";
        switch (choice) {
            case 1: chooseTopic(); break;
            case 2: chooseQuestionCount(); break;
            case 3: generateExamTicket(); break;
            case 4: sortQuestionsByTopicMenu(); break;
            case 5: generateLuckyTicket(); break;
            case 6: viewTicketsByTopic(); break;
            case 7: addQuestionToBank(); break;
            case 8: deleteQuestionFromBank(); break;
            case 9: searchSolvedTicketMenu(); break;
            case 10: cout << "Выход из программы...\n"; exit(0);
            default: cout << "Неверный выбор. Повторите попытку.\n"; break;
        }
    }
}

void chooseTopic() {
    cout << "\nВыберите тему из списка:\n";
    for (int i = 0; i < MAX_TOPICS; i++) {
        cout << i + 1 << ". " << topics[i].name << endl;
    }
    int choice;
    cout << "Введите номер темы: ";
    cin >> choice;
    while (choice < 1 || choice > MAX_TOPICS) {
        cout << "Неверный выбор. Повторите ввод: ";
        cin >> choice;
    }
    for (int i = 0; i < MAX_NAME_LENGTH; i++) {
        selectedTopic[i] = topics[choice - 1].name[i];
        if (topics[choice - 1].name[i] == '\0') break;
    }
    cout << "Вы выбрали тему: " << selectedTopic << endl;
    waitForReturn();
    teacherMenu();
}

void chooseQuestionCount() {
    cout << "\nВведите количество вопросов в билете: ";
    cin >> questionCount;
    while (questionCount <= 0 || questionCount > 100) {
        cout << "Некорректное число. Повторите ввод: ";
        cin >> questionCount;
    }
    cout << "Вы выбрали " << questionCount << " вопросов.\n";
    waitForReturn();
    teacherMenu();
}

void generateExamTicket() {
    if (selectedTopic[0] == '\0') {
        cout << "Сначала выберите тему!\n";
        waitForReturn();
        teacherMenu();
        return;
    }
    if (questionCount <= 0) {
        cout << "Сначала выберите количество вопросов через пункт 2 в меню.\n";
        waitForReturn();
        teacherMenu();
        return;
    }
    int numberOfTickets;
    cout << "\nСколько билетов вы хотите сгенерировать по выбранной теме? ";
    cin >> numberOfTickets;
    while (numberOfTickets <= 0 || numberOfTickets > 100) {
        cout << "Некорректное число. Повторите ввод: ";
        cin >> numberOfTickets;
    }
    int topicIndex = -1;
    for (int i = 0; i < MAX_TOPICS; i++) {
        if (strcmp(selectedTopic, topics[i].name) == 0) {
            topicIndex = i + 1;
            break;
        }
    }
    cout << "Выбранная тема: " << selectedTopic << ", номер темы: " << topicIndex << endl;
    ifstream questionFile("data/questions.txt");
    if (!questionFile) {
        cout << "Ошибка: не удалось открыть файл data/questions.txt\n";
        return;
    }
    const int MAX_QUESTIONS = 100;
    string questions[MAX_QUESTIONS];
    int count = 0;
    string line;
    while (getline(questionFile, line) && count < MAX_QUESTIONS) {
        if (line.empty()) continue;
        if (!isdigit(line[0])) continue;
        int lineTopicNum = line[0] - '0';
        if (lineTopicNum == topicIndex) {
            questions[count] = line.substr(2);
            count++;
        }
    }
    questionFile.close();
    cout << "Найдено вопросов по теме: " << count << endl;
    int totalQuestionsNeeded = numberOfTickets * questionCount;
    if (count < totalQuestionsNeeded) {
        cout << "Предупреждение: количество доступных вопросов (" << count << ") меньше, чем необходимо для "
             << numberOfTickets << " билетов по " << questionCount << " вопроса(ов) без повторений ("
             << totalQuestionsNeeded << ").\n";
        cout << "Некоторые вопросы могут повторяться в разных билетах.\n";
        cout << "Нажмите 1 для продолжения или 0 для отмены: ";
        int choice;
        cin >> choice;
        if (choice != 1) {
            cout << "Генерация билетов отменена.\n";
            waitForReturn();
            teacherMenu();
            return;
        }
    } else if (count < questionCount) {
        cout << "Недостаточно вопросов по выбранной теме. Нужно минимум " << questionCount << ".\n";
        return;
    }
    srand(static_cast<unsigned int>(time(0)));
    ofstream outFile("data/examcard.txt", ios::app);
    if (!outFile) {
        cout << "Ошибка: не удалось открыть файл data/examcard.txt для записи\n";
        return;
    }
    if (currentTicketNumber == -1) {
        currentTicketNumber = 1;
    }
    bool usedQuestions[MAX_QUESTIONS] = {false};
    int usedCount = 0;
    for (int t = 0; t < numberOfTickets; t++) {
        outFile << "Основы алгоритмизации и программирования\n";
        outFile << "Билет №" << currentTicketNumber++ << "\n";
        outFile << selectedTopic << "\n";
        outFile << "Преподаватель: Конон С.И.\n";
        int ticketQuestions[100];
        int selectedForTicket = 0;
        if (usedCount + questionCount > count) {
            cout << "Внимание: сброс использованных вопросов для билета №" << (currentTicketNumber - 1) << endl;
            memset(usedQuestions, false, sizeof(usedQuestions));
            usedCount = 0;
        }
        while (selectedForTicket < questionCount) {
            int randomIndex = rand() % count;
            if (!usedQuestions[randomIndex]) {
                ticketQuestions[selectedForTicket] = randomIndex;
                usedQuestions[randomIndex] = true;
                selectedForTicket++;
                usedCount++;
            }
        }
        for (int i = 0; i < questionCount; i++) {
            outFile << "\nВопрос " << (i + 1) << ":\n";
            outFile << questions[ticketQuestions[i]] << "\n";
        }
        outFile << "==================================\n";
    }
    outFile.close();
    cout << numberOfTickets << " билет(ов) успешно сгенерировано и сохранено в data/examcard.txt!" << endl;
    waitForReturn();
    teacherMenu();
}

void sortQuestionsByTopicMenu() {
    int sortChoice;
    cout << "\nВыберите способ сортировки вопросов по теме:" << endl;
    cout << "1. Пузырьковая сортировка" << endl;
    cout << "2. Сортировка вставками" << endl;
    cout << "3. Быстрая сортировка (quicksort)" << endl;
    cout << "Ваш выбор: ";
    cin >> sortChoice;
    while (sortChoice < 1 || sortChoice > 3) {
        cout << "Неверный выбор. Повторите ввод: ";
        cin >> sortChoice;
    }
    switch (sortChoice) {
        case 1:
            bubbleSortQuestionsByTopic();
            break;
        case 2:
            insertionSortQuestionsByTopic();
            break;
        case 3:
            quickSortQuestionsByTopic();
            break;
    }
}

bool isLuckyTicketExists() {
    ifstream file("data/examcard.txt");
    if (!file) {
        return false;
    }
    string line;
    while (getline(file, line)) {
        if (line == "Счастливый билет") {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void generateLuckyTicket() {
    if (isLuckyTicketExists()) {
        cout << "Счастливый билет уже существует!\n";
    }
    else {
        ofstream out("data/examcard.txt", ios::app);
        out << "Основы алгоритмизации и программирования\n";
        out << "Счастливый билет\n";
        out << "Преподаватель: Конон С.И.\n\n";
        out << "Вопрос 1:\nПо какой дисциплине у нас экзамен?\n\n";
        out << "Вопрос 2:\nКакая фамилия у преподавателя?\n\n";
        out << "==================================" << endl << endl;
        out.close();
        cout << "Счастливый билет успешно добавлен!\n";
    }
    waitForReturn();
    teacherMenu();
}

void viewTicketsByTopic() {
    cout << "\nВыберите тему для просмотра билетов:\n";
    for (int i = 0; i < MAX_TOPICS; i++) {
        cout << i + 1 << ". " << topics[i].name << endl;
    }
    int choice;
    cout << "Введите номер темы: ";
    cin >> choice;
    while (choice < 1 || choice > MAX_TOPICS) {
        cout << "Неверный выбор. Повторите ввод: ";
        cin >> choice;
    }
    const string chosenTopic = topics[choice - 1].name;
    cout << "\nБилеты по теме: " << chosenTopic << "\n\n";
    ifstream file("data/examcard.txt");
    if (!file) {
        cout << "Файл examcard.txt не найден!\n";
        waitForReturn();
        teacherMenu();
        return;
    }
    string line;
    bool ticketFound = false;
    bool inTicket = false;
    bool topicMatched = false;
    string currentTicket;
    while (getline(file, line)) {
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line.find("Билет №") == 0) {
            if (inTicket && topicMatched) {
                cout << currentTicket << endl;
                ticketFound = true;
            }
            inTicket = true;
            topicMatched = false;
            currentTicket = line + "\n";
        } else if (inTicket) {
            currentTicket += line + "\n";
            if (line == chosenTopic) {
                topicMatched = true;
            }
            if (line.find("==") == 0) {
                if (topicMatched) {
                    cout << currentTicket << endl;
                    ticketFound = true;
                }
                inTicket = false;
                currentTicket.clear();
            }
        }
    }
    if (inTicket && topicMatched) {
        cout << currentTicket << endl;
        ticketFound = true;
    }
    if (!ticketFound) {
        cout << "\nНет билетов по данной теме.\n";
    }
    file.close();
    waitForReturn();
    teacherMenu();
}

void addQuestionToBank() {
    int repeat = 1;
    while (repeat == 1) {
        cout << "\nВыберите раздел, к которому относится новый вопрос:\n";
        for (int i = 0; i < MAX_TOPICS; i++) {
            cout << i + 1 << ". " << topics[i].name << endl;
        }
        int topicNumber;
        cout << "Введите номер раздела: ";
        cin >> topicNumber;
        while (topicNumber < 1 || topicNumber > MAX_TOPICS) {
            cout << "Неверный номер. Повторите ввод: ";
            cin >> topicNumber;
        }
        cout << "Введите текст вопроса (на одной строке):\n";
        char question[200];
        cin.ignore();
        cin.getline(question, 200);
        ofstream file("data/questions.txt", ios::app);
        if (!file) {
            cout << "Ошибка при открытии файла questions.txt\n";
        }
        else {
            file << topicNumber << " " << question << endl;
            file.close();
            cout << "\nВопрос успешно добавлен в банк заданий!\n";
        }
        cout << "\nХотите добавить ещё один вопрос? (1 — да, 0 — выход в меню): ";
        cin >> repeat;
        while (repeat != 0 && repeat != 1) {
            cout << "Неверный выбор. Повторите ввод.";
            cout << "Введите 1 или 0: ";
            cin >> repeat;
        }
    }
    cout << "\nВозврат в меню преподавателя...\n";
    teacherMenu();
}

void deleteQuestionFromBank() {
    int repeat = 1;
    while (repeat == 1) {
        cout << "\nВыберите раздел, из которого хотите удалить вопрос:\n";
        for (int i = 0; i < MAX_TOPICS; i++) {
            cout << i + 1 << ". " << topics[i].name << endl;
        }
        int topicNumber;
        cout << "Введите номер раздела: ";
        cin >> topicNumber;
        while (topicNumber < 1 || topicNumber > MAX_TOPICS) {
            cout << "Неверный номер. Повторите ввод: ";
            cin >> topicNumber;
        }
        ifstream file("data/questions.txt");
        if (!file) {
            cout << "Ошибка: не удалось открыть файл questions.txt\n";
            return;
        }
        const int MAX_QUESTIONS = 1000;
        string allQuestions[MAX_QUESTIONS];
        string topicQuestions[MAX_QUESTIONS];
        int questionIndices[MAX_QUESTIONS];
        int totalCount = 0;
        int topicCount = 0;
        string line;
        while (getline(file, line) && totalCount < MAX_QUESTIONS) {
            if (line.empty()) continue;
            allQuestions[totalCount] = line;
            if (line[0] == (topicNumber + '0')) {
                size_t spacePos = line.find(' ');
                if (spacePos != string::npos) {
                    topicQuestions[topicCount] = line.substr(spacePos + 1);
                    questionIndices[topicCount] = totalCount;
                    topicCount++;
                }
            }
            totalCount++;
        }
        file.close();
        if (topicCount == 0) {
            cout << "\nВ выбранном разделе нет вопросов для удаления.\n";
        } else {
            cout << "\nВопросы по теме \"" << topics[topicNumber - 1].name << "\":\n";
            for (int i = 0; i < topicCount; i++) {
                cout << i + 1 << ". " << topicQuestions[i] << endl;
            }
            int questionToDelete;
            cout << "\nВведите номер вопроса для удаления (1-" << topicCount << "): ";
            cin >> questionToDelete;
            while (questionToDelete < 1 || questionToDelete > topicCount) {
                cout << "Неверный номер. Повторите ввод: ";
                cin >> questionToDelete;
            }
            int indexToDelete = questionIndices[questionToDelete - 1];
            ofstream outFile("data/questions.txt");
            if (!outFile) {
                cout << "Ошибка: не удалось открыть файл questions.txt для записи\n";
                return;
            }
            for (int i = 0; i < totalCount; i++) {
                if (i != indexToDelete) {
                    outFile << allQuestions[i] << endl;
                } else {
                    cout << "Удалено: " << allQuestions[i] << endl;
                }
            }
            outFile.close();
            cout << "\nВопрос успешно удален из банка заданий!\n";
        }
        cout << "\nХотите удалить ещё один вопрос? (1 — да, 0 — выход в меню): ";
        cin >> repeat;
        while (repeat != 0 && repeat != 1) {
            cout << "Введите 1 или 0: ";
            cin >> repeat;
        }
    }
    cout << "\nВозврат в меню преподавателя...\n";
    teacherMenu();
}

void searchSolvedTicketMenu() {
    int searchChoice;
    cout << "\nВыберите способ поиска решённого билета:" << endl;
    cout << "1. Линейный поиск" << endl;
    cout << "2. Бинарный поиск" << endl;
    cout << "Ваш выбор: ";
    cin >> searchChoice;
    while (searchChoice < 1 || searchChoice > 2) {
        cout << "Неверный выбор. Повторите ввод: ";
        cin >> searchChoice;
    }
    switch (searchChoice) {
        case 1:
            viewSolvedTickets();
            break;
        case 2:
            binarySearchSolvedTicket();
            break;
    }
}

void viewSolvedTickets() {
    ifstream file("data/solutioncard.txt");
    if (!file) {
        cout << "Файл solutioncard.txt не найден!\n";
        return;
    }
    string lines[MAX_LINES];
    int lineCount = 0;
    while (getline(file, lines[lineCount]) && lineCount < MAX_LINES) {
        lineCount++;
    }
    file.close();
    int ticketStarts[MAX_LINES];
    int ticketNumbers[MAX_LINES];
    int ticketCount = 0;
    for (int i = 0; i < lineCount; ++i) {
        if (lines[i].find("Ответы на билет #") != string::npos) {
            ticketStarts[ticketCount] = i;
            size_t pos = lines[i].find("#");
            if (pos != string::npos) {
                size_t start = pos + 1;
                size_t end = start;
                while (end < lines[i].size() && isdigit(lines[i][end])) end++;
                ticketNumbers[ticketCount] = stoi(lines[i].substr(start, end - start));
            } else {
                ticketNumbers[ticketCount] = -1;
            }
            ticketCount++;
        }
    }
    if (ticketCount == 0) {
        cout << "Нет решённых билетов.\n";
        return;
    }
    cout << "\nРешённые билеты:\n";
    for (int i = 0; i < ticketCount; ++i) {
        cout << "Билет №" << ticketNumbers[i] << ":" << endl;
    }
    int inputNumber;
    bool found = false;
    int ticketIdx = -1;
    do {
        cout << "\nВведите номер билета для просмотра: ";
        cin >> inputNumber;
        found = false;
        for (int i = 0; i < ticketCount; ++i) {
            if (ticketNumbers[i] == inputNumber) {
                found = true;
                ticketIdx = i;
                break;
            }
        }
        if (!found) {
            cout << "Неверный выбор. Повторите ввод.";
        }
    } while (!found);
    int start = ticketStarts[ticketIdx];
    int end = (ticketIdx + 1 < ticketCount) ? ticketStarts[ticketIdx + 1] : lineCount;
    cout << "\n--- Решённый билет №" << inputNumber << " ---\n\n";
    for (int i = start; i < end; ++i) {
        cout << lines[i] << endl;
    }
    waitForReturn();
    teacherMenu();
}

void bubbleSortQuestionsByTopic() {
    ifstream inputFile("data/questions.txt");
    if (!inputFile) {
        cout << "Ошибка: не удалось открыть файл questions.txt\n";
        return;
    }
    const int MAX_QUESTIONS = 1000;
    string questions[MAX_QUESTIONS];
    int topicNumbers[MAX_QUESTIONS];
    int count = 0;
    string line;
    while (getline(inputFile, line) && count < MAX_QUESTIONS) {
        if (!line.empty() && isdigit(line[0])) {
            questions[count] = line;
            topicNumbers[count] = line[0] - '0';
            count++;
        }
    }
    inputFile.close();
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (topicNumbers[j] > topicNumbers[j + 1]) {
                swap(topicNumbers[j], topicNumbers[j + 1]);
                swap(questions[j], questions[j + 1]);
            }
        }
    }
    ofstream outputFile("data/questions.txt");
    if (!outputFile) {
        cout << "Ошибка: не удалось открыть файл questions.txt для записи\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        outputFile << questions[i] << "\n";
    }
    outputFile.close();
    cout << "Вопросы успешно отсортированы по темам (пузырьком)!\n";
    waitForReturn();
    teacherMenu();
}

void insertionSortQuestionsByTopic() {
    ifstream inputFile("data/questions.txt");
    if (!inputFile) {
        cout << "Ошибка: не удалось открыть файл questions.txt\n";
        return;
    }
    const int MAX_QUESTIONS = 1000;
    string questions[MAX_QUESTIONS];
    int topicNumbers[MAX_QUESTIONS];
    int count = 0;
    string line;
    while (getline(inputFile, line) && count < MAX_QUESTIONS) {
        if (!line.empty() && isdigit(line[0])) {
            questions[count] = line;
            topicNumbers[count] = line[0] - '0';
            count++;
        }
    }
    inputFile.close();
    for (int i = 1; i < count; i++) {
        int key = topicNumbers[i];
        string qKey = questions[i];
        int j = i - 1;
        while (j >= 0 && topicNumbers[j] > key) {
            topicNumbers[j + 1] = topicNumbers[j];
            questions[j + 1] = questions[j];
            j--;
        }
        topicNumbers[j + 1] = key;
        questions[j + 1] = qKey;
    }
    ofstream outputFile("data/questions.txt");
    if (!outputFile) {
        cout << "Ошибка: не удалось открыть файл questions.txt для записи\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        outputFile << questions[i] << "\n";
    }
    outputFile.close();
    cout << "Вопросы успешно отсортированы по темам (вставками)!\n";
    waitForReturn();
    teacherMenu();
}

void quickSortQuestionsByTopicHelper(string questions[], int topicNumbers[], int left, int right) {
    if (left >= right) return;
    int pivot = topicNumbers[(left + right) / 2];
    int i = left, j = right;
    while (i <= j) {
        while (topicNumbers[i] < pivot) i++;
        while (topicNumbers[j] > pivot) j--;
        if (i <= j) {
            swap(topicNumbers[i], topicNumbers[j]);
            swap(questions[i], questions[j]);
            i++;
            j--;
        }
    }
    if (left < j) quickSortQuestionsByTopicHelper(questions, topicNumbers, left, j);
    if (i < right) quickSortQuestionsByTopicHelper(questions, topicNumbers, i, right);
}

void quickSortQuestionsByTopic() {
    ifstream inputFile("data/questions.txt");
    if (!inputFile) {
        cout << "Ошибка: не удалось открыть файл questions.txt\n";
        return;
    }
    const int MAX_QUESTIONS = 1000;
    string questions[MAX_QUESTIONS];
    int topicNumbers[MAX_QUESTIONS];
    int count = 0;
    string line;
    while (getline(inputFile, line) && count < MAX_QUESTIONS) {
        if (!line.empty() && isdigit(line[0])) {
            questions[count] = line;
            topicNumbers[count] = line[0] - '0';
            count++;
        }
    }
    inputFile.close();
    quickSortQuestionsByTopicHelper(questions, topicNumbers, 0, count - 1);
    ofstream outputFile("data/questions.txt");
    if (!outputFile) {
        cout << "Ошибка: не удалось открыть файл questions.txt для записи\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        outputFile << questions[i] << "\n";
    }
    outputFile.close();
    cout << "Вопросы успешно отсортированы по темам (быстрая сортировка)!\n";
    waitForReturn();
    teacherMenu();
}

void binarySearchSolvedTicket() {
    ifstream file("data/solutioncard.txt");
    if (!file) {
        cout << "Файл solutioncard.txt не найден!\n";
        return;
    }
    string lines[MAX_LINES];
    int lineCount = 0;
    while (getline(file, lines[lineCount]) && lineCount < MAX_LINES) {
        lineCount++;
    }
    file.close();
    // Собираем массив билетов
    SolvedTicket tickets[MAX_LINES];
    int ticketCount = 0;
    for (int i = 0; i < lineCount; ++i) {
        if (lines[i].find("Ответы на билет #") != string::npos) {
            tickets[ticketCount].startLine = i;
            // Получаем номер билета
            size_t pos = lines[i].find("#");
            if (pos != string::npos) {
                tickets[ticketCount].number = stoi(lines[i].substr(pos + 1));
            } else {
                tickets[ticketCount].number = -1;
            }
            ticketCount++;
        }
    }
    // Устанавливаем endLine для каждого билета
    for (int i = 0; i < ticketCount; ++i) {
        if (i + 1 < ticketCount) {
            tickets[i].endLine = tickets[i + 1].startLine;
        } else {
            tickets[i].endLine = lineCount;
        }
    }
    // Сортируем билеты по номеру (на случай, если не отсортированы)
    for (int i = 0; i < ticketCount - 1; ++i) {
        for (int j = 0; j < ticketCount - i - 1; ++j) {
            if (tickets[j].number > tickets[j + 1].number) {
                swap(tickets[j], tickets[j + 1]);
            }
        }
    }
    if (ticketCount == 0) {
        cout << "Нет решённых билетов.\n";
        return;
    }
    cout << "\nРешённые билеты:\n";
    for (int i = 0; i < ticketCount; ++i) {
        cout << "Билет №" << tickets[i].number << ":" << endl;
    }
    cout << "\nВведите номер билета для просмотра: ";
    int searchNumber;
    cin >> searchNumber;
    // Бинарный поиск
    int left = 0, right = ticketCount - 1, found = -1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (tickets[mid].number == searchNumber) {
            found = mid;
            break;
        } else if (tickets[mid].number < searchNumber) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    if (found == -1) {
        cout << "Билет с таким номером не найден.\n";
        waitForReturn();
        teacherMenu();
        return;
    }
    cout << "\n--- Решённый билет №" << searchNumber << " ---\n\n";
    for (int i = tickets[found].startLine; i < tickets[found].endLine; ++i) {
        cout << lines[i] << endl;
    }
    waitForReturn();
    teacherMenu();
}