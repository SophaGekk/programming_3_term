#ifndef DOCUMENTCONTROLLER_H
#define DOCUMENTCONTROLLER_H

#include <memory>
#include "Document.h"

class DocumentController {
public:
    void createDocument() {
        std::cout << "Введите имя нового документа: ";
        std::string name;
        std::cin >> name;
        document = std::make_shared<Document>(name);
        std::cout << "Создан документ " << document->getName() << std::endl;
    }

    void openDocument(const std::string& filename) {
        if (document->getName() == filename) {
            std::cout << "Открыт документ " << filename << std::endl;
            std::cout << "Примитивы в документе:" << std::endl;
            document->drawAllPrimitives();
        } else {
            std::cout << "Документ не создан."<<std::endl;
        }
    }

    void saveDocument(const std::string& filename) {
        if (document->getName() == filename) {
            document->saveToFile(filename);
            std::cout << "Примитивы в документе:" << std::endl;
            document->drawAllPrimitives();//вызов функции отрисовки для вывода в консоль всех примитивов добавленных в данный документ
        } else {
            std::cout << "Документ не создан."<<std::endl;
        }
    }

    void addPrimitive(const std::string& type) {
    if (document) {
        std::cout << "Введите имя примитива: ";
        std::string name;
        std::cin >> name;

        if (type == "rectangle") {
            std::cout << "Введите ширину: ";
            double width;
            std::cin >> width;
            std::cout << "Введите высоту: ";
            double height;
            std::cin >> height;
            document->addPrimitive(std::make_shared<Rectangle>(name, width, height));

        } else if (type == "square") {
            std::cout << "Введите длину стороны: ";
            double side;
            std::cin >> side;
            document->addPrimitive(std::make_shared<Square>(name, side));

        } else if (type == "circle") {
            std::cout << "Введите радиус: ";
            double radius;
            std::cin >> radius;
            document->addPrimitive(std::make_shared<Circle>(name, radius));

        } else if (type == "triangle") {
            std::cout << "Введите длину стороны a: ";
            double a;
            std::cin >> a;
            std::cout << "Введите длину стороны b: ";
            double b;
            std::cin >> b;
            std::cout << "Введите длину стороны c: ";
            double c;
            std::cin >> c;
            document->addPrimitive(std::make_shared<Triangle>(name, a, b, c));

        } else {
            std::cout << "Неподдерживаемый тип примитива.\n";
        }
    } else {
        std::cout << "Документ не создан.\n";
    }
}
    
    void removePrimitive(const std::string& primitiveName, const std::string& primitiveType) {
        if (document) {
            document->removePrimitive(primitiveName, primitiveType);
        } else {
            std::cout << "Документ не создан." << std::endl;
        }
    }

    void removeDocument(const std::string& filename) {
        if (document->getName() == filename) {
            std::cout << "Удаление документа '" << filename << "' и всех содержащихся примитивов:" << std::endl;
            document->deleteAllPrimitives();
            document.reset(); // Удаляет указатель на документ; примитивы удаляются автоматически, если нет других ссылок
        } else {
            std::cout << "Документ не создан." << std::endl;
        }
    }
private:
    std::shared_ptr<Document> document;
};

#endif