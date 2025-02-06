#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <shlobj.h> // Для выбора папки

// Преобразование std::string в std::wstring
std::wstring stringToWString(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}

// Функция выбора папки
std::wstring chooseFolder() {
    wchar_t folderPath[MAX_PATH];
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = L"Выберите папку для установки";
    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);

    if (pidl != NULL) {
        SHGetPathFromIDListW(pidl, folderPath);
        CoTaskMemFree(pidl); // Освободить память
        return std::wstring(folderPath);
    }
    else {
        std::wcerr << L"Выбор папки отменен!" << std::endl;
        return L"";
    }
}

// Функция записи подписи в реестр
void createSignatureInRegistry(const std::string& signature) {
    HKEY hKey;
    std::wstring registryPath = L"Software\\Фамилия_студента";
    std::wstring signatureW = stringToWString(signature);

    if (RegCreateKeyExW(HKEY_CURRENT_USER, registryPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueExW(hKey, L"Signature", 0, REG_SZ, (const BYTE*)signatureW.c_str(), (signatureW.size() + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
        std::wcout << L"Подпись успешно записана в реестр." << std::endl;
    }
    else {
        std::wcerr << L"Ошибка записи подписи в реестр." << std::endl;
    }
}

// Основная функция инсталлятора
int main() {
    std::wcout << L"Запуск программы установки." << std::endl;

    // Шаг 1: Выбор папки
    std::wstring installFolder = chooseFolder();
    if (installFolder.empty()) {
        return 1;
    }

    // Шаг 2: Копирование файлов
    std::wstring sysTatPath = installFolder + L"\\sys.tat";
    createSysTatFile(sysTatPath);

    std::wstring securExeSource = L"secur.exe"; // Путь к исходному secur.exe
    std::wstring securExeDest = installFolder + L"\\secur.exe";
    CopyFileW(securExeSource.c_str(), securExeDest.c_str(), FALSE);

    // Шаг 3: Запись подписи в реестр
    createSignatureInRegistry("UserSignature123");

    // Шаг 4: Привязка secur.exe к sys.tat
    std::wstring command = securExeDest + L" \"%1\"";
    RegSetValueW(HKEY_CLASSES_ROOT, L".tat\\shell\\open\\command", REG_SZ, command.c_str(), (command.size() + 1) * sizeof(wchar_t));

    std::wcout << L"Установка завершена." << std::endl;
    return 0;
}
