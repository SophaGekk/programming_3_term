#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <string>
#include <sstream>

class Primitive {
public:
    Primitive(const std::string& name, const std::string& type) : name(name), type(type) {}

    std::string getName() const {
        return name;
    }

    std::string getType() const {
        return type;
    }

    virtual ~Primitive() = default; // Важно для полиморфизма

    virtual void draw() = 0; // Виртуальный метод для отрисовки (будет реализован в производных классах)
    virtual std::string get() const = 0; // Чисто виртуальный метод

private:
    std::string name;
    std::string type;
};

class Rectangle : public Primitive {
public:
    Rectangle(const std::string& name, double width, double height) :
        Primitive(name, "rectangle"), width(width), height(height) {}

    void draw() override {
        std::cout << "Отрисовка прямоугольника " << getName() << " (стороны: " << width << ", " << height << ")" << std::endl;
    }
    double getWidth() const { return width; }

    std::string get() const override {
        return "прямоугольника " + getName() + " (стороны: " + std::to_string(width) + ", " + std::to_string(height) + ")";
    }

private:
    double width;
    double height;
};

class Square : public Rectangle { // Наследуется от Rectangle
public:
    Square(const std::string& name, double side) : Rectangle(name, side, side) {} // side используется для ширины и высоты
    void draw() override {
        std::cout << "Отрисовка квадрата " << getName() << " (сторона: " <<  getWidth() << ")" << std::endl; // Выводим только сторону
    }
    std::string get() const override {
        return "квадрата " + getName() + " (сторона: " + std::to_string(getWidth()) + ")";
    }
  
};


class Circle : public Primitive {
public:
    Circle(const std::string& name, double radius) : Primitive(name, "circle"), radius(radius) {}
    void draw() override {
        std::cout << "Отрисовка круга " << getName() << " (радиус: " << radius << ")" << std::endl;
    }
    std::string get() const override {
        return "круга " + getName() + " (радиус: " + std::to_string(radius) + ")";
    }

private:
    double radius;
};

class Triangle : public Primitive {
public:
    Triangle(const std::string& name, double a, double b, double c) : Primitive(name, "triangle"), a(a), b(b), c(c) {}
    void draw() override {
        std::cout << "Отрисовка треугольника " << getName() << " (стороны: " << a << ", " << b << ", " << c << ")" << std::endl;
    }
    std::string get() const override {
        return "треугольника " + getName() + " (стороны: " + std::to_string(a) + ", " + std::to_string(b)  + ", " + std::to_string(c)+ ")";
    }
private:
    double a, b, c;
};

#endif