// d-clutch_v1.5eng
//
// консольное приложение для учета финансов на банковских картах

#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <string>
#include <limits>
#include <filesystem>
#include <cmath>
#include <algorithm>
#include <cctype>

using namespace std;
namespace fs = std::filesystem;

void nowData(int w, int d, int m, int y);
void nowData(int d, int m, int y);
void allFunctions(int j, int const m);
void functions(int j);
void totally(int total, int month, string dCluthcPath, string generalName);
void changeCardValue(string dCluthcPath, string *events, int j, bool plusminus, int day, int month, int year, string generalName);
void autorun(int tog);
void manual(string dCluthcPath, string generalName);
int checkDigit();
int checkNumber();
bool checkStringContains(const string &str);
bool fileExists(const string &filename);
string checkString();

int main()
{
    setlocale(LC_ALL, "RU");

    // НАСТРОЙКА ОТОБРАЖЕНИЯ КОНСОЛИ
    // MoveWindow(GetConsoleWindow(), 1150, 220, 740, 670, TRUE); // гориз, верт, шир, выс, ХЗ
    system("color 1F");                                        // установка цвета консоли/цвета шрифта
    // HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE ); // установка шрифта
    // CONSOLE_FONT_INFOEX fontInfo;
    // fontInfo.cbSize = sizeof( fontInfo );
    // GetCurrentConsoleFontEx( hConsole, TRUE, &fontInfo );
    // wcscpy( fontInfo.FaceName, L"Lucida Console" ); // установка типа шрифта
    // fontInfo.dwFontSize.Y = 15; // установка размера шрифта
    // SetCurrentConsoleFontEx( hConsole, TRUE, &fontInfo );

    cout << "\n ---     d-clutch     ---\n";

    /// 0. добавить иконку
    /// 1. Оптимизировать код
    // вынести в функцию remainds -- total
    // добавить точки при отображении больших чисел
    // неправильно считает баланс по лимиту и экономи/расход, если считать сразу
    /// 2. ввести день зарплаты

    //// остановился на: 

    time_t now = time(0); // текущая дата/время, основанные на текущей системе <ctime>
    struct tm *ltm = localtime(&now);
    int year, month, day, wday;
    year = 1900 + ltm->tm_year;
    month = 1 + ltm->tm_mon;
    day = ltm->tm_mday;
    wday = ltm->tm_wday;

    const int *n = new const int{3210}; // просто здоровое число для счётчиков
    const int *m = new const int{99};   // переменные там всякие
    int question = 123;                 // 0 занят
    int remaind = 0;
    int limit = 0;
    int total = 0;
    int j = 0;
    int credordebt;
    string *events = new string[*m];
    string *remainds = new string[*m];
    bool plusminus = true;

    struct tm a = {0, 0, 0, day, month - 1, 101, 0, 0, 0}; // текущая дата
    time_t x = mktime(&a);                                 //
    // struct tm b = { 0,0,0,0,credordebt,101,0,0,0 }; // ожидаемая дата
    // time_t y = mktime(&b); //

    nowData(wday, day, month, year); // отображение текущей даты

    char *re = new char[MAX_PATH]{};
    string dCluthcPath = string(re, GetModuleFileNameA(NULL, re, MAX_PATH));

    //   cout << "\n15:41\n"; // для тестов

    fs::path getName = fs::path(dCluthcPath).stem();
    string generalName = getName.generic_string();


    if (generalName == "d-clutch_v1.4eng" && !fileExists(generalName + ".txt"))
    {
        cout << "\n To keep your personal data hidden, close the";
        cout << "\n application, move it to any folder and rename";
        cout << "\n (do not use spaces and characters ; - / | *)." << "\n\n";
    }

    // ОТКРЫВАЕТ СОХРАНЕННЫЕ СОБЫТИЯ
    ifstream file1;
    file1.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
    if (file1.is_open())
    {
        char buf;
        for (int i{}, a{}; i < *n; i++)
        {
            file1 >> buf;
            if (buf != '|')
                a = 0;
            if (buf == '|')
            {
                j++;
                a++;
            }
            if (a > 1 || buf == ';')
                break;
        }
        file1.close();
        cout << "\n";
    }
    ifstream file2;
    file2.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
    if (file2.is_open())
    {
        string buffer0;
        string buffer1;
        int k = j;
        for (int i{}; i < *n; i++)
        {
            file2 >> buffer0;
            if (buffer0 == "|")
            {
                file2 >> buffer0;
                file2 >> buffer1;
                file2 >> remainds[k];
                cout << " " << buffer0 << "-" << remainds[k] << "." << endl;
            }
            if (buffer0 == "/")
                k--;
            if (k < 0 || buffer0 == "" || buffer0 == ";")
                break;
        }
        file2.close();
    }

    for (int i = 1; i <= j; i++) // подсчёт общего остатка
        total += atoi(remainds[i].c_str());
    if (j > 0)
        totally(total, month, dCluthcPath, generalName);

    // ИНДЕКСАЦИЯ СОБЫТИЙ
    ifstream file3;
    bool event = false;
    string buffer00;
    file3.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::in);
    if (file3.is_open())
    {

        int k = j, q = 1;
        for (int i = 0; i < *n; i++)
        {
            file3 >> buffer00;
            if (event == true)
                events[q] = buffer00;
            if (buffer00 == "/")
            {
                k--;
                q++;
            }
            if (k < 0)
                break;
            if (buffer00 == "|")
                event = true;
            if (buffer00 != "|")
                event = false;
        }
        file3.close();
    }

    // ОТОБРАЖЕНИЕ СЕРВИСНЫХ ФУНКЦИЙ
    allFunctions(j, *m);

    // ЦИКЛ ВВОДА ДАННЫХ
    for (int i{}; i < *m; i++)
    {
        if (question == 11)
            break;
        if (question >= 0 && question != 11)
        {
            question = checkDigit();

            // ИНСТРУКЦИЯ
            if (i >= 0 && question == 0)
                manual(dCluthcPath, generalName);

            // ОБНОВЛЕНИЕ ДАННЫХ
            else if (j > 0 && question == 1)
            {
                int *newData = new int[*m]{};
                string *buff0 = new string[*m];
                string *buff = new string[*n];
                string *buffer = new string[*n];
                string buf{};
                total = 0;

                for (int i = 1; i <= j; i++)
                { // ввод новых данных о балансах
                    cout << events[i] << " = ";
                    newData[i] = checkNumber();
                    cout << "\n";
                }

                for (int i = 1; i <= j; i++) // подсчёт общего остатка
                    total += newData[i];
                // добавление заголовка о дате и тотале в buff
                buff[0] = "Date " + to_string(day) + " " + to_string(month) + " " + to_string(year) + "  -  " + to_string(total) + " ru";

                // добавление остальной информации в buff (до знака ;)
                ifstream file4(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::in);
                int o = 0;
                bool flag = false;
                for (int i = 1; file4; i++)
                {
                    file4 >> buf;
                    if (flag == true)
                        buff[i - 3] = buf;
                    else if (buf == "|" && flag == false)
                    {
                        buff[i - 3] = buf;
                        flag = true;
                    }
                    o++;
                    if (buf == ";")
                        break;
                }
                file4.close();
                // замена в buff старых на новые значения балансов
                for (int i = 0, l = 1; i < o; i++)
                {
                    if (buff[i] == "-")
                    {
                        buff[i + 1] = to_string(newData[l]);
                        l++;
                    }
                }

                // считывание даты в buff0
                ifstream file5(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::in);
                for (int i{}; i < 4; i++)
                    file5 >> buff0[i];
                file5.close();

                // если обновление данных происходило сегодня, то файл ПЕРЕписывается
                if (day == stoi(buff0[1]) && month == stoi(buff0[2]) && year == stoi(buff0[3]))
                {
                    int k{};
                    // считывание старых данных из файла
                    ifstream file06(fs::path(dCluthcPath).replace_filename(generalName + ".txt"),
                                    ios::in);
                    for (int i = 0; file06; i++)
                    {
                        file06 >> buffer[i];
                        buffer[i] += " ";
                        k++;
                    }
                    file06.close();
                    // замена новыми данными
                    ofstream file6(fs::path(dCluthcPath).replace_filename(generalName + ".txt"),
                                   ios::out);
                    for (int i = 0; i < o - 3; i++)
                    {
                        buff[i] += " ";
                        file6 << buff[i];
                    }
                    file6 << ";\n\n";
                    // добавление старых данных
                    for (int i = 0; i < k + o; i++)
                    {
                        file6 << buffer[i + o];
                        if (buffer[i + o] == "; ")
                            file6 << "\n\n";
                    }
                    file6.close();

                    // подсчёт общего остатка
                    total = 0;
                    for (int i = 1; i <= j; i++)
                        total += newData[i];
                    if (j > 0)
                        totally(total, month, dCluthcPath, generalName);
                }
                // если обновление данных происходило НЕ сегодня, то файл ДОписывается
                else
                {
                    int k{};
                    // считывание старых данных из файла
                    ifstream file07(fs::path(dCluthcPath).replace_filename(generalName + ".txt"),
                                    ios::in);
                    for (int i = 0; file07; i++)
                    {
                        file07 >> buffer[i];
                        buffer[i] += " ";
                        k++;
                    }
                    file07.close();
                    // замена новыми данными
                    ofstream file7(fs::path(dCluthcPath).replace_filename(generalName + ".txt"),
                                   ios::out);

                    for (int i = 0; i < o - 3; i++)
                    {
                        buff[i] += " ";
                        file7 << buff[i];
                    }
                    file7 << ";\n\n";
                    file7.close();

                    // добавление старых данных
                    ofstream file70(fs::path(dCluthcPath).replace_filename(generalName + ".txt"),
                                    ios::app);
                    for (int i = 0; i < k; i++)
                    {
                        file70 << buffer[i];
                        if (buffer[i] == "; ")
                            file70 << "\n\n";
                    }
                    file70.close();

                    // подсчёт общего остатка
                    total = 0;
                    for (int i = 1; i <= j; i++) // подсчёт общего остатка
                        total += newData[i];
                    if (j > 0)
                        totally(total, month, dCluthcPath, generalName);
                }
                delete[] buff;
                delete[] buffer;
                delete[] newData;
                delete[] buff0;
            }

            // ДОБАВЛЕНИЕ ПОСТУПЛЕНИЯ НА ОДНУ ИЗ КАРТ (функция "Добавить доход")
            else if (j > 0 && question == 2)
            {
                plusminus = true;
                changeCardValue(dCluthcPath, events, j, plusminus, day, month, year, generalName);
            }

            // ДОБАВЛЕНИЕ ЕДИНИЧНОГО РАСХОДА(ПОКУПКИ) (функция "Добавить расход")
            else if (j > 0 && question == 3)
            {
                plusminus = false;
                changeCardValue(dCluthcPath, events, j, plusminus, day, month, year, generalName);
            }

            // СОЗДАНИЕ
            else if (j < *m && question == 7)
            {
                remaind = 0;
                total = 0;
                int create{};
                int quest = 1;
                string *buf = new string[*n];
                string message;
                for (int i{}; i < *m; i++)
                {
                    if (quest == 1)
                    {
                        cout << "  Name of the new card? (Don't use: spaces or -;/|*)\n";
                        message = checkString();
                        limit = 0;
                        cout << "  Enter the remainder\n";
                        remaind = checkNumber();
                        cout << "  Credit card (press 1). Debit (press 0).\n";
                        credordebt = checkNumber();
                        if (credordebt == 1)
                        {
                            cout << "  Credit limit\n";
                            limit = checkNumber();
                        }
                        buf[i] = "| " + message + " - ";
                        buf[i] += to_string(remaind) + " ";
                        buf[i] += to_string(limit) + " /";
                    }
                    cout << "  Add another resource? (Yes-1 / No-0)\n";
                    create++;
                    quest = checkNumber();
                    if (quest == 0)
                        break;
                }
                // обновление в текстовике
                int k{};
                string *buff = new string[*n];

                if (fileExists(generalName) == false)
                {
                    ofstream name0;
                    name0.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::out);
                    if (name0.is_open())
                    {
                        name0 << "Date " + to_string(day) + " " + to_string(month) + " " + to_string(year) + " - " + to_string(total) + " ru ";
                        for (int i{}; i < create; i++)
                        {
                            name0 << buf[i] + " ";
                        }
                        name0 << "; \n\n";
                        name0.close();
                    }
                    else
                        cout << "\nname0 error\n\n";
                }
                else
                {
                    ifstream name;
                    name.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                    if (name.is_open())
                    {
                        for (int i{}; name; i++)
                        {
                            name >> buff[i];
                            buff[i] += " ";
                            k++;
                        }
                        bool flag = true;
                        for (int i{}, j{}; i < k; i++)
                        {
                            if (buff[i] == "; " && flag == true)
                            {
                                buff[i] = "";
                                for (; j < create; j++)
                                    buff[i] += buf[j] += " ";
                                flag = false;
                                buff[i] += "; \n\n";
                            }
                            if (buff[i] == "; " && flag == false)
                                buff[i] = "; \n\n";
                        }
                        name.close();
                    }
                    else
                        cout << "\nname error\n\n";
                    ofstream name1;
                    name1.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                    if (name1.is_open())
                    {
                        for (int i{}; i < k; i++)
                            name1 << buff[i];
                        name1.close();
                    }
                    else
                        cout << "\nname1 error\n\n";
                }

                ifstream name2;
                name2.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                if (name2.is_open())
                {
                    string buffer0;
                    string buffer1;
                    k = 0;
                    for (int i{}; i < *n; i++)
                    {
                        name2 >> buffer0;
                        if (buffer0 == "|")
                        {
                            name2 >> buffer0;
                            name2 >> buffer1;
                            name2 >> remainds[k];
                            k++;
                        }
                        if (buffer0 == ";")
                            break;
                    }
                    for (int i{}; i < k; i++) // подсчёт общего остатка
                        total += stoi(remainds[i]);
                    name2.close();
                }
                else
                    cout << "\nname2 error\n\n";

                totally(total, month, dCluthcPath, generalName);
                j = 1;

                delete[] buff;
                delete[] buf;
            }

            // УДАЛЕНИЕ
            else if (j > 0 && question == 8)
            {
                cout << "  To delete a resource:\n";
                for (int i = 1; i <= j; i++)
                    cout << "  " << events[i] << "  (press " << i << ")" << "\n";

                int quest{};
                quest = checkNumber();

                if (quest > 0 && quest <= j)
                {
                    string *buff = new string[*n];
                    int k{};
                    ifstream delfile(fs::path(dCluthcPath).replace_filename(generalName + ".txt"),
                                     ios::in);
                    for (int i{}; delfile; i++)
                    {
                        delfile >> buff[i];
                        buff[i] += " ";
                        k++;
                    }
                    delfile.close();

                    ofstream delfiles(fs::path(dCluthcPath).replace_filename(generalName + ".txt"),
                                      ios::out);
                    for (int i{}, l{}; i < k - 1; i++)
                    {
                        if (buff[i] == "| " || buff[i] == "; ")
                            l++;
                        if (quest == l)
                            continue;
                        if (quest != l)
                            delfiles << buff[i];
                        if (buff[i] == "; ")
                            delfiles << " \n\n";
                    }
                    cout << "  " << events[quest] << " deleted\n\n";
                    delfiles.close();

                    total = 0;
                    ifstream delfiles2;
                    delfiles2.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                    if (delfiles2.is_open())
                    {
                        string buffer0;
                        string buffer1;
                        k = 0;
                        for (int i{}; i < *n; i++)
                        {
                            delfiles2 >> buffer0;
                            if (buffer0 == "|")
                            {
                                delfiles2 >> buffer0;
                                delfiles2 >> buffer1;
                                delfiles2 >> remainds[k];
                                cout << " " << buffer0 << "-" << remainds[k] << ".\n";
                                k++;
                            }
                            if (buffer0 == ";")
                                break;
                        }
                        for (int i{}; i < k; i++) // подсчёт общего остатка
                            total += atoi(remainds[i].c_str());
                        delfiles2.close();
                    }
                    else
                        cout << "\nname error\n\n";

                    totally(total, month, dCluthcPath, generalName);
                    j = 1;

                    delete[] buff;
                }
            }

            // АВТОЗАГРУЗКА
            else if (question == 10)
            {
                LONG check = RegGetValueA(HKEY_CURRENT_USER,
                                          "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", "d-clutch",
                                          RRF_RT_REG_SZ, 0, 0, 0);
                if (check == 0)
                    autorun(8);
                if (check == 2)
                    autorun(9);
            }

            // ПЕРЕСЧЁТ ДО 1-ГО ЧИСЛА СЛЕД. МЕСЯЦА (функция "On next month")
            else if (j > 0 && question == 5)
            {
                int quest{1000};
                int answ{};
                string buffer0;
                total = 0;
                cout << "\n  On next month - ";

                ifstream onNextMonth;
                onNextMonth.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                if (onNextMonth.is_open())
                {
                    for (int i{}; onNextMonth; i++)
                    {
                        onNextMonth >> buffer0;
                        if (buffer0 == "*")
                        {
                            onNextMonth >> quest;
                            break;
                        }
                    }
                    onNextMonth.close();
                }

                for (int i = 1; i <= j; i++) // подсчёт общего остатка
                    total += atoi(remainds[i].c_str());

                struct tm c = {0, 0, 0, 0, month + 1, 101, 0, 0, 0}; // пересчёт на дни
                time_t y = mktime(&c);

                if (x != (time_t)(-1) && y != (time_t)(-1))

                    cout << floor(total / (difftime(y, x) / (60 * 60 * 24)))
                         << " ru./day.\n\n\n";

                if (x != (time_t)(-1) && y != (time_t)(-1))
                    answ = total - (difftime(y, x) / (60 * 60 * 24)) * quest;
                cout << "  The balance at the end of the month (limit "
                     << quest << " ru) = " << answ << " ru.\n\n";
            }

            // ПЕРЕСЧЁТ НА КОЛ-ВО ДНЕЙ ИСХОДЯ ИЗ ЗАДАННОГО ЛИМИТА (функция "Balance at the limit")
            else if (j > 0 && question == 4)
            {
                int quest{1000};
                int answ{};
                int diff{};
                int k{};
                string *buffer0 = new string;
                string *buffer = new string[*n];
                total = 0;

                fstream limit0;
                limit0.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                if (limit0.is_open())
                {
                    for (int i{}; limit0; i++)
                    {
                        limit0 >> *buffer0;
                        if (*buffer0 == "*")
                        {
                            limit0 >> quest;
                            break;
                        }
                    }
                    limit0.close();
                }

                int change{};
                cout << "  Limit is: " << quest << ". Change the limit press (Yes-1 / No-0)\n";
                change = checkNumber();
                if (change == 1)
                {
                    cout << "  Enter limit on day\n"; // для ввода лимита
                    quest = checkNumber();            // для ввода лимита

                    bool flag = false;
                    fstream limit1;
                    limit1.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                    if (limit1.is_open())
                    {
                        for (int i{}; limit1; i++)
                        {
                            limit1 >> buffer[i];
                            buffer[i] += " ";
                            if (buffer[i] == "* ")
                            {
                                buffer[i] = "* " + quest;
                                flag = true;
                                break;
                            }
                            if (buffer[i] == "; ")
                                buffer[i] = "; \n\n";
                            k++;
                        }
                        limit1.close();
                    }
                    else
                        cout << "\nlimit1 error\n\n";

                    if (flag == false)
                    {
                        fstream limit2;
                        limit2.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::app);
                        if (limit2.is_open())
                        {
                            limit2 << "* " << quest;
                            limit2.close();
                        }
                        else
                            cout << "\nlimit2 error\n\n";
                    }
                    else
                    {
                        fstream limit3;
                        limit3.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                        if (limit3.is_open())
                        {
                            for (int i{}; i < k; i++)
                            {
                                limit3 << buffer[i];
                            }
                            limit3 << "* ";
                            limit3 << to_string(quest);
                            limit3 << " ";
                            limit3.close();
                        }
                        else
                            cout << "\nlimit3 error\n\n";
                    }
                }

                string *buf = new string{};
                string *buff = new string[6];
                string *buff2 = new string[5];
                string *bufer = new string[*m];
                ifstream file8(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::in);
                for (int i{}; i < 6; i++)
                    file8 >> buff[i];
                for (int i{}; i < *m; i++)
                    file8 >> bufer[i];
                for (int i{}, l{}; i < *m; i++)
                {
                    if (bufer[i] == ";")
                    {
                        buff2[l] = bufer[i + 2];
                        buff2[l + 1] = bufer[i + 3];
                        buff2[l + 2] = bufer[i + 4];
                        buff2[l + 3] = bufer[i + 6];
                        break;
                    }
                }
                file8.close();

                // дата посл.ввода

                ifstream file88;
                file88.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                if (file88.is_open())
                {
                    string *buffer0 = new string;
                    string *buffer1 = new string;
                    k = 0;
                    for (int i{}; i < *n; i++)
                    {
                        file88 >> *buffer0;
                        if (*buffer0 == "|")
                        {
                            file88 >> *buffer0;
                            file88 >> *buffer1;
                            file88 >> remainds[k];
                            k++;
                        }
                        if (*buffer0 == ";")
                            break;
                    }
                    for (int i{}; i < k; i++) // подсчёт общего остатка
                        total += stoi(remainds[i]);
                    file88.close();
                    delete buffer0;
                    delete buffer1;
                }
                else
                {
                    cout << "\nfile88 error\n\n";
                }

                struct tm a = {0, 0, 0, stoi(buff[1]), stoi(buff[2]) - 1, 101, 0, 0, 0};
                time_t x = mktime(&a);
                struct tm b = {0, 0, 0, 0, month, 101, 0, 0, 0}; // ожидаемая дата
                time_t y = mktime(&b);

                if (x != (time_t)(-1) && y != (time_t)(-1))
                    answ = total - (difftime(y, x) / (60 * 60 * 24)) * quest;
                cout << "  The balance at the end of the month (limit "
                     << quest << " ru) = " << answ << " ru.\n\n";

                if (buff2[1].empty() == false)
                {
                    // дата посл.ввода
                    a = {0, 0, 0, stoi(buff2[0]), stoi(buff2[1]) - 1, 101, 0, 0, 0};
                    x = mktime(&a);
                    // ожидаемая дата
                    b = {0, 0, 0, stoi(buff[1]), stoi(buff[2]) - 1, 101, 0, 0, 0};
                    y = mktime(&b);

                    if (x != (time_t)(-1) && y != (time_t)(-1))
                        diff = (stoi(buff[3]) - total) - ((difftime(y, x) / (60 * 60 * 24)) * quest);
                    if (diff > 0)
                        cout << "  Overdraft from " << stoi(buff2[0]) << "."
                             << stoi(buff2[1]) << "." << stoi(buff2[2])
                             << " = " << diff << " ru.\n\n";
                    else if (diff < 0)
                        cout << "  Economy from " << stoi(buff2[0]) << "."
                             << stoi(buff2[1]) << "." << stoi(buff2[2])
                             << " = " << abs(diff) << " ru.\n\n";
                    else
                        cout << "  Expenses meet the limit.\n\n";
                }
                delete buf;
                delete buffer0;
                delete[] buff;
                delete[] buff2;
                delete[] bufer;
                delete[] buffer;
            }

            // ОТОБРАЖЕНИЕ ПОЛНОЙ ЗАДОЛЖЕННОСТИ (функция "Total debt")
            else if (j < *m && question == 6)
            {
                int d{};
                int debt{};
                string *buff = new string[*m];
                string *bufDebt = new string[*m];
                string *cardDebt = new string[*m];
                ifstream file10(fs::path(dCluthcPath).replace_filename(generalName + ".txt"),
                                ios::in);
                for (int i{}, l{}; i < *m; i++)
                {
                    file10 >> buff[i];
                    if (buff[i] == "/")
                    {
                        bufDebt[l] = buff[i - 1];
                        debt += stoi(bufDebt[l]);
                        if (stoi(bufDebt[l]) > 0)
                        {
                            cardDebt[d] = buff[i - 4];
                            cardDebt[d + 1] = buff[i - 2];
                            cardDebt[d + 2] = buff[i - 1];
                            d += 3;
                        }
                        l++;
                    }
                    if (buff[i] == ";")
                        break;
                }
                if (debt - total > 0)
                {
                    cout << "  Total debt = " << debt - total << "\n"
                         << "  Your credit card(s):\n";
                    for (int i{}; i < d; i += 3)
                    {
                        cout << "  " << cardDebt[i] << " - balance " << cardDebt[i + 1]
                             << ", debt " << cardDebt[i + 2] << "\n";
                    }
                    cout << "\n";
                }
                if (debt - total <= 0)
                    cout
                        << "  The debt balance is positive = "
                        << abs(debt - total) << "\n";
                file10.close();

                cout << "\n";

                delete[] buff;
                delete[] bufDebt;
                delete[] cardDebt;
            }

            // ОБНОВЛЕНИЕ ЛИМИТА КАРТЫ (функция "Update card limit")
            else if (j < *m && question == 9)
            {
                int o{};
                int quest{};
                int upLimit{};
                string *buff = new string[*n];
                string *buffer = new string[*n];

                ifstream file11(fs::path(dCluthcPath).replace_filename(generalName + ".txt"),
                                ios::in);
                for (int i = 0; file11; i++)
                {
                    file11 >> buff[i];
                    o++;
                }

                cout << "  To update a card limit:\n";

                // отображение кредитных карт
                for (int i{}, l = 1; i < o; i++)
                {
                    if (buff[i] == "|" && stoi(buff[i + 4]) > 0)
                    {
                        events[l] = buff[i + 1];
                        cout << "  " << events[l] << " - limit = " << buff[i + 4] << " (press " << l << ")"
                             << "\n";
                        l++;
                    }
                    if (buff[i] == ";")
                        break;
                }
                file11.close();

                // выбор карты
                quest = checkNumber();
                // ввод нового лимита
                cout << "\n  Enter new limit:\n";
                upLimit = checkNumber();

                // перезапись нового лимита
                if (quest > 0 && quest <= j)
                {
                    ofstream file12(fs::path(dCluthcPath).replace_filename(generalName + ".txt"),
                                    ios::out);
                    bool flag = true;
                    for (int i{}, l{}; i < o; i++)
                    {
                        file12 << buff[i] << " ";
                        if (buff[i] == "|" && stoi(buff[i + 4]) > 0)
                            l++;
                        if (l == quest && flag == true)
                        {
                            if (buff[i + 1] == ";")
                                file12 << buff[i + 1] << "\n\n";
                            else
                                file12 << buff[i + 1] << " ";
                            if (buff[i + 2] == ";")
                                file12 << buff[i + 2] << "\n\n";
                            else
                                file12 << buff[i + 2] << " ";
                            if (buff[i + 3] == ";")
                                file12 << buff[i + 3] << "\n\n";
                            else
                                file12 << buff[i + 3] << " ";
                            if (buff[i + 4] == ";")
                                file12 << upLimit << "\n\n";
                            else
                                file12 << upLimit << " ";
                            flag = false;
                            i += 4;
                        }

                        if (buff[i] == ";")
                            file12 << "\n\n";
                    }
                    file12.close();
                }

                cout << "\n\n";

                delete[] buffer;
                delete[] buff;
            }
            functions(j);
            cout << "        To exit, press   (11)\n\n";
        }
    }

    cout << "\n\n\n* Restart to enter new data\n\n";

    delete n;
    delete[] re;
    delete[] events;
    delete[] remainds;

    // system("pause");
}

