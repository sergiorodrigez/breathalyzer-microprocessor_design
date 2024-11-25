/* 
 * File:   UART.c
 * Author: sergi
 *
 * Created on 24 de abril de 2023, 17:50
 */

#include <xc.h>
#include <stdint.h>
#include "Pic32Ini.h"
#include "Servo.h"
#include "UART.h"

static char msj1[] = "\n\r NEGATIVO! disfruta la travesia\n\r";
static char msj2[] = "\n\r POSITVO! no conduzcas que te matas animal\n\r";
/*
 * 
 */
void initUART(void){
    
    SYSKEY=0xAA996655;
    SYSKEY=0x556699AA;
    U1RXR = 3;
    RPB7R = 1;
    SYSKEY=0x1CA11CA1;
    
    U1BRG = 32; // 9600 baudios
    
    U1STAbits.URXEN = 1;
    U1MODE = 0x8000;
}

void writeUART(char *msj){
    int i = 0;
    U1STAbits.UTXEN = 1;
                
    while (msj[i] != '\0') {
                    
        U1TXREG = msj[i];
        while (U1STAbits.TRMT == 0);
        i++;
    }
                
    U1STAbits.UTXEN = 0;
}

void escribirUART(uint32_t valor){
    if (valor < 880){
        writeUART(msj1);
    }
    else{  
        writeUART(msj2);
    }
}
