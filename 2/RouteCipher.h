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
#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <map>

/**
 * @class CipherError
 * @brief Класс исключения для обработки ошибок шифрования
 * @details Наследуется от std::invalid_argument. Используется для генерации
 *          исключительных ситуаций при работе с шифром маршрутной перестановки.
 */
class CipherError : public std::invalid_argument {
public:
    /**
     * @brief Конструктор с параметром типа std::string
     * @param WhatArg Сообщение об ошибке в виде строки
     */
    explicit CipherError(const std::string& WhatArg) : 
        std::invalid_argument(WhatArg) {}
    
    /**
     * @brief Конструктор с параметром типа const char*
     * @param WhatArg Сообщение об ошибке в виде C-строки
     */
    explicit CipherError(const char* WhatArg) : 
        std::invalid_argument(WhatArg) {}
};

/**
 * @class RouteCipher
 * @brief Класс для шифрования методом маршрутной перестановки
 * @details Реализует алгоритм шифрования, при котором символы текста
 *          размещаются в таблице по строкам, а затем считываются по столбцам
 *          справа налево. Ключом является количество столбцов таблицы.
 * 
 * Алгоритм работы:
 * 1. Текст очищается от пробелов и приводится к верхнему регистру
 * 2. Символы записываются в таблицу по строкам слева направо
 * 3. Таблица считывается по столбцам сверху вниз, начиная с правого столбца
 * 4. Дешифрование выполняет обратную операцию
 */
class RouteCipher {
private:
    int Columns; ///< Количество столбцов таблицы (ключ шифрования)
    
    /**
     * @brief Проверяет корректность ключа шифрования
     * @param Key Проверяемый ключ
     * @throws CipherError если ключ некорректен
     * @details Ключ должен быть положительным числом и не превышать 50
     */
    void ValidateKey(int Key);
    
    /**
     * @brief Проверяет корректность входного текста
     * @param Text Проверяемый текст
     * @throws CipherError если текст пустой
     */
    void ValidateText(const std::wstring& Text);
    
    /**
     * @brief Подготавливает текст к шифрованию
     * @param Text Исходный текст
     * @return Подготовленный текст (без пробелов, в верхнем регистре)
     * @throws CipherError если после очистки текст стал пустым
     */
    std::wstring PrepareText(const std::wstring& Text);
    
    /**
     * @brief Очищает текст от пробелов и управляющих символов
     * @param Text Исходный текст
     * @return Текст без пробелов, табуляций и символов новой строки
     */
    std::wstring CleanText(const std::wstring& Text);
    
    /**
     * @brief Преобразует текст к верхнему регистру
     * @param Text Исходный текст
     * @return Текст в верхнем регистре
     * @details Поддерживает русские и английские буквы
     */
    std::wstring ToUpperCase(const std::wstring& Text);
    
public:
    /**
     * @brief Конструктор класса
     * @param Key Количество столбцов таблицы
     * @throws CipherError если ключ некорректен
     */
    RouteCipher() = delete;
    explicit RouteCipher(int Key);
    
    /**
     * @brief Шифрует текст методом маршрутной перестановки
     * @param Text Исходный текст для шифрования
     * @return Зашифрованный текст
     * @throws CipherError если текст пустой или содержит только пробелы
     * 
     * Пример работы:
     *   Ключ: 3, Текст: "ABC"
     *   Таблица 1x3: [A, B, C]
     *   Чтение справа налево: C, B, A
     *   Результат: "CBA"
     */
    std::wstring Encrypt(const std::wstring& Text);
    
    /**
     * @brief Дешифрует текст, зашифрованный методом маршрутной перестановки
     * @param Text Зашифрованный текст
     * @return Расшифрованный текст
     * @throws CipherError если текст пустой или содержит только пробелы
     */
    std::wstring Decrypt(const std::wstring& Text);
    
    /**
     * @brief Возвращает текущее значение ключа
     * @return Количество столбцов таблицы
     */
    int GetColumns() const { return Columns; }
};
