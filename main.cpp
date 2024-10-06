#include <iostream>

// Структура для последовательного контейнера (массив)
// template <typename T>
struct SequentialContainer {
    int* data; // Указатель на массив для хранения элементов
    int size; // Текущее количество элементов
    int capacity; // Максимальная емкость массива

    // Конструктор
    SequentialContainer() : data(nullptr), size(0), capacity(0) {}
    
    // Перемещающий конструктор
    SequentialContainer(SequentialContainer&& other) noexcept
    : data(std::move(other.data)), size(other.size), capacity(other.capacity) {
        other.data = nullptr; // Освобождаем указатель у другого объекта
        other.size = 0;
        other.capacity = 0;
    }

    // SequentialContainer(SequentialContainer&& other) noexcept: data(other.data), size(other.size), capacity(other.capacity) {
    //     other.data = nullptr; // Освобождаем указатель у другого объекта
    //     other.size = 0;
    //     other.capacity = 0;
    // }

    // Перемещающий оператор присваивания
    SequentialContainer& operator=(SequentialContainer&& other) noexcept {
        if (this != &other) {
        delete[] data; // Освобождаем текущие ресурсы
        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = nullptr; // Освобождаем указатель у другого объекта
        other.size = 0;
        other.capacity = 0;
        }
        return *this;
    }

    // Функция для увеличения емкости
    void resize() {
        capacity = capacity == 0 ? 1 : static_cast<int>(capacity * 1.5); // Увеличиваем емкость на 50%
        int* newData = new int[capacity]; // Выделяем новую память
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i]; // Копируем старые данные в новую память
        }
        delete[] data; // Освобождаем старую память
        data = newData; // Перенаправляем указатель на новую память
    }

    // Добавление элемента в конец
    void push_back(int value) {
        if (size == capacity) {
            resize(); // Изменяем размер, если емкость заполнена
        }
        data[size++] = value; // Добавляем элемент и увеличиваем размер
    }
    // Добавление элемента в начало
    void push_front(int value) {
        if (size == capacity) {
            resize(); // Изменяем размер, если емкость заполнена
        }
        // Сдвигаем все элементы на один вправо
        for (int i = size; i > 0; --i) {
            data[i] = data[i - 1];
        }
        data[0] = value; // Вставляем новый элемент в начало
        ++size;
    }

    // Добавление элемента в указанный индекс
    void insert(int index, int value) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Индекс вне диапазона"); // Исключение для недопустимого индекса
        }
        if (size == capacity) {
            resize(); // Изменяем размер, если емкость заполнена
        }
        // Сдвигаем элементы вправо, начиная с указанного индекса
        for (int i = size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value; // Вставляем значение в указанный индекс
        ++size;
    }


    // Удаление элемента по индексу
    void erase(int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Индекс вне диапазона"); // Исключение для недопустимого индекса
    }
    for (int i = index; i < size - 1; ++i) {
        data[i] = data[i + 1];// Сдвинаем элементы влево
    }
    --size;// Уменьшаем размер
    }

    // Получение размера контейнера
    int getSize() const {
        return size; // Возвращаем текущее количество элементов
    }

    // Получение размера контейнера
    int getCapacity() const {
        return capacity; // Возвращаем текущее количество элементов
    }

    // Вывод содержимого контейнера
    void print() const {
    for (int i = 0; i < size; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
    }

    // Оператор [] для доступа к элементам по индексу (новый)
    int& operator[](int index) {
        if (index < 0 || index >= size) {      
            // Обработка недопустимого индекса (выбрасывание исключения)
            throw std::out_of_range("Индекс вне диапазона");
        }
        return data[index];// Возвращаем элемент по индексу
    }

    // Структура итератора для SequentialContainer
    struct Iterator {
        int* ptr;

        // Конструктор
        Iterator(int* ptr) : ptr(ptr) {}

        // Оператор разыменования
        int& operator*() {
            if (ptr==nullptr) {
                throw std::out_of_range("Индекс вне диапазона"); // Исключение для недопустимого индекса
            }
            return *ptr;
        }
        int& get() {
            if (ptr == nullptr) {
                throw std::out_of_range("Индекс вне диапазона"); // Исключение для недопустимого индекса
            }
            return *ptr;
        }

        // Оператор сравнения (для проверки конца итерации)
        bool operator!=(const Iterator& other) {
            return ptr != other.ptr;
        }

        // Перемещение итератора на следующий элемент
        Iterator& operator++() {
            ptr++;
            return *this;
        }

    };
    // Возвращает итератор на начало контейнера
    Iterator begin() {
        return Iterator(data);
    }

    // Возвращает итератор на конец контейнера
    Iterator end() {
        return Iterator(data + size);
    }

    // Деструктор
    ~SequentialContainer() {
    delete[] data;
    }
};


// Класс для спискового контейнера (связь через указатели)
class DoubleLinkedList {
private:
    struct Node {
        int value;
        Node* next;
        Node* prev;

        Node(int value) : value(value), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    // Конструктор
    DoubleLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Деструктор
    ~DoubleLinkedList() {
        while (head != nullptr) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }

