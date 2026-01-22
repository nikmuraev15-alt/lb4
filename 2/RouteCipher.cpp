/**
 * @file main.cpp
 * @author Мураев Никита
 * @version 1.0
 * @date 17.12.2025
 * @brief Модульное тестирование шифра методом перестановки
 * @details Программа выполняет автоматическое тестирование
 *          всех методов класса RouteCipher для русского алфавита.
 *          Тестируются конструктор, шифрование и дешифрование.
 * @copyright ИБСТ ПГУ
 * @warning Для корректной работы требуется русская локаль
 */
#include "RouteCipher.h"
#include <algorithm>
#include <cctype>
#include <locale>
#include <iostream>

/**
 * @brief Конструктор класса RouteCipher
 * @param Key Количество столбцов таблицы
 * @throws CipherError если ключ некорректен
 */
RouteCipher::RouteCipher(int Key) {
    ValidateKey(Key);
    Columns = Key;
}

/**
 * @brief Проверяет корректность ключа шифрования
 * @param Key Проверяемый ключ
 * @throws CipherError если ключ ≤ 0 или > 50
 */
void RouteCipher::ValidateKey(int Key) {
    if (Key <= 0) {
        throw CipherError("Ключ должен быть положительным числом");
    }
    if (Key > 50) {
        throw CipherError("Слишком большой ключ");
    }
}

/**
 * @brief Проверяет корректность входного текста
 * @param Text Проверяемый текст
 * @throws CipherError если текст пустой
 */
void RouteCipher::ValidateText(const std::wstring& Text) {
    if (Text.empty()) {
        throw CipherError("Текст не может быть пустым");
    }
}

/**
 * @brief Очищает текст от пробелов и управляющих символов
 * @param Text Исходный текст
 * @return Текст без пробелов, табуляций и символов новой строки
 * @details Удаляет только: пробелы (L' '), табуляции (L'\t'),
 *          символы новой строки (L'\n') и возврата каретки (L'\r')
 */
std::wstring RouteCipher::CleanText(const std::wstring& Text) {
    std::wstring Result;
    for (wchar_t c : Text) {
        // Убираем только пробелы, табуляции, новые строки
        if (c != L' ' && c != L'\t' && c != L'\n' && c != L'\r') {
            Result += c;
        }
    }
    return Result;
}

/**
 * @brief Преобразует текст к верхнему регистру
 * @param Text Исходный текст
 * @return Текст в верхнем регистре
 * @details Поддерживает:
 *   - Английские буквы: a-z → A-Z
 *   - Русские буквы: а-я → А-Я
 *   - Букву "ё" → "Ё"
 *   - Цифры и знаки препинания остаются без изменений
 */
std::wstring RouteCipher::ToUpperCase(const std::wstring& Text) {
    std::wstring Result = Text;
    
    for (wchar_t& c : Result) {
        // Английские буквы
        if (c >= L'a' && c <= L'z') {
            c = c - L'a' + L'A';
        }
        // Русские буквы
        else if (c >= L'а' && c <= L'я') {
            c = c - L'а' + L'А';
        }
        // Особые русские буквы
        else if (c == L'ё') {
            c = L'Ё';
        }
        // Все остальные символы (цифры, знаки препинания) остаются как есть
    }
    
    return Result;
}

/**
 * @brief Подготавливает текст к шифрованию
 * @param Text Исходный текст
 * @return Подготовленный текст (без пробелов, в верхнем регистре)
 * @throws CipherError если после очистки текст стал пустым
 */
std::wstring RouteCipher::PrepareText(const std::wstring& Text) {
    std::wstring Cleaned = CleanText(Text);
    if (Cleaned.empty()) {
        throw CipherError("После удаления пробелов текст пуст");
    }
    return ToUpperCase(Cleaned);
}

/**
 * @brief Шифрует текст методом маршрутной перестановки
 * @param Text Исходный текст для шифрования
 * @return Зашифрованный текст
 * @throws CipherError если текст пустой или содержит только пробелы
 * 
 * Алгоритм:
 * 1. Подготавливается текст (очистка + верхний регистр)
 * 2. Вычисляется количество строк: Rows = ceil(TextLength / Columns)
 * 3. Создается таблица Rows×Columns
 * 4. Таблица заполняется по строкам слева направо
 * 5. Пустые ячейки заполняются символом 'X'
 * 6. Таблица считывается по столбцам справа налево, сверху вниз
 * 
 * Пример для Text="HELLO", Columns=3:
 *   TextLength=5, Rows=2
 *   Таблица:
 *     [H, E, L]
 *     [L, O, X]
 *   Результат: "LXOLHE"
 */
std::wstring RouteCipher::Encrypt(const std::wstring& Text) {
    ValidateText(Text);
    std::wstring PreparedText = PrepareText(Text);
    
    int TextLength = PreparedText.length();
    int Rows = (TextLength + Columns - 1) / Columns; // Округление вверх
    
    // Создаем таблицу и заполняем по строкам слева направо
    std::vector<std::vector<wchar_t>> Table(Rows, std::vector<wchar_t>(Columns));
    
    int index = 0;
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Columns; j++) {
            if (index < TextLength) {
                Table[i][j] = PreparedText[index++];
            } else {
                // Заполняем оставшиеся ячейки специальным символом
                Table[i][j] = L'X';
            }
        }
    }
    
    // Читаем по столбцам сверху вниз, справа налево
    std::wstring Result;
    for (int j = Columns - 1; j >= 0; j--) {
        for (int i = 0; i < Rows; i++) {
            Result += Table[i][j];
        }
    }
    
    return Result;
}

/**
 * @brief Дешифрует текст, зашифрованный методом маршрутной перестановки
 * @param Text Зашифрованный текст
 * @return Расшифрованный текст
 * @throws CipherError если текст пустой или содержит только пробелы
 * 
 * Алгоритм:
 * 1. Подготавливается шифротекст
 * 2. Вычисляется количество строк
 * 3. Таблица заполняется по столбцам справа налево, сверху вниз
 * 4. Таблица считывается по строкам слева направо
 * 5. Удаляются символы 'X', добавленные при шифровании
 */
std::wstring RouteCipher::Decrypt(const std::wstring& Text) {
    ValidateText(Text);
    std::wstring PreparedText = PrepareText(Text);
    
    int TextLength = PreparedText.length();
    int Rows = (TextLength + Columns - 1) / Columns;
    
    // Создаем пустую таблицу
    std::vector<std::vector<wchar_t>> Table(Rows, std::vector<wchar_t>(Columns));
    
    // Заполняем таблицу по столбцам сверху вниз, справа налево
    int index = 0;
    for (int j = Columns - 1; j >= 0; j--) {
        for (int i = 0; i < Rows; i++) {
            if (index < TextLength) {
                Table[i][j] = PreparedText[index++];
            } else {
                Table[i][j] = L'X';
            }
        }
    }
    
    // Читаем по строкам слева направо
    std::wstring Result;
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Columns; j++) {
            Result += Table[i][j];
        }
    }
    
    // Убираем добавленные символы 'X' в конце
    while (!Result.empty() && Result.back() == L'X') {
        Result.pop_back();
    }
    
    return Result;
}
