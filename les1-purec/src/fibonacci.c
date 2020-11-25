int fibonacci(unsigned int n)
{
    if (n <= 1)
    {
        return n;
    }

    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(void)
{
    return fibonacci(10);
}