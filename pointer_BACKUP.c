#include<18F4550.h>
#include <stdlib.h>
#fuses HS, NOWDT, PUT, BROWNOUT, NOLVP, CPUDIV1 
//#device adc=10
#use delay (clock=8000000) 
#use rs232(baud=9600, xmit = PIN_C6, rcv = PIN_C7)

//DISPLAY
//
//
#use i2c(Master,Fast=100000, sda=PIN_B0, scl=PIN_B1, force_sw)
#include <i2c_Flex_LCD.c>

#define SENSOR_ENTRADA pin_b2
#define SENSOR_SAIDA pin_b3
#define LIGADO 0
#define DESLIGADO 1

int vagas=5;
int carros=0;

int existe_carro_entrada  = 0 ;
int existe_carro_saida  = 0 ;
enum cancela{entrada,saida};

void LEITURA_CANCELA(cancela tipo_de_cancela);
void PROCESSA_CANCELA(cancela tipo_de_cancela);



void main(){

    //enable_interrupts(GLOBAL);
    //enable_interrupts(INT_timer1);
    //setup_timer_1(T1_INTERNAL|T1_DIV_BY_4);
    port_b_pullups (true); 
    set_timer1(3036);
    set_tris_d(0x00); 
    set_tris_b(0xFF);
    
   //DISPLAY
   lcd_init(0x4E,16,2);
   lcd_backlight_led(ON); //LIGAR LUZ DE FUNDO

    while(true)
    {
        LEITURA_CANCELA(entrada);
        LEITURA_CANCELA(saida);
    }

}


void LEITURA_CANCELA(cancela tipo_de_cancela)
{
    if(tipo_de_cancela == entrada)
    {
        if(input(SENSOR_ENTRADA) == LIGADO)
        {
            existe_carro_entrada = 1;
            PROCESSA_CANCELA(tipo_de_cancela);
            
            //DISPLAY
            lcd_clear();  //Limpa o LCD
            // Envio de Strings pro LCD usando a função printf
            printf(lcd_putc, "\fBEM VINDO!!!");// \F Limpa tudo na tela 
            delay_ms(1000);
            
            printf(lcd_putc, "\nVAGAS: %d c: %d", vagas, carros);
            delay_ms(1000); 
        }
        if(tipo_de_cancela != saida)
     {
            existe_carro_entrada = 0;
            PROCESSA_CANCELA(tipo_de_cancela);               
    }  
    }
    
    if(tipo_de_cancela == saida)
        {
         if(input(SENSOR_SAIDA) == LIGADO)
         {
            existe_carro_saida = 1;
            PROCESSA_CANCELA(tipo_de_cancela);
            
            //DISPLAY
            lcd_clear();  //Limpa o LCD
            // Envio de Strings pro LCD usando a função printf
            printf(lcd_putc, "\fVOLTE SEMPRE!!!");// \F Limpa tudo na tela 
            delay_ms(1000);
            
            printf(lcd_putc, "\nVAGAS: %d c: %d", vagas, carros);
            delay_ms(1000);
        }
            
     if(tipo_de_cancela != entrada)
     {
            existe_carro_saida = 0;
            PROCESSA_CANCELA(tipo_de_cancela);               
    }
    }
}

void PROCESSA_CANCELA(cancela tipo_de_cancela)
{
    if(existe_carro_entrada == 1)
     { 
        if((vagas > 0) && (carros < 6))
        {
        output_bit (pin_d2,DESLIGADO);
        //display cancela abrindo
        //while (input(SENSOR_ENTRADA) == LIGADO)
        //{
               
               //printf(" %d ",vagas);
        //}
        delay_ms(1000);
        output_bit (pin_d2,LIGADO);
        //display cancela fechando
        vagas=vagas-1;
        carros=carros+1;
        existe_carro_entrada = 0;
        }
        else
        {
            lcd_clear();  //Limpa o LCD
            printf(lcd_putc, "\fnao ha vagas");// \F Limpa tudo na tela 
            delay_ms(5000);
        }
     }
    if(existe_carro_saida == 1) 
    {
     if((carros > 0) && (vagas < 6))
       { output_bit (pin_d3,DESLIGADO);
        //while (input(SENSOR_SAIDA) == LIGADO)
        //{

        //}
        delay_ms(1000);
        output_bit (pin_d3,LIGADO);
        vagas=vagas+1;
        carros=carros-1;
        existe_carro_saida = 0;
    }
    }
}
