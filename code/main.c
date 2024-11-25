#include <xc.h>
#include <stdint.h>
#include "Pic32Ini.h"
#include "Servo.h"
#include "UART.h"

#define PIN_PUL 5
#define PIN_LED_0 0
#define PIN_LED_1 1
#define vRef 3.3

void initSensor(void);

    
uint32_t valor = 0 ;

int main(void) {
    
    int puls_ant, puls_act;
    
    ANSELC &= ~0xF;

    // Definición de entradas
    TRISA = 0;
    TRISB |=(1<<PIN_PUL); //pulsador, potenciometro como entradas
    TRISC = 0;

    LATC = 0xF; // Apago los LEDs 
    
    initSensor();
    initServo();
    initUART();
    
    puls_ant = (PORTB >> PIN_PUL) & 1;
    
    while(1){
        
        initPines(valor);
        
        puls_act = (PORTB >> PIN_PUL) & 1;
        
        if( (puls_act != puls_ant) && (puls_act == 0)){
            
            posicionServo(valor);
            escribirUART(valor);
          
            puls_ant = puls_act;
        }
    }
}

void initPines(uint32_t valorAlcohol){
    
    if(valorAlcohol < 880){
        LATC = 0xF;
        LATCCLR = 1 << PIN_LED_0;
    }
    else{
        LATC = 0xF;
        LATCCLR = 1 << PIN_LED_1;
    }
}

void initSensor(void){
    
    AD1CON1 = 0; 
    AD1CON2 = 3<<2; 
    AD1CON3 = 0x0200; 
    AD1CHS = 0x00050000; // CH0SA = AN5, CH0NA = GND
    
    AD1CON1bits.SSRC = 2; // Conv. Disparadas por T3
    AD1CON1bits.ASAM = 1; // Automatic Sampling
    IFS0bits.AD1IF = 0; // Borro el flag de interrupción
    IEC0bits.AD1IE = 1; // Habilito la interrupción
    IPC5bits.AD1IP = 2; // Prioridad 2
    IPC5bits.AD1IS = 0; // Subprioridad 0
    AD1CON1bits.ON = 1; // Arranco el ADC

    T3CON = 0;
    TMR3 = 0;
    PR3 = 5000; 
    T3CON = 0x8000; 

    INTCONbits.MVEC = 1; // Modo multivector
    asm(" ei"); 
}

__attribute__((vector(23), interrupt(IPL2SOFT), nomips16))
 void interrupcionSensor(void)
 {

    IFS0bits.AD1IF = 0; // Se borra el flag

    valor = (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3) >> 2;
 }
