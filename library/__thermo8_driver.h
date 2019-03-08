/*
    __thermo8_driver.h

-----------------------------------------------------------------------------

  This file is part of mikroSDK.
  
  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

/**
@file   __thermo8_driver.h
@brief    Thermo8 Driver
@mainpage Thermo8 Click
@{

@image html libstock_fb_view.jpg

@}

@defgroup   THERMO8
@brief      Thermo8 Click Driver
@{

| Global Library Prefix | **THERMO8** |
|:---------------------:|:-----------------:|
| Version               | **1.0.0**    |
| Date                  | **nov 2018.**      |
| Developer             | **Dusan Poluga**     |

*/
/* -------------------------------------------------------------------------- */

#include "stdint.h"

#ifndef _THERMO8_H_
#define _THERMO8_H_

/** 
 * @macro T_THERMO8_P
 * @brief Driver Abstract type 
 */
#define T_THERMO8_P    const uint8_t*

/** @defgroup THERMO8_COMPILE Compilation Config */              /** @{ */

//  #define   __THERMO8_DRV_SPI__                            /**<     @macro __THERMO8_DRV_SPI__  @brief SPI driver selector */
   #define   __THERMO8_DRV_I2C__                            /**<     @macro __THERMO8_DRV_I2C__  @brief I2C driver selector */                                          
// #define   __THERMO8_DRV_UART__                           /**<     @macro __THERMO8_DRV_UART__ @brief UART driver selector */ 

                                                                       /** @} */
/** @defgroup THERMO8_VAR Variables */                           /** @{ */

const uint8_t THERMO8_ADDR0           ;
const uint8_t THERMO8_ADDR1           ;
const uint8_t THERMO8_ADDR2           ;
const uint8_t THERMO8_ADDR3           ;
const uint8_t THERMO8_ADDR4           ;
const uint8_t THERMO8_ADDR5           ;
const uint8_t THERMO8_ADDR6           ;
const uint8_t THERMO8_ADDR7           ;

const uint8_t THERMO8_CONFIG          ;

const uint8_t THERMO8_TUPPER          ;
const uint8_t THERMO8_TLOWER          ;
const uint8_t THERMO8_TCRIT           ;
const uint8_t THERMO8_TUPPER_REACHED  ;
const uint8_t THERMO8_TLOWER_REACHED  ;
const uint8_t THERMO8_TCRIT_REACHED   ;
const uint8_t THERMO8_TCRIT_ONLY_ALERT;
const uint8_t THERMO8_ALERT_ON_ALL    ;
const uint8_t THERMO8_THYS_0C         ;
const uint8_t THERMO8_THYS_1C5        ;
const uint8_t THERMO8_THYS_3C         ;
const uint8_t THERMO8_THYS_6C         ;

const uint8_t THERMO8_TA              ;
const uint8_t THERMO8_MANID           ;
const uint8_t THERMO8_DEVID           ;

const uint8_t THERMO8_RESOLUTION_REG  ;
const uint8_t THERMO8_R05C_30MS       ;
const uint8_t THERMO8_R025C_65MS      ;
const uint8_t THERMO8_R0125C_130MS    ;
const uint8_t THERMO8_R00625C_250MS   ;
                                                                       /** @} */
/** @defgroup THERMO8_TYPES Types */                             /** @{ */



                                                                       /** @} */
