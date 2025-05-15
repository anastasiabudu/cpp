#include <stdio.h>
#include <math.h>

double x_start, x_stop, step;

void funkcia(double x) {
    int b = 4;
    double bin, f2, f3, f4, reLU, result7, f7;

    if (x >= 0) {
        bin = 1;
    } else {
        bin = 0;
    }

    if (x <= 0) {
        reLU = 0;
    } else {
        reLU = x;
    }

    f2 = 1 / (1 + exp(-x));
    f3 = (exp(x) - exp(-x)) / (exp(x) + exp(-x));
    f4 = exp(-pow(x, 2));
    result7 = x + sqrt(pow(x, 2) + b);
    f7 = 0.5 * result7;

    printf("%10.2lf%10.2lf%10.2lf%10.2lf%10.2lf%10.2lf%10.2lf\n", x, bin, f2, f3, f4, reLU, f7);
}

void urcenie() {
    int rezim_fungovania;

    if (scanf("%d", &rezim_fungovania) != 1 || (rezim_fungovania != 0 && rezim_fungovania != 1)) {
        printf("E1\n");
        return;
    }

    if (rezim_fungovania == 0) {
        if (scanf("%lf %lf %lf", &x_start, &x_stop, &step) != 3 || isnan(x_start) || isnan(x_stop) || isnan(step)) {
            printf("E2\n");
            return;
        }

        else if (x_stop <= x_start) {
            printf("E3\n");
            return;
        }
        for (double x = x_start; x <= x_stop; x += step) {
            funkcia(x);
        }
    } else if (rezim_fungovania == 1) {
        double x;

        while (scanf("%lf", &x) == 1) {
            funkcia(x);
        }
    }
}


int main() {
    urcenie();
    return 0;
}