///////////////////////////////////////////////////////////////////////////////////

void nowData(int w, int d, int m, int y)
{ // отображение текущей даты
    cout << "\n - Today ";
    switch (w)
    {
    case 1:
        cout << "mon";
        break;
    case 2:
        cout << "tue";
        break;
    case 3:
        cout << "wed";
        break;
    case 4:
        cout << "thu";
        break;
    case 5:
        cout << "fri";
        break;
    case 6:
        cout << "sat";
        break;
    case 7:
        cout << "sun";
        break;
    }
    cout << "." << d << "." << m << "." << y << ". -\n\n";
}

void nowData(int d, int m, int y)
{ // отображение текущей даты (без дня недели)
    cout << "\n - Today ";
    cout << "." << d << "." << m << "." << y << ". -\n";
}

void allFunctions(int j, int const m)
{
    cout << "        Manual      (press 0)\n";
    if (j > 0)
    {
        cout << "        ---------------------\n";
        cout << "        Update data       (1)\n";
        cout << "        Add income        (2)\n";
        cout << "        Add purchase      (3)\n";
        cout << "        Balance at limit  (4)\n";
        cout << "        On next montn     (5)\n";
        cout << "        ---------------------\n";
        cout << "        Total debt        (6)\n";
    }
    if (j < m)
        cout << "        Add source        (7)\n";
    if (j > 0)
    {
        cout << "        Delete source     (8)\n";
        cout << "        Update card limit (9)\n";
        cout << "        ---------------------\n";
    }
    LONG check = RegGetValueA(HKEY_CURRENT_USER,
                              "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", "d-clutch",
                              RRF_RT_REG_SZ, 0, 0, 0);
    if (check == 0)
        cout << "        Delete autorun   (10)\n";
    if (check == 2)
        cout << "        Autorun          (10)\n";
}

