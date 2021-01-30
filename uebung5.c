#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"
#define FORMAT "NJBF"
#define VERSION 5

#define SIGN_EXTEND(i) ((i)&0x00800000 ? (i) | 0xFF000000 : (i))
#define IMMEDIATE(x) ((x)&0x00FFFFFF)

#define HALT 0
#define PUSHC 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define MOD 6
#define RDINT 7
#define WRINT 8
#define RDCHR 9
#define WRCHR 10
#define PUSHG 11
#define POPG 12
#define ASF 13
#define RSF 14
#define PUSHL 15
#define POPL 16

#define BRT 25
#define CALL 26
#define RET 27
#define DROP 28
#define PUSHR 29
#define POPR 30
#define DUP 31

FILE *file;
char *filename;
char c;
int feld[4];
char streamsFile[4];   //4 bytes -->NJBF
unsigned int versi[3]; //version,instruction et var statik

int *progSpeicher;   // je speichern les instruction que jaurais lu
ObjRef *staticDataArea; // ici j alloquieren la place pr le anzahl d instruction qui yaura zb 13 dans le prog01.bin
int pc = 0;
unsigned int nummer = 0;
int fp = 0;
char *a = "Ninja Virtual Machine started";
char *b = "Ninja Virtual Machine stopped";
ObjRef returnValue;
//ObjRef objref;

/*void aendereEndung(char *filename, char *endung)
{
  int len = strlen(filename) - 4;
  int i = 0;


  endung[4] = '\0';
  while (len < strlen(filename))
  {
    endung[i] = filename[len];
    i++;
    len++;
  }
}

int isBinaerfile(char *filename)
{
  // endung ist unser lösung eimer.
  // ist erstmal leer
  // es wird in dem getEndung funktion gefühlt
  // pass by ref
  char *endung = malloc(5 * sizeof(char));


  aendereEndung(filename, endung);
  if (strcmp(endung, ".bin") == 0)
  {
    printf("endung = %s\n", endung);
    return 1;
  }
  else
  {
    printf("endung = %s\n", endung);
    return 0;
  }
}

int isBin(char *filename)
{
  int sizeOfFile = strlen(filename) - 4;
  if (filename[sizeOfFile++] == '.')
  {
    if (filename[sizeOfFile++] == 'b')
    {
      if (filename[sizeOfFile++] == 'i')
      {
        if (filename[sizeOfFile++] == 'n')
        {
          return 1;
        }
        else
        {
          return 0;
        }
      }
      else
      {
        return 0;
      }
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return 0;
  }
}*/

ObjRef anlegen(int value)
{

  ObjRef objRef = malloc(sizeof(unsigned int) + sizeof(int));
  objRef->size = sizeof(int);
  *(int *)objRef->data = value;

  return objRef;
}

void halt()
{
  exit(0);
}

void add()
{
  int a = *(int *)popo()->data;
  int b = *(int *)popo()->data;
  int c = a + b;

  pusho(anlegen(c));
}
void sub()
{
  int a =*(int *) popo()->data;
  int b =*(int *)popo()->data;
  int c = b - a;

  pusho(anlegen(c));
}
void mul()
{
  int a =*(int *)popo()->data;;
  int b =*(int *) popo()->data;;
  int c = a * b;

  pusho(anlegen(c));
}
void division()
{
  int a =*(int *)popo()->data;;
  int b =*(int *) popo()->data;;
  int c = b / a;

  pusho(anlegen(c));
}
void mod()
{

  int a = *(int *)popo()->data;;
  int b =*(int *)popo()->data;;
  int c = b % a;

  pusho(anlegen(c));
}

void rdint()
{
  int eingabe;
  scanf("%d", &eingabe);
  pusho(anlegen(eingabe));
}

void wrint()
{
  
  int a = *(int *)popo()->data;;
  printf("%d", a);
}

void rdchr()
{
  char eingabe;
  scanf("%c", &eingabe);
  pusho(anlegen(eingabe));
}
void wrchr()
{
  int a = *(int*)popo()->data;
  printf("%c", a);
}

void pushg(int element)
{

ObjRef y = staticDataArea[element];
  printf("pushg %d\n", y);
  pusho(y);
}

void popg(int element)
{

  ObjRef x = popo();
  printf("popg %d\n", x);
  staticDataArea[element] = x;
}

void asf(int n)
{

  push(fp);
  fp = sp;
  sp = sp + n;
}

void rsf()
{
  sp = fp;
  fp = pop();
}

