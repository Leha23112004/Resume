#include <iostream>
#include <string>
#include "crypto.h"

void displayMenu() {
    std::cout << "==============================\n";
    std::cout << "   Исторические алгоритмы шифрования\n";
    std::cout << "==============================\n";
    std::cout << "Выберите алгоритм шифрования:\n";
    std::cout << "1. Шифровка последовательностей нулей и единиц (ШПНиЕ)\n";
    std::cout << "2. Шифр Виженера для русских букв\n";
    std::cout << "0. Выход\n";
    std::cout << "Ваш выбор: ";
}

int main() {
    int algorithmChoice = -1;

    while (true) {
        displayMenu();
        std::cin >> algorithmChoice;
        std::cin.ignore(); // Игнорируем оставшийся символ перевода строки

        if (algorithmChoice == 0) {
            std::cout << "Выход из программы. До свидания!\n";
            break;
        }

        if (algorithmChoice != 1 && algorithmChoice != 2) {
            std::cout << "Неверный выбор. Попробуйте снова.\n\n";
            continue;
        }

        std::cout << "\nВыберите режим:\n";
        std::cout << "1. Шифрование\n";
        std::cout << "2. Дешифрование\n";
        std::cout << "Ваш выбор: ";

        int modeChoice;
        std::cin >> modeChoice;
        std::cin.ignore(); // Игнорируем оставшийся символ перевода строки

        if (modeChoice != 1 && modeChoice != 2) {
            std::cout << "Неверный выбор режима. Возврат в главное меню.\n\n";
            continue;
        }

        if (algorithmChoice == 1) { // ШПНиЕ
            if (modeChoice == 1) { // Шифрование
                std::cout << "\n=== Шифрование ШПНиЕ ===\n";
                std::cout << "Введите текст для шифрования: ";
                std::string inputText;
                std::getline(std::cin, inputText);

                std::string encrypted = textToBinary(inputText);
                std::cout << "Зашифрованный текст: " << encrypted << "\n\n";
            }
            else { // Дешифрование
                std::cout << "\n=== Дешифрование ШПНиЕ ===\n";
                std::cout << "Введите бинарный текст для дешифрования: ";
                std::string binaryText;
                std::getline(std::cin, binaryText);

                try {
                    std::string decrypted = binaryToText(binaryText);
                    std::cout << "Расшифрованный текст: " << decrypted << "\n\n";
                }
                catch (const std::invalid_argument& e) {
                    std::cout << "Ошибка: " << e.what() << "\n\n";
                }
            }
        }
        else if (algorithmChoice == 2) { // Виженер
            if (modeChoice == 1) { // Шифрование
                std::cout << "\n=== Шифрование Виженера ===\n";
                std::cout << "Введите текст для шифрования (русские буквы): ";
                std::string inputText;
                std::getline(std::cin, inputText);

                std::cout << "Введите ключевое слово: ";
                std::string key;
                std::getline(std::cin, key);

                if (key.empty()) {
                    std::cout << "Ключевое слово не может быть пустым.\n\n";
                    continue;
                }

                std::string encrypted = vigenereEncrypt(inputText, key);
                std::cout << "Зашифрованный текст: " << encrypted << "\n\n";
            }
            else { // Дешифрование
                std::cout << "\n=== Дешифрование Виженера ===\n";
                std::cout << "Введите зашифрованный текст (русские буквы): ";
                std::string encryptedText;
                std::getline(std::cin, encryptedText);

                std::cout << "Введите ключевое слово: ";
                std::string key;
                std::getline(std::cin, key);

                if (key.empty()) {
                    std::cout << "Ключевое слово не может быть пустым.\n\n";
                    continue;
                }

                std::string decrypted = vigenereDecrypt(encryptedText, key);
                std::cout << "Расшифрованный текст: " << decrypted << "\n\n";
            }
        }
    }

    // Пауза перед выходом
    std::cout << "Нажмите Enter для выхода...";
    std::cin.ignore();
    std::cin.get();
    return 0;
}
