/*
 * Archivo: Postlab.c
 * Dispositivo: PIC16F887
 * Compilador:  XC8, MPLABX v5.40
 * Autor: José Fernando de León González
 * Programa: Contador utilizando push buttons
 * Hardware: leds y resistencias en PORTA, botones en PORTB, displays multiplexados utilizando 2N222 en PORTD & PORTC
 * 
 * Creado: 4/04/22
 * Última modificación: 4/04/22
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>



/*------------------------------------------------------------------------------
 * Constantes
------------------------------------------------------------------------------*/
#define _XTAL_FREQ 4000000      // Oscilador de 4 MHz
#define AUMENTAR PORTBbits.RB0     // Asignamos un alias a RB0
#define DISMINUIR PORTBbits.RB1     // Asignamos un alias a RB1

/*------------------------------------------------------------------------------
 * Variables
------------------------------------------------------------------------------*/
uint8_t centenas;
uint8_t decenas;
uint8_t unidades;
uint8_t residuo;

uint8_t flags = 0;
uint8_t display_val [3]; 

uint8_t tabla [16] = {0b00111111,       //[0] 0
                      0b00000110,       //[1] 1
                      0b01011011,       //[2] 2
                      0b01001111,       //[3] 3
                      0b01100110,       //[4] 4
                      0b01101101,       //[5] 5
                      0b01111101,       //[6] 6
                      0b00000111,       //[7] 7
                      0b01111111,       //[8] 8
                      0b01101111,       //[9] 9
                      0b01110111,       //[10] A
                      0b01111100,       //[11] B
                      0b00111001,       //[12] C
                      0b01011110,       //[13] D
                      0b01111001,       //[14] E
                      0b01110001};      //[15] F

/*------------------------------------------------------------------------------
 * Prototipos de funciones
------------------------------------------------------------------------------*/
void setup (void);
void displays (uint8_t num);

/*------------------------------------------------------------------------------
 * Interrupciones
------------------------------------------------------------------------------*/
void __interrupt() isr (void)
{
    if (INTCONbits.RBIF)  
    {
        
        if (!AUMENTAR)
        {
            PORTA = ++PORTA;    
        }    
        if (!DISMINUIR)
        {
            PORTA = --PORTA;       
        }
        INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupción RBIF
    }
    if (INTCONbits.T0IF)
    {
        PORTD = 0;
        
        if (flags == 0)
        {
            PORTC = display_val[0];
            PORTD = 1;
            flags = 1;
        }
        else if (flags == 1)
        {
            PORTC = display_val[1];
            PORTD = 2;
            flags = 2;
        }
        else if (flags == 2)
        {
            PORTC = display_val[2];
            PORTD = 4;
            flags = 0;
        }
        TMR0 = 6;
        INTCONbits.T0IF = 0;
    }
    return;
}
/*------------------------------------------------------------------------------
 * Ciclo principal
------------------------------------------------------------------------------*/
void main(void) {
    setup();
    while(1){
    
    displays(123);
    
    display_val [0] = tabla[unidades];
    display_val [1] = tabla[decenas];
    display_val [2] = tabla[centenas];
            
    }        
    return;
}

/*------------------------------------------------------------------------------
 * Configuración
------------------------------------------------------------------------------*/
void setup (void){
    ANSEL = 0;                  //pines digitales
    ANSELH = 0;
    
    TRISA = 0;                  // PORTA como salida
    PORTA = 0;                  // Limpiamos PORTA 
    
    TRISC = 0;                  // PORTC como salida
    PORTC = 0;                  // Limpiamos PORTC 
    
    TRISD = 0;                  // PORTD como salida
    PORTD = 0;                  // limpiamos PORTD
    
    TRISB = 0b00000011;         // RB0 & RB1 de PORTB como entradas
    
    
    
    OSCCONbits.IRCF = 0b011;    // IRCF <2:0> -> 011 500 kHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    
    
    
    OPTION_REGbits.nRBPU = 0;   // Habilitamos resistencias de pull-up del PORTB
    WPUBbits.WPUB0 = 1;         // Habilitamos resistencia de pull-up de RB0 & RB1
    WPUBbits.WPUB1 = 1;
    
    
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones de los puertos
    
    INTCONbits.RBIE = 1;        // Habilitamos interrupciones del PORTB
    INTCONbits.TMR0IE = 1;     // activamos interrupciones del TIMER0
    
    
    
    
    IOCBbits.IOCB0 = 1;         // Habilitamos IOCB en RB0
    IOCBbits.IOCB1 = 1;         // Habilitamos IOCB en RB1
    INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupción de PORTB
    
    
    OPTION_REGbits.T0CS = 0;    // TIMER0 como temporizador
    OPTION_REGbits.PSA = 0;     // Prescaler a TIMER0
    OPTION_REGbits.PS = 0b000;  // PS<2:0> -> 000 Prescaler 1:1
    
    TMR0 = 6;
    INTCONbits.T0IF = 0;
    
    
   
}
/*------------------------------------------------------------------------------
 * Funciones
------------------------------------------------------------------------------*/

void displays (uint8_t num)
{

    centenas = num/100;
    residuo = num%100;
    
    decenas = residuo/10;
    residuo = residuo%10;
    
    unidades = residuo;
    
    return;
}