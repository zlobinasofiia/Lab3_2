#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h> //Локалізація консолі Windows

//Досліджувана функція f(x)
double f(double x){
    return 0.4 * pow(x - 100, 3) + 0.3 * pow(x, 2) - 0.2 * x +200;
}

//Чисельне знаходження перщої похідної f`(x)
double df(double x) {
    double h = 1e-6;
    return (f(x+h) - f(x - h))/(2*h);
}

//Метод половинного ділення
void solve_bisection(double a, double b, double eps) {
    printf("\n=== МЕТОД ПОЛОВИННОГО ДІЛЕННЯ ===\n");

    if (f(a) * f(b) >= 0) {
        printf("Помилка: На кінцях відрізка [a, b] функція має однакові знаки!\n");
        return;
    }

    int iter = 0;
    double c = a;

    while (1){
        iter++;
        c = (a + b)/ 2.0;
        double fc = f(c);

        if (fabs(b - a)/ 2.0 < eps || fabs(fc) < eps){
            break;
        }

        if (f(a) * fc < 0){
            b = c;
        } else {
            a = c;}
    }

    printf("\n--- РЕЗУЛЬТАТ (Метод половинного ділення) ---\n");
    printf("Знайдений корінь x     = %.8lf\n", c);
    printf("Значення функції f(x)  = %.8e\n", f(c));
    printf("Значення похідної f'(x)= %.8lf\n", df(c));
    printf("Кількість ітерацій    = %d\n", iter);
}

//Метод хорд
void solve_chord(double a, double b, double eps) {
    printf("\n=== МЕТОД ХОРД ===\n");

    if (f(a) * f(b) >= 0){
        printf("Помилка: На кінцях відрізка [a, b] функція має однакові знаки!\n");
        return;
    }

    int iter = 0;
    double x_curr;
    double x_prev;

    int case_type = (f(a) > 0) ? 1 : 2;
    x_curr = (case_type == 1) ? b : a;

    while (1){
        iter++;
        x_prev = x_curr;

        if (case_type == 1){
            x_curr = x_prev - (f(x_prev) * (x_prev - a))/(f(x_prev)-f(a));
        } else {
            x_curr = x_prev - (f(x_prev) * (b - x_prev))/(f(b) - f(x_prev));
            }
        double fx = f(x_curr);

        if (fabs(x_curr - x_prev) < eps || fabs(fx) < eps){
            break;
        }
    }

    printf("\n--- РЕЗУЛЬТАТ (Метод хорд) ---\n");
    printf("Знайдений корінь x     = %.8lf\n", x_curr);
    printf("Значення функції f(x)  = %.8e\n", f(x_curr));
    printf("Значення похідної f'(x)= %.8lf\n", df(x_curr));
    printf("Кількість ітерацій    = %d\n", iter);

}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    double a, b, eps;
    int max_iter, debug_mode, choice;

    printf("=========================================================\n");
    printf(" ЛАБОРАТОРНА РОБОТА №3 (Частина 2). Розв'язання нелінійних рівнянь\n");
    printf("=========================================================\n\n");

    printf("Уведіть ліву межу проміжку ізоляції (a): ");
    scanf("%lf", &a);
    printf("Уведіть праву межу проміжку ізоляції (b): ");
    scanf("%lf", &b);
    printf("Уведіть похибку обчислення (eps, наприклад 0.0001): ");
    scanf("%lf", &eps);
    printf("Уведіть максимальну кількість ітерацій: ");
    scanf("%d", &max_iter);
    printf("Увімкнути режим налагодження (вивід кожної ітерації)? (1 - Так, 0 - Ні): ");
    scanf("%d", &debug_mode);

    do {
        printf("\n================ ТЕКСТОВЕ МЕНЮ ================\n");
        printf("1 - Розв'язати методом половинного ділення\n");
        printf("2 - Розв'язати методом хорд\n");
        printf("3 - Розв'язати обома методами та порівняти\n");
        printf("0 - Вихід з програми\n");
        printf("Ваш вибір: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                solve_bisection(a, b, eps);
                break;
            case 2:
                solve_chord(a, b, eps);
                break;
            case 3:
                solve_bisection(a, b, eps);
                solve_chord(a, b, eps);
                break;
            case 0:
                printf("\nПрограма завершила роботу.\n");
                break;
            default:
                printf("\nНекоректний вибір! Спробуйте ще раз.\n");
        }
    } while (choice != 0);

    return 0;
}
