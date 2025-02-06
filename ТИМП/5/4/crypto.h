#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>

// Функции для ШПНиЕ
std::wstring textToBinary(const std::wstring& text);
std::wstring binaryToText(const std::wstring& binaryStr);

// Функции для шифра Виженера
std::wstring generateKey(const std::wstring& text, const std::wstring& key);
std::wstring vigenereEncrypt(const std::wstring& text, const std::wstring& key);
std::wstring vigenereDecrypt(const std::wstring& encryptedText, const std::wstring& key);

#endif // CRYPTO_H
