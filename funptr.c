// Defines the function callback type.  
typedef int (*pFun)(int paraA, int paraB);  
  
struct FuncPtr  
{  
    int x;  
    int y;  
  
    // A function pointer to the implementation of the Summary.  
    pFun GetSum;  
};  
  
// The function of summary.  
int GetSum(int paraA, int paraB)  
{  
    return (paraA + paraB);  
}  
  
void main(void)  
{  
    struct FuncPtr fp;  
    int result = 0;  
  
    fp.x = 1987;  
    fp.y = 1988;  
  
    fp.GetSum = GetSum;  
    result = fp.GetSum(fp.x, fp.y);  
    printf("\n result = %d\n",result);  
  
    getchar();  
}  

