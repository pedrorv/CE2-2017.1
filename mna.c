#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "mna.h"
#include "tipos.h"
#include "resolver.h"

void somar(int *Q, int a, int b) {
  int i,a1,b1;

  if (Q[a]>Q[b]) {
    a1=Q[b]; b1=Q[a];
  }
  else {
    a1=Q[a]; b1=Q[b];
  }

  if (a1==b1) {
    printf("Circuito invalido"); 
    exit(1);
  }

  for (i=1; i<=MAX_NOS; i++) {
    if (Q[i]==b1) Q[i]=a1;
    if (Q[i]>b1) Q[i]--;
  }
}

void operacional (int na,int nb,int nc,int nd, tabela L, tabela C) {
  #ifdef DEBUG
    printf("Saida: %d %d; entrada %d %d\n",na,nb,nc,nd);
  #endif

  somar(L,na,nb);
  somar(C,nc,nd);
}

void transcondutancia(double gm,int n1,int n2,int n3,int n4, double Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C) {
  Yn[L[n1]][C[n3]]+=gm;
  Yn[L[n2]][C[n4]]+=gm;
  Yn[L[n1]][C[n4]]-=gm;
  Yn[L[n2]][C[n3]]-=gm;
}

void transadmitancia(double _Complex ym,int n1,int n2,int n3,int n4, double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C) {
  Yn[L[n1]][C[n3]]+=ym;
  Yn[L[n2]][C[n4]]+=ym;
  Yn[L[n1]][C[n4]]-=ym;
  Yn[L[n2]][C[n3]]-=ym;
}

void condutancia(double g, int a, int b,double Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C){
  transcondutancia(g,a,b,a,b,Yn,L,C);
}

void admitancia(double _Complex  y, int a, int b, double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C){
  transadmitancia(y,a,b,a,b,Yn,L,C);
}

void corrente(double i, int a, int b,double  Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, contagem *cont) {
  Yn[L[a]][cont->neq+1]-=i;
  Yn[L[b]][cont->neq+1]+=i;
  condutancia(GMIN,a,b,Yn,L,C);
}

void fasorcorrente(double modulo, double fase, int a, int b, double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, contagem *cont) {
  Yn[L[a]][cont->neq+1]-=modulo*(cos(fase)+sin(fase)*I);
  Yn[L[b]][cont->neq+1]+=modulo*(cos(fase)+sin(fase)*I);
}

void acoplamento(double k,char la[],char lb[],elemento netlist[MAX_ELEM],double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, double f, contagem *cont){
  int i,a,b;
  double l1,l2,m;
  double _Complex jw=2*M_PI*f*I;
  
  for (i=1; i<=cont->ne; i++) {
    if((strcmp(netlist[i].nome,la)==0)){
      l1=netlist[i].valor;
      a=i;
    }
    if((strcmp(netlist[i].nome,lb)==0)){
      l2=netlist[i].valor;
      b=i;
    }
  }
  m=k*sqrt(l1*l2);
  transadmitancia(m*jw,netlist[a].x,0,netlist[b].x,0,Yn,L,C);
  transadmitancia(m*jw,netlist[b].x,0,netlist[a].x,0,Yn,L,C);
}

