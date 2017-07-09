# Análise de Circuitos Lineares

Analisa circuitos lineares encontrando seu ponto de operação e a resposta em frequência para pequenos sinais.

## Compilar
gcc -lm main.c mna.c resolver.c netlist.c -o main

## Netlists
Formato aceito de netlists:

Primeira linha: Comentário  
Linhas seguintes: Descrição do circuito, com um elemento por linha. A primeira letra determina o tipo de elemento.  
Resistor: R[nome] [nó1] [nó2] [Resistência]  
Indutor: L[nome] [nó1] [nó2] [Indutância]  
Acoplamento entre indutores: K[nome] [La] [Lb] [k] (La e Lb nomes de indutores já declarados.)  
Capacitor: C[nome] [nó1] [nó2] [Capacitância]  
Fonte de tensão controlada a tensão: E[nome] [nóV+] [nóV-] [nóv+] [nóv-] [Av]  
Fonte de corrente controlada a corrente: F[nome] [nóI+] [nóI-] [nói+] [nói-] [Ai]  
Fonte de corrente controlada a tensão: G[nome] [nóI+] [nóI-] [nóv+] [nóv-] [Gm]  
Fonte de tensão controlada a corrente: H[nome] [nóV+] [nóV-] [nói+] [nói-] [Rm]  
Fonte de corrente: I[nome] [nó+] [nó-] [módulo] [fase (graus)] [valor contínuo]  
Fonte de tensão: V[nome] [nó+] [nó-] [módulo] [fase (graus)] [valor contínuo]  
Amplificador operacional ideal: O[nome] [nó saída+] [nó saída-] [nó entrada+] [nó entrada-]  
Transistor bipolar: Q[nome] [nóc] [nób] [nóe] [tipo] [α] [αr] [Isbe] [VTbe] [Isbc] [VTbc] [VA] [C0be] [C1be] [C0bc] [C1bc]  
.AC [LIN ou OCT ou DEC] [pontos] [início] [fim]  
Comentário: *[comentário]  

## Saída
O programa salva um arquivo .tab de mesmo nome do netlist analisado. O resultado pode ser plotado utilizando o [rfnlin](http://www.coe.ufrj.br/~acmq/cursos/rfnlin.zip)