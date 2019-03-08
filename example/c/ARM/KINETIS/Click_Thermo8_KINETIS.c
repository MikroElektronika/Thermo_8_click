/*
Example for Thermo8 Click

    Date          : nov 2018.
    Author        : Dusan Poluga

Test configuration KINETIS :
    
    MCU              : MK64
    Dev. Board       : HEXIWEAR
    ARM Compiler ver : v6.0.0.0

---

Description :

The application is composed of three sections :

- System Initialization - Initialize the GPIO, I2C and LOG structures.
- Application Initialization - Initialize the communication interface and
                               configure the click board.
- Application Task - Wait for the interrupt pin to be triggered. When the
                   measured temperature breaches the upper or lower limit the
                   temperature value as well as the status of the breach is
                   is shown on the serial port (UART).
*/

#include "Click_Thermo8_types.h"
#include "Click_Thermo8_config.h"

void systemInit()
{
     mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_INT_PIN, _GPIO_INPUT );
     mikrobus_i2cInit( _MIKROBUS1, &_THERMO8_I2C_CFG[0] );
     mikrobus_logInit( _MIKROBUS2, 57600 );
     Delay_ms( 100 );
}

void applicationInit()
{
     thermo8_i2cDriverInit( (T_THERMO8_P)&_MIKROBUS1_GPIO, (T_THERMO8_P)&_MIKROBUS1_I2C, THERMO8_ADDR0 );
     mikrobus_logWrite("System initialized",_LOG_LINE);
     Delay_ms(100);
     
     thermo8_setResolution( THERMO8_R025C_65MS );
     thermo8_limitSet(THERMO8_TLOWER, 27.0);
     thermo8_limitSet(THERMO8_TUPPER, 28.0);
     thermo8_alertEnable(THERMO8_THYS_0C,THERMO8_ALERT_ON_ALL);
}

void applicationTask()
{
   float T_Data;
   char text[15];
   char alert;
   char alertOn;

   Delay_ms(2000);
   alert = thermo8_aleGet();

   if(alert == 0)
   {
      T_Data  = thermo8_getTemperatue();
      alertOn = thermo8_getAlertstat();
      FloatToStr(T_Data,&text[0]);
      text[5] = 0;
   }

   if(alertOn & THERMO8_TLOWER_REACHED)
   {
      mikrobus_logWrite("Temperature under the low limit: ",_LOG_TEXT);
      mikrobus_logWrite(&text[0],_LOG_TEXT);
      mikrobus_logWrite("°C",_LOG_LINE);
   }

   if(alertOn & THERMO8_TUPPER_REACHED)
   {
      mikrobus_logWrite("Temperature over the high limit: ",_LOG_TEXT);
      mikrobus_logWrite(&text[0],_LOG_TEXT);
      mikrobus_logWrite("°C",_LOG_LINE);
   }
}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
            applicationTask();
    }
}
