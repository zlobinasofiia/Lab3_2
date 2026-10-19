#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
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

// Функція обробки зупинки за кількістю ітерацій
// Повертає: 1 - продовжити, 2 - виконати до кінця, 0 - вийти з програми
int handle_max_iter_exceeded(int current_iter, double current_x, double current_fx){
    int choice;
    printf("\n---------------------------------------------------\n");
    printf("[ПОПЕРЕДЖЕННЯ] Досягнуто максимальну кількість ітерацій (%d)!\n", current_iter);
    printf("Поточне проміжне значення кореня x = %.8lf, f(x) = %.8e\n", current_x, current_fx);
    printf("Оберіть подальшу дію:\n");
    printf("  1 - Продовжити виконання ще на таку ж кількість ітерацій\n");
    printf("  2 - Виконати програму до кінця (без обмеження ітерацій)\n");
    printf("  0 - Вийти з програми з виводом проміжного результату\n");
    printf("Ваш вибір: ");
    scanf("%d", &choice);
    printf("---------------------------------------------------\n\n");
    return choice;
}

//Метод половинного ділення (дихотомії)
void solve_bisection(double a, double b, double eps, double max_iter, int debug_mode) {
    printf("\n=== МЕТОД ПОЛОВИННОГО ДІЛЕННЯ ===\n");

    if (f(a) * f(b) >= 0) {
        printf("Помилка: На кінцях відрізка [a, b] функція має однакові знаки!\n");
        return;
    }

    int iter = 0;
    int current_max_iter = max_iter;
    double c = a;
    clock_t start_time = clock();

    if (debug_mode){
        printf("\n%-5s | %-15s | %-15s | %-15s | %-15s\n", "Ітер", "a", "b", "c (корінь)", "f(c)");
        printf("-----------------------------------------------------------------------------\n");
    }

    while (1){
        iter++;
        c = (a + b)/ 2.0;
        double fc = f(c);

        if (debug_mode){
          printf("%-5d | %-15.8lf | %-15.8lf | %-15.8lf | %-15.8e\n", iter, a, b, c, fc);
        }

        //Перевірка умови зупинки за точністю
        if (fabs(b - a)/ 2.0 < eps || fabs(fc) < eps){
            break;
        }

        //Вибір нового інтервалу
        if (f(a) * fc < 0){
            b = c;
        } else {
            a = c;
        }

        //Перевірка перевищення максимальної кількості ітерацій
        if (current_max_iter > 0 && iter >= current_max_iter){
            int action = handle_max_iter_exceeded(iter, c, fc);
            if (action == 1){
                current_max_iter += max_iter; //Продовжити ще на N ітерацій
            } else if (action == 2){
                current_max_iter = 0; //До кінця
            } else {
                printf("\nОбчислення перервано користувачем.\n");
                break;
            }
        }
    }

    clock_t end_time = clock();
    double cpu_time_used = ((double)(end_time - start_time))/ CLOCKS_PER_SEC;

    printf("\n--- РЕЗУЛЬТАТ (Метод половинного ділення) ---\n");
    printf("Знайдений корінь x     = %.8lf\n", c);
    printf("Значення функції f(x)  = %.8e\n", f(c));
    printf("Значення похідної f'(x)= %.8lf\n", df(c));
    printf("Кількість ітерацій    = %d\n", iter);
    printf("--------------------------------------------\n");
}