#ifdef __cplusplus
extern "C"{
#endif

/** @defgroup THERMO8_INIT Driver Initialization */              /** @{ */
#ifdef   __THERMO8_DRV_I2C__
void thermo8_i2cDriverInit(T_THERMO8_P gpioObj, T_THERMO8_P i2cObj, uint8_t slave);
#endif
                                                                       /** @} */
/** @defgroup THERMO8_FUNC Driver Functions */                   /** @{ */

/**
   Function for returning the state of the alert pin.
*/
uint8_t thermo8_aleGet();

/**
   Generic function for writing to 16 bit registers.
*/
void thermo8_writeReg(uint8_t rAddr, uint16_t rData);

/**
   Generic function for reading from 16 bit registers.
*/
uint16_t thermo8_readReg(uint8_t rAddr);

/**
   Generic function for single byte writes.
*/
void thermo8_writeReg8(uint8_t rAddr, uint8_t rData);

/**
   Generic function for single byte read's.
*/
uint8_t thermo8_readReg8(uint8_t rAddr);

/**
   Function will return the temperature value in °C.
*/
float thermo8_getTemperatue();

/**
   Function will set the conversion resoult temperature step depending on the
   passed constant.
   
   Available constants:

     - THERMO8_R05C_30MS     - Change of 0.5°C
     - THERMO8_R025C_65MS    - Change of 0.25°C
     - THERMO8_R0125C_130MS  - Change of 0.125°C
     - THERMO8_R00625C_250MS - Change of 0.0625°C
*/
void thermo8_setResolution(uint8_t rCfg);

/**
   Function will return the device ID.
   Example: 0x0400
*/
uint16_t thermo8_getDevid();

/**
   Function will return the manufacturer ID.
   Example: 0x0054
*/
uint16_t thermo8_getManid();

/**
   Function will place Thermo 8 to the low power mode.
   To read the data from the sensor you will need to call the
   thermo8_wakeup() function.
*/
void thermo8_sleep();

/**
   Function for waking up the click board from the sleep mode.
*/
void thermo8_wakeup();

/**
   Function for setting the temperature alarm levels for the
   lower, upper and critical alert levels.
   
   @params:
       limitRegaddr - limit register address constant
       limit - the temperature value that you want to limit the alert to.

   @example:
    -thermo8_limitSet(THERMO8_TCRIT, 30.0); - limit the critical alert to 30.0°C
*/
void thermo8_limitSet(uint8_t limitRegaddr, float limit);

/**
   Function will return all alert states. The alert can be tested 
   with the predefined constants.
   
   Available constants:
   
    - THERMO8_TUPPER_REACHED
    - THERMO8_TLOWER_REACHED
    - THERMO8_TCRIT_REACHED
*/
uint8_t thermo8_getAlertstat();

/**
   Function for setting the Tupper and Tlower hysterezis level as well as the
   alert mode.
   
   @params:
           - thys - Hysterezis level
           * Possible setting constants: 
                                         - THERMO8_THYS_0C
                                         - THERMO8_THYS_1C5
                                         - THERMO8_THYS_3C
                                         - THERMO8_THYS_6C
           - alertCfg - Set the alert trigger.
            *Possible setting constants: 
                                         - THERMO8_TCRIT_ONLY_ALERT
                                         - THERMO8_ALERT_ON_ALL
*/
void thermo8_alertEnable(uint8_t thys, uint8_t alertCfg);

/**
   Function for locking the critical temperature setting register.
   By default at powerup the register is unlocked.
*/
void thermo8_tcritLock();

/**
   Function for unlocking the critical temperature setting register.
*/
void thermo8_tcritUnlock();

/**
   Function for locking the Tupper and Tlower registers.
   By default at powerup the registers are unlocked.
*/
void thermo8_winLock();

/**
   Function for unlocking the Tupper and Tlower registers.
*/
void thermo8_winUnlock();






                                                                       /** @} */
#ifdef __cplusplus
} // extern "C"
#endif
#endif

/**
    @example Click_Thermo8_STM.c
    @example Click_Thermo8_TIVA.c
    @example Click_Thermo8_CEC.c
    @example Click_Thermo8_KINETIS.c
    @example Click_Thermo8_MSP.c
    @example Click_Thermo8_PIC.c
    @example Click_Thermo8_PIC32.c
    @example Click_Thermo8_DSPIC.c
    @example Click_Thermo8_AVR.c
    @example Click_Thermo8_FT90x.c
    @example Click_Thermo8_STM.mbas
    @example Click_Thermo8_TIVA.mbas
    @example Click_Thermo8_CEC.mbas
    @example Click_Thermo8_KINETIS.mbas
    @example Click_Thermo8_MSP.mbas
    @example Click_Thermo8_PIC.mbas
    @example Click_Thermo8_PIC32.mbas
    @example Click_Thermo8_DSPIC.mbas
    @example Click_Thermo8_AVR.mbas
    @example Click_Thermo8_FT90x.mbas
    @example Click_Thermo8_STM.mpas
    @example Click_Thermo8_TIVA.mpas
    @example Click_Thermo8_CEC.mpas
    @example Click_Thermo8_KINETIS.mpas
    @example Click_Thermo8_MSP.mpas
    @example Click_Thermo8_PIC.mpas
    @example Click_Thermo8_PIC32.mpas
    @example Click_Thermo8_DSPIC.mpas
    @example Click_Thermo8_AVR.mpas
    @example Click_Thermo8_FT90x.mpas
*/                                                                     /** @} */
/* -------------------------------------------------------------------------- */
/*
  __thermo8_driver.h

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */