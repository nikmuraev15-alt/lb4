/**
 * @file modAlphaCipher.h
 * @author Мураев Никита
 * @version 1.0
 * @date 03.12.2025
 * @brief Заголовочный файл для шифрования методом Гронсфельда
 * @copyright ИБСТ ПГУ
 * @warning Реализация для русского языка
 */

#ifndef MODALPHACIPHER_H
#define MODALPHACIPHER_H

#include <string>
#include <vector>
#include <stdexcept>

/**
 * @brief Класс исключений для модуля шифрования
 * @details Наследуется от std::invalid_argument
 */
class cipher_error : public std::invalid_argument {
public:
    /**
     * @brief Конструктор с строкой
     * @param what_arg Сообщение об ошибке
     */
    explicit cipher_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}
    
    /**
     * @brief Конструктор с C-строкой
     * @param what_arg Сообщение об ошибке
     */
    explicit cipher_error(const char* what_arg) : std::invalid_argument(what_arg) {}
};

/**
 * @brief Класс для шифрования методом Гронсфельда
 * @details Реализует шифрование и дешифрование текста на русском языке
 *          с использованием ключевого слова. Алгоритм основан на сложении
 *          символов сообщения с символами ключа по модулю размера алфавита.
 */
class modAlphaCipher
{
private:
    const std::string numAlpha = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; ///< Русский алфавит в верхнем регистре
    std::vector<int> key; ///< Ключ в числовом виде

    /**
     * @brief Удаление пробелов из строки
     * @param [in] s Входная строка
     * @return Строка без пробелов
     */
    std::string removeSpaces(const std::string& s) const;
    
    /**
     * @brief Преобразование текста в числовые индексы
     * @param [in] text Текст для преобразования
     * @return Вектор индексов символов в алфавите
     * @throw cipher_error если текст пуст или содержит недопустимые символы
     */
    std::vector<int> textToIndices(const std::string& text) const;
    
    /**
     * @brief Преобразование индексов в текст
     * @param [in] indices Вектор индексов символов
     * @return Текст, соответствующий индексам
     */
    std::string indicesToText(const std::vector<int>& indices) const;

public:
    modAlphaCipher() = delete; ///< Конструктор по умолчанию запрещен
    
    /**
     * @brief Основной конструктор с ключом
     * @param [in] skey Ключ шифрования в виде строки
     * @throw cipher_error если ключ пуст или содержит недопустимые символы
     */
    modAlphaCipher(const std::string& skey);
    
    /**
     * @brief Шифрование открытого текста
     * @param [in] open_text Открытый текст для шифрования
     * @return Зашифрованный текст
     * @throw cipher_error если текст пуст или содержит недопустимые символы
     */
    std::string encrypt(const std::string& open_text);
    
    /**
     * @brief Дешифрование зашифрованного текста
     * @param [in] cipher_text Зашифрованный текст
     * @return Расшифрованный текст
     * @throw cipher_error если текст пуст или содержит недопустимые символы
     */
    std::string decrypt(const std::string& cipher_text);
};

#endif // MODALPHACIPHER_H
