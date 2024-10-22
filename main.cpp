#include <iostream>
#include <map>
#include <memory>
#include <cstddef>

// Шаблонный класс-аллокатор, реализующий выделение памяти блоками заданного размера
// T - тип данных, для которых используется аллокатор
// BlockSize - размер блока (по умолчанию 10)
template <class T, size_t BlockSize = 10>
struct allocator_с11 {// Тип данных, который может хранить аллокатор
    using value_type = T;
    // Указатель на пул памяти (начало блока)
    void* pool;

    // Конструктор аллокатора
    // Инициализирует пул памяти, выделяя блок размера BlockSize * sizeof(T)
    // current_block_size - текущий размер блока
    // allocated_elements - количество выделенных элементов
    // block - указатель на начало блока
    allocator_с11 () : current_block_size(BlockSize), allocated_elements(0),
        block(static_cast<T*>(std::malloc(BlockSize * sizeof(T)))) {
        if (!block)
            throw std::bad_alloc();  // Если выделение памяти не удалось, выбрасываем исключение std::bad_alloc
    }
    
    // Деструктор аллокатора
    // Освобождает память, выделенную для пула
    ~allocator_с11() {
        std::free(block);
    }
    
    // Конструктор копирования (запрещен, так как аллокатор не должен копироваться)
    template <class U> allocator_с11 (const allocator_с11<U>&) noexcept {}

    // Выделение памяти
    // Выделяет n элементов типа T 
    // Возвращает указатель на выделенную память (использует ::operator new)
    T* allocate (std::size_t n) {
        return static_cast<T*>(::operator new(n*sizeof(T)));
    }
    

    // Освобождение памяти
    // Освобождает память, выделенную по указателю p (использует ::operator delete)
    void deallocate (T* p, std::size_t n) { ::operator delete(p); }

    // Метод rebind, который позволяет создавать аллокатор для другого типа данных
    template< class U >
    struct rebind {
        typedef allocator_с11<U> other;
    };
    
    private:
        size_t current_block_size; //текущий размер блока
        size_t allocated_elements; //количество выделенных элементов
        T * block; //указатель на начало блока
};

// Оператор == для сравнения аллокаторов (всегда возвращает true)
template <class T, class U>
constexpr bool operator== (const allocator_с11<T>& a1, const allocator_с11<U>& a2) noexcept {
    return true;
}

// Оператор != для сравнения аллокаторов (всегда возвращает false)
template <class T, class U>
constexpr bool operator!= (const allocator_с11<T>& a1, const allocator_с11<U>& a2) noexcept {
    return false;
}


// T - тип данных, которые хранит контейнер
// MaxSize - максимальное количество элементов в контейнере
// Allocator - тип аллокатора (по умолчанию allocator_с11<T>)
template <typename T, size_t MaxSize, typename Allocator = allocator_с11<T>>
class LimitedContainer {
private:
    Allocator alloc; // Экземпляр аллокатора
    T* data; // Указатель на данные (динамический массив)
    size_t size; // Текущий размер контейнера

public:
    // Конструктор по умолчанию
    // Инициализирует данные, размер и аллокатор
    LimitedContainer() : alloc(), data(nullptr), size(0) {}
    
    ~LimitedContainer() {
        if (data) {
            alloc.deallocate(data, size);
        }
    }

    // Добавляет элемент в конец контейнера
    // Если контейнер уже заполнен, выбрасывается исключение std::runtime_error
    
    void push_back(const T& value) { 
        // Проверка на заполненность контейнера
        if (size == MaxSize) { 
            throw std::runtime_error("Контейнер уже заполнен"); 
        } 
        // Если контейнер пустой, выделяем память для данных
        if (size == 0) { 
            data = alloc.allocate(MaxSize); 
        } 
        // Используем placement new для создания нового элемента типа T 
        new (&data[size]) T(value); 
        // Увеличиваем размер контейнера
        ++size; 
    }

    // Возвращает ссылку на элемент по заданному индексу
    // Если индекс недействителен, выбрасывается исключение std::out_of_range
    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Индекс вне допустимого диапазона");
        }
        return data[index];
    }

    // Возвращает текущий размер контейнера
    size_t getSize() const {
        return size;
    }

    // Проверяет, пуст ли контейнер
    bool empty() const {
        return size == 0;
    }
};

// Класс для спискового контейнера (связь через указатели)
// Двусвязный список, где каждый элемент хранит ссылку на предыдущий и следующий
template <typename T, typename Allocator = allocator_с11<T>>
class DoubleLinkedList {
private:
    struct Node {
        T value;
        Node* next;
        Node* prev;

