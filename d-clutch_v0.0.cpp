// d-clutch v0.0
//
// программа, которая считает деньги на всех картах

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

using namespace std;
namespace fs = std::filesystem;

void nowData(int w, int d, int m, int y);
void nowData(int d, int m, int y);
void functions(int j, int const m);
void totally(int total, int month, string FP);
void autorun(int tog);
void manual();

int main() {


// НАСТРОЙКА ОТОБРАЖЕНИЯ КОНСОЛИ
    MoveWindow(GetConsoleWindow(), 1150, 220, 740, 670, TRUE); // гориз, верт, шир, выс, ХЗ
    system("color 1F"); // установка цвета консоли/цвета шрифта
    // HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE ); // установка шрифта
    // CONSOLE_FONT_INFOEX fontInfo;
    // fontInfo.cbSize = sizeof( fontInfo );
    // GetCurrentConsoleFontEx( hConsole, TRUE, &fontInfo );
    // wcscpy( fontInfo.FaceName, L"Lucida Console" ); // установка типа шрифта
    // fontInfo.dwFontSize.Y = 15; // установка размера шрифта
    // SetCurrentConsoleFontEx( hConsole, TRUE, &fontInfo );

    cout << "\n ---     d-clutch     ---\n";

/// 1. добавить проверку и в мануал запрет на использование ; - / |
/// 2. глюки при удалении (///////)
/// 3. залить на Гитхаб
/// 4. сделать мануал и Readme
/// 5. обновление лимитов
/// 6. отображение всей задолженности с отдельной кнопки (нужно беречь нервы)
/// 7. сравнение предыдущего и последнего ввода исходя из лимита (овердрафт/экономия)
/// 8. сохрань в d-clutch_data.txt каждый ввод (не стирая старое)

//// остановился на: пункте 8, не отображаются функции после обновления данных

    time_t now = time(0); // текущая дата/время, основанные на текущей системе <ctime>
    struct tm* ltm = localtime(&now);
    int year, month, day, wday;
    year = 1900 + ltm->tm_year;
    month = 1 + ltm->tm_mon;
    day = ltm->tm_mday;
    wday = ltm->tm_wday;

    int const n = 3210; // просто здоровое число для счётчиков
    int const m = 99; // переменные там всякие
    int question = 123; // 0 занят
    int remaind = 0;
    int limit = 0;
    // int limitOnDay = 0;
    int total = 0;
    int j = 0;
    char credordebt;
    char quit = '_';
    string events[m];
    string remainds[m];

    struct tm a = { 0,0,0,day,month-1,101,0,0,0 }; // текущая дата
    time_t x = mktime(&a); // 
    // struct tm b = { 0,0,0,0,credordebt,101,0,0,0 }; // ожидаемая дата
    // time_t y = mktime(&b); // 

    nowData(wday, day, month, year); // отображение текущей даты

    char re[MAX_PATH];
    string FP = string(re, GetModuleFileNameA(NULL, re, MAX_PATH));

    //   cout << "\n15:30\n"; // для тестов

// ОТКРЫВАЕТ СОХРАНЕННЫЕ СОБЫТИЯ
    ifstream file1;
    file1.open(fs::path(FP).replace_filename("d-clutch_data.txt"));
    if (file1.is_open())
    {
        char buf;
        for (int i{}, a{}; i < n; i++)
        {
            file1 >> buf;
            if (buf != '|') a = 0;
            if (buf == '|')
            {
                j++;
                a++;
            }
            if (a > 1 || buf == ';') break;
        }
        file1.close();
        cout << "\n";
    }
    ifstream file2;
    file2.open(fs::path(FP).replace_filename("d-clutch_data.txt"));
    if (file2.is_open())
    {
        string buffer0;
        string buffer1;
        int k = j;
        for (int i{}; i < n; i++)
        {
            file2 >> buffer0;
            // пока не удалять \/
            // if (buffer0 != "|" && buffer0 != "/" &&  buffer0 != "") cout << " " << buffer0;
            if (buffer0 == "|")
            {
                file2 >> buffer0;
                file2 >> buffer1;
                file2 >> remainds[k];
                cout << " " << buffer0 << "-" << remainds[k] << "." << endl;
            }
            if (buffer0 == "/") k--;
            if (k < 0 || buffer0 == "" || buffer0 == ";") break;
        }
        file2.close();
    }

    for (int i=1; i<=j; i++) // подсчёт общего остатка
    total += atoi(remainds[i].c_str());
    if (j > 0) totally(total, month, FP);

    
// ИНДЕКСАЦИЯ СОБЫТИЙ
    ifstream file3;
    bool event = false;
    string buffer00;
    file3.open(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::in);
    if (file3.is_open()) {

        int k=j, q=1;
        for (int i = 0; i < n; i++)
        {
            file3 >> buffer00;
            if (event == true) events[q] = buffer00;
            if (buffer00 == "/")
            {
                k--;
                q++;
            }
            if (k < 0) break;
            if (buffer00 == "|") event = true;
            if (buffer00 != "|") event = false;
        }
        file3.close();
    }
    

// ОТОБРАЖЕНИЕ СЕРВИСНЫХ ФУНКЦИЙ
    functions(j, m);


// ЦИКЛ ВВОДА ДАННЫХ
    for (int i{}; i<23; i++)
    {
        if (quit == 'q' || quit == 'Q') break;
        else if (quit != 'q' || quit != 'Q')
        {


// ВВОД КОМАНДЫ
    if (atoi(&quit) >= 0 && atoi(&quit) <= 9) question = atoi(&quit);
    else if (atoi(&quit) < 0 && atoi(&quit) > 9 && i==0) cin >> question;

        
// ИНСТРУКЦИЯ
    if (i>0 && question == 0) manual();
        

// ОБНОВЛЕНИЕ ДАННЫХ
    else if (j > 0 && question == 1)
    {
        int newData[m];
        string buff0[m];
        string buff[n];
        string buffer[n];
        string buf{};
        for (int i=1; i<=j; i++) // ввод новых данных о балансах
        {
            cout << events[i] << " = ";
            cin >> newData[i];
            cout << "\n";
        }
        // добавление заголовка о дате и тотале в buff
        buff[0] = "Date " + to_string(day) + " " + to_string(month)
         + " " + to_string(year) + "  -  " + to_string(total) + " ru";
        
         // добавление остальной информации в buff (до знака ;)
        ifstream file4(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::in);
        int o = 0;
        bool flag = false;
        for (int i = 1; file4; i++)
        {
            file4 >> buf;
            if (flag == true) buff[i-3] = buf;
            else if (buf == "|" && flag == false)
            {
                buff[i-3] = buf;
                flag = true;
            }
            o++;
            if (buf == ";") break;
        }
        file4.close();

        for (int i=0, l=1; i<o; i++) // замена в buff старых на новые значения балансов
        {
            if (buff[i] == "-")
            {
                buff[i+1] = to_string(newData[l]);
                l++;
            }
        }

        // считывание даты в buff0
        ifstream file5(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::in);
        for (int i{}; i<4; i++) file5 >> buff0[i];
        file5.close();

        // если обновление данных происходило сегодня, то файл ПЕРЕписывается
        if (day == stoi(buff0[1]) && month == stoi(buff0[2]) && year == stoi(buff0[3]))
        {
        // считывание старых данных из файла
        ifstream file06(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::in);
        for (int i = 0; file06; i++)
        {
            file06 >> buffer[i];
            buffer[i] += " ";
        }
        file06.close();
        // замена новыми данными
        ofstream file6(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::out);
        for (int i=0; i<o-3; i++)
        {
            buff[i] += " ";
            file6 << buff[i];
        }
        file6 << ";\n\n";
        file6.close();
        
        total = 0;
        for (int i=1; i<=j; i++) // подсчёт общего остатка
        total += newData[i];
        if (j > 0) totally(total, month, FP);
        // добавление старых данных
        ofstream file60(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::app);
        for (int i = 0; n; i++)
        {
            file60 << buffer[i];
            // buffer[i] += " ";
            if (buffer[i] == ";") file60 << buffer[i] << " \n\n";
        }
        file60.close();
        }
        else // если обновление данных происходило НЕ сегодня, то файл ДОписывается
        {
        // считывание старых данных из файла
        ifstream file07(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::in);
        for (int i = 0; file07; i++)
        {
            file07 >> buffer[i];
            buffer[i] += " ";
        }
        file07.close();
        // замена новыми данными
           ofstream file7(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::out);
    
            for (int i=0; i<o-3; i++)
            {
                    buff[i] += " ";
                    file7 << buff[i];
            }
            file7 << ";\n\n";
            file7.close();
    
            total = 0;
            for (int i=1; i<=j; i++) // подсчёт общего остатка
            total += newData[i];
            if (j > 0) totally(total, month, FP);
        // добавление старых данных
        ofstream file70(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::app);
        for (int i = 0; n; i++)
        {
            file70 << buffer[i];
            buffer[i] += " ";
            if (buffer[i] == ";") file70 << buffer[i] << " \n\n";
        }
        file70.close();
        }
    }


// СОЗДАНИЕ
    else if (j < m && question == 2)
    {
        string message;
        cout << "  Name of the new card?\n";

        getline(cin, message);
        while (message.empty())
        {
            cin.clear();
            getline(cin, message);
        }

        ofstream name(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::app);
        name << "Date " + to_string(day) + " " + to_string(month) + " " + to_string(year)
         + "  -  " + to_string(total) + " ru\n | " << message << " - ";
        name.close();

        cout << "  Enter the remainder\n";
        ofstream name2(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::app);
        if (name2.is_open())
        {
            cin >> remaind;
            ofstream name2(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::app);
            name2 << remaind << " ";
            name2.close();
            cout << "  Credit card (press \"c\"). Debit (press \"d\").\n";
            cin >> credordebt;
            if (credordebt == 'c' || credordebt == 'C')
            {
                cout << "  Credit limit\n";
                cin >> limit;
            }
            ofstream name4(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::app);
            name4 << limit << " /";
            name4 << ";\n\n";
            name4.close();
        }
    }


// УДАЛЕНИЕ
    else if (j > 0 && question == 3)
    {
    cout << "  To delete a resource:\n";
    for (int i=1; i<=j; i++)
     cout << " " << events[i] << " (press " << i << ")" << "\n";

    int quest{};
    cin >> quest;

    if (quest > 0 && quest <= j)
    {
        char buff[n];
        char buf{};
        int o = 0;
        ifstream delfile(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::in);
        for (int i=0; delfile; i++)
        {
            delfile.get(buf);
            buff[i] = buf;
            o++;
        }
        delfile.close();
        ofstream delfiles(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::out);
        for (int i = 0, l = 0; i < o; i++)
        {
            if (buff[i] == '|') l++;
            if (quest == l) continue;
            else if (quest != l && l != 0) delfiles << buff[i];
        }
        cout << "  " << events[quest] << " deleted";
        delfiles.close();
    }
    }


// АВТОЗАГРУЗКА
    else if (question == 9)
    {
        autorun(question);
        MessageBox(NULL, _T("Autorun is ON"), _T("Autorun"), 0);
    }
    else if (question == 8)
    {
        autorun(question);
        MessageBox(NULL, _T("Autorun is OFF"), _T("Autorun"), 0);
    }


// ПЕРЕСЧЁТ ДО 1-ГО ЧИСЛА СЛЕД. МЕСЯЦА (функция "On next montn")
    else if (j > 0 && question == 4)
    {
    cout << "\n\n  On next montn - ";

    struct tm c = { 0,0,0,0, month+1 ,101,0,0,0 }; // пересчёт на дни
    time_t y = mktime(&c);
    
    if (x != (time_t)(-1) && y != (time_t)(-1))
    cout << floor(total/(difftime(y, x)/(60 * 60 * 24)))
     << " ru./day.\n\n\n";
    }


// ПЕРЕСЧЁТ НА КОЛ-ВО ДНЕЙ ИСХОДЯ ИЗ ЗАДАННОГО ЛИМИТА (функция "Balance at the limit")
    else if (j > 0 && question == 5)
    {
        int quest{500};
        int answ{};
        // cout << "  Enter limit on day\n"; // для ввода лимита
        // cin >> quest;                     // для ввода лимита
        cout << "  Limit on day = 500\n";    // при заранее заданом лимите
        
        string buff[m];
        ifstream file8(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::in);
        for (int i{}; i<4; i++) file8 >> buff[i];
        file8.close();

        struct tm a = { 0,0,0,stoi(buff[1]),stoi(buff[2])-1,101,0,0,0 }; // дата посл.ввода
        time_t x = mktime(&a);
        struct tm b = { 0,0,0,0,month,101,0,0,0 }; // ожидаемая дата
        time_t y = mktime(&b);

        if (x != (time_t)(-1) && y != (time_t)(-1))
        answ = total - (difftime(y, x)/(60 * 60 * 24)) * quest;
        cout << "  The balance at the end of the month (limit "
         << quest << " ru) = " << answ << " ru.\n\n";
    }


// ПОВТОРНОЕ ОТОБРАЖЕНИЕ СЕРВИСНЫХ ФУНКЦИЙ
    if (i>0)
    {
        functions(j, m);
        cout << "     To exit, press   (Q)\n\n";
    }
    cin >> quit;
}
}

    cout << "\n\n\n* Restart to enter new data\n\n";

    // system("pause");
}

