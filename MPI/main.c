#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

double f1(double x) {
    return x * x * x;
}

double f2(double x) {
    return sqrt(1 - pow((1 / (1 + exp(-x))), 4));
}

double f3(double x) {
    return sqrt(1 - sin(x + pow(x, 2) + pow(x, 3) + pow(x, 4) + pow(x, 5) + pow(x, 6) + pow(x, 7)));
}

double integral(double (*f)(double), 
    const double a, const double b, const long long N, const double h) {
    double sum = 0.0;
    double sum1 = 0.0;
    double sum2 = 0.0;
    for (long long j = 1; j < N; j += 2) {
        sum1 += f(a + j * h);
        sum2 += f(a + (j + 1) * h);
    }
    sum += f(a) - f(b) + 4 * sum1 + 2 * sum2;
    return sum * h / 3;
}

int main(int argc, char** argv) {
    int rank, size, mode;
    long long n = 0;
    double a, b, result, total;

    MPI_Init(NULL, NULL);
    double start = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        sscanf(argv[1], "%d", &mode);
        sscanf(argv[2], "%lf", &a);
        sscanf(argv[3], "%lf", &b);
        sscanf(argv[4], "%lld", &n);
    }

    MPI_Bcast(&mode, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double h = (b - a) / (2 * n);
    long long my_n = (2 * n) / size;
    double my_a = a + rank * my_n * h;
    double my_b = my_a + my_n * h;

    if (mode == 1) {
        result = integral(f1, my_a, my_b, my_n, h);
    } else if (mode == 2) {
        result = integral(f2, my_a, my_b, my_n, h);
    } else if (mode == 3) {
        result = integral(f3, my_a, my_b, my_n, h);
    }

    MPI_Reduce(&result, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double end = MPI_Wtime();
        printf("#N of intervals: %lld\n", n);
        printf("#N of cores: %d\n", size);
        printf("TIME: %lf\n", end - start);
    }

    MPI_Finalize();
}