void pushlo(int element)
{
  //todo
//stack[fp + objref].isObjRef=true;

    // stack[sp].isObjRef=true;
  pusho(stack[fp + element].u.objRef);
}

void poplo(int element)
{
  stack[fp + element].u.objRef = popo();
  stack[fp + element].isObjRef=true;
}

void eq()

{
  int a = *(int*)popo()->data;
  int b = *(int*)popo()->data;
  if (a == b)
  {
    push(1);
  }
  else
  {
    push(0);
  }
}

void ne()
{

  int a = *(int*)popo()->data;
  int b = *(int*)popo()->data;
  if (a != b)
  {
    push(1);
  }
  else
  {
    push(0);
  }
}

void lt()
{
  int a = *(int*)popo()->data;
  int b = *(int*)popo()->data;
  if (b > a)
  {
    push(1);
  }
  else
  {
    push(0);
  }
}

void le()
{
  int a = *(int*)popo()->data;
  int b = *(int*)popo()->data;
  if (b <= a)
  {
    push(1);
  }
  else
  {
    push(0);
  }
}

void gt()
{
  int a = *(int*)popo()->data;
  int b = *(int*)popo()->data;
  if (b > a)
  {
    push(1);
  }
  else
  {
    push(0);
  }
}

void ge()
{
  int a = *(int*)popo()->data;
  int b = *(int*)popo()->data;
  if (b >= a)
  {
    push(1);
  }
  else
  {
    push(0);
  }
}

void brt(int a)
{
  int b = pop();

  if (b == 1)
    pc = a;
}

void brf(int a)
{
  int b = pop();

  if (b == 0)
    pc = a;
}

void jmp(int a)
{
  pc = a;
}

void call(int x)
{
  //ZB
  push(pc);
  pc = x;
}

void ret()
{
  pc = pop();
}

void drop(int n)
{
  //loescht alle lokale var die in der methode waren
  while (n != 0)
  {
    popo();
    n--;
  }
}

void pushr()
{
  //push la valeur ki est dans le return value dans le stack
  //cette valeur qui etait le resultat d une methode kon avait appeler
  pusho(returnValue);
}

void popr()
{

  //pop die oberste value und speichert in returnvalue

  returnValue = popo();
}

