#include "tipos.h"
#include "macros.h"

void somar(int *Q, int a, int b);

void operacional (int na,int nb,int nc,int nd, tabela L, tabela C);

void transcondutancia(double gm,int n1,int n2,int n3,int n4, double Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C);

void transadmitancia(double _Complex ym,int n1,int n2,int n3,int n4, double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C);

void condutancia(double g, int a, int b,double Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C);
                 
void admitancia(double _Complex  y, int a, int b, double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C);

void corrente(double i, int a, int b,double  Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C);

void fasorcorrente(double modulo, double fase, int a, int b, double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C);

void acoplamento(double k,char la[],char lb[],elemento netlist[MAX_ELEM],double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, double f, contagem *cont);

void mnaPO(elemento netlist[MAX_ELEM],double  Yn[MAX_NOS+1][MAX_NOS+2], double Yn1[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, contagem *cont);

void mnaPS(elemento netlist[MAX_ELEM],double  YnPO[MAX_NOS+1][MAX_NOS+2], double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, double f, contagem *cont);
