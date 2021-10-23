// BlockDiskConsole.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

int win_error(string error)
{
    cout << "Error ocured" << endl << error << endl;
    return 0;
}



void write(BYTE* buf, bool what)
{
    string fsName = "";
    for (int i = 3; i < 7; i++)
    {
        fsName += buf[i];
    }
    
    if (what) //1 - lock 0 - unlock
    {
        for (int i = 0; i < sizeof(buf); i++)
        {
            buf[i] = buf[i]++;
        }
        /*if (fsName == "NTFS")
        {
            
        }
        else cout << "Error of file system inditifaction\n";*/
        
    }
    else 
    {
        for (int i = 0; i < sizeof(buf); i++)
        {
            buf[i] = buf[i]--;
        }
        /*if (fsName == "OUGT")
        {
            
        }
        else cout << "Error of signature\n";*/
    }
   
}

void block(HANDLE hVolume, bool what)
{
    BYTE buf[4096];
    DWORD dwRead;
    int count;
    SetFilePointer(hVolume, 0, NULL, FILE_BEGIN);

    /*for (int i = 0; i < 15; i++)
    {
        if (!::ReadFile(hVolume, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) throw win_error("Can't read a volume.");
        count = SetFilePointer(hVolume, i*4096, NULL, FILE_BEGIN);
        write(buf, what);
        if (!::WriteFile(hVolume, buf, sizeof(buf), &dwRead, NULL))  throw win_error("Can't write a volume.");
    }*/
    //в конце отработать
    count = SetFilePointer(hVolume, 512, NULL, FILE_END);
    BYTE buf_2[512];
    if (!::ReadFile(hVolume, buf_2, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) throw win_error("Can't read a volume.");
   // write(buf, what);
    //count = SetFilePointer(hVolume, 512, NULL, FILE_END);
    //if (!::WriteFile(hVolume, buf_2, sizeof(buf), &dwRead, NULL))  throw win_error("Can't write a volume.");
}

void umount(HANDLE hDisk)
{
    DWORD status;
    if (!DeviceIoControl(hDisk, FSCTL_DISMOUNT_VOLUME,
        NULL, 0, NULL, 0, &status, NULL))
    {
        DWORD err = GetLastError();
    }
    cout << "UNMOUNT" << endl;
}
    

int main()
{
    wstring valueName;
    bool what;
    cout << "Text name of value (C, D or other letter): ";
    wcin >> valueName;
    cout << "bloc or unbloc (1 or 0)?: ";
    cin >> what;
    valueName = L"\\\\.\\" + valueName + L":";

    HANDLE hDisk = CreateFile(valueName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    umount(hDisk);
    if (hDisk == INVALID_HANDLE_VALUE) throw win_error("INVALID_HANDLE_VALUE");//\\\\.\\PhysicalDrive0 \\\\.\\D:
    block(hDisk,what);
    ::CloseHandle(hDisk);
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

//if (!::ReadFile(hDisk, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) throw win_error();
    //if(! ::WriteFile(hDisk, wr, sizeof(wr), &dwWrite, NULL)) cout  << "erorrrrrrr ";
    //cout << dwWrite << endl;
   // DWORD dd1 = SetFilePointer(hDisk, 512, 0, FILE_BEGIN);
    //if (!::ReadFile(hDisk, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) throw win_error();

    //LARGE_INTEGER li = {0x387FFC00,0x1};
    //LARGE_INTEGER li = { 0x70AFFE00,0x74 };
    //DWORD dd2 = SetFilePointer(hDisk, li.LowPart, &li.HighPart, FILE_BEGIN);

   /* if (!::WriteFile(hDisk, wr, sizeof(wr), &dwWrite, NULL)) cout << "erorrrrrrr "; 74 70AFFE00
    cout << dwWrite << endl;*/
    //int fff = 4;


    /*ofstream out("save.l");
    for (int i = 0; i < 512; i++)
    {
        out << buf[i];
        cout << buf[i];
    }
   /* for (int i = 0; i < 512; i++)
    {
        char f[3];  _itoa_s(buf[i], f, 16);
        out << i << "\t" << f << "\t" << buf[i] << '\n';
        cout << buf[i];
    }
    out.close();*/
    //system("save.l");
    //system("1.txt");
    //cin.get();
