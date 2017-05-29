void somar(double *a,double *b, double *c)
{
  a[0]=b[0]+c[0];
  a[1]=b[1]+c[1];
}
void subtrair(double *a,double *b, double *c)
{
  a[0]=b[0]-c[0];
  a[1]=b[1]-c[1];
}
void multiplicar(double *a,double *b, double *c)
{
  a[0]=b[0]*c[0]-b[1]*c[1];
  a[1]=b[0]*c[1]+b[1]*c[0];
}
void dividir(double *a,double *b, double *c)
{
  double d;
  d=c[0]*c[0]+c[1]c[1];
  a[0]=b[0]*c[0]+b[1]*c[1];
  a[0]=a[0]/d;
  a[1]=b[0]*c[1]-b[1]*c[0];
  a[1]=a[1]/d;
}
