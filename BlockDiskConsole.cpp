// BlockDiskConsole.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <windows.h>

#define SIZE_ARRAY 512 

using namespace std;

int win_error(string error)
{
    cout << "Error ocured" << endl << error << endl;
    return 0;
}

void umount(HANDLE hDisk)
{
    DWORD status;
    if (!DeviceIoControl(hDisk, FSCTL_DISMOUNT_VOLUME,
        NULL, 0, NULL, 0, &status, NULL))
    {
        DWORD err = GetLastError();
    }
}

DWORD64 getVolumeSize(BYTE* buf)
{
    DWORD64 sz = *(DWORD64*)&buf[40];
    int16_t sectors_numb = *(int16_t*)&buf[11];
    sz *= sectors_numb;
    return sz;
}

DWORD64 firs_sector_handl(BYTE* buf, bool what)
{
    DWORD64 sz = -1;
    string fsName = "";
    for (int i = 3; i < 7; i++)
    {
        fsName += buf[i];
    }
    
    if (what) //1 - lock 0 - unlock
    {
        
        if (fsName == "NTFS")
        {
            int r = sizeof(buf);
            sz = getVolumeSize(buf);
            for (int i =0; i < SIZE_ARRAY; i++) buf[i] = buf[i]++;
        }
        else cout << "Error of file system inditifaction\n";
        
    }
    else 
    {
        if (fsName == "OUGT")
        {
            for (int i = 0; i < SIZE_ARRAY; i++)  buf[i] = buf[i]--;
            sz = getVolumeSize(buf);
        }
        else cout << "Error of signature\n";
    }
    return sz;
}



void block(HANDLE hVolume, wstring valueName, bool what)
{
    BYTE buf[SIZE_ARRAY];
    DWORD dwRead;    
    SetFilePointer(hVolume, 0, NULL, FILE_BEGIN);
    if (!::ReadFile(hVolume, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) throw win_error("Can't read a volume.");
   
    int r = sizeof(buf);
    int g = 45;
    SetFilePointer(hVolume, 0, NULL, FILE_BEGIN);
    DWORD64 sz = firs_sector_handl(buf, what);
    LARGE_INTEGER li = { (DWORD)sz,sz >> 32 };
    if (!::WriteFile(hVolume, buf, sizeof(buf), &dwRead, NULL))  throw win_error("Can't write a volume.");
    umount(hVolume);
    SetFilePointer(hVolume, li.LowPart, &li.HighPart, FILE_BEGIN);
    if (!::ReadFile(hVolume, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) throw win_error("Can't read a volume.");
    SetFilePointer(hVolume, li.LowPart, &li.HighPart, FILE_BEGIN);
    firs_sector_handl(buf, what);
    if (!::WriteFile(hVolume, buf, sizeof(buf), &dwRead, NULL))  throw win_error("Can't write a volume.");
}
void show_device()
{
    char drivers[256];
    GetLogicalDriveStrings((DWORD)sizeof(drivers), (LPWSTR)drivers);
    int k = GetLastError();
    cout << "Discs in computer: ";
    for (int i = 0; i < sizeof(drivers); i++)
    {
        
        if (i % 8 == 0)
        {
            if (drivers[i] == '\0') return;
            cout << drivers[i] << ":\\ ";
        }
    }
}
    

int main()
{
    wstring valueName;
    bool what;
    show_device();
    cout << "\nChoose the disk (C, D or other letter): ";
    wcin >> valueName;
    cout << "bloc or unbloc (1 or 0)?: ";
    cin >> what;
    wstring valueNameFile = L"\\\\.\\" + valueName + L":";
    valueName = valueName + L":\\";
    HANDLE hDisk = CreateFile(valueNameFile.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    DWORD dwBytesRead;

    if (hDisk == INVALID_HANDLE_VALUE) throw win_error("INVALID_HANDLE_VALUE");//\\\\.\\PhysicalDrive0 \\\\.\\D:
    DeviceIoControl(hDisk, FSCTL_ALLOW_EXTENDED_DASD_IO, NULL, 0, NULL, 0, &dwBytesRead, NULL);
    block(hDisk, valueName, what);
    ::CloseHandle(hDisk);
    cout << "DONE!\n";
    system("pause");
}