/* 

Trabalho Final de Circuitos Elétricos 2 - 2017.1

Emerson Santos
Juan Vazquez
Pedro Reis

Elementos aceitos:

Resistor: R<nome> <nó1> <nó2> <Resistência>
Indutor: L<nome> <nó1> <nó2> <Indutância>
Acoplamento entre indutores: K<nome> <La> <Lb> <k> (La e Lb nomes de indutores já declarados.)
Capacitor: C<nome> <nó1> <nó2> <Capacitância>
Fonte de tensão controlada a tensão: E<nome> <nóV+> <nóV-> <nóv+> <nóv-> <Av>
Fonte de corrente controlada a corrente: F<nome> <nóI+> <nóI-> <nói+> <nói-> <Ai>
Fonte de corrente controlada a tensão: G<nome> <nóI+> <nóI-> <nóv+> <nóv-> <Gm>
Fonte de tensão controlada a corrente: H<nome> <nóV+> <nóV-> <nói+> <nói-> <Rm>
Fonte de corrente: I<nome> <nó+> <nó-> <módulo> <fase (graus)> <valor contínuo>
Fonte de tensão: V<nome> <nó+> <nó-> <módulo> <fase (graus)> <valor contínuo>
Amplificador operacional ideal: O<nome> <nó saída+> <nó saída-> <nó entrada+> <nó entrada->
Transistor bipolar: Q<nome> <nóc> <nób> <nóe> <tipo> <α> <αr> <Isbe> <VTbe> <Isbc> <VTbc> <VA>
<C0be> <C1be> <C0bc> <C1bc>

*/

#define OK 0
#include <stdio.h>

int main (void) {
  printf("Ok\n");

  return OK;
}