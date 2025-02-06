#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <stdlib.h>

const double precision = 0.0001;
const double left = -40000;
const double right = 40000;

std::vector<double> cardano_method(std::vector<double> polynom) {
    if (polynom.size() != 4) {
        throw "roots_of_poly3: invalid input";
    }

    // Приведение к виду уравнения
    double first_coef = polynom[0];
    for (int i = 0; i < polynom.size(); i++) {
        polynom[i] /= first_coef;
    }

    // Преобразование коэффициентов
    double A = polynom[1];

    double p = polynom[2] - pow(A, 2) / 3;
    double q = polynom[3] + 2 * pow(A, 3) / 27 - A * polynom[2] / 3;

    polynom[1] = 0;
    polynom[2] = p;
    polynom[3] = q;

    std::vector<double> ans;

    // Дискриминант кубического уравнения
    double D = std::pow(q / 2, 2) + std::pow(p / 3, 3);

    if (D > 0) {
        double z1 = -q / 2 + sqrt(D), z2 = -q / 2 - sqrt(D);

        ans.push_back(cbrt(z1) + cbrt(z2));
    }
    else if (D == 0) {
        double y1 = 2 * cbrt((-q / 2)), y2 = -cbrt((-q / 2));

        ans.push_back(y1);
        if (y1 != y2) ans.push_back(y2);
    }
    else {
        double omega;
        if (q < 0) {
            omega = atan(sqrt(-D) / (-q / 2));
        }
        else if (q > 0) {
            omega = atan(sqrt(-D) / (-q / 2)) + M_PI;
        }
        else {
            omega = M_PI_2;
        }

        double y1 = 2 * sqrt(-p / 3) * cos(omega / 3);
        double y2 = 2 * sqrt(-p / 3) * cos(omega / 3 + (2 * M_PI / 3));
        double y3 = 2 * sqrt(-p / 3) * cos(omega / 3 + (4 * M_PI / 3));

        ans.push_back(y1);
        ans.push_back(y2);
        ans.push_back(y3);
    }

    // Коррекция на начальное смещение
    for (int i = 0; i < ans.size(); i++) {
        ans[i] -= A / 3;
    }

    return ans;
}


std::vector<double> solve_2(std::vector<double> coef) {
    if (coef.size() != 3) {
        throw "Ошибка: уравнение не второй степени";
    }

    std::vector<double> ans;

    double D = coef[1] * coef[1] - 4 * coef[0] * coef[2];

    if (D < 0) {
        return ans;
    }

    if (coef[0] == 0) {
        ans.push_back(-coef[2] / coef[1]);
        return ans;
    }

    double x1 = (-coef[1] - std::sqrt(D)) / (2 * coef[0]);
    double x2 = (-coef[1] + std::sqrt(D)) / (2 * coef[0]);

    ans.push_back(x1);
    if (x1 != x2) ans.push_back(x2);

    return ans;
}

std::vector<double> df(std::vector<double> coef) {
    int n = coef.size() - 1;
    std::vector<double> ans(n);

    for (int i = 0; i < ans.size(); i++) {
        ans[i] = n * coef[i];
        n--;
    }

    return ans;
}

double fx(std::vector<double> coef, double x) {
    double ans = 0;
    int n = coef.size() - 1;

    for (int i = 0; i < coef.size(); i++) {
        ans += coef[i] * std::pow(x, n);
        n--;
    }

    return ans;
}

std::vector<double> solve(std::vector<double> coef, double tool, double left, double right) {
    if (coef[0] == 0) {
        throw "Ошибка: старший коэффициент не может быть 0";
    }

    std::vector<double> ans;
    std::vector<double> df1 = df(coef);
    std::vector<double> df2 = df(df1);

    std::vector<double> roots_of_df1;

    if (df1.size() == 4) {
        try {
            roots_of_df1 = solve(df1, tool, left, right);
        }
        catch (const char* err) {
            std::cout << err << std::endl;
            return ans;
        }
    }
    else {
        roots_of_df1 = solve_2(df1);
    }

    if (roots_of_df1.size() == 0) {
        std::cout << "На интервале [" << left << "," << right << "] нет значимых корней. ";
        std::cout << "Попробуйте другие границы интервала" << std::endl;
        return ans;
    }

    std::sort(roots_of_df1.begin(), roots_of_df1.end());

    std::vector<bool> sign(roots_of_df1.size() + 2);
    sign[0] = fx(coef, left) > 0;
    sign.back() = fx(coef, right) > 0;

    for (int i = 0; i < roots_of_df1.size(); i++) {
        double val = fx(coef, roots_of_df1[i]);
        if (val == 0) {
            ans.push_back(roots_of_df1[i]);
            roots_of_df1.erase(roots_of_df1.begin() + i);
            sign.erase(sign.begin() + i);
            i--;
            continue;
        }
        sign[i + 1] = val > 0;
    }

    std::vector<std::pair<double, double>> borders;
    for (int i = 1; i < sign.size(); i++) {
        if (sign[i - 1] != sign[i]) {
            if (i == 1) {
                borders.push_back(std::make_pair(left, roots_of_df1[0]));
            }
            else if (i == sign.size() - 1) {
                borders.push_back(std::make_pair(roots_of_df1.back(), right));
            }
            else {
                borders.push_back(std::make_pair(roots_of_df1[i - 2], roots_of_df1[i - 1]));
            }
        }
    }

    for (int i = 0; i < borders.size(); i++) {
        double a = borders[i].first, b = borders[i].second;
        double dif = b - a;
        double x;

        while (dif > tool) {
            x = (a + b) / 2;

            double df1_value, df2_value;
            df1_value = fx(df1, x);
            df2_value = fx(df2, x);

            double newA, newB;

            if (df1_value * df2_value > 0) {
                newA = a - (b - a) / (fx(coef, b) - fx(coef, a)) * fx(coef, a);
                newB = b - (fx(coef, b) / fx(df1, b));
            }
            else {
                newA = a - (fx(coef, a) / fx(df1, a));
                newB = b - (b - a) / (fx(coef, b) - fx(coef, a)) * fx(coef, b);
            }

            a = newA;
            b = newB;
            dif = b - a;
        }
        ans.push_back(x);
    }
    return ans;
}

int main() {
    srand(time(NULL));
    std::vector<double> polynom(5);
    int count = 0;

    std::cout << "Многочлен:" << std::endl;
    for (int i = 0; i < 5; i++) {
        polynom[i] = -10 + rand() % 21;
        if (polynom[i] == 0) count++;
        std::cout << polynom[i] << "x^" << 5 - i - 1 << " ";
    }
    std::cout << std::endl;

    std::vector<double> roots;
    try {
        roots = solve(polynom, precision, left, right);
    }
    catch (const char* err) {
        std::cout << err << std::endl;
        return -1;
    }
    std::cout << "Найдены корни уравнения:" << std::endl;
    for (int i = 0; i < roots.size(); i++) {
        std::cout << roots[i] << " ";
    }
    if (count == 5) {
        std::cout << "x принадлежит R" << std::endl;
    }
    else if (roots.size() == 0) {
        std::cout << "Корни не найдены" << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