void dup()
{

  //duplik le oberste wert sur le stack
  //push(stack[sp-1]) cad push(5)
  ObjRef x;
  x = popo();
  pusho(x);
  pusho(x);
}
void executable(unsigned int IR)
{

  if (IR >> 24 == PUSHC)
  {
    pusho(anlegen(SIGN_EXTEND(IMMEDIATE(IR))));
  }
  else if (IR >> 24 == ADD)
  {
    add();
  }
  else if (IR >> 24 == SUB)
  {
    sub();
  }

  else if (IR >> 24 == MUL)
  {
    mul();
  }

  else if (IR >> 24 == DIV)
  {
    division();
  }

  else if (IR >> 24 == MOD)
  {
    mod();
  }

  else if (IR >> 24 == RDINT)
  {
    rdint();
  }

  else if (IR >> 24 == WRINT)
  {
    wrint();
  }
  else if (IR >> 24 == RDCHR)
  {
    rdchr();
  }
  else if (IR >> 24 == WRCHR)
  {

    wrchr();
  }
  else if (IR >> 24 == PUSHG)
  {

    pushg(SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == POPG)
  {

    popg(SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == ASF)
  {

    asf(SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == RSF)
  {

    rsf();
  }

  else if (IR >> 24 == PUSHL)
  {

    pushlo(SIGN_EXTEND(IMMEDIATE(IR)));
  }
  else if (IR >> 24 == POPL)
  {

    poplo(SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == EQ)
  {

    eq();
  }
  else if (IR >> 24 == NE)
  {

    ne();
  }
  else if (IR >> 24 == LT)
  {

    lt();
  }

  else if (IR >> 24 == LE)
  {

    le();
  }
  else if (IR >> 24 == GT)
  {

    gt();
  }
  else if (IR >> 24 == GE)
  {

    ge();
  }

  else if (IR >> 24 == BRF)
  {

    brf(SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == BRT)
  {

    brt(SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == JMP)
  {
    jmp(SIGN_EXTEND(IMMEDIATE(IR)));
  }
  else if (IR >> 24 == CALL)
  {
    call(SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == RET)
  {
    ret();
  }
  else if (IR >> 24 == DROP)
  {
    drop(SIGN_EXTEND(IMMEDIATE(IR)));
  }
  else if (IR >> 24 == PUSHR)
  {
    pushr();
  }
  else if (IR >> 24 == POPR)
  {
    popr();
  }

  else if (IR >> 24 == HALT)
  {
    halt();
  }

  else;
}
void listener(unsigned int IR)
{

  if (IR >> 24 == PUSHC)
  {
    printf(" %03d:\t PUSHC\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == ADD)
  {
    printf(" %03d:\t ADD\n", nummer);
  }
  else if (IR >> 24 == SUB)
  {
    printf(" %03d:\t SUB\n", nummer);
  }

  else if (IR >> 24 == MUL)
  {
    printf(" %03d:\t MUL\n", nummer);
  }

  else if (IR >> 24 == DIV)
  {
    printf(" %03d:\t DIV\n", nummer);
  }

  else if (IR >> 24 == MOD)
  {
    printf(" %03d:\t MOD\n", nummer);
  }
  else if (IR >> 24 == RDINT)
  {
    printf(" %03d:\t RDINT\n", nummer);
  }

  else if (IR >> 24 == WRINT)
  {
    printf(" %03d:\t WRINT\n", nummer);
  }
  else if (IR >> 24 == RDCHR)
  {
    printf(" %03d:\t RDCHR\n", nummer);
  }
  else if (IR >> 24 == WRCHR)
  {
    printf(" %03d:\t WRCHR\n", nummer);
  }
  else if (IR >> 24 == HALT)
  {
    printf(" %03d:\t halt\n", nummer);
  }
  else if (IR >> 24 == PUSHG)
  {
    printf(" %03d:\t PUSHG\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }
  else if (IR >> 24 == POPG)
  {
    printf(" %03d:\t POPG\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }
  else if (IR >> 24 == ASF)
  {
    printf(" %03d:\t ASF\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }
  else if (IR >> 24 == RSF)
  {
    printf(" %03d:\t RSF\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }
  else if (IR >> 24 == PUSHL)
  {
    printf(" %03d:\t PUSHL\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == POPL)
  {
    printf(" %03d:\t POPL\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == EQ)
  {
    printf(" %03d:\t EQ\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == NE)
  {
    printf(" %03d:\t NE\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == LT)
  {
    printf(" %03d:\t LT\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == LE)
  {
    printf(" %03d:\t LE\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == GT)
  {
    printf(" %03d:\t GT\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == CALL)
  {
    printf(" %03d:\t call\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == RET)
  {
    printf(" %03d:\t GE\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }
  else if (IR >> 24 == DROP)
  {
    printf(" %03d:\t drop\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }
  else if (IR >> 24 == PUSHR)
  {
    printf(" %03d:\t GE\t%d\n", nummer);
  }
  else if (IR >> 24 == POPR)
  {
    printf(" %03d:\t popr\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }
  else
  {
    printf("i dont know\n");
  }
}

void show(char *msg)
{
  printf("%s\n", msg);
}

void execute()
{
  int IR = 0;

  do
  {

    IR = progSpeicher[pc];
    pc = pc + 1;
    executable(IR);
  } while (IR >> 24 != HALT);

  printf("%s", b);
}

int isbin(char *filename)
{
  //
  char *sub = malloc(sizeof(char) * 5);
  int len = strlen(filename);
  int position = len - 4;

  int c = 0;
  sub[4] = '\0';

  while (position < len)
  {
    sub[c] = filename[position];
    c++;
    position++;
  }

  if (strcmp(sub, ".bin") == 0)
  {

    // printf("binary datei%s", sub);
    return 1;
  }

  else
  {
    return 0;
  }
}

int main(int argc, char *argv[])
{

  if (argc > 1)
  {
    if (argc > 2)
    {
      printf("err >2");
    }
    else
    {
      filename = argv[1];

      file = fopen(argv[1], "r"); // je lis l argument par ex prog01.bin
      if (file == NULL)
      {
        printf("Error cannot open file %s", argv[1]);
      }
      else
      {

        // printf("hier lesen faengt an");
        if (isbin(filename) == 1)
        {
          // printf("binary file\n");

          fread(streamsFile, sizeof(char), 4, file); // streamfile speicher les 4 premiers bytes Format

          /* for (int i = 0; i < 4; i++)
          {
            printf("%.2x\n", streamsFile[i]); //2x pr l hexadezimal
          }
*/
          fread(versi, sizeof(int), 3, file); //versi  hat 3 plaetze version-->instruction-->statik var

          /*4 number shoul be compare 

          */

          if (strncmp(streamsFile, FORMAT, 4) == 0 && (int)versi[0] == VERSION)
          {
            progSpeicher = malloc(sizeof(int) * (int)versi[1]);
            if ((int)versi[2] != 0)
            {
              staticDataArea = malloc(sizeof(int) * (int)versi[2]);
            }
            fread(progSpeicher, sizeof(int), versi[1], file);

            show(a);

            execute();
            printf("%s", b);
          }
          else
          {
          }
        }

        else
        {
          printf("Error file %s is not a Ninja binary", argv[1]);
        }
      }
    }
  }
  else
  {
    printf("error : no code file specified");
  }

  return 0;
}
/*

unsigned int pc = 0;
unsigned int IR; //INFO KON PREND O PROGRAMM SPEICHER


ARRAY KI CONTIENT CHAK Instruction
unsigned int code1[] = {
    (PUSHC << 24) | IMMEDIATE(3),
    (PUSHC << 24) | IMMEDIATE(4),
    (ADD << 24),
    (PUSHC << 24) | IMMEDIATE(10),
    (PUSHC << 24) | IMMEDIATE(6),
    (SUB << 24),
    (MUL << 24),
    (WRINT << 24),
    (PUSHC << 24) | IMMEDIATE(10),
    (WRCHR << 24),
    (HALT << 24)};

unsigned int code2[] = {
    (PUSHC << 24) | IMMEDIATE(-2),
    (RDINT << 24),
    (MUL << 24),
    (PUSHC << 24) | IMMEDIATE(3),
    (ADD << 24),
    (WRINT << 24),
    (PUSHC << 24) | IMMEDIATE(10),
    (WRCHR << 24),
    (HALT << 24)};

unsigned int code3[] = {
    (RDCHR << 24),
    (WRINT << 24),
    (PUSHC << 24) | IMMEDIATE(10),
    (WRCHR << 24),
    (HALT << 24)};

info von code1-->zb  je verifie ==push usw
IR--->vient du programmspeicher





void executable(unsigned int IR)
{
  printf("inside excecutable\n");
  int summe = 0;
  if (IR >> 24 == PUSHC)
  {
    push(SIGN_EXTEND(IMMEDIATE(IR)));
  }
else if (IR >> 24 == ADD){
    int erste = pop();
  
    int zweite = pop();
    summe = erste + zweite;
   printf("check add\n");
    push(summe);
  }
  else if (IR >> 24 == SUB)
  {
    int erste = pop();
    int zweite = pop();
    int rest =  zweite - erste;
   printf("check sub\n");
    push(rest);
  }

  else if (IR >> 24 == MUL)
  {
    int erste = pop();
    int zweite = pop();
    int multi = erste * zweite;
    printf("check mul\n");
    push(multi);
  }

  else if (IR >> 24 == DIV)
  {
    int erste = pop();
    int zweite = pop();
    int divis = zweite / erste;
   printf("check div\n");
    push(divis);
  }

  else if (IR >> 24 == MOD)
  {
    int erste = pop();
    int zweite = pop();
    int modul = erste % zweite;
    push(modul);
  }

  else if (IR >> 24 == RDINT)
  {
    int eingabe;
    scanf("%d", &eingabe);
    push(eingabe);
  }

  else if (IR >> 24 == WRINT)
  {
    int elem = pop();
    printf("check WRINT\n");
    printf("%d",elem);
  }
  else if (IR >> 24 == RDCHR)
  {
    char eing;

    scanf("%c", &eing);

    push(eing);
  }
  else if (IR >> 24 == WRCHR)
  {

    char elem = pop();
    printf("check WCHAR\n");
    printf("%c",elem);
  }

  else if (IR >> 24 == HALT){
      exit(0);
    }
}

void listener(unsigned int IR)
{
  SIGN_EXTEND(IMMEDIATE(IR))

  if (IR >> 24 == PUSHC)
  {
    printf("i push something\n");
    printf(" %03d:\t PUSHC\t%d\n", nummer, SIGN_EXTEND(IMMEDIATE(IR)));
  }

  else if (IR >> 24 == ADD)
  {
    printf(" %03d:\t ADD\n", nummer);
  }
  else if (IR >> 24 == 3)
  {
    printf(" %03d:\t SUB\n", nummer);
  }

  else if (IR >> 24 == MUL)
  {
    printf(" %03d:\t MUL\n", nummer);
  }

  else if (IR >> 24 == DIV)
  {
    printf(" %03d:\t DIV\n", nummer);
  }

  else if (IR >> 24 == MOD)
  {
    printf(" %03d:\t MOD\n", nummer);
  }
  else if (IR >> 24 == RDINT)
  {
    printf(" %03d:\t RDINT\n", nummer);
  }

  else if (IR >> 24 == WRINT)
  {
    printf(" %03d:\t WRINT\n", nummer);
  }
  else if (IR >> 24 == RDCHR)
  {
    printf(" %03d:\t RDCHR\n", nummer);
  }
  else if (IR >> 24 == WRCHR)
  {
    printf(" %03d:\t WRCHR\n", nummer);
  }
  else if (IR >> 24 == HALT)
  {option] ...\n\t--version \t\t\tshow version and exit\n\t--help\t\t\tshow this help and exit\n");

}

    printf(" %03d:\t halt\n", nummer);
    exit(0);
  }
}

int main(int argc, char *argv[])
{

  char *a = "Ninja Virtual Machine started\n";
  char *b = "Ninja Virtual Machine stopped\n";

  char *help = "--help";
  char *version = "--version"
  int a = atoi(argv[1]);
int b = atoi(argv[2]);
int result  = a+b;
printf("The reuslt ist %d\n",result);

int i=0;
do{
     printf( "%s\n",argv[i]);
     i++;
}while (i<argc);

for(int i=0;i<argc;i++){
    printf( "%s\n",argv[i]);
}


f (argc > 1)
  {

    if (strcmp(help, argv[1]) == 0)
    {

      // printf("usage: ./njvm [option] [option] ...\n\t--version \t\t\tshow version and exit\n\t--help\t\t\tshow this help and exit\n");
      printf("usage: ./njvm [option] [option] ...");
      printf("  --prog1       select program 1 to execute\n");
      printf("  --prog2            select program 2 to execute\n");
      printf("  --prog3           select program 3 to execute\n");
      printf(" --version   show version and exit\n\t--help\t\t\tshow this help and exit\n");
    }
    else if (strcmp("programm1", argv[1]) == 0)
    {

      printf("%s", a);

     do {
       printf("inside listener code1\n");
        IR = code1[pc];
         listener(IR);
       
         printf("test1\n");
         pc = pc + 1;
      
         printf("test2\n");
        nummer++;
      
        printf("test3");
       
      } 
      while (IR>>24!=HALT);
 
      pc=0;
      nummer=0;
      while (!HALT)
      {
       
        IR = code1[pc];
        executable(IR);
         pc = pc + 1;
        nummer++;
      }
    
      printf("%s", b);
      exit(1);

    }

    else if (strcmp("programm2", argv[1]) == 0)
    {

 printf("inside code2\n");
     do
      {

        IR = code2[pc];
        listener(IR);
          pc = pc + 1;
         nummer++;
      } while (IR>>24!=HALT);
      nummer=0;
      pc=0;
      while (!HALT)
      {
      printf("excecutable");
        IR = code2[pc];
        pc = pc + 1;
        executable(IR);
        nummer++;
      }
    }
    else if (strcmp("programm3", argv[1]) == 0)
    {

      
     do {

        IR = code3[pc];
       
        listener(IR);
         pc = pc + 1;
        nummer++;
      }while (IR>>24!=HALT);
  pc=0;
  nummer=0;
      while (!HALT)
      {

        IR = code3[pc];
        pc = pc + 1;
        executable(IR);
        nummer++;
      }
    }
  }

  else
  {
    printf("Error: no program selected\n");
  }

  return 0;
}

if(argc>1){

if(strcmp(help,argv[1])==0){


   printf("usage: ./njvm [option] [option] ...\n\t--version \t\t\tshow version and exit\n\t--help\t\t\tshow this help and exit\n");
    printf("usage: ./njvm [option] [option] ...");
    printf("  --prog1       select program 1 to execute\n");
      printf("  --prog2            select program 2 to execute\n");
        printf("  --prog3           select program 3 to execute\n");
          printf(" --version   show version and exit\n\t--help\t\t\tshow this help and exit\n");
          

}

 else if(strcmp(version,argv[1])==0)
{
   printf("Ninja Virtual Machine version 0 (compiled April 25 2020, 10:36:52\n"); 
}

else{
  
  printf("unkwon command line argument %s,try %s --help ", argv[1],argv[0]);

}

}
else{

    for(int i=0;i<argc;i++){

    printf( "%s\n",argv[i]);
}


 printf("%s%s",a ,b);
  
}*/
