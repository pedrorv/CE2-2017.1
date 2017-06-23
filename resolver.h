#include "macros.h"
#include "tipos.h"

int resolversistemaPO(double  Yn[MAX_NOS+1][MAX_NOS+2],contagem *cont);

int testeconvergenciaPO(double  Yn[MAX_NOS+1][MAX_NOS+2], double  Yn1[MAX_NOS+1][MAX_NOS+2], contagem *cont);

int resolversistemaPS(double _Complex Yn[MAX_NOS+1][MAX_NOS+2], contagem *cont);

void imprimeSistemaDouble(double Yn[MAX_NOS+1][MAX_NOS+2], contagem *cont);

void zerarMatrizesDouble(double Yn[MAX_NOS+1][MAX_NOS+2], double Yn1[MAX_NOS+1][MAX_NOS+2], contagem *cont);

void zerarMatrizDoubleComplex(double _Complex YnPS[MAX_PONTOS+1][MAX_NOS+1][MAX_NOS+2], contagem *cont, frequencia *freq);

void imprimeSistemaDoubleComplex(double _Complex YnPS[MAX_NOS+1][MAX_NOS+2], contagem *cont);

double graus(double radianos);

void guardarResultados(double _Complex YnPontos[MAX_PONTOS+1][MAX_NOS+1], double YnPO[MAX_NOS+1][MAX_NOS+2], double _Complex YnPS[MAX_PONTOS+1][MAX_NOS+1][MAX_NOS+2], contagem *cont, int ponto, double freq);