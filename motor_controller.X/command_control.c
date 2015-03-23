#include "p24Hxxxx.h"
#include <spi.h>
#include "pwm_drive.h"

void __attribute__((__interrupt__, no_auto_psv)) _SPI1Interrupt(void)
{
    unsigned int data;
    unsigned int direction;
    unsigned int distance=0;
    if(DataRdySPI1()){
        data = ReadSPI1();
        distance = data & 0x7f;
        direction = data >> 7;
        right_drive(distance, direction);
        left_drive(distance, direction);
    }
    IFS0bits.SPI1IF = 0;
    SPI1STATbits.SPIROV = 0;
}

void configure_command_control(){
    CloseSPI1();

    OpenSPI1(
            ENABLE_SCK_PIN & ENABLE_SDO_PIN & SPI_MODE16_OFF & SPI_SMP_OFF & SPI_CKE_OFF & SLAVE_ENABLE_OFF & CLK_POL_ACTIVE_HIGH & MASTER_ENABLE_OFF,
            FRAME_ENABLE_OFF,
            SPI_ENABLE & SPI_IDLE_CON & SPI_RX_OVFLOW_CLR
    );
    ConfigIntSPI1(SPI_INT_EN & SPI_INT_PRI_1);
}
/**
 // Following code snippet shows SPI register configuration for SLAVE Mode
SPI1BUF = 0;
IFS0bits.SPI1IF = 0; //Clear the Interrupt Flag
IEC0bits.SPI1IE = 0; //Disable The Interrupt
// SPI1CON1 Register Settings
SPI1CON1bits.DISSCK = 0; //Internal Serial Clock is Enabled
SPI1CON1bits.DISSDO = 0; //SDOx pin is controlled by the module
SPI1CON1bits.MODE16 = 1; //Communication is word-wide (16 bits)
SPI1CON1bits.SMP = 0; //Input Data is sampled at the middle of data
//Output time
SPI1CON1bits.CKE = 0; //Serial output data changes on transition
//from Idle clock state to active clock state
SPI1CON1bits.CKP = 0; //Idle state for clock is a low level; active
//state is a high level
SPI1CON1bits.MSTEN = 0; //Master Mode disabled
SPI1STATbits.SPIROV=0; //No Receive Overflow has Occurred
SPI1STATbits.SPIEN = 1; //Enable SPI Module
//Interrupt Controller Settings
IFS0bits.SPI1IF = 0; //Clear the Interrupt Flag
IEC0bits.SPI1IE = 1; //Enable The Interrupt
 */