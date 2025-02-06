#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>

// Функции для ШПНиЕ
std::string textToBinary(const std::string& text);
std::string binaryToText(const std::string& binaryStr);

// Функции для шифра Виженера
std::string generateKey(const std::string& text, const std::string& key);
std::string vigenereEncrypt(const std::string& text, const std::string& key);
std::string vigenereDecrypt(const std::string& encryptedText, const std::string& key);

#endif // CRYPTO_H
