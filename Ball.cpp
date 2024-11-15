#include "Ball.hpp"
#include <cmath>
/// @brief Конструктор класса Ball. 
/// @param velocity Скорость мяча.
/// @param center Центральная точка мяча.
/// @param radius Радиус мяча.
/// @param color Цвет мяча.
/// @param isCollidable Логическое значение, указывающее, может ли мяч участвовать в столкновениях.
Ball::Ball(Velocity velocity, Point center, double radius, Color color, bool isCollidable)
    : velocity(velocity), center(center), radius(radius), color(color), isCollidable(isCollidable) {}

/// @brief Возвращает состояние возможности столкновения мяча. 
/// @return True, если мяч может участвовать в столкновениях, false в противном случае.
bool Ball::getIsCollidable() const {
    return isCollidable; 
}

/// @brief Задает скорость объекта
/// @param velocity значение скорости
void Ball::setVelocity(const Velocity& velocity) {
    this->velocity = velocity;
}

/// @brief Возваращает скорость объекта
/// @return скорость объекта
Velocity Ball::getVelocity() const {
    return velocity;
}

/// @brief Выполняет отрисовку объекта
/// @details объект Ball абстрагирован от способа отображения пикселей на экране, знаком с интерфейсом, который предоставляет Painter (выполняется путем вызова painter.draw(...))
/// @param painter контекст отрисовки
void Ball::draw(Painter& painter) const { 
    painter.draw(center, radius, color);
}

/// @brief Задает координаты центра объекта
/// @param center центр объекта
void Ball::setCenter(const Point& center) {
    this->center = center;
}

/// @brief Возвращает координаты центра объекта
/// @return центр объекта
Point Ball::getCenter() const {
    return center;
}

/// @brief Возвращает радиус объекта
/// @return радиус объекта
double Ball::getRadius() const {
    return radius;
}

/// @brief Возвращает массу объекта
///@details В нашем приложении считаем, что все шары
/// состоят из одинакового материала с фиксированной
/// плотностью. В этом случае масса в условных единицах
/// эквивалентна объему: PI * radius^3 * 4. / 3.
/// @return масса объекта
double Ball::getMass() const {
    return M_PI * pow(radius, 3) * 4. / 3.;
}