void functions(int j)
{
    if (j > 0)
    {
        cout << "        Update data       (1)\n";
        cout << "        Add income        (2)\n";
        cout << "        Add purchase      (3)\n";
        cout << "        Balance at limit  (4)\n";
        cout << "        On next montn     (5)\n";
        cout << "        ---------------------\n";
    }
}

void totally(int total, int month, string dCluthcPath, string generalName)
{
    string buff[4];
    ifstream file9(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::in);
    for (int i{}; i < 4; i++)
        file9 >> buff[i];
    file9.close();

    cout << "\n\n  TOTAL on " << buff[1] << "." << buff[2] << "." << buff[3]
         << " = " << total << ". ";
    // дата посл.ввода
    struct tm a = {0, 0, 0, stoi(buff[1]), stoi(buff[2]) - 1, 101, 0, 0, 0};
    time_t x = mktime(&a);
    // пересчёт на дни
    struct tm c = {0, 0, 0, 0, month, 101, 0, 0, 0};
    time_t summer = mktime(&c);

    if (x != (time_t)(-1) && summer != (time_t)(-1) && summer != x)
        cout << floor(total / (difftime(summer, x) / (60 * 60 * 24))) << " ru./day.\n\n\n";
    if (x != (time_t)(-1) && summer != (time_t)(-1) && summer == x)
        cout << total << " ru./day.\n\n\n";
}

