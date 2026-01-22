/**
 * @file main.cpp
 * @author Мураев Никита
 * @version 1.0
 * @date 17.12.2025
 * @brief Модульное тестирование шифра маршрутной перестановки
 * @details Программа выполняет автоматическое тестирование
 *          всех методов класса RouteCipher для русского алфавита.
 *          Тестируются конструктор, шифрование и дешифрование.
 * @copyright ИБСТ ПГУ
 * @warning Для корректной работы требуется русская локаль
 */
#include "RouteCipher.h"
#include <iostream>
#include <locale>
#include <cwchar>

/**
 * @brief Функция запуска модульных тестов шифра маршрутной перестановки
 * @details Тестирует:
 *   - Конструктор с различными ключами
 *   - Шифрование разных текстов
 *   - Дешифрование и цикл шифрование-дешифрование
 *   - Обработку ошибок
 * 
 * Всего выполняется 9 тестов, результаты выводятся в консоль
 */
void runTests() {
    std::cout << "==========================================" << std::endl;
    std::cout << "ЛАБОРАТОРНАЯ РАБОТА №3: МОДУЛЬНЫЕ ТЕСТЫ" << std::endl;
    std::cout << "Шифр маршрутной перестановки" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    int passed = 0;  ///< Количество пройденных тестов
    int total = 0;   ///< Общее количество тестов
    
    // ТЕСТ 1: Конструктор
    std::cout << "\n1. Тесты конструктора:" << std::endl;
    
    // 1.1 Валидный ключ
    try {
        total++;
        RouteCipher cipher(3);
        std::cout << "✓ 1.1 Ключ=3 - OK" << std::endl;
        passed++;
    } catch (...) {
        std::cout << "✗ 1.1 Ключ=3 - ОШИБКА" << std::endl;
    }
    
    // 1.2 Отрицательный ключ (должно быть исключение)
    try {
        total++;
        RouteCipher cipher(-5);
        std::cout << "✗ 1.2 Ключ=-5 - ОШИБКА (должно быть исключение)" << std::endl;
    } catch (const CipherError& e) {
        std::cout << "✓ 1.2 Ключ=-5 - OK: " << e.what() << std::endl;
        passed++;
    } catch (...) {
        std::cout << "✗ 1.2 Ключ=-5 - НЕВЕРНОЕ ИСКЛЮЧЕНИЕ" << std::endl;
    }
    
    // 1.3 Ключ 0 (должно быть исключение)
    try {
        total++;
        RouteCipher cipher(0);
        std::cout << "✗ 1.3 Ключ=0 - ОШИБКА (должно быть исключение)" << std::endl;
    } catch (const CipherError& e) {
        std::cout << "✓ 1.3 Ключ=0 - OK: " << e.what() << std::endl;
        passed++;
    } catch (...) {
        std::cout << "✗ 1.3 Ключ=0 - НЕВЕРНОЕ ИСКЛЮЧЕНИЕ" << std::endl;
    }
    
    // ТЕСТ 2: Шифрование
    std::cout << "\n2. Тесты шифрования:" << std::endl;
    
    // 2.1 Шифрование простого текста "ABC" с ключом 3
    try {
        total++;
        RouteCipher cipher(3);
        std::wstring text = L"ABC";
        std::wstring result = cipher.Encrypt(text);
        
        // Для ключа=3 и текста "ABC":
        // Таблица 1x3: A B C
        // Чтение справа налево по столбцам: C B A
        std::wstring expected = L"CBA";
        
        if (result == expected) {
            std::wcout << L"✓ 2.1 Encrypt('ABC') = " << result << std::endl;
            passed++;
        } else {
            std::wcout << L"✗ 2.1 Encrypt('ABC') - ОЖИДАЛОСЬ: CBA, ПОЛУЧЕНО: " 
                      << result << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "✗ 2.1 Encrypt('ABC') - ОШИБКА: " << e.what() << std::endl;
    }
    
    // 2.2 Шифрование текста с символами и цифрами
    try {
        total++;
        RouteCipher cipher(4);
        std::wstring text = L"@825B";
        std::wstring result = cipher.Encrypt(text);
        
        // Выводим результат для проверки
        std::wcout << L"  2.2 Encrypt('@825B') = " << result << std::endl;
        
        // Проверяем длину результата (должна быть кратна 4)
        if (result.length() % 4 == 0) {
            std::wcout << L"✓ 2.2 Результат корректен, длина = " << result.length() << std::endl;
            passed++;
        } else {
            std::wcout << L"✗ 2.2 Некорректная длина результата" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "✗ 2.2 Encrypt('@825B') - ОШИБКА: " << e.what() << std::endl;
    }
    
    // 2.3 Пустой текст (должно быть исключение)
    try {
        total++;
        RouteCipher cipher(3);
        cipher.Encrypt(L"");
        std::cout << "✗ 2.3 Encrypt('') - ОШИБКА (должно быть исключение)" << std::endl;
    } catch (const CipherError& e) {
        std::cout << "✓ 2.3 Encrypt('') - OK: " << e.what() << std::endl;
        passed++;
    } catch (...) {
        std::cout << "✗ 2.3 Encrypt('') - НЕВЕРНОЕ ИСКЛЮЧЕНИЕ" << std::endl;
    }
    
    // ТЕСТ 3: Дешифрование
    std::cout << "\n3. Тесты дешифрования:" << std::endl;
    
    // 3.1 Полный цикл шифрование-дешифрование
    try {
        total++;
        RouteCipher cipher(3);
        std::wstring original = L"HELLO";
        std::wstring encrypted = cipher.Encrypt(original);
        std::wstring decrypted = cipher.Decrypt(encrypted);
        
        if (decrypted == original) {
            std::wcout << L"✓ 3.1 Decrypt(Encrypt('HELLO')) = " << decrypted << std::endl;
            passed++;
        } else {
            std::wcout << L"✗ 3.1 Ожидалось 'HELLO', получено: " << decrypted << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "✗ 3.1 Decrypt - ОШИБКА: " << e.what() << std::endl;
    }
    
    // 3.2 Дешифрование заданного шифротекста "CBA" с ключом 3
    try {
        total++;
        RouteCipher cipher(3);
        std::wstring text = L"CBA"; // Зашифрованный "ABC" с ключом 3
        std::wstring decrypted = cipher.Decrypt(text);
        
        // Ожидаем "ABC"
        std::wstring expected = L"ABC";
        
        if (decrypted == expected) {
            std::wcout << L"✓ 3.2 Decrypt('CBA') = " << decrypted << " (ожидалось: ABC)" << std::endl;
            passed++;
        } else {
            std::wcout << L"✗ 3.2 Decrypt('CBA') - ожидалось ABC, получено: " << decrypted << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "✗ 3.2 Decrypt - ОШИБКА: " << e.what() << std::endl;
    }
    
    // 3.3 Пустой шифротекст (должно быть исключение)
    try {
        total++;
        RouteCipher cipher(3);
        cipher.Decrypt(L"");
        std::cout << "✗ 3.3 Decrypt('') - ОШИБКА (должно быть исключение)" << std::endl;
    } catch (const CipherError& e) {
        std::cout << "✓ 3.3 Decrypt('') - OK: " << e.what() << std::endl;
        passed++;
    } catch (...) {
        std::cout << "✗ 3.3 Decrypt('') - НЕВЕРНОЕ ИСКЛЮЧЕНИЕ" << std::endl;
    }
    
    // ИТОГИ тестирования
    std::cout << "\n==========================================" << std::endl;
    std::cout << "ИТОГИ ТЕСТИРОВАНИЯ:" << std::endl;
    std::cout << "Всего тестов: " << total << std::endl;
    std::cout << "Пройдено: " << passed << std::endl;
    std::cout << "Не пройдено: " << (total - passed) << std::endl;
    double successRate = (total > 0) ? (passed * 100.0 / total) : 0.0;
    std::cout << "Успешность: " << successRate << "%" << std::endl;
    std::cout << "==========================================" << std::endl;
}

/**
 * @brief Главная функция программы
 * @return Код завершения программы (0 - успешное выполнение)
 * 
 * Устанавливает русскую локаль для корректной работы с Unicode,
 * запускает модульные тесты и ожидает нажатия Enter перед выходом.
 */
int main() {
    // Устанавливаем локаль для корректной работы с русскими символами
    setlocale(LC_ALL, "ru_RU.UTF-8");
    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    // Запускаем тесты
    runTests();
    
    // Пауза для удобства просмотра результатов
    std::cout << "\nНажмите Enter для выхода..." << std::endl;
    std::cin.get();
    
    return 0;
}
