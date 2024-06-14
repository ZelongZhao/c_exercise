#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/** Our old friend die from ex17. */
void die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

// a typedef creates a fake type, in this
// case for a function pointer
typedef int (*compare_cb)(int a, int b);
typedef int* (*sort_cb)(int*, int, compare_cb);

/**
 * A classic bubble sort function that uses the
 * compare_cb to do the sorting.
 */
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
    int temp = 0;
    int i = 0;
    int j = 0;
    int *target = malloc(count * sizeof(int));

    if(!target) die("Memory error.");

    memcpy(target, numbers, count * sizeof(int));

    for(i = 0; i < count; i++) {
        for(j = 0; j < count - 1; j++) {
            if(cmp(target[j], target[j+1]) > 0) {
                temp = target[j+1];
                target[j+1] = target[j];
                target[j] = temp;
            }
        }
    }

    return target;
}

//TODO fix it
void quick_sort_internal(int* num,int n, compare_cb cmp){
    if(n < 1)
        return;
    int pivot = num[n / 2];
    int left = 0;
    int right = n - 1;
    while (left < right){
        if(cmp(num[left] , pivot) < 0) left++;
        if(cmp(num[right],pivot)>0)  right--;

        if(num[left] < num [right]){
            int tmp = num[left];
            num[left] = num[right];
            num[right] = tmp;

            left++;
            right--;
        }
    }
    quick_sort_internal(num,right,cmp);
    quick_sort_internal(&num[left],n - left,cmp);
}

int *quick_sort(int *numbers, int count, compare_cb cmp)
{
    int * ret = malloc(count * sizeof (int ));
    ret = memcpy(ret,numbers,count);

    quick_sort_internal(ret,count,cmp);

    return ret;
}

int sorted_order(int a, int b)
{
    return a - b;
}

int reverse_order(int a, int b)
{
    return b - a;
}

int strange_order(int a, int b)
{
    if(a == 0 || b == 0) {
        return 0;
    } else {
        return a % b;
    }
}

/**
 * Used to test that we are sorting things correctly
 * by doing the sort and printing it out.
 */
void test_sorting(int *numbers, int count, sort_cb sort,compare_cb cmp)
{
    int i = 0;
    int *sorted = sort(numbers, count, cmp);

    if(!sorted) die("Failed to sort as requested.");

    for(i = 0; i < count; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    free(sorted);
}

int main(int argc, char *argv[])
{
    if(argc < 2) die("USAGE: ex18 4 3 1 5 6");

    int count = argc - 1;
    int i = 0;
    char **inputs = argv + 1;

    int *numbers = malloc(count * sizeof(int));
    if(!numbers) die("Memory error.");

    for(i = 0; i < count; i++) {
        numbers[i] = atoi(inputs[i]);
    }

    test_sorting(numbers, count,quick_sort, sorted_order);
    test_sorting(numbers, count,quick_sort, reverse_order);
    test_sorting(numbers, count,quick_sort, strange_order);

    free(numbers);

    return 0;
}