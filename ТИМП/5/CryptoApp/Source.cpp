#include "crypto.h"
#include <bitset>
#include <sstream>
#include <algorithm>
#include <stdexcept>

// Функция для преобразования текста в бинарный вид
std::string textToBinary(const std::string& text) {
    std::ostringstream binaryStream;
    for (unsigned char c : text) {
        binaryStream << std::bitset<8>(c) << ' ';
    }
    std::string binaryStr = binaryStream.str();
    if (!binaryStr.empty()) {
        binaryStr.pop_back(); // Удаление последнего пробела
    }
    return binaryStr;
}

// Функция для преобразования бинарного вида обратно в текст
std::string binaryToText(const std::string& binaryStr) {
    std::ostringstream textStream;
    std::istringstream iss(binaryStr);
    std::string byte;
    while (iss >> byte) {
        try {
            char c = static_cast<char>(std::bitset<8>(byte).to_ulong());
            textStream << c;
        }
        catch (const std::exception& e) {
            // В случае ошибки преобразования
            throw std::invalid_argument("Неверный формат бинарного текста.");
        }
    }
    return textStream.str();
}

// Генерация ключа до длины текста
std::string generateKey(const std::string& text, const std::string& key) {
    std::string extendedKey = key;
    while (extendedKey.size() < text.size()) {
        extendedKey += key;
    }
    extendedKey = extendedKey.substr(0, text.size());
    return extendedKey;
}

// Шифрование текста с использованием шифра Виженера
std::string vigenereEncrypt(const std::string& text, const std::string& key) {
    std::string russianAlphabet = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::string upperText = text;
    std::string upperKey = key;
    std::transform(upperText.begin(), upperText.end(), upperText.begin(), ::toupper);
    std::transform(upperKey.begin(), upperKey.end(), upperKey.begin(), ::toupper);

    std::string extendedKey = generateKey(upperText, upperKey);
    std::string encrypted;

    for (size_t i = 0; i < upperText.size(); ++i) {
        char currentChar = upperText[i];
        char keyChar = extendedKey[i];
        size_t pos = russianAlphabet.find(currentChar);
        size_t keyPos = russianAlphabet.find(keyChar);

        if (pos != std::string::npos && keyPos != std::string::npos) {
            encrypted += russianAlphabet[(pos + keyPos) % russianAlphabet.size()];
        }
        else {
            encrypted += currentChar; // Если символ не найден в алфавите, оставляем без изменений
        }
    }
    return encrypted;
}

// Дешифрование текста с использованием шифра Виженера
std::string vigenereDecrypt(const std::string& encryptedText, const std::string& key) {
    std::string russianAlphabet = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::string upperEncrypted = encryptedText;
    std::string upperKey = key;
    std::transform(upperEncrypted.begin(), upperEncrypted.end(), upperEncrypted.begin(), ::toupper);
    std::transform(upperKey.begin(), upperKey.end(), upperKey.begin(), ::toupper);

    std::string extendedKey = generateKey(upperEncrypted, upperKey);
    std::string decrypted;

    for (size_t i = 0; i < upperEncrypted.size(); ++i) {
        char currentChar = upperEncrypted[i];
        char keyChar = extendedKey[i];
        size_t pos = russianAlphabet.find(currentChar);
        size_t keyPos = russianAlphabet.find(keyChar);

        if (pos != std::string::npos && keyPos != std::string::npos) {
            decrypted += russianAlphabet[(pos + russianAlphabet.size() - keyPos) % russianAlphabet.size()];
        }
        else {
            decrypted += currentChar; // Если символ не найден в алфавите, оставляем без изменений
        }
    }
    return decrypted;
}