void changeCardValue(string dCluthcPath, string *events, int j, bool plusminus, int day, int month, int year, string generalName)
{
    int k{};
    const int *n = new const int{3210};
    int quest{};
    int value{};
    int newValue{};
    int total{};
    string *buff = new string[*n];
    string *buff0 = new string[*n];
    string *buffer = new string[*n];
    // отображение карт
    cout << "\n  Change value on:\n\n";
    for (int i = 1; i <= j; i++)
        cout << "  " << events[i] << "  (press " << i << ")" << "\n";
    // выбор карты
    quest = checkNumber();
    // ввод нового расхода
    if (plusminus == true)
        cout << "\n  Enter new income:\n";
    if (plusminus == false)
        cout << "\n  Enter new purchase:\n";
    value = checkNumber();
    // загрузка текстовика в буфер
    if (quest > 0 && quest <= j)
    {
        ifstream changeCardFile0(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
        for (int i = 1; changeCardFile0; i++)
        {
            changeCardFile0 >> buff[i];
            buff[i] += " ";
            k++;
        }
        changeCardFile0.close();
        // подсчёт нового тотала
        string remainds[99];
        string buffer0;
        string buffer1;
        ifstream changeCardFile03;
        changeCardFile03.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
        if (changeCardFile03.is_open())
        {
            int k = 1;
            for (int i{}; i < *n; i++)
            {
                changeCardFile03 >> buffer0;
                if (buffer0 == "|" && k != quest)
                {
                    changeCardFile03 >> buffer0;
                    changeCardFile03 >> buffer0;
                    changeCardFile03 >> remainds[k];
                    k++;
                }
                if (buffer0 == "|" && k == quest)
                {
                    changeCardFile03 >> buffer0;
                    changeCardFile03 >> buffer0;
                    changeCardFile03 >> buffer1;
                    if (plusminus == true)
                        newValue = value + stoi(buffer1);
                    if (plusminus == false)
                        newValue = (value - value * 2) + stoi(buffer1); // value to minus
                    remainds[k] = to_string(newValue);
                    k++;
                }
                if (buffer0 == ";")
                    break;
            }
            changeCardFile03.close();
            cout << "\nNow value: " << newValue << "\n\n";
            for (int i = 1; i <= j; i++)
            { // подсчёт общего остатка
                total += atoi(remainds[i].c_str());
            }
            // считывание даты в buff0
            ifstream changeCardFile(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::in);
            for (int i{}; i < 4; i++)
                changeCardFile >> buff0[i];
            changeCardFile.close();
            // перезапись нового значения
            // если обновление данных происходило сегодня, то файл ПЕРЕписывается
            bool flag = true;
            if (day == stoi(buff0[1]) && month == stoi(buff0[2]) && year == stoi(buff0[3]))
            {
                ofstream changeCardFile01(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                for (int i = 1; i < *n; i++)
                {
                    if (buff[i] == events[quest] + " " && flag == true)
                    {
                        buff[i + 2] = to_string(newValue);
                        changeCardFile01 << buff[i];
                        changeCardFile01 << buff[i + 1];
                        changeCardFile01 << buff[i + 2] << " ";
                        flag = false;
                        i += 2;
                    }
                    else
                        changeCardFile01 << buff[i];
                    if (buff[i] == "; ")
                        changeCardFile01 << "\n\n";
                    if (i == 5)
                    {
                        changeCardFile01 << total << " ";
                        i++;
                    }
                }
                changeCardFile01.close();
            }
            // если обновление данных происходило НЕ сегодня, то файл ДОписывается
            else
            {
                // добавление заголовка о дате и тотале в buff
                buff[0] = "Date " + to_string(day) + " " + to_string(month) + " " + to_string(year) + "  -  " + to_string(total) + " ru ";
                ofstream changeCardFile02(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                flag = true;
                bool flag2 = true;
                for (int i{}; i < *n; i++)
                {
                    if (i == 0)
                        changeCardFile02 << buff[i];
                    if (buff[i] == "| " && flag2 == true)
                        flag2 = false;
                    if (flag2 == false)
                        changeCardFile02 << buff[i];
                    if (buff[i] == events[quest] + " " && flag == true)
                    {
                        buff[i + 2] = to_string(newValue);
                        changeCardFile02 << buff[i + 1];
                        changeCardFile02 << buff[i + 2] << " ";
                        flag = false;
                        i += 2;
                    }
                    if (buff[i] == "; ")
                        changeCardFile02 << "\n\n";
                }
                changeCardFile02.close();
            }
        }
        totally(total, month, dCluthcPath, generalName);
    }
    delete[] buff;
    delete[] buff0;
    delete[] buffer;
}

void autorun(int tog)
{
    char re[MAX_PATH];
    string dCluthcPath = string(re, GetModuleFileNameA(NULL, re, MAX_PATH));

    RegGetValueA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                 "d-clutch", RRF_RT_REG_SZ, 0, 0, 0);

    if (tog == 8)
    {
        cout << "OFF" << endl;
        HKEY hkey = HKEY_CURRENT_USER;
        RegOpenKeyA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                    &hkey);
        RegDeleteValueA(hkey, "d-clutch");
        RegCloseKey(hkey);
    }
    else if (tog == 9)
    {
        cout << "ON" << endl;
        HKEY hkey;
        LONG key = RegOpenKeyExA(HKEY_CURRENT_USER,
                                 "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                                 0, KEY_WRITE, &hkey);
        if (ERROR_SUCCESS == key)
            key = RegSetValueExA(hkey, "d-clutch", 0, REG_SZ,
                                 (BYTE *)dCluthcPath.c_str(), strlen(dCluthcPath.c_str()) + 111);
    }
}

int checkDigit()
{
    int number;
    while (true)
    {
        cin >> number;
        if (cin.fail())
        { // Если ввод не удался (например, ввели буквы)
            cout << "Please enter an integer.\n";
            cin.clear();                                                     // Сброс флага ошибки
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // Очистка буфера
        }
        else if (number < 0 || number > 11)
        { // Проверка диапазона
            cout << "Number must be between 0 and 11.\n";
        }
        else
        {
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // Удаляем лишние символы
            return number;
        }
    }
}

int checkNumber()
{
    int number;
    while (true)
    {
        cin >> number;
        if (cin.fail())
        { // Если ввод не удался (например, ввели буквы)
            cout << "Please enter an integer.\n";
            cin.clear();                                                     // Сброс флага ошибки
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // Очистка буфера
        }
        else
        {
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // Удаляем лишние символы
            return number;
        }
    }
}

bool checkStringContains(const string &str)
{ // Функция проверки на запрещённые символы
    const string forbiddenChars = " ;-/|*";

    return any_of(str.begin(), str.end(), [&forbiddenChars](char c)
                  {
        // Проверяем, есть ли символ в списке запрещённых
        return forbiddenChars.find(c) != string::npos; });
}

string checkString()
{ // Функция для получения валидной строки
    string input;
    while (true)
    {
        getline(cin, input);
        // Проверка на пустую строку
        if (input.empty())
        {
            cout << "Input cannot be empty\n";
            continue;
        }
        // Проверка на запрещённые символы
        if (checkStringContains(input))
        {
            cout << "Input cannot contain spaces or ; - / | *\n";
            cout << "Please try again\n";
            continue;
        }
        // Если все проверки пройдены
        return input;
    }
}

void manual(string dCluthcPath, string generalName)
{
    SetConsoleOutputCP(65001); // SetConsoleOutputCP(65001);
    SetConsoleCP(65001);       //       SetConsoleCP(65001);
    cout << " --------------------------------------------------------------------\n";
    cout << "       Update data (1) - Обновить данные по всем картам\n";
    cout << "        Add income (2) - Добавить доход на выбранную карту\n";
    cout << "      Add purchase (3) - Добавить расход (покупку) на карту\n";
    cout << "  Balance at limit (4) - Рассчитать остаток исходя из лимита на день\n";
    cout << "     On next month (5) - Пересчет до 1-го числа последующего месяца\n";
    cout << "        Total debt (6) - Показать полную задолженность\n";
    cout << "        Add source (7) - Добавить новую карту\n";
    cout << "     Delete source (8) - Удалить карту\n";
    cout << " Update card limit (9) - Обновить лимит кредитной карты\n";
    cout << " Autorun/Delete a.(10) - Управление автозагрузкой\n\n";
    cout << " РАБОТА С ПРОГРАММОЙ\n\n";
    cout << " Добавление карт:\n";
    cout << " Нажмите 7 для добавления новой карты\n";
    cout << " Введите название карты (не используйте символы: -;/|*)\n";
    cout << " Укажите текущий остаток\n";
    cout << " Выберите тип карты: (кредитная (нажмите 1), дебетовая (0))\n";
    cout << " Для кредитной карты укажите лимит\n\n";
    cout << " Обновление данных:\n";
    cout << " Нажмите 1 для обновления остатков на всех картах\n";
    cout << " Введите новые значения для каждой карты\n\n";
    cout << " Добавление данных:\n";
    cout << " 2 - добавить доход (увеличит баланс выбранной карты)\n";
    cout << " 3 - добавить расход (уменьшит баланс выбранной карты)\n\n";
    cout << " Анализ расходов:\n";
    cout << " 4 - расчет баланса при заданном дневном лимите\n";
    cout << " 5 - пересчет расходов до конца месяца\n";
    cout << " 6 - просмотр общей задолженности по кредитным картам\n\n";
    cout << " Управление картами:\n";
    cout << " 8 - удалить карту\n";
    cout << " 9 - изменить лимит кредитной карты\n\n";
    cout << " Автозагрузка:\n";
    cout << " 10 - включить или выключить автозагрузку при старте системы\n\n";
    cout << " ВАЖНО!\n";
    cout << " Данные автоматически сохраняются в файл " << generalName << ".txt\n";
    cout << " в той же папке, где находится программа\n\n";
    cout << " Не изменяйте структуру файла данных вручную - это может привести \n";
    cout << " к ошибкам\n";
    cout << " --------------------------------------------------------------------\n\n";

    ofstream manualFiles;
    manualFiles.open(fs::path(dCluthcPath).replace_filename(generalName + " info.txt"));
    if (manualFiles.is_open())
    {
        if (filesystem::is_empty("d-clutch_MANUAL.txt"))
        {
            manualFiles << " Инструкция пользователя для программы " << generalName << "\n\n";
            manualFiles << " Update data (1) - Обновить данные по всем картам.\n";
            manualFiles << " Add income (2) - Добавить доход на выбранную карту.\n";
            manualFiles << " Add purchase (3) - Добавить расход (покупку) на карту.\n";
            manualFiles << " Balance at limit (4) - Рассчитать остаток исходя из заданного";
            manualFiles << " лимита на день.\n";
            manualFiles << " On next month (5) - Пересчет до первого числа последующего месяца.\n";
            manualFiles << " Total debt (6) - Показать полную задолженность.\n";
            manualFiles << " Add source (7) - Добавить новую карту.\n";
            manualFiles << " Delete source (8) - Удалить карту.\n";
            manualFiles << " Update card limit (9) - Обновить лимит кредитной карты.\n";
            manualFiles << " Autorun/Delete a.(10) - Управление автозагрузкой.\n\n";
            manualFiles << " РАБОТА С ПРОГРАММОЙ\n\n";
            manualFiles << " Добавление карт:\n";
            manualFiles << " Нажмите 7 для добавления новой карты.\n";
            manualFiles << " Введите название карты (не используйте символы: -;/|*).\n";
            manualFiles << " Укажите текущий остаток.\n";
            manualFiles << " Выберите тип карты: (кредитная (нажмите 1), дебетовая (0)).\n";
            manualFiles << " Для кредитной карты укажите лимит.\n\n";
            manualFiles << " Обновление данных:\n";
            manualFiles << " Нажмите 1 для обновления остатков на всех картах.\n";
            manualFiles << " Введите новые значения для каждой карты.\n\n";
            manualFiles << " Добавление данных:\n";
            manualFiles << " 2 - добавить доход (увеличит баланс выбранной карты).\n";
            manualFiles << " 3 - добавить расход (уменьшит баланс выбранной карты).\n\n";
            manualFiles << " Анализ расходов:\n";
            manualFiles << " 4 - расчет баланса при заданном дневном лимите.\n";
            manualFiles << " 5 - пересчет расходов до конца месяца.\n";
            manualFiles << " 6 - просмотр общей задолженности по кредитным картам.\n\n";
            manualFiles << " Управление картами:\n";
            manualFiles << " 8 - удалить карту.\n";
            manualFiles << " 9 - изменить лимит кредитной карты.\n\n";
            manualFiles << " Автозагрузка:\n";
            manualFiles << " 10 - включить или выключить автозагрузку при старте системы.\n\n";
            manualFiles << " ВАЖНО!\n";
            manualFiles << " Данные автоматически сохраняются в файл " << generalName << ".txt, ";
            manualFiles << " в той же папке, где находится программа.\n\n";
            manualFiles << " Не изменяйте структуру файла данных вручную - это может привести к ошибкам.";
            cout << " ИНСТРУКЦИЮ ТАКЖЕ МОЖНО НАЙТИ ПО АДРЕСУ:\n " << fs::path(dCluthcPath).replace_filename(generalName + " info.txt") << "\n\n";
        }
        else
        {
            cout << " ИНСТРУКЦИЮ ТАКЖЕ МОЖНО НАЙТИ ПО АДРЕСУЕ:\n " << fs::path(dCluthcPath).replace_filename(generalName + " info.txt") << "\n\n";
        }
    }
    else
        cout << "\nmanualFiles error\n\n";
}

bool fileExists(const string &filename)
{
    return fs::exists(filename);
}