    // Перемещающий конструктор
    // DoubleLinkedList(DoubleLinkedList&& other) noexcept: head(other.head), tail(other.tail), size(other.size) {
    DoubleLinkedList(DoubleLinkedList&& other) noexcept
    : head(std::move(other.head)), tail(std::move(other.tail)), size(other.size){
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }

    // Перемещающий оператор присваивания
    DoubleLinkedList& operator=(DoubleLinkedList&& other) noexcept {
        if (this != &other) {
        // Освобождение текущих ресурсов
        while (head != nullptr) {
            Node* next = head->next;
            delete head;
            head = next;
        }

        head = other.head;
        tail = other.tail;
        size = other.size;

        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
        }
        return *this;
    }
    
    // Добавление элемента в конец
    void push_back(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++size;
    }

    // Добавление элемента в начало
    void push_front(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++size;
    }

    // Добавление элемента по индексу
    void insert(int index, int value) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Индекс вне допустимого диапазона");
        }
        if (index == 0) {
            push_front(value);
            return;
        } else if (index == size) {
            push_back(value);
            return;
        }

        Node* newNode = new Node(value);
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }

        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;

        ++size;
    }

    // Удаление элемента по индексу
    void erase(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Индекс вне допустимого диапазона");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }

        if (current->prev) {
            current->prev->next = current->next;
        } else {
            head = current->next; // Удаление головного элемента
        }

        if (current->next) {
            current->next->prev = current->prev;
        } else {
            tail = current->prev; // Удаление хвостового элемента
        }

        delete current;
        --size;
    }

    // Получение размера контейнера
    int getSize() const {
        return size;
    }

    // Вывод содержимого контейнера
    void print() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // Оператор [] для доступа к элементам по индексу
    int& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Индекс вне диапазона");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->value;
    }
    // Структура итератора для DoubleLinkedList
    struct Iterator {
        Node* ptr;

        // Конструктор
        Iterator(Node* ptr) : ptr(ptr) {}

        // Оператор разыменования
        int operator*() {
            if (ptr == nullptr) {
                throw std::out_of_range("Индекс вне диапазона");
            }
            return ptr->value;
        }

        // Оператор сравнения (для проверки конца итерации)
        bool operator!=(const Iterator& other) {
            return ptr != other.ptr;
        }

        // Перемещение итератора на следующий элемент
        Iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        int& get() {
            if (ptr == nullptr) {
                throw std::out_of_range("Индекс вне диапазона");
            }
            return ptr->value;
        }
    };

    // Возвращает итератор на начало контейнера
    Iterator begin() {
        return Iterator(head);
    }

    // Возвращает итератор на конец контейнера
    Iterator end() {
        return Iterator(nullptr);
    }
};

// Однонаправленный списковый контейнер
class SinglyLinkedList {
private:
    struct Node {
        int value;
        Node* next;

        Node(int value) : value(value), next(nullptr) {}
    };

    Node* head;
    int size;

public:
    // Конструктор
    SinglyLinkedList() : head(nullptr), size(0) {}

    // Деструктор
    ~SinglyLinkedList() {
        while (head != nullptr) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }

    // Перемещающий конструктор
    SinglyLinkedList(SinglyLinkedList&& other) noexcept:  head(other.head), size(other.size) {
        other.head = nullptr;
        other.size = 0;
    }

    // Перемещающий оператор присваивания
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
        if (this != &other) {
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            head = other.head;
            size = other.size;

            other.head = nullptr;
            other.size = 0;
        }
        return *this;
    }    

    // Добавление элемента в конец
    void push_back(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        ++size;
    }

    // Добавление элемента в начало
    void push_front(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        ++size;
    }


    // Метод для вставки элемента по индексу
    void insert(int index, int value) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Индекс вне допустимого диапазона");
        }
        if (index == 0) {
            push_front(value);
            return;
        }
        if (index == size) {
            push_back(value);
            return;
        }

        Node* newNode = new Node(value);
        Node* current = head;
        for (int i = 0; i < index - 1; ++i) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        ++size;
    }

    // Удаление элемента по индексу
    void erase(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Индекс вне допустимого диапазона");
        }
        Node* current = head;
        if (index == 0) {
            head = current->next;
            delete current;
        } else {
            Node* prev = nullptr;
            for (int i = 0; i < index; ++i) {
                prev = current;
                current = current->next;
            }
            prev->next = current->next;
            delete current;
        }
        --size;
    }

    // Получение размера контейнера
    int getSize() const {
        return size;
    }

    // Вывод содержимого контейнера
    void print() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // Оператор [] для доступа к элементам по индексу
    int& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Индекс вне диапазона");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->value;
    }

    // Структура итератора для SinglyLinkedList
    struct Iterator {
        Node* ptr;

        // Конструктор
        Iterator(Node* ptr) : ptr(ptr) {}

        // Оператор разыменования
        int operator*() {
            if (ptr == nullptr) {
                throw std::out_of_range("Индекс вне диапазона");
            }            
            return ptr->value;
        }

        // Оператор сравнения (для проверки конца итерации)
        bool operator!=(const Iterator& other) {
            return ptr != other.ptr;
        }

        // Перемещение итератора на следующий элемент
        Iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        int& get() {
            if (ptr == nullptr) {
                throw std::out_of_range("Индекс вне диапазона");
            }
            return ptr->value;
        }
    };
    // Возвращает итератор на начало контейнера
    Iterator begin() {
        return Iterator(head);
    }

    // Возвращает итератор на конец контейнера
    Iterator end() {
        return Iterator(nullptr);
    }
};