        Node(const T& value) : value(value), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t size;
    Allocator alloc;

public:
    // Инициализирует данные, размер и аллокатор
    DoubleLinkedList() : head(nullptr), tail(nullptr), size(0), alloc() {}
   
    // Деструктор
    ~DoubleLinkedList() { clear(); }

    // Добавление элемента в конец
    void push_back(const T& value) {
        // Node* newNode = static_cast<Node*>(alloc.allocate(sizeof(Node))); 
        // new (newNode) Node(value);
        typename Allocator::template rebind<Node>::other nodeAlloc;
        Node* newNode = nodeAlloc.allocate(1);
        new(newNode) Node(value); // Используем placement new для инициализации узла

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

    // Добавление элемента по индексу
    void insert(size_t index, const T& value) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Индекс вне допустимого диапазона");
        }
        if (index == size) {
            push_back(value);
            return;
        }
        
        Node* newNode = static_cast<Node*>(alloc.allocate(sizeof(Node))); 
        new (newNode) Node(value);

        Node* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }

        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;

        ++size;
    }

    // Удаление элемента по индексу
    void erase(size_t index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Индекс вне допустимого диапазона");
        }
        Node* current = head;
        for (size_t i = 0; i < index; ++i) {
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

        // alloc.destroy(current);
        alloc.deallocate(current, 1);
        --size;
    }

    // Получение размера контейнера
    size_t getSize() const {
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
    T& operator[](size_t index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Индекс вне диапазона");
        }
        Node* current = head;
        for (size_t i = 0; i < index; ++i) {
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
        T& operator*() {
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

        // Конструктор перемещения
        Iterator(Iterator&& other) noexcept : ptr(std::move(other.ptr)) {
            other.ptr = nullptr;
        }

        // Оператор присваивания перемещения
        Iterator& operator=(Iterator&& other) noexcept {
            if (this != &other) {
                ptr = std::move(other.ptr);
                // alloc = std::move(other.alloc);
                other.ptr = nullptr;
            }
            return *this;
        }

        T& get() {
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

    // Очистка контейнера
    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    // Проверка на пустоту
    bool empty() const {
        return size == 0;
    }
};

// Функция для вычисления факториала
int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

int main() {
    
    // 1) Создание экземпляра std::map<int, int>
    std::map<int, int> map1;

    // 2) Заполнение 10 элементами
    for (int i = 0; i < 10; ++i) {
        map1[i] = factorial(i);
    }

    // 3) Создание экземпляра std::map<int, int> с новым аллокатором
    std::map<int, int, std::less<int>, allocator_с11<std::pair<const int, int>, 10>> map2;

    // 4) Заполнение 10 элементами
    for (int i = 0; i < 10; ++i) {
            map2[i] = factorial(i);
    }

    // 5) Вывод на экран всех значений
    std::cout << "Map1: ";
    for (const auto& [key, value] : map1) {
        std::cout << key << " " << value << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Map2: ";
    // вывод на экран всех значений хранящихся в контейнере
    for (const auto& pair : map2) {
        std::cout << pair.first << " " << pair.second << std::endl;
    }
    std::cout << std::endl;

    
    // 3) Создание экземпляра LimitedContainer с новым аллокатором, ограниченным 10 элементами
    LimitedContainer<std::pair<const int, int>, 10, allocator_с11<std::pair<const int, int>, 10>> limited_map; 

    // 4) Заполнение 10 элементами
    for (int i = 0; i < 10; ++i) {
        limited_map.push_back(std::make_pair(i, factorial(i)));
    }

    
    std::cout << "LimitedContainer: ";
    for (size_t i = 0; i < limited_map.getSize(); ++i) {
        std::cout << limited_map[i].first << " " << limited_map[i].second << std::endl;
    }
    std::cout << std::endl;

    DoubleLinkedList<int> list1;
    // Добавляем элементы в список
    for(int i = 0; i < 10; ++i)
    {
        list1.push_back(i);
    }
    // Выводим элементы списка
   
    std::cout << "List1: ";
    for (const auto& value : list1) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // Создаем список с использованием `allocator_с11`
    DoubleLinkedList<int,allocator_с11<int>> list;

    // Добавляем элементы в список
    for(int i = 0; i < 10; ++i)
    {
        list.push_back(i);
    }
    std::cout << "List2: ";
    // Выводим элементы списка
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}

