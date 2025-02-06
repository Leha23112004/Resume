#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

// Функция чтения подписи из реестра
bool verifySignature(const std::string& inputSignature) {
    HKEY hKey;
    wchar_t regSignature[256];
    DWORD regSize = sizeof(regSignature);

    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Ахраров", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExW(hKey, L"Signature", NULL, NULL, (LPBYTE)regSignature, &regSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            std::wstring signatureW = regSignature;
            std::string signature(signatureW.begin(), signatureW.end());
            return inputSignature == signature;
        }
    }
    return false;
}

// Основная функция
int main() {
    std::cout << "Введите подпись для проверки: ";
    std::string inputSignature;
    std::cin >> inputSignature;

    if (verifySignature(inputSignature)) {
        std::cout << "Доступ к файлу sys.tat разрешен.\n";

        // Чтение содержимого файла
        std::ifstream file("sys.tat");
        if (file.is_open()) {
            std::cout << "Содержимое файла:\n" << file.rdbuf() << std::endl;
            file.close();
        }
        else {
            std::cerr << "Ошибка при открытии файла sys.tat." << std::endl;
        }
    }
    else {
        std::cerr << "Ошибка проверки подписи. Доступ запрещен!" << std::endl;
        return 1;
    }

    return 0;
}
