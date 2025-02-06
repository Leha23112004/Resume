#include <windows.h>
#include "crypto.h"
#include <string>
#include <stdexcept> // Для std::invalid_argument
#include <algorithm>
#include <locale>
#include <codecvt>

// Идентификаторы элементов управления
#define ID_COMBOBOX_ALGO 101
#define ID_LABEL_KEY     102
#define ID_EDIT_KEY      103
#define ID_LABEL_INPUT   104
#define ID_EDIT_INPUT    105
#define ID_BUTTON_ENCRYPT 106
#define ID_BUTTON_DECRYPT 107
#define ID_LABEL_OUTPUT  108
#define ID_EDIT_OUTPUT   109

// Глобальные переменные для элементов управления
HWND hwndComboBox, hwndLabelKey, hwndEditKey;
HWND hwndLabelInput, hwndEditInput;
HWND hwndButtonEncrypt, hwndButtonDecrypt;
HWND hwndLabelOutput, hwndEditOutput;

// Прототипы функций
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddControls(HWND hwnd);
void ShowHideKeyField(HWND hwnd, int index);

// Функция конвертации из std::string в std::wstring
std::wstring stringToWstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

// Главная функция
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"CryptoAppWindowClass";
    const wchar_t* windowTitle = L"Исторические алгоритмы шифрования";

    WNDCLASSW wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, windowTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Обработчик сообщений окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        AddControls(hwnd);
        break;

    case WM_COMMAND:
        if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == ID_COMBOBOX_ALGO)
        {
            int index = SendMessageW(hwndComboBox, CB_GETCURSEL, 0, 0);
            ShowHideKeyField(hwnd, index);
        }
        else if (HIWORD(wParam) == BN_CLICKED)
        {
            if (LOWORD(wParam) == ID_BUTTON_ENCRYPT || LOWORD(wParam) == ID_BUTTON_DECRYPT)
            {
                int algoIndex = SendMessageW(hwndComboBox, CB_GETCURSEL, 0, 0);

                wchar_t inputText[1024];
                GetWindowTextW(hwndEditInput, inputText, sizeof(inputText) / sizeof(wchar_t));
                std::wstring inputStr(inputText);

                std::wstring keyStr;
                if (algoIndex == 1) // Виженер
                {
                    wchar_t keyText[256];
                    GetWindowTextW(hwndEditKey, keyText, sizeof(keyText) / sizeof(wchar_t));
                    keyStr = std::wstring(keyText);
                    if (keyStr.empty())
                    {
                        MessageBoxW(hwnd, L"Введите ключевое слово для шифра Виженера.", L"Ошибка", MB_OK | MB_ICONERROR);
                        break;
                    }
                }

                std::wstring result;
                try
                {
                    if (LOWORD(wParam) == ID_BUTTON_ENCRYPT)
                    {
                        if (algoIndex == 0)
                        {
                            result = textToBinary(inputStr);
                        }
                        else if (algoIndex == 1)
                        {
                            result = vigenereEncrypt(inputStr, keyStr);
                        }
                    }
                    else if (LOWORD(wParam) == ID_BUTTON_DECRYPT)
                    {
                        if (algoIndex == 0)
                        {
                            result = binaryToText(inputStr);
                        }
                        else if (algoIndex == 1)
                        {
                            result = vigenereDecrypt(inputStr, keyStr);
                        }
                    }

                    SetWindowTextW(hwndEditOutput, result.c_str());
                }
                catch (const std::invalid_argument& e)
                {
                    std::wstring errorMessage = stringToWstring(e.what());
                    MessageBoxW(hwnd, errorMessage.c_str(), L"Ошибка", MB_OK | MB_ICONERROR);
                }
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Функция добавления элементов управления
void AddControls(HWND hwnd)
{
    CreateWindowW(L"static", L"Выберите алгоритм шифрования:", WS_VISIBLE | WS_CHILD,
        50, 30, 300, 20, hwnd, NULL, NULL, NULL);

    hwndComboBox = CreateWindowW(L"combobox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
        50, 60, 300, 100, hwnd, (HMENU)ID_COMBOBOX_ALGO, NULL, NULL);
    SendMessageW(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)L"Шифровка последовательностей нулей и единиц (ШПНиЕ)");
    SendMessageW(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)L"Шифр Виженера для русских букв");
    SendMessageW(hwndComboBox, CB_SETCURSEL, 0, 0);

    hwndLabelKey = CreateWindowW(L"static", L"Ключ (для Виженера):", WS_VISIBLE | WS_CHILD,
        50, 100, 200, 20, hwnd, (HMENU)ID_LABEL_KEY, NULL, NULL);
    hwndEditKey = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
        50, 130, 300, 25, hwnd, (HMENU)ID_EDIT_KEY, NULL, NULL);
    ShowWindow(hwndEditKey, SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_LABEL_KEY), SW_HIDE);

    CreateWindowW(L"static", L"Введите текст:", WS_VISIBLE | WS_CHILD,
        50, 170, 200, 20, hwnd, NULL, NULL, NULL);
    hwndEditInput = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
        50, 200, 700, 100, hwnd, (HMENU)ID_EDIT_INPUT, NULL, NULL);

    hwndButtonEncrypt = CreateWindowW(L"button", L"Шифровать", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        50, 320, 150, 40, hwnd, (HMENU)ID_BUTTON_ENCRYPT, NULL, NULL);
    hwndButtonDecrypt = CreateWindowW(L"button", L"Дешифровать", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        220, 320, 150, 40, hwnd, (HMENU)ID_BUTTON_DECRYPT, NULL, NULL);

    CreateWindowW(L"static", L"Результат:", WS_VISIBLE | WS_CHILD,
        50, 380, 200, 20, hwnd, NULL, NULL, NULL);
    hwndEditOutput = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
        50, 410, 700, 100, hwnd, (HMENU)ID_EDIT_OUTPUT, NULL, NULL);
}

// Функция отображения или скрытия поля ключа
void ShowHideKeyField(HWND hwnd, int index)
{
    if (index == 1)
    {
        ShowWindow(GetDlgItem(hwnd, ID_LABEL_KEY), SW_SHOW);
        ShowWindow(GetDlgItem(hwnd, ID_EDIT_KEY), SW_SHOW);
    }
    else
    {
        ShowWindow(GetDlgItem(hwnd, ID_LABEL_KEY), SW_HIDE);
        ShowWindow(GetDlgItem(hwnd, ID_EDIT_KEY), SW_HIDE);
    }
}