///////////////////////////////////////////////////////////////////////////////////

void nowData(int w, int d, int m, int y) // отображение текущей даты
{
    cout << "\n - Today ";
    switch (w)
    {
    case 1: cout << "mon"; break;
    case 2: cout << "tue"; break;
    case 3: cout << "wed"; break;
    case 4: cout << "thu"; break;
    case 5: cout << "fri"; break;
    case 6: cout << "sat"; break;
    case 7: cout << "sun"; break;
    }
    cout << "." << d << "." << m << "." << y << ". -" << endl;
}

void nowData(int d, int m, int y) // отображение текущей даты (без дня недели)
{
    cout << "\n - Today ";
    cout << "." << d << "." << m << "." << y << ". -" << endl;
}

void functions(int j, int const m)
    {
                    cout << "     Manual     (press 0)\n";
         if (j > 0) cout << "     Update data      (1)\n";
         if (j < m) cout << "     Add source       (2)\n";
         if (j > 0) cout << "     Delete source    (3)\n";
         if (j > 0) cout << "     On next montn    (4)\n";
         if (j > 0) cout << "     Balance at limit (5)\n";
    LONG check = RegGetValueA(HKEY_CURRENT_USER, 
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", "d-clutch", 
            RRF_RT_REG_SZ, 0, 0, 0);
    if (check == 0) cout << "     Delete autorun   (8)\n";
    if (check == 2) cout << "     Autorun          (9)\n";
    }

