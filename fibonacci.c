#include <stdio.h>

int fib(int n)
{
  if(1 == n || 2 == n)
  {
      return 1;
  }
  else
  {
      return fib(n-1) + fib(n-2);
  }

}

int main()
{
    int i;
    for(i=1; i<=10; i++)
    {
	printf("%d\n", fib(i));
    }
    return 0;
}
