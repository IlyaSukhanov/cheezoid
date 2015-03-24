#include "p24Hxxxx.h"
#include <spi.h>
#include "pwm_drive.h"

//int bias_compensation;
//int rotation_distance = 0;
//int drive_distance = 0;
//unsigned int word_index = 0;

void __attribute__((__interrupt__, no_auto_psv)) _SPI1Interrupt(void)
{
    int data;
    if(DataRdySPI1()){
        data = ReadSPI1();
        //move_to(10,10);
        if (data==0xaa){
            move_to(50, 0);
        }

        /*
        switch(word_index++){
            case 0:
                bias_compensation = ReadSPI1();
                break;
            case 1:
                rotation_distance = ReadSPI1();
                break;
            case 0:
                drive_distance = ReadSPI1();
                word_index = 0;
                move(drive_distance, drive_distance);
        }
        */
    }
    //move(10,10);
    //DisableIntSPI1;
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
    ConfigIntSPI1(SPI_INT_EN & SPI_INT_PRI_2);
}
