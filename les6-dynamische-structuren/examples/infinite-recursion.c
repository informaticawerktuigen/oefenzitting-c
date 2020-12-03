int infinite_recursion(int a)
{
    infinite_recursion(a);
    //unreachable code
}

int main(void)
{
    infinite_recursion(0);
    return 0;
}