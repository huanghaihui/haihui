#include "stdio.h"

int a(int t) {
    return t;
}

int  main()
{
    printf("hello pass !!!");

    a(10);
    int i=0;
    int j=0;
    for(i=0;i<10;i++)
	j++;
    printf("j:%d\n",j);
   
    int (*b)(int x);
    //b = a;
    return 0;
}
