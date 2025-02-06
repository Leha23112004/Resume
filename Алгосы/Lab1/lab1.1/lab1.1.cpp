#include <iostream>
#include <cstdlib>  // Для использования rand() и srand()
#include <ctime>    // Для использования time() в srand()

// Функция возведения в степень через while
double pow(double base, int exponent) {
    if (exponent == 0) return 1;  // Любое число в степени 0 равно 1
    double result = 1;
    while (exponent > 0) {
        result *= base;
        --exponent;
    }
    return result;
}

// Функция для вычисления абсолютного значения
double fabs(double value) {
    return (value < 0) ? -value : value;
}

// Основная функция уравнения четвертого порядка ax^4 + bx^3 + cx^2 + dx + e
double f(double x, double a, double b, double c, double d, double e) {
    return a * pow(x, 4) + b * pow(x, 3) + c * pow(x, 2) + d * x + e;
}

// Первая производная функции f(x)
double df(double x, double a, double b, double c, double d) {
    return 4 * a * pow(x, 3) + 3 * b * pow(x, 2) + 2 * c * x + d;
}

// Вторая производная функции f(x)
double ddf(double x, double a, double b, double c) {
    return 12 * a * pow(x, 2) + 6 * b * x + 2 * c;
}

// Функция для определения изменения знака между двумя точками
bool sign_change(double x0, double x1) {
    return x0 * x1 < 0;
}

// Комбинированный метод хорд и касательных
double combined_method(double a, double b, double c, double d, double e, double x0, double x1, double tol, int max_iter) {
    int iter = 0;
    double x2 = x1;

    while (fabs(x1 - x0) > tol && iter < max_iter) {
        double f_x0 = f(x0, a, b, c, d, e);
        double f_x1 = f(x1, a, b, c, d, e);
        double df_x1 = df(x1, a, b, c, d);
        double ddf_x1 = ddf(x1, a, b, c);

        // Проверяем изменение знака между f(x0) и f(x1)
        if (sign_change(f_x0, f_x1)) {
            // Если вторая производная > 0, выбираем метод касательных (Ньютона)
            if (fabs(df_x1) < tol) {
                std::cout << "Производная слишком мала, метод не может продолжаться." << std::endl;
                break;
            }
            x2 = x1 - f_x1 / df_x1;
        }
        // Иначе выбираем метод хорд (секущих)
        else {
            if (fabs(f_x1 - f_x0) < tol) {
                std::cout << "Знаменатель слишком мал, метод не может продолжаться." << std::endl;
                break;
            }
            x2 = x1 - f_x1 * (x1 - x0) / (f_x1 - f_x0);
        }

        // Обновляем значения для следующей итерации
        x0 = x1;
        x1 = x2;
        iter++;
    }

    if (iter == max_iter) {
        std::cout << "Метод не сошелся за " << max_iter << " итераций." << std::endl;
    }

    return x2;
}

// Функция для уточнения корня с использованием формулы из изображения
double refine_root(double x_n, double x_n_bar) {
    return (x_n + x_n_bar) / 2;
}

int main() {
    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned>(time(0)));

    // Генерация случайных коэффициентов для уравнения
    double a = rand() % 51;  // Значение от 0 до 50
    double b = rand() % 51;  // Значение от 0 до 50
    double c = rand() % 51;  // Значение от 0 до 50
    double d = rand() % 51;  // Значение от 0 до 50
    double e = rand() % 51;  // Значение от 0 до 50

    std::cout << "Случайные коэффициенты уравнения: " << std::endl;
    std::cout << "a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << ", e = " << e << std::endl;

    // Генерация случайных начальных приближений для метода хорд и касательных
    double x0 = static_cast<double>(rand() % 100) / 10; // Начальная точка для метода хорд (с недостатком)
    double x1 = static_cast<double>(rand() % 100) / 10 + 1; // Начальная точка для метода Ньютона (с избытком)

    std::cout << "Случайные начальные приближения: " << std::endl;
    std::cout << "x0 = " << x0 << ", x1 = " << x1 << std::endl;

    // Параметры точности и максимального количества итераций
    double tol = 1e-6;
    int max_iter = 100;

    // Нахождение корня с использованием комбинированного метода
    double root_approx = combined_method(a, b, c, d, e, x0, x1, tol, max_iter);

    // Уточнение корня на основе формулы с недостатком и избытком
    double refined_root = refine_root(x0, x1);

    // Вывод результатов
    std::cout << "Приближенный корень: " << root_approx << std::endl;
    std::cout << "Уточненный корень: " << refined_root << std::endl;

    return 0;
}
