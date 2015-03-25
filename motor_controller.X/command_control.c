#include "p24Hxxxx.h"
#include <spi.h>
#include "pwm_drive.h"

int spi_buffer[2];
int spi_index = 0;

void __attribute__((__interrupt__, no_auto_psv)) _SPI1Interrupt(void)
{
    while(!DataRdySPI1());
    spi_buffer[spi_index++]=ReadSPI1();
    if (spi_index >= 2){
        spi_index = 0;
        move(spi_buffer[0], spi_buffer[1]);
    }
    IFS0bits.SPI1IF = 0;
    SPI1STATbits.SPIROV = 0;
}

void configure_command_control(){
    TRISBbits.TRISB9 = 1;
    TRISBbits.TRISB10 = 1;
    TRISBbits.TRISB14 = 1;
    TRISBbits.TRISB11 = 0;

    CloseSPI1();

    OpenSPI1(
            ENABLE_SCK_PIN & ENABLE_SDO_PIN & SPI_MODE16_ON & SPI_SMP_OFF & SPI_CKE_OFF & SLAVE_ENABLE_ON & CLK_POL_ACTIVE_HIGH & MASTER_ENABLE_OFF,
            FRAME_ENABLE_OFF,
            SPI_ENABLE & SPI_IDLE_CON & SPI_RX_OVFLOW_CLR
    );
    ConfigIntSPI1(SPI_INT_EN & SPI_INT_PRI_1);
}
