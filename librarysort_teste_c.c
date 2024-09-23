#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE 1000000
#define EPSILON 1

long long operations = 0;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    operations++;
}

int binary_search(int arr[], int left, int right, int key) {
    while (left <= right) {
        operations++;
        int mid = left + (right - left) / 2;
        if (arr[mid] == key) return mid;
        if (arr[mid] < key) left = mid + 1;
        else right = mid - 1;
    }
    return left;
}

void insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        operations++;
        int key = arr[i];
        int j = i - 1;
        int pos = binary_search(arr, 0, j, key);
        while (j >= pos) {
            operations++;
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void library_sort(int arr[], int n) {
    int *temp = (int *)malloc(sizeof(int) * n * (1 + EPSILON));
    int inserted = 1;
    int gap_size = n * EPSILON;

    temp[0] = arr[0];

    for (int i = 1; i < n; i++) {
        operations++;
        if (i == inserted) {
            memcpy(arr, temp, sizeof(int) * i);
            for (int j = 0; j < i; j++) {
                operations++;
                temp[j * (1 + EPSILON)] = arr[j];
            }
            inserted *= 2;
            gap_size = inserted * EPSILON;
        }

        int pos = binary_search(temp, 0, i * (1 + EPSILON) - 1, arr[i]);
        for (int j = i * (1 + EPSILON) - 1; j > pos; j--) {
            operations++;
            temp[j] = temp[j - 1];
        }
        temp[pos] = arr[i];
    }

    memcpy(arr, temp, sizeof(int) * n);
    free(temp);
}

void generate_numbers(int arr[], int n, const char* order) {
    if (strcmp(order, "crescente") == 0) {
        for (int i = 0; i < n; i++) {
            arr[i] = i + 1;
        }
    } else if (strcmp(order, "decrescente") == 0) {
        for (int i = 0; i < n; i++) {
            arr[i] = n - i;
        }
    } else if (strcmp(order, "aleatorio") == 0) {
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % n + 1;
        }
    }
}

void run_experiment(int size, const char* order) {
    int *arr = (int *)malloc(sizeof(int) * size);
    generate_numbers(arr, size, order);

    operations = 0;
    clock_t start = clock();
    library_sort(arr, size);
    clock_t end = clock();

    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("%d,%s,%.6f,%lld\n", size, order, cpu_time_used, operations);

    free(arr);
}

int main() {
    int sizes[] = {1000, 10000, 100000, 1000000};
    const char* orders[] = {"crescente", "decrescente", "aleatorio"};

    srand(time(NULL));

    printf("Tamanho,Ordem,Tempo (s),Operacoes\n");

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            run_experiment(sizes[i], orders[j]);
        }
    }

    return 0;
}