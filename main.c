#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double f1(double x) {
    return x * x * x;
}

double f2(double x) {
    return sqrt(1 - pow((1 / (1 + exp(-x))), 4));
}

double f3(double x) {
    return sqrt(1 - sin(x + pow(x, 2) + pow(x, 3) + pow(x, 4) + pow(x, 5) + pow(x, 6) + pow(x, 7)));
}

double bench_t_start, bench_t_end;

static double rtclock() {
    struct timeval Tp;
    int stat = gettimeofday(&Tp, NULL);
    if (stat) {
        printf("Error");
    }
    return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}

void bench_timer_start() {
    bench_t_start = rtclock();
}
void bench_timer_end() {
    bench_t_end = rtclock();
}
void bench_timer_print() {
    printf("%lf\n", bench_t_end - bench_t_start);
}

double integral(double (*f)(double), 
    const double a, const double b, const long long n) {
    double h = (b - a) / (2 * n);
    double sum = 0.0;
    long long N = 2 * n;
    double sum1 = 0.0;
    double sum2 = 0.0;
    #pragma omp parallel for reduction(+:sum1) reduction(+:sum2)
    for (long long j = 1; j < N; j += 2) {
        sum1 += f(a + j * h);
        sum2 += f(a + (j + 1) * h);
    }
    sum += f(a) - f(b) + 4 * sum1 + 2 * sum2;
    return sum * h / 3;
}

double integral_with_check(double (*f)(double), const double a, const double b,         
                const long long n) {
    double h = (b - a) / (2 * n);
    double sum = 0.0;
    long long N = 2 * n;

    sum += f(a);
    for (long long j = 1; j <= N - 1; ++j) {
        if (j % 2 == 1) {
            sum += 4 * f(a + j * h);
        } else {
            sum += 2 * f(a + j * h);
        }
    }
    sum += f(b);
    return sum * h / 3;
}

double integral_base(double (*f)(double), const double a, const double b,         
                const long long n) {
    double h = (b - a) / (2 * n);
    double sum = 0.0;
    long long N = 2 * n;
    for (long long k = 1; k <= N - 1; k += 2) {
        double x_k = a + k * h;
        sum += f(x_k - h)  + 4 * f(x_k)+ f(x_k + h);
	}
    return sum * h / 3;
}


double integral_single(double (*f)(double), const double a, const double b,         
                const long long n) {
    double h = (b - a) / (2 * n);
    double sum = 0.0;
    long long N = 2 * n;

    double sum1 = 0.0;
    double sum2 = 0.0;
    for (long long j = 1; j < N; j += 2) {
        sum1 += f(a + j * h);
        sum2 += f(a + (j + 1) * h);
    }
    sum += f(a) - f(b) + 4 * sum1 + 2 * sum2;
    return sum * h / 3;
}


int main(int argc, char **argv) {
    int mode;
    double a, b, n, result;
    sscanf(argv[1], "%d", &mode);
    sscanf(argv[2], "%lf", &a);
    sscanf(argv[3], "%lf", &b);
    sscanf(argv[4], "%lf", &n);
    if (mode == 1) {
        bench_timer_start();
        result = integral(f1, a, b, n);
        bench_timer_end();
    } else if (mode == 2) {
        bench_timer_start();
        result = integral(f2, a, b, n);
        bench_timer_end();
    } else if (mode == 3) {
        bench_timer_start();
        result = integral(f3, a, b, n);
        bench_timer_end();
    } else if (mode == 4) {
        bench_timer_start();
        result = integral_base(f3, a, b, n);
        bench_timer_end();
    } else if (mode == 5) {
        bench_timer_start();
        result = integral_single(f3, a, b, n);
        bench_timer_end();
    } else if (mode == 6) {
    	bench_timer_start();
        result = integral_with_check(f3, a, b, n);
        bench_timer_end();
    }
    //printf("%lf\n", result);
    bench_timer_print();
    return result;
}