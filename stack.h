
#include <stdio.h>
#include <stdbool.h>



//global structure of objref
typedef struct {
unsigned int size;  /* byte count of payload data */
unsigned char data [1]; /* payload data , size as needed */
} * ObjRef ;

typedef struct {

bool isObjRef;
 union {
       ObjRef objRef;
        int number;
 }u;
 
}StackSlot;


StackSlot stack[100000];


int sp = 0;


void pusho(ObjRef objRef){

    stack[sp].isObjRef=true;
    stack[sp].u.objRef=objRef;
    sp++;
}
void push(int element)
{   
    stack[sp].isObjRef=false;
    stack[sp].u.number=element;
    sp++;
}

int pop()
{
    int ele;
    //printf("pop %d\n",stack[sp-1]);

    ele = stack[--sp].u.number;

    return ele;
}

ObjRef popo()
{
     ObjRef ele;
    //printf("pop %d\n",stack[sp-1]);

    ele = stack[--sp].u.objRef;

   // printf("poped %d",*(int*)ele->data);

    return ele;
}

extern int pc;