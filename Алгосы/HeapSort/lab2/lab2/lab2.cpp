#include <ctime>
#include <iostream>
#include <cstdlib>

// Функция Heapify для поддержания свойства кучи
void Heap(int a[], int size, int i) {
    while (true) {
        int largest = i; // Инициализируем наибольший элемент как корень
        int left = 2 * i + 1; // Левый потомок
        int right = 2 * i + 2; // Правый потомок

        // Если левый потомок больше корня
        if (left < size && a[left] > a[largest]) {
            largest = left;
        }

        // Если правый потомок больше, чем самый большой элемент на данный момент
        if (right < size && a[right] > a[largest]) {
            largest = right;
        }

        // Если самый большой элемент – корень, то выход из цикла
        if (largest == i) {
            break;
        }

        // Меняем местами корень и самый большой элемент
        int temp = a[i];
        a[i] = a[largest];
        a[largest] = temp;

        // Рекурсивно Heapify поддерево
        i = largest;
    }
}

// Функция пирамидальной сортировки
void Sort(int a[], int n) {
    // Построение кучи (перегруппируем массив)
    for (int i = n / 2 - 1; i >= 0; i--) {
        Heap(a, n, i);
    }

    // Один за другим извлекаем элементы из кучи
    for (int i = n - 1; i > 0; i--) {
        // Перемещаем текущий корень в конец
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;

        // Вызываем Heapify на уменьшенной куче
        Heap(a, i, 0);
    }
}

// Функция для заполнения массива случайными числами
void Randomizer(int a[], int size) {
    std::srand(std::time(nullptr)); // Инициализируем генератор случайных чисел

    for (int i = 0; i < size; i++) {
        a[i] = std::rand() % 5000 + 1000; // Генерируем числа от 1000 до 5999
    }
    std::cout << "Source array:\n";
    for (int i = 0; i < size; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << "\n";
}

// Главная функция
int main() {
    int size = 100; // Размер массива
    int* a = new int[size];

    Randomizer(a, size); // Заполняем массив случайными числами

    Sort(a, size); // Сортируем массив

    std::cout << "\nSorted array:\n";
    for (int i = 0; i < size; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
    delete [] a;
    return 0;
}
