#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

// Функция создания файла sys.tat с системной информацией
void createSysTatFile(const std::wstring& filePath) {
    std::wofstream file(filePath);
    if (file.is_open()) {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        MEMORYSTATUSEX memStatus;
        memStatus.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memStatus);

        file << L"Имя пользователя: " << std::wstring(getenv("USERNAME"), getenv("USERNAME") + strlen(getenv("USERNAME"))) << std::endl;
        file << L"Имя компьютера: " << std::wstring(getenv("COMPUTERNAME"), getenv("COMPUTERNAME") + strlen(getenv("COMPUTERNAME"))) << std::endl;
        file << L"Количество процессоров: " << sysInfo.dwNumberOfProcessors << std::endl;
        file << L"Объем оперативной памяти: " << memStatus.ullTotalPhys / (1024 * 1024) << L" MB" << std::endl;
        file << L"Версия ОС: " << L"Windows 10 или выше" << std::endl; // Упрощённая версия

        file.close();
        std::wcout << L"Файл sys.tat успешно создан." << std::endl;
    }
    else {
        std::wcerr << L"Ошибка при создании файла sys.tat!" << std::endl;
    }
}
