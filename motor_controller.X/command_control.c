#include "p24Hxxxx.h"
#include <spi.h>
#include "pwm_drive.h"

int spi_buffer[2];
int spi_index = 0;
int spi_buff_full = 0;
int spi_write = 0;

void __attribute__((__interrupt__, no_auto_psv)) _SPI1Interrupt(void) {
    while(!DataRdySPI1());
    if(spi_buff_full){
        //discard data
        ReadSPI1();
        spi_index++;
        if (spi_index >= 2){
            spi_index = 0;
            WriteSPI1(0xbeef);
        }else{
            WriteSPI1(0xcafe);
        }
    }else{
        spi_buffer[spi_index++]=ReadSPI1();
        if (spi_index >= 2){
            spi_index = 0;
            spi_buff_full = 1;
            WriteSPI1(0xbabe);
        }else{
            WriteSPI1(0x0000);
        }
    }
    IFS0bits.SPI1IF = 0;
    SPI1STATbits.SPIROV = 0;
}

// return address of buffer if its full, else NULL
int *read_spi_buffer(){
    WriteSPI1(0x0000); 
    if (spi_buff_full){
        spi_buff_full = 0;
        return spi_buffer;
    }else{
        return 0;
    }
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