void totally(int total, int month, string FP)
{
    string buff[4];
    ifstream file9(fs::path(FP).replace_filename("d-clutch_data.txt"), ios::in);
    for (int i{}; i<4; i++) file9 >> buff[i];
    file9.close();

    cout << "\n\n  TOTAL on " << buff[1] << "." << buff[2] << "." << buff[3]
     << " = " << total << ". ";

    struct tm a = { 0,0,0,stoi(buff[1]),stoi(buff[2])-1,101,0,0,0 }; // дата посл.ввода
    time_t x = mktime(&a);
    struct tm c = { 0,0,0,0, month ,101,0,0,0 }; // пересчёт на дни
    time_t summer = mktime(&c);

    if (x != (time_t)(-1) && summer != (time_t)(-1) && summer != x)
    cout << floor(total/(difftime(summer, x)/(60 * 60 * 24))) << " ru./day.\n\n\n";
    if (x != (time_t)(-1) && summer != (time_t)(-1) && summer == x) 
    cout << total << " ru./day.\n\n\n";
}

void autorun(int tog)
{
    char re[MAX_PATH];
    string FP = string(re, GetModuleFileNameA(NULL, re, MAX_PATH));

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
            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hkey);
        if (ERROR_SUCCESS == key) key = RegSetValueExA(hkey, "d-clutch", 0, REG_SZ,
            (BYTE*)FP.c_str(), strlen(FP.c_str()) + 111);
    }
}

void manual()
{
    cout << " --------------------------------------------------------------------\n";
    cout << "                       Manual in development.\n";
    cout << " --------------------------------------------------------------------\n\n";
}