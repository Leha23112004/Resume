#include "crypto.h"
#include <bitset>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <locale>
#include <codecvt>

// Русский алфавит в верхнем и нижнем регистре
const std::wstring russianAlphabetUpper = L"АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
const std::wstring russianAlphabetLower = L"абвгдежзийклмнопрстуфхцчшщъыьэюя";

// Функция для преобразования текста в бинарный вид
std::wstring textToBinary(const std::wstring& text) {
    std::wostringstream binaryStream;
    for (wchar_t c : text) {
        binaryStream << std::bitset<16>(c) << L' ';
    }
    std::wstring binaryStr = binaryStream.str();
    if (!binaryStr.empty()) {
        binaryStr.pop_back();
    }
    return binaryStr;
}

// Функция для преобразования бинарного вида обратно в текст
std::wstring binaryToText(const std::wstring& binaryStr) {
    std::wostringstream textStream;
    std::wistringstream iss(binaryStr);
    std::wstring byte;
    while (iss >> byte) {
        try {
            wchar_t c = static_cast<wchar_t>(std::bitset<16>(byte).to_ulong());
            textStream << c;
        }
        catch (const std::exception&) {
            throw std::invalid_argument("Неверный формат бинарного текста.");
        }
    }
    return textStream.str();
}

// Генерация ключа до длины текста (ключ всегда верхнего регистра, 
// так как для вычислений удобно работать с одним регистром)
std::wstring generateKey(const std::wstring& text, const std::wstring& key) {
    std::wstring extendedKey;
    size_t keyIndex = 0;

    for (wchar_t c : text) {
        // Проверим, принадлежит ли символ русскому алфавиту (верхнему или нижнему регистру)
        wchar_t upperChar = c;
        if (russianAlphabetLower.find(c) != std::wstring::npos) {
            // Если это строчная буква, то переводим в верхний регистр для проверки
            size_t pos = russianAlphabetLower.find(c);
            upperChar = russianAlphabetUpper[pos];
        }

        if (russianAlphabetUpper.find(upperChar) != std::wstring::npos) {
            extendedKey += key[keyIndex % key.size()];
            keyIndex++;
        }
        else {
            extendedKey += c; // Символы вне алфавита не меняют ключ
        }
    }
    return extendedKey;
}

// Шифрование текста с использованием шифра Виженера
std::wstring vigenereEncrypt(const std::wstring& text, const std::wstring& key) {
    std::wstring upperKey = key;
    std::transform(upperKey.begin(), upperKey.end(), upperKey.begin(), ::towupper);
    std::wstring extendedKey = generateKey(text, upperKey);

    std::wstring encrypted;
    for (size_t i = 0; i < text.size(); ++i) {
        wchar_t currentChar = text[i];
        wchar_t keyChar = extendedKey[i];

        // Определим, является ли символ буквой русского алфавита
        // Проверим в верхнем и нижнем алфавите
        bool isLower = false;
        size_t pos = russianAlphabetUpper.find(currentChar);
        if (pos == std::wstring::npos) {
            // Возможно, символ в нижнем регистре
            pos = russianAlphabetLower.find(currentChar);
            if (pos != std::wstring::npos) {
                isLower = true;
            }
        }

        if (pos != std::wstring::npos) {
            // Определяем позицию символа ключа (он уже в верхнем регистре)
            size_t keyPos = russianAlphabetUpper.find(keyChar);
            // Вычисляем зашифрованный символ в верхнем регистре
            wchar_t encryptedCharUpper = russianAlphabetUpper[(pos + keyPos) % russianAlphabetUpper.size()];
            // Если исходный был строчным, берем соответствующую строчную букву
            wchar_t encryptedChar = isLower ? russianAlphabetLower[(pos + keyPos) % russianAlphabetLower.size()] : encryptedCharUpper;
            encrypted += encryptedChar;
        }
        else {
            encrypted += currentChar; // Символы вне алфавита остаются как есть
        }
    }
    return encrypted;
}

// Дешифрование текста с использованием шифра Виженера
std::wstring vigenereDecrypt(const std::wstring& encryptedText, const std::wstring& key) {
    std::wstring upperKey = key;
    std::transform(upperKey.begin(), upperKey.end(), upperKey.begin(), ::towupper);
    std::wstring extendedKey = generateKey(encryptedText, upperKey);

    std::wstring decrypted;
    for (size_t i = 0; i < encryptedText.size(); ++i) {
        wchar_t currentChar = encryptedText[i];
        wchar_t keyChar = extendedKey[i];

        bool isLower = false;
        size_t pos = russianAlphabetUpper.find(currentChar);
        if (pos == std::wstring::npos) {
            pos = russianAlphabetLower.find(currentChar);
            if (pos != std::wstring::npos) {
                isLower = true;
            }
        }

        if (pos != std::wstring::npos) {
            size_t keyPos = russianAlphabetUpper.find(keyChar);
            wchar_t decryptedCharUpper = russianAlphabetUpper[(pos + russianAlphabetUpper.size() - keyPos) % russianAlphabetUpper.size()];
            wchar_t decryptedChar = isLower ? russianAlphabetLower[(pos + russianAlphabetLower.size() - keyPos) % russianAlphabetLower.size()] : decryptedCharUpper;
            decrypted += decryptedChar;
        }
        else {
            decrypted += currentChar; // Символы вне алфавита остаются как есть
        }
    }
    return decrypted;
}
