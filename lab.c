/*
 * Archivo: Prelab.c
 * Dispositivo: PIC16F887
 * Compilador:  XC8, MPLABX v5.40
 * Autor: José Fernando de León González
 * Programa: Contador utilizando push buttons
 * Hardware: leds y resistencias en PORTA, botones en PORTB
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


/*------------------------------------------------------------------------------
 * Prototipos de funciones
------------------------------------------------------------------------------*/
void setup (void);

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
    if (PIR1bits.TMR1IF)
    {
        PORTC = PORTC++;
        
        TMR1H = 0x0B;               // Reiniciar TIMER1
        TMR1L = 0xDC;
        PIR1bits.TMR1IF = 0;
    }    
    return;
}
/*------------------------------------------------------------------------------
 * Ciclo principal
------------------------------------------------------------------------------*/
void main(void) {
    setup();
    while(1){
         
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
    
    TRISB = 0b00000011;         // RB0 & RB1 de PORTB como entradas
    
    OPTION_REGbits.nRBPU = 0;   // Habilitamos resistencias de pull-up del PORTB
    WPUBbits.WPUB0 = 1;         // Habilitamos resistencia de pull-up de RB0 & RB1
    WPUBbits.WPUB1 = 1;
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones de los puertos
    
    INTCONbits.RBIE = 1;        // Habilitamos interrupciones del PORTB
    PIE1bits.TMR1IE = 1;       // Activamos interrupciones del TIMER1
    
    IOCBbits.IOCB0 = 1;         // Habilitamos IOCB en RB0
    IOCBbits.IOCB1 = 1;         // Habilitamos IOCB en RB1
    INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupción de PORTB

    OSCCONbits.IRCF = 0b011;    // IRCF <2:0> -> 011 500 kHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    
    T1CONbits.TMR1GE = 0;       // Siempre contando
    T1CONbits.T1CKPS = 0b01;    // T1CKPS <1:0> -> 01 Prescaler 1:2 
    T1CONbits.T1OSCEN = 0;      // reloj interno
    T1CONbits.TMR1CS = 0;
    T1CONbits.TMR1ON = 1;       // Encender TIMER1
    
    TMR1H = 0x0B;               // Reiniciar TIMER1
    TMR1L = 0xDC;
    
    PIR1bits.TMR1IF = 0;       // Limpiamos la bandera de interrupción del TIMER1
}
/*------------------------------------------------------------------------------
 * Funciones
------------------------------------------------------------------------------*/

