#define led1 pin_d1
#include <18F4550.h>
#fuses HS, CPUDIV1, PLL5, USBDIV //Configuração dos fusíveis
#use delay(clock = 20000000)
#use rs232(baud = 9600, xmit = PIN_C6, rcv = PIN_C7)

const int16 pos_min = 500, pos_max = 2400; //Valores de ajuste do servo
long int angulo = 0, tempo2;
float tempo;
int selecao;
int estado_anterior, n2 = 20;

void servo(int angulo)
{
   
   do {
      output_high(pin_d6);
      tempo = (angulo * 0.00555555555);//converte para % em float
      tempo2 = (tempo * (pos_max - pos_min)) + pos_min; //converte para tempo
      delay_us(tempo2);
      output_low(pin_d6);
      delay_ms(20);
      n2--;
      }
      
   while (n2 > 0);
   n2 = 20;
}

void main()
{
   port_b_pullups(TRUE);
   while (true)
   {
      if (input(pin_b0) == 0)
      {
         estado_anterior = 1;
      }
      
      if ((input(pin_b0) == 1) && (estado_anterior == 1))
      {
         estado_anterior = 0; selecao = selecao + 1; if (selecao > 5) selecao = 1;
      }
      
      if (selecao == 1)
      {
         printf(" Posicao 1 "); servo(0);
      }
      
      if (selecao == 2)
      {
         printf(" Posicao 2 "); servo(45);
      }
      
      if (selecao == 3)
      {
         printf(" Posicao 3"); servo(90);
      }

      if (selecao == 4)
      {
         printf(" Posicao 4 "); servo(135);
      }
      
      if (selecao == 5)
      {
         printf(" Posicao 5 "); servo(180);
      }
   }
}