void mnaPO(elemento netlist[MAX_ELEM], double YnPO[MAX_NOS+1][MAX_NOS+2], double Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, contagem *cont){
  char tipo;
  int i;
  double VBE,GBE,VBC,GBC,VEB,GEB,VCB,GCB,VCE,VEC,GCE,GEC,IBE,IBC,IEB,ICB,ICE,IEC;

  for (i=1; i<=cont->ne; i++) {
    tipo=netlist[i].nome[0];

    if (tipo=='R') {
      condutancia(1.0/netlist[i].valor,netlist[i].a,netlist[i].b,Yn,L,C);
    }
    else if (tipo=='L') {
      transcondutancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      transcondutancia(1,netlist[i].a,netlist[i].b,netlist[i].x,0,Yn,L,C);
      condutancia(GMIN,netlist[i].x,0,Yn,L,C);
    }
    else if (tipo=='C') {
      condutancia(GMIN,netlist[i].a,netlist[i].b,Yn,L,C);
    }
    else if (tipo=='G') {
      transcondutancia(netlist[i].valor,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='K');
    else if (tipo=='I') {
      corrente(netlist[i].valor,netlist[i].a,netlist[i].b,Yn,L,C,cont);
    }
    else if (tipo=='V') {
      transcondutancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      transcondutancia(1,netlist[i].a,netlist[i].b,netlist[i].x,0,Yn,L,C);
      corrente(netlist[i].valor,netlist[i].x,0,Yn,L,C,cont);
    }
    else if (tipo=='E') {
      transcondutancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      transcondutancia(1,netlist[i].a,netlist[i].b,netlist[i].x,0,Yn,L,C);
      transcondutancia(netlist[i].valor,netlist[i].x,0,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='F') {
      transcondutancia(netlist[i].valor,netlist[i].a,netlist[i].b,netlist[i].x,0,Yn,L,C);
      transcondutancia(1,netlist[i].c,netlist[i].d,netlist[i].x,0,Yn,L,C);
      transcondutancia(1,0,netlist[i].x,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='H') {
      transcondutancia(1,0,netlist[i].y,netlist[i].a,netlist[i].b,Yn,L,C);
      transcondutancia(1,netlist[i].c,netlist[i].d,netlist[i].y,0,Yn,L,C);
      transcondutancia(netlist[i].valor,netlist[i].y,0,netlist[i].x,0,Yn,L,C);
      transcondutancia(1,netlist[i].c,netlist[i].d,netlist[i].x,0,Yn,L,C);
      transcondutancia(1,0,netlist[i].x,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='O');
    else if (tipo=='Q') {              
      VBE=YnPO[L[netlist[i].b]][cont->neq+1]-YnPO[L[netlist[i].a]][cont->neq+1];
      VBC=YnPO[L[netlist[i].b]][cont->neq+1]-YnPO[L[netlist[i].c]][cont->neq+1]; 
      VEB=-VBE;
      VCB=-VBC;
      VCE=VBE+VCB;
      VEC=-VCE;


      if (netlist[i].modelo[0] == 'N') {
        if (VBE > 0.7) VBE = 0.7;
        if (VBE < 0) VBE = 0;
        if (VBC > 0.7) VBC = 0.7;
        if (VBC < 0) VBC = 0;
                
        GBE=netlist[i].isbe*exp(VBE/netlist[i].vtbe)/netlist[i].vtbe;
        IBE=netlist[i].isbe*(exp(VBE/netlist[i].vtbe)-1) - GBE*VBE;
        GBC=netlist[i].isbc*exp(VBC/netlist[i].vtbc)/netlist[i].vtbc;
        IBC=netlist[i].isbc*(exp(VBC/netlist[i].vtbc)-1) - GBC*VBC;

        GCE=(netlist[i].alfa*(GBE*VBE+IBE)-(GBC*VBC+IBC))/netlist[i].va;
        ICE=GCE*VCE;
        
        if (VCE>0){      
          condutancia(GCE,netlist[i].c,netlist[i].a,Yn,L,C);
          corrente(ICE,netlist[i].c,netlist[i].a,Yn,L,C,cont);          
        }
       
        condutancia(GBE,netlist[i].b,netlist[i].a,Yn,L,C);
        corrente(IBE,netlist[i].b,netlist[i].a,Yn,L,C,cont);
        corrente(netlist[i].alfar*IBC,netlist[i].a,netlist[i].b,Yn,L,C,cont);
        transcondutancia(netlist[i].alfar*GBC,netlist[i].a,netlist[i].b,netlist[i].b,netlist[i].c,Yn,L,C);

        condutancia(GBC,netlist[i].b,netlist[i].c,Yn,L,C);
        corrente(IBC,netlist[i].b,netlist[i].c,Yn,L,C,cont);
        corrente(netlist[i].alfa*IBE,netlist[i].c,netlist[i].b,Yn,L,C,cont);
        transcondutancia(netlist[i].alfa*GBE,netlist[i].c,netlist[i].b,netlist[i].b,netlist[i].a,Yn,L,C);
        
        printf("Transistor %s VBE %lg VBC %lg GBE %lg GBC %lg GCE %lg IBE %lg IBC %lg ICE %lg\n", netlist[i].nome,VBE,VBC,GBE,GBC,GCE,IBE,IBC,ICE);
      }
      else if (netlist[i].modelo[0] == 'P') {
        if (VEB > 0.7) VEB = 0.7;
        if (VEB < 0) VEB = 0;
        if (VCB > 0.7) VCB = 0.7;
        if (VCB < 0) VCB = 0;
        
        GEB=netlist[i].isbe*exp(VEB/netlist[i].vtbe)/netlist[i].vtbe;
        IEB=netlist[i].isbe*(exp(VEB/netlist[i].vtbe)-1) - GEB*VEB;
        GCB=netlist[i].isbc*exp(VCB/netlist[i].vtbc)/netlist[i].vtbc;
        ICB=netlist[i].isbc*(exp(VCB/netlist[i].vtbc)-1) - GCB*VCB;
        GEC=(netlist[i].alfa*(GEB*VEB+IEB)-(GCB*VCB+ICB))/netlist[i].va;
        IEC=GEC*VEC;
        
        if (VEC>0){
          condutancia(GEC,netlist[i].a,netlist[i].c,Yn,L,C);
          corrente(IEC,netlist[i].a,netlist[i].c,Yn,L,C,cont);      
        }
       
        condutancia(GEB,netlist[i].a,netlist[i].b,Yn,L,C);
        corrente(IEB,netlist[i].a,netlist[i].b,Yn,L,C,cont);
        corrente(netlist[i].alfar*ICB,netlist[i].b,netlist[i].a,Yn,L,C,cont);
        transcondutancia(netlist[i].alfar*GCB,netlist[i].b,netlist[i].a,netlist[i].c,netlist[i].b,Yn,L,C);

        condutancia(GCB,netlist[i].c,netlist[i].b,Yn,L,C);
        corrente(ICB,netlist[i].c,netlist[i].b,Yn,L,C,cont);
        corrente(netlist[i].alfa*IEB,netlist[i].b,netlist[i].c,Yn,L,C,cont);
        transcondutancia(netlist[i].alfa*GEB,netlist[i].b,netlist[i].c,netlist[i].a,netlist[i].b,Yn,L,C);
        
        printf("Transistor %s VEB %lg VCB %lg GEB %lg GCB %lg GEC %lg IEB %lg ICB %lg IEC %lg\n", netlist[i].nome,VEB,VCB,GEB,GCB,GEC,IEB,ICB,IEC);
      }
    }

    //printf("Sistema apos a estampa de %s\n", netlist[i].nome);
    //imprimeSistemaDouble(Yn, cont);
  }
}

void mnaPS(elemento netlist[MAX_ELEM], double YnPO[MAX_NOS+1][MAX_NOS+2], double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, double f, contagem *cont) {
  char tipo;
  int i;
  double _Complex jw=2*M_PI*f*I;
  double VBE,GBE,VBC,GBC,VEB,GEB,VCB,GCB,GCE,GEC,IBE,IBC;
  double VCE,VEC;
  double CBE,CBC,CEB,CCB;
   
  for (i=1; i<=cont->ne; i++) {
    tipo=netlist[i].nome[0];

    if (tipo=='R') {
      admitancia(1/netlist[i].valor,netlist[i].a,netlist[i].b,Yn,L,C);
    }
    else if (tipo=='L') {
      transadmitancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      transadmitancia(1,netlist[i].a,netlist[i].b,netlist[i].x,0,Yn,L,C);
      admitancia((netlist[i].valor*jw),netlist[i].x,0,Yn,L,C);
    }
    else if (tipo=='C') {
      admitancia((netlist[i].valor*jw),netlist[i].a,netlist[i].b,Yn,L,C);
    }
    else if (tipo=='G') {
      transadmitancia(netlist[i].valor,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='K') {
      acoplamento(netlist[i].valor,netlist[i].la,netlist[i].lb,netlist,Yn,L,C,f,cont);
    }
    else if (tipo=='I') {
      fasorcorrente(netlist[i].modulo,netlist[i].fase,netlist[i].a,netlist[i].b,Yn,L,C,cont);
    }
    else if (tipo=='V') {
      transadmitancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      transadmitancia(1,netlist[i].a,netlist[i].b,netlist[i].x,0,Yn,L,C);
      fasorcorrente(netlist[i].modulo,netlist[i].fase,netlist[i].x,0,Yn,L,C,cont);
    }
    else if (tipo=='E') {
      transadmitancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      transadmitancia(1,netlist[i].a,netlist[i].b,netlist[i].x,0,Yn,L,C);
      transadmitancia(netlist[i].valor,netlist[i].x,0,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='F') {
      transadmitancia(netlist[i].valor,netlist[i].a,netlist[i].b,netlist[i].x,0,Yn,L,C);
      transadmitancia(1,netlist[i].c,netlist[i].d,netlist[i].x,0,Yn,L,C);
      transadmitancia(1,0,netlist[i].x,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='H') {
      transadmitancia(1,0,netlist[i].y,netlist[i].a,netlist[i].b,Yn,L,C);
      transadmitancia(1,netlist[i].a,netlist[i].b,netlist[i].y,0,Yn,L,C);
      transadmitancia(netlist[i].valor,netlist[i].y,0,netlist[i].x,0,Yn,L,C);
      transadmitancia(1,netlist[i].c,netlist[i].d,netlist[i].x,0,Yn,L,C);
      transadmitancia(1,0,netlist[i].x,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='Q') {
      VBE=YnPO[L[netlist[i].b]][cont->neq+1]-YnPO[L[netlist[i].a]][cont->neq+1];
      VBC=YnPO[L[netlist[i].b]][cont->neq+1]-YnPO[L[netlist[i].c]][cont->neq+1]; 
      VEB=-VBE;
      VCB=-VBC;
      VCE=YnPO[L[netlist[i].c]][cont->neq+1]-YnPO[L[netlist[i].a]][cont->neq+1];
      VEC=-VCE;

      if (netlist[i].modelo[0] == 'N') {
        if (VBE > 0.8) {
          GBE=netlist[i].isbe*exp(0.8/netlist[i].vtbe)/netlist[i].vtbe;
          IBE=netlist[i].isbe*(exp(0.8/netlist[i].vtbe)-1) - GBE*0.8;
        } else {
          GBE=netlist[i].isbe*exp(VBE/netlist[i].vtbe)/netlist[i].vtbe;
          IBE=netlist[i].isbe*(exp(VBE/netlist[i].vtbe)-1) - GBE*VBE;
        }

        if (VBC > 0.8) {
          GBC=netlist[i].isbc*exp(0.8/netlist[i].vtbc)/netlist[i].vtbc;
          IBC=netlist[i].isbc*(exp(0.8/netlist[i].vtbc)-1) - GBC*0.8;
        } else {
          GBC=netlist[i].isbc*exp(VBC/netlist[i].vtbc)/netlist[i].vtbc;
          IBC=netlist[i].isbc*(exp(VBC/netlist[i].vtbc)-1) - GBC*VBC;
        }        

        GCE=(netlist[i].alfa*(GBE*VBE+IBE)-(GBC*VBC+IBC))/netlist[i].va;
        
        if (VCE>0) admitancia(GCE,netlist[i].c,netlist[i].a,Yn,L,C);       

        admitancia(GBE, netlist[i].b, netlist[i].a, Yn, L, C);
        admitancia(GBC, netlist[i].b, netlist[i].c, Yn, L, C);
        
        if (VBE>0.3)
          CBE=netlist[i].c0be / sqrt(0.5);
        else
          CBE=netlist[i].c0be / sqrt(1 - VBE/0.6); /* Creversa BE */
        
        if (VBE>0) {
          //admitancia(((netlist[i].c1be * (exp(0.6/netlist[i].vtbe) - 1)) * jw), netlist[i].b, netlist[i].a, Yn, L, C); /* Cdireta BE */
          if (VBE > 0.8) {
            CBE += netlist[i].c1be * (exp(0.8/(netlist[i].vtbe)) - 1);
          } else {
            CBE += netlist[i].c1be * (exp(VBE/(netlist[i].vtbe)) - 1);
          }
        }
        
        admitancia((CBE * jw), netlist[i].b, netlist[i].a, Yn, L, C);

        transadmitancia(netlist[i].alfar*GBC, netlist[i].a, netlist[i].b, netlist[i].b, netlist[i].c, Yn, L, C); 

        if (VBC>0.3)
          CBC=netlist[i].c0bc / sqrt(0.5);
        else
          CBC=netlist[i].c0bc / sqrt(1 - VBC/0.6); /* Creversa BC */        
        
        if (VBC>0) {
          //admitancia(((netlist[i].c1bc * (exp(0.6/netlist[i].vtbc) - 1)) * jw), netlist[i].b, netlist[i].c, Yn, L, C);; /* Cdireta BC */
          if (VBC > 0.8) {
            CBC+=(netlist[i].c1bc * (exp(0.8/netlist[i].vtbc) - 1));
          } else {
            CBC+=(netlist[i].c1bc * (exp(VBC/netlist[i].vtbc) - 1));
          }
        }

        admitancia((CBC * jw), netlist[i].b, netlist[i].c, Yn, L, C);

        transadmitancia(netlist[i].alfa*GBE, netlist[i].c, netlist[i].b, netlist[i].b, netlist[i].a, Yn, L, C); 
        
        printf("Transistor %s VBE %lg VBC %lg GBE %lg GBC %lg GCE %lg CBE %lg CBC %lg\n", netlist[i].nome,VBE,VBC,GBE,GBC,GCE,CBE,CBC);
      }
      else if (netlist[i].modelo[0] == 'P') {

        GEB=netlist[i].isbe*exp(VEB/netlist[i].vtbe)/netlist[i].vtbe;
        GCB=netlist[i].isbc*exp(VCB/netlist[i].vtbc)/netlist[i].vtbc;
        GEC=(netlist[i].alfa*GEB*VEB-GCB*VCB)/netlist[i].va;  
             
        if (VEC>0) admitancia(GEC,netlist[i].a,netlist[i].c,Yn,L,C);   

        admitancia(GEB, netlist[i].a, netlist[i].b, Yn, L, C);
        if (VEB>0.3)
          CEB=netlist[i].c0be / sqrt(0.5);
        else
          CEB=netlist[i].c0be / sqrt(1 - VEB/0.6); /* Creversa EB */
        //if (VEB>0) admitancia((netlist[i].c1be * (exp(VBE/netlist[i].vtbe) - 1) * jw), netlist[i].a, netlist[i].b, Yn, L, C); /* Cdireta EB */
        admitancia((CEB * jw), netlist[i].a, netlist[i].b, Yn, L, C);
        transadmitancia(netlist[i].alfar*GCB, netlist[i].b, netlist[i].a, netlist[i].c, netlist[i].b, Yn, L, C); 
        
        admitancia(GCB, netlist[i].c, netlist[i].b, Yn, L, C);
        if (VCB>0.3)
          CCB=netlist[i].c0bc / sqrt(0.5);
        else
          CCB=netlist[i].c0bc / sqrt(1 - VCB/0.6); /* Creversa CB */        
        //if (VCB>0) admitancia((netlist[i].c1bc * (exp(VCB/netlist[i].vtbc) - 1) * jw), netlist[i].c, netlist[i].b, Yn, L, C); /* Cdireta CB */
        admitancia((CCB * jw), netlist[i].c, netlist[i].b, Yn, L, C);
        transadmitancia(netlist[i].alfa*GEB, netlist[i].b, netlist[i].c, netlist[i].a, netlist[i].b, Yn, L, C); 
        
        //printf("Transistor %s VEB %lg VCB %lg GEB %lg GCB %lg GEC %lg CEB %lg CCB %lg\n", netlist[i].nome,VEB,VCB,GEB,GCB,GEC,CEB,CCB);
      }
    }  
    else if (tipo=='O');
    if (f == 1000) {
      printf("Sistema apos a estampa de %s\n", netlist[i].nome);
      imprimeSistemaDoubleComplex(Yn, cont);
    }
  }
}
