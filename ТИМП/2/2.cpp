#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <locale>
#include <codecvt>

// Константы для ограничения программы
const int MAX_RUNS = 5;           // Максимальное количество запусков
const int TIME_LIMIT = 180;       // Время работы программы в секундах (3 минуты)

// Пути к файлам (рекомендуется использовать пути без кириллицы)
const std::wstring USERS_FILE = L"C:\\Users\\Али\\Desktop\\Учеба\\ТИМП\\2\\users.txt";
const std::wstring CONFIG_FILE = L"C:\\Users\\Али\\Desktop\\Учеба\\ТИМП\\2\\config.txt";
const std::wstring PERSISTENT_FILE = L"C:\\Users\\Али\\Desktop\\Учеба\\ТИМП\\persistent.dat";

// Функция для создания папки в Windows
void createDirectory(const std::wstring& path) {
    CreateDirectoryW(path.c_str(), NULL);
}

// Функция для чтения файла конфигурации
void loadConfig(int& runs, int& totalRuns, long long& totalTime) {
    FILE* config = nullptr;
    if (_wfopen_s(&config, CONFIG_FILE.c_str(), L"r, ccs=UTF-8") == 0 && config != nullptr) {
        fwscanf_s(config, L"%d", &runs);
        fclose(config);
    }
    else {
        runs = 0;
    }

    FILE* persistent = nullptr;
    if (_wfopen_s(&persistent, PERSISTENT_FILE.c_str(), L"r, ccs=UTF-8") == 0 && persistent != nullptr) {
        fwscanf_s(persistent, L"%d", &totalRuns);
        fwscanf_s(persistent, L"%lld", &totalTime);
        fclose(persistent);
    }
    else {
        totalRuns = 0;
        totalTime = 0;
    }
}

// Функция для записи файла конфигурации
void saveConfig(int runs, int totalRuns, long long totalTime) {
    FILE* config = nullptr;
    if (_wfopen_s(&config, CONFIG_FILE.c_str(), L"w, ccs=UTF-8") == 0 && config != nullptr) {
        fwprintf(config, L"%d\n", runs);
        fclose(config);
    }

    FILE* persistent = nullptr;
    if (_wfopen_s(&persistent, PERSISTENT_FILE.c_str(), L"w, ccs=UTF-8") == 0 && persistent != nullptr) {
        fwprintf(persistent, L"%d\n%lld\n", totalRuns, totalTime);
        fclose(persistent);
    }
}

// Функция проверки ограничения
bool checkLimits(int totalRuns, long long totalTime) {
    if (totalRuns >= MAX_RUNS) {
        std::wcout << L"Превышено общее количество запусков программы.\n";
        return true;
    }
    if (totalTime >= TIME_LIMIT) {
        std::wcout << L"Превышено общее время использования программы.\n";
        return true;
    }
    return false;
}

// Функция запроса ФИО у пользователя
std::wstring getUserFIO() {
    std::wstring fio;
    std::wcout << L"Введите ваше ФИО: ";
    std::getline(std::wcin, fio);  // Получаем строку с ФИО
    return fio;
}

// Функция проверки и сохранения ФИО
void checkAndSaveFIO(const std::wstring& fio) {
    // Читаем текущие ФИО из файла
    FILE* infile = nullptr;
    if (_wfopen_s(&infile, USERS_FILE.c_str(), L"r, ccs=UTF-8") != 0 || infile == nullptr) {
        std::wcerr << L"Ошибка: файл " << USERS_FILE << L" не найден. Попробуем создать его...\n";
        FILE* outfile = nullptr;
        if (_wfopen_s(&outfile, USERS_FILE.c_str(), L"w, ccs=UTF-8") != 0 || outfile == nullptr) {
            std::wcerr << L"Ошибка: файл " << USERS_FILE << L" не удалось создать. Проверьте права доступа или путь.\n";
            return;
        }
        fclose(outfile);
        std::wcout << L"Файл " << USERS_FILE << L" успешно создан.\n";
        // Открываем файл снова для чтения
        if (_wfopen_s(&infile, USERS_FILE.c_str(), L"r, ccs=UTF-8") != 0 || infile == nullptr) {
            std::wcerr << L"Ошибка открытия файла для чтения.\n";
            return;
        }
    }

    std::vector<std::wstring> names; // Список ФИО
    wchar_t buffer[256];
    while (fgetws(buffer, 256, infile)) {
        // Убираем символ новой строки
        size_t len = wcslen(buffer);
        if (len > 0 && buffer[len - 1] == L'\n') {
            buffer[len - 1] = L'\0';
        }
        names.push_back(buffer);
    }
    fclose(infile);

    // Проверяем, есть ли уже такое ФИО
    for (const auto& name : names) {
        if (name == fio) {
            std::wcout << L"Такое ФИО уже есть в системе.\n";
            return;
        }
    }

    // Добавляем новое имя в список
    names.push_back(fio);

    // Проверяем, если записей больше 5, удаляем старейшие
    while (names.size() > 5) {
        names.erase(names.begin());
    }

    // Перезаписываем файл с актуальным списком
    FILE* outfile = nullptr;
    if (_wfopen_s(&outfile, USERS_FILE.c_str(), L"w, ccs=UTF-8") != 0 || outfile == nullptr) {
        std::wcerr << L"Ошибка открытия файла для записи. Проверьте права доступа.\n";
        return;
    }
    for (const auto& name : names) {
        fwprintf(outfile, L"%s\n", name.c_str());
    }
    if (ferror(outfile)) {
        std::wcerr << L"Ошибка записи в файл " << USERS_FILE << L". Проверьте диск на ошибки.\n";
    }
    else {
        std::wcout << L"ФИО сохранено успешно.\n";
    }
    fclose(outfile);
}

