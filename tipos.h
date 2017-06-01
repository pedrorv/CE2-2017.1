#define MAX_LINHA 80
#define MAX_NOME 11
#define MAX_ELEM 50
#define MAX_NOS 50
#define TOLG 1e-9
#define GMAX 1000000
#define DEBUG

typedef struct elemento { /* Elemento do netlist */
  char nome[MAX_NOME],la[MAX_NOME],lb[MAX_NOME];
  double valor, modulo, fase;
  int a,b,c,d,x,y;
} elemento;

typedef int tabela[MAX_NOS+1];
