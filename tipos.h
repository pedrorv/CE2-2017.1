
typedef struct elemento { /* Elemento do netlist */
  char nome[MAX_NOME],la,lb;
  double valor, modulo, fase;
  int a,b,c,d,x,y;
} elemento;

typedef int tabela[MAX_NOS+1];
