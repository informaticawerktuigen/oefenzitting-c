#include <stdio.h>

int function_call_counter = 0;

int is_odd(int n);

int is_even(int n)
{

    function_call_counter++;
    if (n < 0)
        n = -n;
    else if (n == 0)
    {
        return 1;
    }
    else
    {
        return is_odd(n - 1);
    }
}

int is_odd(int n)
{
    function_call_counter++;
    if (n == 0)
    {
        return 0;
    }
    else
    {
        return is_even(n - 1);
    }
}

int main()
{
    int n = 21;
    if (is_even(n))
    {
        printf("%d is even\n", n);
    }
    else
    {
        printf("%d is odd\n", n);
    }
    printf("Checking the parity of %d only took us %d function calls!\n", n, function_call_counter);
    return 0;
}