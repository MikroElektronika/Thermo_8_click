![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)

---

# Thermo8 Click

- **CIC Prefix**  : THERMO8
- **Author**      : Dusan Poluga
- **Verison**     : 1.0.0
- **Date**        : nov 2018.

---

### Software Support

We provide a library for the Thermo8 Click on our [LibStock](https://libstock.mikroe.com/projects/view/2653/thermo-8-click) 
page, as well as a demo application (example), developed using MikroElektronika 
[compilers](http://shop.mikroe.com/compilers). The demo can run on all the main 
MikroElektronika [development boards](http://shop.mikroe.com/development-boards).

**Library Description**

Library contains generic functions for working with the click board.

Key functions :

- ``` void thermo8_setResolution(uint8_t rCfg) ``` - Function for setting the measurement resolution.
- ``` uint8_t thermo8_aleGet() ``` - Function for returning the state of the alert pin.
- ``` float thermo8_getTemperatue() ``` - Function for reading the temperature.

**Examples Description**

The application is composed of three sections :

- System Initialization - Initialize the GPIO, I2C and LOG structures.
- Application Initialization - Initialize the communication interface and
                               configure the click board.
- Application Task - Wait for the interrupt pin to be triggered. When the
                   measured temperature breaches the upper or lower limit the
                   temperature value as well as the status of the breach is
                   is shown on the serial port (UART).
```.c
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
```

The full application code, and ready to use projects can be found on our 
[LibStock](https://libstock.mikroe.com/projects/view/2653/thermo-8-click) page.

Other mikroE Libraries used in the example:

- Conversions Library
- C_String Library
- I2C Library
- UART Library

**Additional notes and informations**

Depending on the development board you are using, you may need 
[USB UART click](http://shop.mikroe.com/usb-uart-click), 
[USB UART 2 Click](http://shop.mikroe.com/usb-uart-2-click) or 
[RS232 Click](http://shop.mikroe.com/rs232-click) to connect to your PC, for 
development systems with no UART to USB interface available on the board. The 
terminal available in all Mikroelektronika 
[compilers](http://shop.mikroe.com/compilers), or any other terminal application 
of your choice, can be used to read the message.

---
---
