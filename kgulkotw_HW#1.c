#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Func to check if a number is prime or not
bool is_Prime_(int n)
{
    if (n <= 1)
        return false;
    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

// Func to find the prime number nearest below n
int nearestPrimeBelow_(int n)
{
    while (!is_Prime_(--n))
        ;
    return n;
}

// Func to get the sum of digits
int sumOf_Digits(int n)
{
    int sum = 0;
    while (n != 0)
    {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

// Func numberTransformer
void numberTransformer_(int n)
{
    if (is_Prime_(n) && n != 3 && n != 5)
    {
        printf("Go Blazers\n");
    }
    else if ((n & (n - 1)) == 0)
    {
        int nearestPrime_no = nearestPrimeBelow_(n);
        printf("%d\n", n + nearestPrime_no);
    }
    else if (n % 3 == 0)
    {
        if (n % 5 == 0)
        {
            printf("UAB CS 332&532\n");
        }
        else if (sumOf_Digits(n) % 3 == 0)
        {
            printf("CS\n");
        }
    }
    else
    {
        int square = pow(sumOf_Digits(n), 2);
        printf("%d\n", square);
    }
}

// Func UABNumber
bool UABNumber(int n2)
{
    int sum = 0;
    for (int i = 1; i <= n2 / 2; i++)
    {
        if (n2 % i == 0)
        {
            sum += i;
        }
    }
    return (sum == n2);
}

// Func reverseNum
int reverseNum(int n3)
{
    int reversed_no = 0;
    while (n3 != 0)
    {
        reversed_no = reversed_no * 10 + n3 % 10;
        n3 /= 10;
    }
    return reversed_no;
}

// Func smallerThanIndex
int smallerThanIndex(int numbers[], int size)
{
    int count_no = 0;
    for (int i = 0; i < size; i++)
    {
        if (numbers[i] < i)
        {
            count_no++;
        }
    }
    return count_no;
}

// Func arrayDetails
void arrayDetails(int arr[])
{
    int min = arr[0], min_Index = 0, max = arr[0], max_Index = 0, sum = 0;
    for (int i = 0; i < 6; i++)
    {
        sum += arr[i];
        if (arr[i] < min)
        {
            min = arr[i];
            min_Index = i;
        }
        if (arr[i] > max)
        {
            max = arr[i];
            max_Index = i;
        }
    }
    float mean = (float)sum / 6;
    printf("[%d, %d, %.2f, %d, %d]\n", min, min_Index, mean, max, max_Index);
}

int main()
{
    // Test cases
    numberTransformer_(3);
    numberTransformer_(64);
    numberTransformer_(17);
    numberTransformer_(10);
    numberTransformer_(30);

    int n2;
    printf("Enter integer for UABNumber: ");
    scanf("%d", &n2);
    printf("UABNumber result: %s\n", UABNumber(n2) ? "True" : "False");

    int n3;
    printf("Enter integer for reverseNum: ");
    scanf("%d", &n3);
    printf("reverseNum result: %d\n", reverseNum(n3));

    int numbers1[] = {10, 20, 1, 2, 30};
    int numbers2[] = {1, 2, 0, 44, 29};
    int numbers3[] = {-4, -3, 2, 1, 0};

    printf("smallerThanIndex result 1: %d\n", smallerThanIndex(numbers1, 5));
    printf("smallerThanIndex result 2: %d\n", smallerThanIndex(numbers2, 5));
    printf("smallerThanIndex result 3: %d\n", smallerThanIndex(numbers3, 5));

    int arr[] = {-8, 18, 103, 1, -4, 631};
    arrayDetails(arr);

    return 0;
}
