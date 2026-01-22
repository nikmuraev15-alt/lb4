/**
 * @file modAlphaCipher.cpp
 * @author Мураев Никита
 * @version 1.0
 * @date 03.12.2025
 * @brief Реализация шифра Гронсфельда для русского алфавита
 * @copyright ИБСТ ПГУ
 * @details Файл содержит реализацию методов класса modAlphaCipher
 */

#include "modAlphaCipher.h"
#include <stdexcept>
#include <locale>
#include <codecvt>

using namespace std;

/**
 * @brief Удаление пробелов из строки
 * @param [in] s Входная строка
 * @return Строка без пробелов
 */
string modAlphaCipher::removeSpaces(const string& s) const
{
    string result;
    for (char c : s) {
        if (c != ' ') {
            result += c;
        }
    }
    return result;
}

/**
 * @brief Преобразование текста в числовые индексы
 * @param [in] text Текст для преобразования
 * @return Вектор индексов символов в алфавите
 * @throw cipher_error если текст пуст или содержит недопустимые символы
 */
vector<int> modAlphaCipher::textToIndices(const string& text) const
{
    vector<int> indices;
    
    // Русские буквы в UTF-8 занимают 2 байта
    size_t i = 0;
    while (i < text.size()) {
        if (i + 1 >= text.size()) {
            throw cipher_error("Invalid character sequence in input");
        }
        
        string letter = text.substr(i, 2);
        bool found = false;
        
        // Поиск символа в алфавите
        for (size_t pos = 0; pos < numAlpha.size(); pos += 2) {
            if (numAlpha.substr(pos, 2) == letter) {
                indices.push_back(static_cast<int>(pos / 2));
                found = true;
                break;
            }
        }

        if (!found) {
            throw cipher_error("Invalid character in input (not a Russian uppercase letter)");
        }

        i += 2;
    }

    if (indices.empty()) {
        throw cipher_error("Empty text after processing");
    }

    return indices;
}

/**
 * @brief Преобразование индексов в текст
 * @param [in] indices Вектор индексов символов
 * @return Текст, соответствующий индексам
 */
string modAlphaCipher::indicesToText(const vector<int>& indices) const
{
    string result;
    for (int idx : indices) {
        if (idx >= 0 && idx < static_cast<int>(numAlpha.size() / 2)) {
            result += numAlpha.substr(idx * 2, 2);
        }
    }
    return result;
}

/**
 * @brief Основной конструктор с ключом
 * @param [in] skey Ключ шифрования в виде строки
 * @throw cipher_error если ключ пуст или содержит недопустимые символы
 */
modAlphaCipher::modAlphaCipher(const string& skey)
{
    string cleanKey = removeSpaces(skey);
    if (cleanKey.empty()) {
        throw cipher_error("Empty key");
    }
    key = textToIndices(cleanKey);
}

/**
 * @brief Шифрование открытого текста
 * @param [in] open_text Открытый текст для шифрования
 * @return Зашифрованный текст
 * @throw cipher_error если текст пуст или содержит недопустимые символы
 */
string modAlphaCipher::encrypt(const string& open_text)
{
    string cleanText = removeSpaces(open_text);
    if (cleanText.empty()) {
        throw cipher_error("Empty open text");
    }
    
    vector<int> work = textToIndices(cleanText);
    int alphabetSize = static_cast<int>(numAlpha.size() / 2);

    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] + key[i % key.size()]) % alphabetSize;
    }
    return indicesToText(work);
}

/**
 * @brief Дешифрование зашифрованного текста
 * @param [in] cipher_text Зашифрованный текст
 * @return Расшифрованный текст
 * @throw cipher_error если текст пуст или содержит недопустимые символы
 */
string modAlphaCipher::decrypt(const string& cipher_text)
{
    string cleanText = removeSpaces(cipher_text);
    if (cleanText.empty()) {
        throw cipher_error("Empty cipher text");
    }
    
    vector<int> work = textToIndices(cleanText);
    int alphabetSize = static_cast<int>(numAlpha.size() / 2);

    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] - key[i % key.size()] + alphabetSize) % alphabetSize;
    }
    return indicesToText(work);
}
