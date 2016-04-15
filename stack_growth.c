#include<stdio.h>
#include<stdlib.h>

void func(int *p) {
    int i;
        printf("\n i:%p",&i);
    if (!p){
        printf("\n p:%p",p);
        func(&i);
    }
    else if (p < &i){
        printf("Stack grows upward\n");
        printf("\n p:%p &i:%p",p,&i);
    }
    else{
        printf("\n p:%p &i:%p",p,&i);
        printf("Stack grows downward\n");
   }
}

int main()
{

 func(NULL);
  return 0;
}
