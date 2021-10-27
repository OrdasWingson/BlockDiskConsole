// BlockDiskConsole.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <windows.h>
#include <fileapi.h>

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
    cout << "UNMOUNT" << endl;
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
        for (int i = 0; i < 4096; i++) buf[i] = buf[i]++;
        
       /*if (fsName == "NTFS")
        {
            for (int i =0; i < sizeof(buf); i++) buf[i] = buf[i]++;
        }
        else cout << "Error of file system inditifaction\n";*/
        
    }
    else 
    {
        for (int i = 0; i < 4096; i++)  buf[i] = buf[i]--;
        /*if (fsName == "OUGT")
        {
            for (int i = 0; i < sizeof(buf); i++)  buf[i] = buf[i]--;
        }
        else cout << "Error of signature\n";*/
    }
   
}

void block(HANDLE hVolume, DWORD64 sz, bool what)
{
    BYTE buf[512];
    DWORD dwRead;
    int count;
    
    LARGE_INTEGER li = {(DWORD)sz,sz>>32};
    //count = SetFilePointer(hVolume, li.LowPart + 3584, &li.HighPart, FILE_BEGIN);
    SetFilePointer(hVolume, 0, NULL, FILE_BEGIN);
    umount(hVolume);
    if (!::ReadFile(hVolume, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) throw win_error("Can't read a volume.");
    //count = SetFilePointer(hVolume, li.LowPart+3584, &li.HighPart, FILE_BEGIN);
    SetFilePointer(hVolume, 0, NULL, FILE_BEGIN);
    //write(buf, what);
    //if (!::WriteFile(hVolume, buf, sizeof(buf), &dwRead, NULL))  throw win_error("Can't write a volume.");
    
    SetFilePointer(hVolume, li.LowPart+512, &li.HighPart, FILE_BEGIN);
    if (!::ReadFile(hVolume, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) throw win_error("Can't read a volume.");
    int Error = GetLastError();
    SetFilePointer(hVolume, li.LowPart, &li.HighPart, FILE_END);
}

    

int main()
{
    wstring valueName;
    bool what;
    cout << "Text name of value (C, D or other letter): ";
    wcin >> valueName;
    cout << "bloc or unbloc (1 or 0)?: ";
    cin >> what;
    wstring valueNameFile = L"\\\\.\\" + valueName + L":";
    valueName = valueName + L":\\";
    DWORD lpSectorsPerCluster = 0;
    DWORD lpBytesPerSector = 0;
    DWORD lpNumberOfFreeClusters = 0;
    DWORD lpTotalNumberOfClusters = 0;
    bool r = GetDiskFreeSpace(valueName.c_str(), &lpSectorsPerCluster, &lpBytesPerSector, &lpNumberOfFreeClusters, &lpTotalNumberOfClusters);
    DWORD64 sz = lpTotalNumberOfClusters;
    sz *= lpBytesPerSector * lpSectorsPerCluster;
    DWORD d = GetLastError();
    HANDLE hDisk = CreateFile(valueNameFile.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    DWORD dwBytesRead;
    
    if (hDisk == INVALID_HANDLE_VALUE) throw win_error("INVALID_HANDLE_VALUE");//\\\\.\\PhysicalDrive0 \\\\.\\D:
    bool w = DeviceIoControl(hDisk, FSCTL_ALLOW_EXTENDED_DASD_IO, NULL, 0, NULL, 0, &dwBytesRead, NULL);
    block(hDisk, sz,what);
    ::CloseHandle(hDisk);
}