//Метод хорд
void solve_chord(double a, double b, double eps, int max_iter, int debug_mode) {
    printf("\n=== МЕТОД ХОРД ===\n");

    if (f(a) * f(b) >= 0){
        printf("Помилка: На кінцях відрізка [a, b] функція має однакові знаки!\n");
        return;
    }

    int iter = 0;
    int current_max_iter = max_iter;
    double x_curr = a;
    double x_prev;

    //Визначення нерухомої точки за умовою
    int case_type = (f(a) > 0) ? 1 : 2;
    x_curr = (case_type == 1) ? b : a;

    clock_t start_time = clock();

    if (debug_mode){
        printf("\n%-5s | %-15s | %-15s\n", "Ітер", "x (наближення)", "f(x)");
        printf("-----------------------------------------------\n");
    }

    while (1){
        iter++;
        x_prev = x_curr;

        if (case_type == 1){
            x_curr = x_prev - (f(x_prev) * (x_prev - a))/(f(x_prev)-f(a));
        } else {
            x_curr = x_prev - (f(x_prev) * (b - x_prev))/(f(b) - f(x_prev));
            }
        double fx = f(x_curr);

        if (debug_mode == 1) {
            printf("%-5d | %-15.8lf | %-15.8e\n", iter, x_curr, fx);
        }

        //Критерій закінчення пошуку
        if (fabs(x_curr - x_prev) < eps || fabs(fx) < eps){
            break;
        }

        //Перевірка перевищення максимальної кількості ітерацій
        if (current_max_iter > 0 && iter >= current_max_iter){
            int action = handle_max_iter_exceeded(iter, x_curr, fx);
            if (action == 1){
                current_max_iter += max_iter;
            } else if (action == 2){
                current_max_iter = 0;
            } else {
                printf("\nОбчислення перервано користувачем.\n");
                break;
            }
        }
    }

    clock_t end_time = clock();
    double cpu_time_used = ((double)(end_time - start_time))/ CLOCKS_PER_SEC;

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

    // 1. Введення проміжку [a, b] з перевіркою a < b
    do {
        printf("Уведіть ліву межу проміжку ізоляції (a): ");
        scanf("%lf", &a);
        printf("Уведіть праву межу проміжку ізоляції (b): ");
        scanf("%lf", &b);
        if (a >= b) {
            printf("[ПОМИЛКА] Ліва межа (a) повинна бути МЕНШОЮ за праву межу (b)!\n\n");
        }
    } while (a >= b);

    // 2. Введення похибки з перевіркою (0 < eps < 1)
    do {
        printf("Уведіть похибку обчислення (eps, наприклад 0.0001): ");
        scanf("%lf", &eps);
        if (eps <= 0 || eps >= 1) {
            printf("[ПОМИЛКА] Похибка повинна бути дробовим числом від 0 до 1 (наприклад, 0.0001 або 0.001)!\n\n");
        }
    } while (eps <= 0 || eps >= 1);

    // 3. Введення максимальної кількості ітерацій
    do {
        printf("Уведіть максимальну кількість ітерацій (більше 0): ");
        scanf("%d", &max_iter);
        if (max_iter <= 0) {
            printf("[ПОМИЛКА] Кількість ітерацій має бути більше 0!\n\n");
        }
    } while (max_iter <= 0);

    // 4. Введення режиму налагодження (ТІЛЬКИ 1 або 0)
    do {
        printf("Увімкнути режим налагодження (вивід кожної ітерації)? (1 - Так, 0 - Ні): ");
        scanf("%d", &debug_mode);
        if (debug_mode != 0 && debug_mode != 1) {
            printf("[ПОМИЛКА] Введіть тільки 1 (Так) або 0 (Ні)!\n\n");
        }
    } while (debug_mode != 0 && debug_mode != 1);

    // Перевірка умови наявності та єдиності кореня
    printf("\n--- ПЕРЕВІРКА УМОВ НА ПРОМІЖКУ [%.2lf, %.2lf] ---\n", a, b);
    printf("f(a) = %.4lf, f(b) = %.4lf\n", f(a), f(b));
    if (f(a) * f(b) < 0) {
        printf("Знаки f(a) та f(b) протилежні (f(a)*f(b) < 0) -> корінь на проміжку ІСНУЄ.\n");
    } else {
        printf("УВАГА: f(a)*f(b) >= 0. На цьому проміжку може не бути кореня або їх декілька!\n");
    }

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
                solve_bisection(a, b, eps, max_iter, debug_mode);
                break;
            case 2:
                solve_chord(a, b, eps, max_iter, debug_mode);
                break;
            case 3:
                solve_bisection(a, b, eps, max_iter, debug_mode);
                solve_chord(a, b, eps, max_iter, debug_mode);
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
