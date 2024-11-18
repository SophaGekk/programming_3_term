#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Document.h"
#include "Primitive.h"
// #include "Rectangle.h"
#include "DocumentController.h"

// Представление (GUI) - очень упрощенная реализация для демонстрации
void printMenu() {
    std::cout << "\nВыберите действие:\n";
    std::cout << "1. Создать документ\n";
    std::cout << "2. Открыть документ\n";
    std::cout << "3. Сохранить документ\n";
    std::cout << "4. Удалить документ\n";
    std::cout << "5. Добавить примитив\n";
    std::cout << "6. Удалить примитив\n";
    std::cout << "7. Выход\n";
    std::cout << "Введите номер действия: ";
}

int main() {
    std::shared_ptr<DocumentController> controller = std::make_shared<DocumentController>();

    while (true) {
        printMenu();
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                controller->createDocument();
                break;
            case 2: {
                std::string filename;
                std::cout << "Введите имя файла: ";
                std::cin >> filename;
                controller->openDocument(filename);
                break;
            }
            case 3: {
                std::string filename;
                std::cout << "Введите имя файла: ";
                std::cin >> filename;
                controller->saveDocument(filename);
                break;
            }
            case 4: {
                std::string filename;
                std::cout << "Введите имя файла: ";
                std::cin >> filename;
                controller->removeDocument(filename);
                break;
            }
            case 5: {
                std::cout << "Введите тип примитива (rectangle, square, circle, triangle...): ";
                std::string type;
                std::cin >> type;
                controller->addPrimitive(type);
                break;
            }
            case 6: {
                std::cout << "Введите тип примитива (rectangle, square, circle, triangle...): ";
                std::string type;
                std::cin >> type;
                std::cout << "Введите имя примитива: ";
                std::string nameToRemove;
                std::cin >> nameToRemove;
                controller->removePrimitive(nameToRemove, type); 
                break;
            }
            case 7:
                std::cout << "Выход...\n";
                return 0;
            default:
                std::cout << "Некорректный выбор.\n";
        }
    }
}