// Функция проверки установки программы
void checkInstallation() {
    FILE* configFile = nullptr;
    if (_wfopen_s(&configFile, CONFIG_FILE.c_str(), L"r") != 0 || configFile == nullptr) {
        std::wcout << L"Программа установлена впервые.\n";
        createDirectory(L"C:\\DemoProgram");

        // Создаем файл config.txt
        FILE* configFileOut = nullptr;
        if (_wfopen_s(&configFileOut, CONFIG_FILE.c_str(), L"w") != 0 || configFileOut == nullptr) {
            std::wcerr << L"Ошибка при создании config.txt. Проверьте права доступа.\n";
        }
        else {
            fclose(configFileOut);
        }

        // Создаем файл users.txt
        FILE* usersFile = nullptr;
        if (_wfopen_s(&usersFile, USERS_FILE.c_str(), L"w") != 0 || usersFile == nullptr) {
            std::wcerr << L"Ошибка при создании users.txt. Проверьте права доступа.\n";
        }
        else {
            fclose(usersFile);
        }

        // Создаем файл persistent.dat если не существует
        FILE* persistentFile = nullptr;
        if (_wfopen_s(&persistentFile, PERSISTENT_FILE.c_str(), L"r") != 0 || persistentFile == nullptr) {
            FILE* persistentFileOut = nullptr;
            if (_wfopen_s(&persistentFileOut, PERSISTENT_FILE.c_str(), L"w") != 0 || persistentFileOut == nullptr) {
                std::wcerr << L"Ошибка при создании persistent.dat. Проверьте права доступа.\n";
            }
            else {
                fclose(persistentFileOut);
            }
        }
        else {
            fclose(persistentFile);
        }
    }
    else {
        std::wcout << L"Программа уже была установлена ранее.\n";
        fclose(configFile);
    }
}

int wmain() {
    // Устанавливаем локаль для корректного вывода русского языка
    std::locale::global(std::locale("Russian_Russia.1251"));

    // Проверка установки программы
    checkInstallation();

    // Переменные для конфигурации
    int runs = 0;
    int totalRuns = 0;
    long long totalTime = 0;

    // Загрузка конфигурации
    loadConfig(runs, totalRuns, totalTime);

    // Устанавливаем время начала текущего запуска
    auto startTime = std::chrono::system_clock::now();

    // Проверка лимитов
    if (checkLimits(totalRuns, totalTime)) {
        // Предложение приобрести полную версию или деинсталлировать программу
        std::wcout << L"Вы достигли лимита использования программы.\n";
        std::wcout << L"Пожалуйста, приобретите полную версию или деинсталлируйте программу.\n";
        std::wcout << L"1. Приобрести полную версию\n";
        std::wcout << L"2. Деинсталлировать программу\n";
        int choice;
        std::wcin >> choice;
        std::wcin.ignore(); // Чтобы убрать символ новой строки из ввода
        if (choice == 1) {
            std::wcout << L"Спасибо за покупку полной версии!\n";
            // Сброс лимитов
            runs = 0;
            totalRuns = 0;
            totalTime = 0;
            saveConfig(runs, totalRuns, totalTime);
            // Продолжаем работу программы
        }
        else if (choice == 2) {
            // Деинсталляция программы
            _wremove(USERS_FILE.c_str());
            _wremove(CONFIG_FILE.c_str());
            // Не удаляем PERSISTENT_FILE, чтобы сохранить информацию о предыдущих запусках
            RemoveDirectoryW(L"C:\\DemoProgram");
            std::wcout << L"Программа деинсталлирована.\n";
            return 0;
        }
        else {
            std::wcout << L"Некорректный выбор. Программа завершена.\n";
            return 0;
        }
    }

    // Увеличение количества запусков
    runs++;
    totalRuns++;

    // Запрос ФИО
    std::wstring fio = getUserFIO();

    // Проверка и сохранение ФИО
    checkAndSaveFIO(fio);

    // Обновление общего времени использования
    auto now = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
    totalTime += elapsed;

    // Сохранение обновленных данных
    saveConfig(runs, totalRuns, totalTime);

    // Информирование пользователя о оставшихся лимитах
    int runsLeft = MAX_RUNS - totalRuns;
    long long timeLeft = TIME_LIMIT - totalTime;

    if (runsLeft > 0) {
        std::wcout << L"У вас осталось " << runsLeft << L" запусков программы.\n";
    }
    else {
        std::wcout << L"Вы достигли лимита запусков программы.\n";
    }

    if (timeLeft > 0) {
        std::wcout << L"У вас осталось " << timeLeft << L" секунд использования программы.\n";
    }
    else {
        std::wcout << L"Вы достигли лимита времени использования программы.\n";
    }

    std::wcout << L"Программа завершена.\n";
    return 0;
}
