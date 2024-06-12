#include <stdio.h>  // Standard library header files, printf(), scanf()
#include <stdlib.h> // getenv(), ...
#include <assert.h>

/*
    Time complexity:
        O(1)

    Get the array element at the specified index i
 */

int GetArrayElement(int *arr, int n, int i) {
    printf("*******************  GetArrayElement(): O(1) **********************\n");
    assert(i >= 0 && i < n);
    return arr[i];
}



/*
    Time complexity:
        O(n)

    Get the sum of all array elements.
    
 */
int GetSum(int *arr, int n) {
    int res = 0;
    printf("*******************  GetSum(): O(n) **********************\n");
    for (int i = 0; i < n; i++) {
        res += arr[i];
    }

    return res;
}

/*
    Time complexity:
        O(log(n))

    Binary search, also known as half-interval search or logarithmic search, 
    is a search algorithm that finds the position of a target value within a sorted array. 
    It compares the target value to the middle element of the array.

    In this function, the input array should be sorted in an ascending order.

        arr[0], arr[1], ..., arr[n-1]
    
    @return 
        the index of the array element whose value is @targetVal, if found;
        otherwise, -1 is returned.       

    Suppose n is 8,  where log(8) == 3

    Pass 1:

        arr[0]  arr[1]  arr[2]  arr[3]  arr[4]  arr[5]  arr[6]  arr[7]

        mid = (0 + 7) / 2 = 3        

    Pass 2:

        arr[4]  arr[5]  arr[6]  arr[7]

        mid = (4 + 7) / 2 = 5
      
    Pass 3:
        arr[6]  arr[7]  
        
        mid = (6 + 7) / 2 = 6

    Pass 4:
        arr[7]

        mid = (7 + 7) / 2 = 7

    
    At most (log(n) + 1) iterations needed in the while loop, as we always chose 
    the larger half interval (worse case) in each pass.

    So, the time complexity of binary search is O(log(n)).
 */

int BinarySearch(int *arr, int n, int targetVal) {
    int left = 0;
    int right = n - 1;
    printf("*******************  Binary Search: O(log(n)) **********************\n");

    while (left <= right) {
        int mid = (left + right) / 2;
        printf("left = %d, mid = %d, right = %d\n", left, mid, right);

        if (arr[mid] == targetVal) {
            return mid;
        }

        if (arr[mid] < targetVal) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    // Not found
    return -1;
}


// the order of a square matrix
#define  N  4

/*
    O(n*n*n)

    In mathematics, a square matrix is a matrix with the same number of rows and columns. 
    
    An n-by-n matrix is known as a square matrix of order n.

 */
void SquareMatrixMultiply(int mat1[][N], int mat2[][N], int res[][N]) {
    printf("*******************  SquareMatrixMultiply: O(n*n*n) **********************\n\n\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = 0;
            for (int k = 0; k < N; k++) {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

static int mat1[N][N] = {
    {2, 0, 0, 1},
    {0, 3, 0, 2},
    {0, 0, 1, 3},
    {0, 0, 0, 1}
};

static int mat2[N][N] = {
    {10, 20, 30, 40},
    {10, 20, 30, 40},
    {10, 20, 30, 40},
    {1,  1,  1,  1},
};

static int res[N][N];

/*
    Time complexity:

        O(n*n)
 */
void PrintMatrix(int res[][N]) {
    printf("*******************  PrintMatrix: O(n*n) **********************\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("% 8d ", res[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


/*
    Time complexity:

        O(n*(2**n))

    To simplify, let's assume that all elements in the array arr are distinct from each other.

    arr[3] = {20, 24, 90}

    ----------------------------
    curVal      SubSet
    ----------------------------
    000         {}
    001         {20}
    010         {24}
    011         {20, 24}
    100         {90}
    101         {20, 90}
    110         {24, 90}
    111         {20, 24, 90}
    ----------------------------
 */

void PrintSubSets(int *arr, int n) {
    printf("*******************  PrintSubSets: O(n*(2**n))    **********************\n");
    // (1 << n) is  2**n
    unsigned int maxVal = (1 << n);
    // assert(n < 32);
    // printf("sizeof(maxVal) * 8 = %ld\n", sizeof(maxVal) * 8);
    assert(n < (sizeof(maxVal) * 8));

    for (unsigned int curVal = 0; curVal < maxVal; curVal++) { // (2**n) iterations
        // 
        printf("{ ");
        for (int i = 0; i < n; i++) {   // n iterations
             if ((curVal & (1 << i)) != 0) {
                printf("%d ", arr[i]);
             }
        }
        printf("}\n");
    }
}

static void swap(int *pa, int *pb) {
    int tmp;
    
    tmp = *pa;
    *pa = *pb;
    *pb = tmp;
}

static void PrintArray(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/*
    Time complexity:

        O(n!*n)
    
    Given n elements in an array arr,
    there are n! permutations and it requires O(n) time to print a permutation.

    To simplify, let's assume that all elements in the array arr are distinct from each other.
 */
void PrintPermutations(int *arr, int n, int left, int right) {
#if 0    
    static int cnt = 0;
    cnt++;
    printf("PrintPermutations(): cnt = %d, n = %d, left = %d, right = %d\n", 
            cnt, n, left,right);
#endif    
    if (left == right) {
        PrintArray(arr, n);
        return;
    }
    // Depth first search in the call tree
    for (int i = left; i <= right; i++) {
        // Let arr[i] be the first element in [left, right]
        swap(&arr[left], &arr[i]);
        // recursively handle the (n-1) elements
        PrintPermutations(arr, n, left+1, right);
        // Restore
        swap(&arr[left], &arr[i]);
    }
}

static int nums[] = {20, 24, 90};

int main(int argc, char **argv, char **env) {    
    int sortedArr[] = {10, 20, 30, 40, 50, 60, 70, 80};
    int n = sizeof(sortedArr)/sizeof(sortedArr[0]);

    int val = GetArrayElement(sortedArr, n, 0);
    printf("val = %d\n\n", val);

    int sum = GetSum(sortedArr, n);
    printf("sum = %d\n\n", sum);    

    
    int pos = BinarySearch(sortedArr, n, 90);
    
    printf("\npos = %d\n\n", pos);

    PrintMatrix(mat1);

    PrintMatrix(mat2);

    SquareMatrixMultiply(mat1, mat2, res);

    PrintMatrix(res);

    printf("\n************************************************************************\n");
    n = sizeof(nums)/sizeof(nums[0]);

    PrintArray(nums, n);
    printf("\n");

    PrintSubSets(nums, n);

    printf("\n*******************  PrintPermutations: O(n*(n!)) **********************\n");
    PrintPermutations(nums, n, 0, n-1);
    printf("\n");

    printf("\n************************************************************************\n");
    n = sizeof(nums)/sizeof(nums[0]);

    PrintArray(nums, n);
    printf("\n");    
    return 0;
}


