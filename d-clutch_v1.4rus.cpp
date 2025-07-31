// d-clutch_v1.5rus
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

void nowData(int, int, int, int);
void nowData(int, int, int);
void nowData(int, int);
void allFunctions(int);
void functions(int);
void totally(int, int, int);
void changeCardValue(string *, int, bool, int, int, int, int);
void autorun(int);
void manual();
int checkDigit();
int checkNumber();
bool checkStringContains(const string &);
bool fileExists(const string &);
string checkString();
int getDataFromFile(string, int);
void Set65001();
void Set1251();

char *getFileName = new char[MAX_PATH]{};
string dCluthcPath = string(getFileName, GetModuleFileNameA(NULL, getFileName, MAX_PATH));
fs::path getName = fs::path(dCluthcPath).stem();
string generalName = getName.generic_string();
const int *n = new const int{3210}; // здоровое число для счётчиков
const int *m = new const int{99};   // среднее число для счётчиков

int main()
{
    setlocale(LC_ALL, "RU");

    // НАСТРОЙКА ОТОБРАЖЕНИЯ КОНСОЛИ
    MoveWindow(GetConsoleWindow(), 1150, 220, 740, 670, TRUE); // гориз, верт, шир, выс, ХЗ
    system("color 1F");                                        // установка цвета консоли/цвета шрифта
    // HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE ); // установка шрифта
    // CONSOLE_FONT_INFOEX fontInfo;
    // fontInfo.cbSize = sizeof( fontInfo );
    // GetCurrentConsoleFontEx( hConsole, TRUE, &fontInfo );
    // wcscpy( fontInfo.FaceName, L"Lucida Console" ); // установка типа шрифта
    // fontInfo.dwFontSize.Y = 15; // установка размера шрифта
    // SetCurrentConsoleFontEx( hConsole, TRUE, &fontInfo );

    Set65001();
    cout << "\n ---        d-clutch        ---\n";

    /// 0. добавить иконку
    /// 1. Оптимизировать код
    // вынести в функцию remainds -- total
    // добавить точки при отображении больших чисел
    // неправильно считает баланс по лимиту и экономи/расход, если считать сразу
    /// 2. Синхронизировать с eng

    //// остановился на: 2

    time_t now = time(0); // текущая дата/время, основанные на текущей системе <ctime>
    struct tm *ltm = localtime(&now);
    int year, month, day, wday;
    year = 1900 + ltm->tm_year;
    month = 1 + ltm->tm_mon;
    day = ltm->tm_mday;
    wday = ltm->tm_wday;

    int question = 123; // 0 занят
    int payday = 1;
    int remaind = 0;
    int limit = 0;
    int total = 0;
    int j = 0;
    int credordebt;
    string *events = new string[*m];
    string *remainds = new string[*m];
    bool plusminus = true;

    struct tm a = {0, 0, 0, day, month - 1, 101, 0, 0, 0}; // текущая дата
    time_t x = mktime(&a);

    nowData(wday, day, month, year); // отображение текущей даты

    // //   cout << "\n15:30\n"; // для тестов

    if (generalName == "d-clutch_v1.5rus" && !fileExists(generalName + ".txt"))
    {
        cout << "\n Чтобы ваши персональные данные были скрытыми";
        cout << "\n закройте приложение, переместите его в любую папку";
        cout << "\n и переименуйте, используя латинский алфавит";
        cout << "\n (не используйте пробел и символы ; - / | *)." << "\n\n";
    }

    // ОТКРЫВАЕТ СОХРАНЕННЫЕ СОБЫТИЯ
    ifstream file1;
    file1.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
    if (file1.is_open())
    {
        char buf;
        Set1251();
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
    Set65001();
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
                Set1251();
                cout << " " << buffer0 << "-" << remainds[k] << "." << endl;
            }
            if (buffer0 == "/")
                k--;
            if (k < 0 || buffer0 == "" || buffer0 == ";")
                break;
        }
        file2.close();
    }

    payday = getDataFromFile("**", payday);

    { // подсчёт общего остатка
        for (int i = 1; i <= j; i++)
            total += atoi(remainds[i].c_str());
        if (j > 0)
            totally(total, payday, month);
    }

    // ИНДЕКСАЦИЯ СОБЫТИЙ
    ifstream file3;
    bool event = false;
    string buffer00;
    Set1251();
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
    allFunctions(j);

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
                manual();

            // ОБНОВЛЕНИЕ ДАННЫХ
            else if (j > 0 && question == 1)
            {
                int *newData = new int[*m]{};
                string *buff0 = new string[*m];
                string *buff = new string[*n];
                string *buffer = new string[*n];
                string buf{};
                total = 0;

                Set65001();
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
                Set1251();
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
                        totally(total, payday, month);
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
                    for (int i = 1; i <= j; i++)
                        total += newData[i];
                    if (j > 0)
                        totally(total, payday, month);
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
                changeCardValue(events, j, plusminus, day, month, year, payday);
            }

            // ДОБАВЛЕНИЕ ЕДИНИЧНОГО РАСХОДА(ПОКУПКИ) (функция "Добавить расход")
            else if (j > 0 && question == 3)
            {
                plusminus = false;
                changeCardValue(events, j, plusminus, day, month, year, payday);
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
                        Set65001();
                        cout << "  Введите название карты (не используйте пробелы или -;/|*)\n";
                        message = checkString();
                        limit = 0;
                        Set65001();
                        cout << "  Введите текущий остаток\n";
                        remaind = checkNumber();
                        Set65001();
                        cout << "  Кредитная карта (нажмите 1). Дебетовая (0).\n";
                        credordebt = checkNumber();
                        if (credordebt == 1)
                        {
                            Set65001();
                            cout << "  Введите кредитный лимит\n";
                            limit = checkNumber();
                        }
                        Set65001();
                        buf[i] = "| " + message + " - ";
                        buf[i] += to_string(remaind) + " ";
                        buf[i] += to_string(limit) + " /";
                    }
                    cout << "  Добавить еще одну карту? (Да-1 / Нет-0)\n";
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
                    name0.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
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
                    {
                        Set65001();
                        cout << "\nОшибка открытия файла name0\n\n";
                    }
                }
                else
                {
                    ifstream name;
                    name.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                    if (name.is_open())
                    {
                        Set1251();
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
                    {
                        Set65001();
                        cout << "\nОшибка открытия файла name\n\n";
                    }
                    ofstream name1;
                    name1.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                    if (name1.is_open())
                    {
                        for (int i{}; i < k; i++)
                            name1 << buff[i];
                        name1.close();
                    }
                    else
                        cout << "\nОшибка открытия файла name1\n\n";
                }

                ifstream name2;
                name2.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                if (name2.is_open())
                {
                    Set1251();
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
                {
                    Set65001();
                    cout << "\nОшибка открытия файла name2\n\n";
                }

                totally(total, payday, month);
                j = 1;

                delete[] buff;
                delete[] buf;
            }

            // УДАЛЕНИЕ
            else if (j > 0 && question == 8)
            {
                Set65001();
                cout << "  Выберите карту для удаления:\n";
                for (int i = 1; i <= j; i++)
                    cout << "  " << events[i] << "  (нажмите " << i << ")" << "\n";

                int quest{};
                quest = checkNumber();

                if (quest > 0 && quest <= j)
                {
                    Set1251();
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
                    Set65001();
                    cout << "  " << events[quest] << " удалена\n\n";
                    delfiles.close();

                    total = 0;
                    ifstream delfiles2;
                    delfiles2.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                    if (delfiles2.is_open())
                    {
                        Set1251();
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
                        // подсчёт общего остатка
                        for (int i{}; i < k; i++)
                            total += atoi(remainds[i].c_str());
                        delfiles2.close();
                    }
                    else
                    {
                        Set65001();
                        cout << "\nОшибка открытия файла delfiles2\n\n";
                    }

                    totally(total, payday, month);
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

            // ПЕРЕСЧЁТ ДО 1-ГО ЧИСЛА СЛЕД. МЕСЯЦА (функция "До следующего месяца")
            else if (j > 0 && question == 4)
            {
                int quest{1000};
                int answ{};
                string buffer0;
                total = 0;
                Set65001();
                cout << "\n  До ";
                nowData(payday, month + 2);
                cout << ": ";

                ifstream onNextMonth;
                onNextMonth.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                if (onNextMonth.is_open())
                {
                    Set1251();
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

                struct tm c = {0, 0, 0, payday, month + 1, 101, 0, 0, 0}; // пересчёт на дни
                time_t y = mktime(&c);

                if (x != (time_t)(-1) && y != (time_t)(-1))
                    Set65001();
                cout << floor(total / (difftime(y, x) / (60 * 60 * 24)))
                     << " руб./день.\n\n";

                if (x != (time_t)(-1) && y != (time_t)(-1))
                    answ = total - (difftime(y, x) / (60 * 60 * 24)) * quest;
                cout << "  Баланс к этому дню (лимит "
                     << quest << " руб) = " << answ << " руб.\n\n\n";
            }

            // ИЗМЕНЕНИЕ ДНЯ ЗП(payday) И ЛИМИТА НА ДЕНЬ (функция "Изменить день зарплаты / лимит дня")
            else if (j > 0 && question == 5)
            {
                int questLimit{1000};
                int questPayday{1};
                int k{};
                int index{};
                string *buffer0 = new string;
                string *buffer = new string[*n];
                total = 0;
                questLimit = getDataFromFile("*", questLimit);
                questPayday = getDataFromFile("**", questPayday);

                Set65001();
                int change{};
                cout << "  Текущий лимит дня: " << questLimit << ". Изменить? (Да-1 / Нет-0)\n";
                change = checkNumber();
                if (change == 1)
                {
                    cout << "  Введите дневной лимит дня\n"; // для ввода лимита
                    questLimit = checkNumber();              // для ввода лимита
                    bool flag = false;
                    ifstream limit1;
                    limit1.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                    if (limit1.is_open())
                    {
                        Set1251();
                        for (int i{}; limit1; i++)
                        {
                            limit1 >> buffer[i];
                            buffer[i] += " ";
                            if (buffer[i] == "* ")
                            {
                                buffer[i] = "* " + questLimit;
                                flag = true;
                            }
                            if (buffer[i] == "; ")
                            {
                                buffer[i] = "; \n\n";
                                index++;
                            }
                            k++;
                        }
                        limit1.close();
                    }
                    else
                    {
                        Set65001();
                        cout << "\nОшибка открытия файла limit1\n\n";
                    }

                    if (flag == false)
                    {
                        fstream limit2;
                        limit2.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::app);
                        if (limit2.is_open())
                        {
                            limit2 << "* " << questLimit << " ";
                            limit2 << "** " << questPayday << " ";
                            limit2.close();
                        }
                        else
                            cout << "\nОшибка открытия файла limit2\n\n";
                    }
                    else
                    {
                        fstream limit3;
                        limit3.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::out);
                        if (limit3.is_open())
                        {
                            for (int i{}; i < k; i++)
                            {
                                limit3 << buffer[i];
                                if (buffer[i] == "; \n\n")
                                {
                                    index--;
                                    if (index == 0)
                                        break;
                                }
                            }
                            limit3 << "* ";
                            limit3 << to_string(questLimit);
                            limit3 << " ";
                            limit3 << "** ";
                            limit3 << to_string(questPayday);
                            limit3 << " ";
                            limit3.close();
                        }
                        else
                        {
                            Set65001();
                            cout << "\nОшибка открытия файла limit3\n\n";
                        }
                    }
                }
                delete buffer0;
                delete[] buffer;

                string *buffer1 = new string;
                string *buffer2 = new string[*n];

                Set65001();
                change = 0;
                cout << "  Текущий день зарплаты: " << questPayday << ". Изменить? (Да-1 / Нет-0)\n";
                change = checkNumber();
                if (change == 1)
                {
                    cout << "  Введите день зарплаты\n"; // для ввода лимита
                    questPayday = checkNumber();         // для ввода лимита
                    bool flag = false;
                    ifstream limit1;
                    limit1.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
                    if (limit1.is_open())
                    {
                        Set1251();
                        for (int i{}; limit1; i++)
                        {
                            limit1 >> buffer[i];
                            buffer[i] += " ";
                            if (buffer[i] == "** ")
                            {
                                buffer[i] = "** " + questPayday;
                                flag = true;
                            }
                            if (buffer[i] == "; ")
                            {
                                buffer[i] = "; \n\n";
                                index++;
                            }
                            k++;
                        }
                        limit1.close();
                    }
                    else
                    {
                        Set65001();
                        cout << "\nОшибка открытия файла limit1\n\n";
                    }

                    if (flag == false)
                    {
                        fstream limit2;
                        limit2.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::app);
                        if (limit2.is_open())
                        {
                            limit2 << "* " << questLimit << " ";
                            limit2 << "** " << questPayday << " ";
                            limit2.close();
                        }
                        else
                            cout << "\nОшибка открытия файла limit2\n\n";
                    }
                    else
                    {
                        fstream limit3;
                        limit3.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"), ios::out);
                        if (limit3.is_open())
                        {
                            for (int i{}; i < k; i++)
                            {
                                limit3 << buffer[i];
                                if (buffer[i] == "; \n\n")
                                {
                                    index--;
                                    if (index == 0)
                                        break;
                                }
                            }
                            limit3 << "* ";
                            limit3 << to_string(questLimit);
                            limit3 << " ";
                            limit3 << "** ";
                            limit3 << to_string(questPayday);
                            limit3 << " ";
                            limit3.close();
                        }
                        else
                        {
                            Set65001();
                            cout << "\nОшибка открытия файла limit3\n\n";
                        }
                    }
                }
                delete buffer1;
                delete[] buffer2;
            }

            // ОТОБРАЖЕНИЕ ПОЛНОЙ ЗАДОЛЖЕННОСТИ (функция "Общая задолженность")
            else if (j < *m && question == 6)
            {
                int d{};
                int debt{};
                string *buff = new string[*m];
                string *bufDebt = new string[*m];
                string *cardDebt = new string[*m];
                Set1251();
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
                Set65001();
                if (debt - total > 0)
                {
                    cout << "  Общая задолженность = " << debt - total << "\n"
                         << "  Ваши кредитные карты:\n";
                    for (int i{}; i < d; i += 3)
                    {
                        cout << "  " << cardDebt[i] << " - баланс " << cardDebt[i + 1]
                             << ", кредитный лимит " << cardDebt[i + 2] << "\n";
                    }
                    cout << "\n";
                }
                if (debt - total <= 0)
                    cout
                        << "  Положительный баланс задолженности = "
                        << abs(debt - total) << "\n";
                file10.close();

                cout << "\n";

                delete[] buff;
                delete[] bufDebt;
                delete[] cardDebt;
            }

            // ОБНОВЛЕНИЕ ЛИМИТА КАРТЫ (функция "Изменить лимит карты ")
            else if (j < *m && question == 9)
            {
                int o{};
                int quest{};
                int upLimit{};
                string *buff = new string[*n];
                string *buffer = new string[*n];
                Set1251();
                ifstream file11(fs::path(dCluthcPath).replace_filename(generalName + ".txt"),
                                ios::in);
                for (int i = 0; file11; i++)
                {
                    file11 >> buff[i];
                    o++;
                }
                Set65001();
                cout << "  Выберите карту для обновления лимита:\n";

                // отображение кредитных карт
                for (int i{}, l = 1; i < o; i++)
                {
                    if (buff[i] == "|" && stoi(buff[i + 4]) > 0)
                    {
                        events[l] = buff[i + 1];
                        cout << "  " << events[l] << " - лимит = " << buff[i + 4] << " (нажмите " << l << ")"
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
                cout << "\n  Введите новый лимит:\n";
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
            Set65001();
            cout << "        Для выхода из программы           (11)\n\n";
        }
    }

    cout << "\n\n\n* Перезапустите программу для ввода новых данных\n\n";

    delete n;
    delete[] getFileName;
    delete[] events;
    delete[] remainds;

    // system("pause");
}

///////////////////////////////////////////////////////////////////////////////////

void nowData(int w, int d, int m, int y)
{ // отображение текущей даты
    cout << "\n -   Сегодня ";
    switch (w)
    {
    case 1:
        cout << "пн";
        break;
    case 2:
        cout << "вт";
        break;
    case 3:
        cout << "ср";
        break;
    case 4:
        cout << "чт";
        break;
    case 5:
        cout << "пт";
        break;
    case 6:
        cout << "сб";
        break;
    case 7:
        cout << "вс";
        break;
    }
    cout << "." << d << "." << m << "." << y << "г.   -\n\n";
}

void nowData(int d, int m)
{ // отображение текущей даты (только число и месяц)
    cout << d;
    switch (m)
    {
    case 1:
        cout << " января";
        break;
    case 2:
        cout << " февраля";
        break;
    case 3:
        cout << " марта";
        break;
    case 4:
        cout << " апреля";
        break;
    case 5:
        cout << " мая";
        break;
    case 6:
        cout << " июня";
        break;
    case 7:
        cout << " июля";
        break;
    case 8:
        cout << " августа";
        break;
    case 9:
        cout << " сентября";
        break;
    case 10:
        cout << " октября";
        break;
    case 11:
        cout << " ноября";
        break;
    case 12:
        cout << " декабря";
        break;
    }
}

void nowData(int d, int m, int y)
{ // отображение текущей даты (без дня недели)
    cout << "\n -    Сегодня ";
    cout << "." << d << "." << m << "." << y << "г.    -\n";
}

void allFunctions(int j)
{
    Set65001();
    cout << "        Инструкция                         (0)\n";
    if (j > 0)
    {
        cout << "        --------------------------------------\n";
        cout << "        Обновить данные                    (1)\n";
        cout << "        Добавить доход                     (2)\n";
        cout << "        Добавить расход                    (3)\n";
        cout << "        Баланс до следующего месяца        (4)\n";
        cout << "        Изменить день зарплаты / лимит дня (5)\n";
        cout << "        --------------------------------------\n";
        cout << "        Общая задолженность                (6)\n";
    }
    if (j < *m)
        cout << "        Добавить карту                     (7)\n";
    if (j > 0)
    {
        cout << "        Удалить карту                      (8)\n";
        cout << "        Изменить лимит карты               (9)\n";
        cout << "        --------------------------------------\n";
    }
    LONG check = RegGetValueA(HKEY_CURRENT_USER,
                              "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", "d-clutch",
                              RRF_RT_REG_SZ, 0, 0, 0);
    if (check == 0)
        cout << "        Удалить из автозагрузки           (10)\n";
    if (check == 2)
        cout << "        Автозагрузка                      (10)\n";
}

void functions(int j)
{
    Set65001();
    if (j > 0)
    {
        cout << "        Обновить данные                    (1)\n";
        cout << "        Добавить доход                     (2)\n";
        cout << "        Добавить расход                    (3)\n";
        cout << "        Баланс до следующего месяца        (4)\n";
        cout << "        Изменить день зарплаты / лимит дня (5)\n";
        cout << "        --------------------------------------\n";
    }
}

void totally(int total, int payday, int month)
{
    int quest{1000};
    int answ{};
    int diff{};
    string buff[4];
    string *buffer0 = new string;

    quest = getDataFromFile("*", quest);

    ifstream totallyQuestFile1;
    totallyQuestFile1.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
    if (totallyQuestFile1.is_open())
    {
        Set1251();
        for (int i{}; i < 4; i++)
            totallyQuestFile1 >> buff[i];
        totallyQuestFile1.close();
    }
    else
    {
        Set65001();
        cout << "\nОшибка открытия файла totallyQuestFile1\n\n";
    }
    Set65001();
    cout << "\n\n  ИТОГ на " << buff[1] << "." << buff[2] << "." << buff[3]
         << " = " << total << ". До зарплаты (";
    nowData(payday, month + 1);
    cout << "): ";
    // дата посл.ввода
    struct tm a = {0, 0, 0, stoi(buff[1]), stoi(buff[2]) - 1, 101, 0, 0, 0};
    time_t x = mktime(&a);
    // пересчёт на дни
    struct tm c = {0, 0, 0, payday, month, 101, 0, 0, 0};
    time_t y = mktime(&c);

    if (x != (time_t)(-1) && y != (time_t)(-1) && y != x)
        cout << floor(total / (difftime(y, x) / (60 * 60 * 24))) << " руб./день.\n\n";
    if (x != (time_t)(-1) && y != (time_t)(-1) && y == x)
        cout << total << " руб./день.\n\n";

    if (x != (time_t)(-1) && y != (time_t)(-1))
        answ = total - (difftime(y, x) / (60 * 60 * 24)) * quest;
    cout << "  Баланс на день зарплаты (если соблюдать лимит "
         << quest << " руб) = " << answ << " руб.\n\n";

    string *buf = new string{};
    string *buff2 = new string[5];
    string *bufer = new string[*m];
    string *buff3 = new string[6];
    Set1251();
    ifstream file8(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
    if (file8.is_open())
    {
        for (int i{}; i < 6; i++)
            file8 >> buff3[i];
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
    }
    else
    {
        Set65001();
        cout << "\nОшибка открытия файла file8\n\n";
    }

    if (buff2[1].empty() == false)
    {
        // дата посл.ввода
        a = {0, 0, 0, stoi(buff2[0]), stoi(buff2[1]) - 1, 101, 0, 0, 0};
        x = mktime(&a);
        // ожидаемая дата
        struct tm b = {0, 0, 0, stoi(buff3[1]), stoi(buff3[2]) - 1, 101, 0, 0, 0};
        y = mktime(&b);
        Set65001();
        if (x != (time_t)(-1) && y != (time_t)(-1))
            diff = (stoi(buff2[3]) - total) - ((difftime(y, x) / (60 * 60 * 24)) * quest);
        if (diff > 0)
            cout << "  Перерасход с " << stoi(buff2[0]) << "."
                 << stoi(buff2[1]) << "." << stoi(buff2[2])
                 << " = " << diff << " руб.\n\n\n";
        else if (diff < 0)
            cout << "  Экономия с " << stoi(buff2[0]) << "."
                 << stoi(buff2[1]) << "." << stoi(buff2[2])
                 << " = " << abs(diff) << " руб.\n\n\n";
        else
            cout << "  Расходы соответствуют лимиту.\n\n\n";
    }
    delete buf;
    delete[] buff2;
    delete[] buff3;
    delete[] bufer;
    delete buffer0;
}

void changeCardValue(string *events, int j, bool plusminus, int day, int month, int year, int payday)
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
    Set65001();
    cout << "\n  Изменить значение на:\n\n";
    for (int i = 1; i <= j; i++)
        cout << "  " << events[i] << "  (нажмите " << i << ")" << "\n";
    // выбор карты
    quest = checkNumber();
    // ввод нового расхода
    if (plusminus == true)
        cout << "\n  Добавить новый доход:\n";
    if (plusminus == false)
        cout << "\n  Добавить новый расход:\n";
    value = checkNumber();
    // загрузка текстовика в буфер
    if (quest > 0 && quest <= j)
    {
        Set1251();
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
            Set65001();
            cout << "\n  Новое значение: " << newValue << "\n\n";
            for (int i = 1; i <= j; i++)
            { // подсчёт общего остатка
                total += atoi(remainds[i].c_str());
            }
            // считывание даты в buff0
            Set1251();
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
                Set65001();
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
        totally(total, payday, month);
    }
    delete[] buff;
    delete[] buff0;
    delete[] buffer;
}

void autorun(int tog)
{
    char getFileName[MAX_PATH]{};
    string dCluthcPath = string(getFileName, GetModuleFileNameA(NULL, getFileName, MAX_PATH));

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
        Set1251();
        cin >> number;
        if (cin.fail())
        { // Если ввод не удался (например, ввели буквы)
            Set65001();
            cout << "Введите число.\n";
            cin.clear();                                                     // Сброс флага ошибки
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // Очистка буфера
        }
        else if (number < 0 || number > 11)
        { // Проверка диапазона
            cout << "Число должно быть от 0 до 11.\n";
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
            Set65001();
            cout << "Введите число.\n";
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
        Set1251();
        getline(cin, input);
        // Проверка на пустую строку
        if (input.empty())
        {
            Set65001();
            cout << "Ввод не должен быть пустым\n";
            continue;
        }
        // Проверка на запрещённые символы
        if (checkStringContains(input))
        {
            Set65001();
            cout << "Не используйте пробел и символы ; - / | *\n";
            cout << "Попробуйте ещё раз\n";
            continue;
        }
        // Если все проверки пройдены
        return input;
    }
}

void manual()
{
    Set65001();
    cout << " --------------------------------------------------------------------\n";
    cout << "   (1) - Обновить данные на всех картах\n";
    cout << "   (2) - Добавить доход на одну из карт\n";
    cout << "   (3) - Добавить расход (покупку) на одну из карт\n";
    cout << "   (4) - Пересчитать баланс до дня зарплаты следующего месяца\n";
    cout << "   (5) - Изменить день зарплаты или лимит на день\n";
    cout << "   (6) - Показать суммарную задолженность и лимиты кредитных карт\n";
    cout << "   (7) - Добавить новую карту\n";
    cout << "   (8) - Удалить карту\n";
    cout << "   (9) - Обновить лимит кредитной карты\n";
    cout << "   (10) - Управление автозагрузкой\n\n";
    cout << " РАБОТА С ПРОГРАММОЙ\n\n";
    cout << "  Добавление карт:\n";
    cout << "  -Нажмите 7 для добавления новой карты\n";
    cout << "  -Введите название карты (не используйте символы: -;/|*)\n";
    cout << "  -Укажите текущий остаток\n";
    cout << "  -Выберите тип карты: (кредитная (нажмите 1), дебетовая (0))\n";
    cout << "  -Для кредитной карты укажите лимит\n\n";
    cout << "  Обновление данных:\n";
    cout << "  -Нажмите 1 для обновления остатков на всех картах\n";
    cout << "  -Введите новые значения для каждой карты\n\n";
    cout << "  Добавление данных:\n";
    cout << "  -2 - добавить доход (увеличит баланс выбранной карты)\n";
    cout << "  -3 - добавить расход (уменьшит баланс выбранной карты)\n\n";
    cout << "  Анализ расходов:\n";
    cout << "  -4 - расчет остатка до дня зарплаты следующего месяца\n";
    cout << "  -5 - изменение дня зарплаты или суточного лимита\n";
    cout << "  -6 - просмотр общей задолженности по кредитным картам\n\n";
    cout << "  Управление картами:\n";
    cout << "  -8 - удалить карту\n";
    cout << "  -9 - изменить лимит кредитной карты\n\n";
    cout << "  Автозагрузка:\n";
    cout << "  -10 - включить или выключить автозагрузку при старте системы\n\n";
    cout << " ВАЖНО!\n";
    cout << "  Данные автоматически сохраняются в файл " << generalName << ".txt\n";
    cout << "   в той же папке, где находится программа\n\n";
    cout << "  Не изменяйте структуру файла данных вручную - это может привести \n";
    cout << "   к ошибкам\n";
    cout << " --------------------------------------------------------------------\n\n";

    ofstream manualFiles;
    manualFiles.open(fs::path(dCluthcPath).replace_filename(generalName + " info.txt"));
    if (manualFiles.is_open())
    {
        if (filesystem::is_empty(generalName + " info.txt"))
        {
            manualFiles << " Инструкция пользователя для программы " << generalName << "\n\n";
            manualFiles << " (1) - Обновить данные на всех картах.\n";
            manualFiles << " (2) - Добавить доход на одну из карт.\n";
            manualFiles << " (3) - Добавить расход (покупку) на одну из карт.\n";
            manualFiles << " (4) - Пересчитать баланс до дня зарплаты следующего месяца.\n";
            manualFiles << " (5) - Изменить день зарплаты или лимит на день.\n";
            manualFiles << " (6) - Показать суммарную задолженность и лимиты кредитных карт.\n";
            manualFiles << " (7) - Добавить новую карту.\n";
            manualFiles << " (8) - Удалить карту.\n";
            manualFiles << " (9) - Обновить лимит кредитной карты.\n";
            manualFiles << " (10) - Управление автозагрузкой.\n\n";
            manualFiles << "    РАБОТА С ПРОГРАММОЙ\n\n";
            manualFiles << "   Добавление карт:\n";
            manualFiles << " -Нажмите 7 для добавления новой карты.\n";
            manualFiles << " -Введите название карты (не используйте символы: -;/|*).\n";
            manualFiles << " -Укажите текущий остаток.\n";
            manualFiles << " -Выберите тип карты: (кредитная (нажмите 1), дебетовая (0)).\n";
            manualFiles << " -Для кредитной карты укажите лимит.\n\n";
            manualFiles << "   Обновление данных:\n";
            manualFiles << " -Нажмите 1 для обновления остатков на всех картах.\n";
            manualFiles << " -Введите новые значения для каждой карты.\n\n";
            manualFiles << "   Добавление данных:\n";
            manualFiles << " -2 - добавить доход (увеличит баланс выбранной карты).\n";
            manualFiles << " -3 - добавить расход (уменьшит баланс выбранной карты).\n\n";
            manualFiles << "   Анализ расходов:\n";
            manualFiles << " -4 - расчет остатка до дня зарплаты следующего месяца.\n";
            manualFiles << " -5 - изменение дня зарплаты или суточного лимита.\n";
            manualFiles << " -6 - просмотр общей задолженности по кредитным картам.\n\n";
            manualFiles << "   Управление картами:\n";
            manualFiles << " -8 - удалить карту.\n";
            manualFiles << " -9 - изменить лимит кредитной карты.\n\n";
            manualFiles << "   Автозагрузка:\n";
            manualFiles << " -10 - включить или выключить автозагрузку при старте системы.\n\n";
            manualFiles << "    ВАЖНО!\n";
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
        cout << "\nошибка при создании manualFiles\n\n";
}

int getDataFromFile(string marker, int defaultValue)
{
    string *dataFromFile = new string;
    int newData;
    ifstream dataFile;
    dataFile.open(fs::path(dCluthcPath).replace_filename(generalName + ".txt"));
    if (dataFile.is_open())
    {
        Set1251();
        for (int i{}; dataFile; i++)
        {
            dataFile >> *dataFromFile;
            if (*dataFromFile == marker)
            {
                dataFile >> newData;
                break;
            }
            else
            {
                newData = defaultValue;
            }
        }
        dataFile.close();
    }
    delete dataFromFile;

    return newData;
}

bool fileExists(const string &filename)
{
    return fs::exists(filename);
}

void Set65001()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}

void Set1251()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
}