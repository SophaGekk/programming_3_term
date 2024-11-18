#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <vector>
#include <memory>
#include "Primitive.h"
#include <algorithm>

class Document {
public:
    Document(const std::string& name) : name(name) {}

    void addPrimitive(std::shared_ptr<Primitive> primitive) {
        primitives.push_back(primitive);
        std::cout << "В документ " << name << " добавлен примитив " << primitive->getType() << ": "<< primitive->getName() << std::endl;
    }

    void removePrimitive(const std::string& primitiveName, const std::string& primitiveType) {
        auto it = std::find_if(primitives.begin(), primitives.end(),[&](const std::shared_ptr<Primitive>& p) {return p->getName() == primitiveName && p->getType() == primitiveType;});
        if (it != primitives.end()) {
            std::cout << "Из документа " << name << " удален примитив " << (*it)->getName() << " типа " << (*it)->getType() << std::endl;
            primitives.erase(it);
        } else {
            std::cout << "Примитив с именем '" << primitiveName << "' типа '" << primitiveType << "' не найден." << std::endl;
        }
    }

    void saveToFile(const std::string& filename) {
        std::cout << "Документ " << name << " сохранен в файл " << filename << std::endl;
    }

    void loadFromFile(const std::string& filename) {
        std::cout << "Документ " << name << " загружен из файла " << filename << std::endl;
    }

    std::string getName() const {
        return name;
    }

    void drawAllPrimitives() const {
        for (const auto& primitive : primitives) {
            std::cout << "Отрисовка " << primitive->get() << std::endl;;
        }
    }

    void deleteAllPrimitives() const {
        for (const auto& primitive : primitives) {
            std::cout << "Удаление " << primitive->get() << std::endl;;
        }
    }

private:
    std::string name;
    std::vector<std::shared_ptr<Primitive>> primitives;
};

#endif