template <typename T>
// Функция для тестирования контейнеров
void testContainer(const std::string& name, T& container) {
    std::cout << name << ":" << std::endl;

    // Добавление элементов
    for (int i = 0; i < 10; ++i) {
        container.push_back(i);
    }

    // Вывод содержимого
    std::cout << "Содержимое: ";
    container.print();

    // Вывод размера
    std::cout << "Размер: " << container.getSize() << std::endl;

    // Удаление элементов
    container.erase(2); // Удаление третьего элемента
    container.erase(3); // Удаление пятого элемента
    container.erase(4); // Удаление седьмого элемента

    // Вывод содержимого после удаления
    std::cout << "Содержимое после удаления: ";
    container.print();

    // Добавление элемента в начало
    container.push_front(10);

    // Вывод содержимого после добавления в начало
    std::cout << "Содержимое после добавления элемента в начало: ";
    container.print();

    // Вставка в середину
    container.insert(container.getSize() / 2, 20); // Вставка в середину

    // Вывод содержимого после добавления в середину
    std::cout << "Содержимое после добавления элемента в середину: ";
    container.print();

    // Добавление элемента в конец
    container.push_back(30);

    // Вывод содержимого после добавления в конец
    std::cout << "Содержимое после добавления элемента в конец: ";
    container.print();

    std::cout << std::endl;
}
template <typename Type>
void demonstrateMoveSemantics(const std::string& name) {
    std::cout << "Демонстрация семантики перемещения для " << name << ":\n";
    Type conte;
    conte.push_back(0);
    conte.push_back(1);
    conte.push_back(0);
    conte.push_back(1);
    std::cout << "conte не пуст: ";
    conte.print();
    std::cout << std::endl;

    Type moved_container = std::move(conte); // Перемещение контейнера
    moved_container.print();
    std::cout << std::endl;

    std::cout << "Содержимое conte после перемещения: ";
    conte.print(); // Проверяем содержимое первого контейнера (должен быть пустым)
    std::cout << std::endl;

    if (conte.getSize() == 0) {
        std::cout << "conte успешно перемещен и теперь пуст." << std::endl;
    } else {
        std::cout << "Ошибка: conte не пуст!" << std::endl;
    }
    std::cout << std::endl;

    Type movements_conteiner;
    movements_conteiner.push_back(10);
    movements_conteiner.push_back(11);
    movements_conteiner.push_back(2);
    std::cout << "movements_conteiner не пуст: ";
    movements_conteiner.print();
    std::cout << std::endl;

    movements_conteiner = std::move(moved_container); // Перемещение контейнера
    movements_conteiner.print();
    std::cout << std::endl;

    std::cout << "Содержимое moved_container после перемещения: ";
    moved_container.print(); // Проверяем содержимое первого контейнера (должен быть пустым)
    std::cout << std::endl;

    if (moved_container.getSize() == 0) {
        std::cout << "moved_container успешно перемещен и теперь пуст." << std::endl;
    } else {
        std::cout << "Ошибка: moved_container не пуст!" << std::endl;
    }
    std::cout << std::endl;

}

template <typename Type>
void demonstrateIterator(const std::string& name) 
{
    std::cout << "Демонстрация Iterator для " << name << ":\n";
    Type conteiner;
    // Добавление элементов
    for (int i = 0; i < 10; ++i) {
        conteiner.push_back(i);
    }

    std::cout << "Содержимое контейнера, выведенное с помощью итератора: ";
    for (auto iter = conteiner.begin(); iter != conteiner.end(); ++iter) {
        // std::cout << *iter << " ";
        std::cout << iter.get()<< " ";
    }
    std::cout << std::endl;    
    

}
int main() {
    // system("chcp 866"); 
    // Создание объектов контейнеров
    SequentialContainer vec;
    DoubleLinkedList  Double_lst;
    SinglyLinkedList  Singl_lst;

    // Вызов функции тестирования для каждого контейнера
    testContainer("SequentialContainer", vec);
    testContainer("DoubleLinkedList", Double_lst);
    testContainer("SinglyLinkedList", Singl_lst);

    demonstrateMoveSemantics<SequentialContainer>("SequentialContainer");
    demonstrateMoveSemantics<DoubleLinkedList>("DoubleLinkedList");
    demonstrateMoveSemantics<SinglyLinkedList>("SinglyLinkedList");

    demonstrateIterator<SequentialContainer>("SequentialContainer");
    demonstrateIterator<DoubleLinkedList>("DoubleLinkedList");
    demonstrateIterator<SinglyLinkedList>("SinglyLinkedList");
    return 0;
}