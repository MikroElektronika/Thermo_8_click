/*
    __thermo8_driver.c

-----------------------------------------------------------------------------

  This file is part of mikroSDK.

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

#include "__thermo8_driver.h"
#include "__thermo8_hal.c"

/* ------------------------------------------------------------------- MACROS */
// Temperature range -20 - +100
const uint8_t THERMO8_ADDR0                           = 0x18; //def addr
const uint8_t THERMO8_ADDR1                           = 0x18 | 0x01;
const uint8_t THERMO8_ADDR2                           = 0x18 | 0x02;
const uint8_t THERMO8_ADDR3                           = 0x18 | 0x03;
const uint8_t THERMO8_ADDR4                           = 0x18 | 0x04;
const uint8_t THERMO8_ADDR5                           = 0x18 | 0x05;
const uint8_t THERMO8_ADDR6                           = 0x18 | 0x06;
const uint8_t THERMO8_ADDR7                           = 0x18 | 0x07;

const uint8_t THERMO8_CONFIG                          = 0x01;

const uint8_t THERMO8_TUPPER                          = 0x02;
const uint8_t THERMO8_TLOWER                          = 0x03;
const uint8_t THERMO8_TCRIT                           = 0x04;
const uint8_t THERMO8_TUPPER_REACHED                  = 0x03;
const uint8_t THERMO8_TLOWER_REACHED                  = 0x0C;
const uint8_t THERMO8_TCRIT_REACHED                   = 0x30;
const uint8_t THERMO8_TCRIT_ONLY_ALERT                = 1;
const uint8_t THERMO8_ALERT_ON_ALL                    = 0;
const uint8_t THERMO8_THYS_0C                         = 0x00;
const uint8_t THERMO8_THYS_1C5                        = 0x01;
const uint8_t THERMO8_THYS_3C                         = 0x02;
const uint8_t THERMO8_THYS_6C                         = 0x03;

const uint8_t THERMO8_TA                              = 0x05;
const uint8_t THERMO8_MANID                           = 0x06;
const uint8_t THERMO8_DEVID                           = 0x07;

const uint8_t THERMO8_RESOLUTION_REG                  = 0x08;
const uint8_t THERMO8_R05C_30MS                       = 0x00;
const uint8_t THERMO8_R025C_65MS                      = 0x01;
const uint8_t THERMO8_R0125C_130MS                    = 0x02;
const uint8_t THERMO8_R00625C_250MS                   = 0x03;

/* ---------------------------------------------------------------- VARIABLES */

#ifdef   __THERMO8_DRV_I2C__
static uint8_t _slaveAddress;
#endif

static uint16_t thermo8Limitstatus;

/* -------------------------------------------- PRIVATE FUNCTION DECLARATIONS */
float _btoTconversion(uint16_t rData);


/* --------------------------------------------- PRIVATE FUNCTION DEFINITIONS */
float _btoTconversion(uint16_t rData)
{
    float T;
    uint8_t regUpper = (uint8_t)( rData>>8 );
    uint8_t regLower = (uint8_t)( rData );
    
    regUpper &= 0x1F;
    if( regUpper & 0x10 )
    {
      regUpper &= 0x0F;
      T = 256.0 - ( ( (float)(regUpper) * 16.0 ) + ( (float)(regLower)/16.0 ) );
    }
    else
    {
      T = ( ( (float)(regUpper) * 16.0 ) + ( (float)(regLower)/16.0 ) );
    }
    
    return T;
}


/* --------------------------------------------------------- PUBLIC FUNCTIONS */
#ifdef   __THERMO8_DRV_I2C__

void thermo8_i2cDriverInit(T_THERMO8_P gpioObj, T_THERMO8_P i2cObj, uint8_t slave)
{
    _slaveAddress = slave;
    hal_i2cMap( (T_HAL_P)i2cObj );
    hal_gpioMap( (T_HAL_P)gpioObj );
}

#endif
/* ----------------------------------------------------------- IMPLEMENTATION */
uint8_t thermo8_aleGet()
{
    return hal_gpio_intGet();
}

void thermo8_writeReg(uint8_t rAddr, uint16_t rData)
{
   uint8_t rBuf[3];
   rBuf[0] = rAddr;
   rBuf[1] = (uint8_t)((rData>>8) & 0xFF);
   rBuf[2] = (uint8_t)(rData & 0xFF);
   hal_i2cStart();
   hal_i2cWrite(_slaveAddress,rBuf,3,END_MODE_STOP);
}

uint16_t thermo8_readReg(uint8_t rAddr)
{
  uint8_t rBuf[3];
  rBuf[0] = rAddr;
  hal_i2cStart();
  hal_i2cWrite(_slaveAddress,rBuf,1,END_MODE_RESTART);
  hal_i2cRead(_slaveAddress,rBuf,2,END_MODE_STOP);

  return ( (uint16_t)rBuf[0]<<8 | rBuf[1] ) ;
}

void thermo8_writeReg8(uint8_t rAddr, uint8_t rData)
{
   uint8_t rBuf[3];
   rBuf[0] = rAddr;
   rBuf[1] = rData;
   hal_i2cStart();
   hal_i2cWrite(_slaveAddress,rBuf,2,END_MODE_STOP);
}
uint8_t thermo8_readReg8(uint8_t rAddr)
{
  uint8_t rBuf[2];
  rBuf[0] = rAddr;
  hal_i2cStart();
  hal_i2cWrite(_slaveAddress,rBuf,1,END_MODE_RESTART);
  hal_i2cRead(_slaveAddress,rBuf,1,END_MODE_STOP);

  return rBuf[0];
}

float thermo8_getTemperatue()
{
  float tTemp;
  uint16_t tData;

  tData=thermo8_readReg(THERMO8_TA);
  thermo8Limitstatus = tData;
  tTemp = _btoTconversion(tData);
  return tTemp;
}

void thermo8_setResolution(uint8_t rCfg)
{
   uint8_t tmp;

   tmp = rCfg & 0x03;
   thermo8_writeReg8(THERMO8_RESOLUTION_REG,rCfg);
}

uint16_t thermo8_getDevid()
{
   return thermo8_readReg(THERMO8_DEVID);
}

uint16_t thermo8_getManid()
{
   return thermo8_readReg(THERMO8_MANID);
}

void thermo8_sleep()
{
  uint16_t tmp;
  
  tmp = thermo8_readReg(THERMO8_CONFIG);
  tmp |= 1<<8;
  thermo8_writeReg(THERMO8_CONFIG,tmp);                                         //wait for the device to gi ti skeeo
  Delay_100ms();
}

void thermo8_wakeup()
{
  uint16_t tmp;

  tmp = thermo8_readReg(THERMO8_CONFIG);
  tmp &= ~( 1<<8 );
  thermo8_writeReg(THERMO8_CONFIG,tmp);
  Delay_100ms();                                                                //wait for the device to wakeup
}

void thermo8_limitSet(uint8_t limitRegaddr, float limit)
{
    float climit;
    uint16_t xlimit;
    
    climit = (limit * 16.0);
    xlimit = (uint16_t)climit;
    
    if(xlimit & 0x03)
    {
       xlimit +=1;
    }
    
    thermo8_writeReg(limitRegaddr,xlimit);
}

uint8_t thermo8_getAlertstat()
{
    uint8_t alertGen = 0;
    if(thermo8Limitstatus & 1<<13)
    {
      alertGen |= THERMO8_TLOWER_REACHED;
    }
    if(thermo8Limitstatus & 1<<14)
    {
      alertGen |= THERMO8_TUPPER_REACHED;
    }
    if(thermo8Limitstatus & 1<<15)
    {
      alertGen |= THERMO8_TCRIT_REACHED;
    }
    
    return alertGen;
}

void thermo8_alertEnable(uint8_t thys, uint8_t alertCfg)
{
     uint16_t cfg;
     // Int_out | AlertCnt
     cfg = 0x0001 | 0x0008 | 0x0010 ;
     thys &= 0x03;
     if(THERMO8_TCRIT_ONLY_ALERT == alertCfg)
     {
        cfg |= 0x0004;
     }
     else
     {
       cfg &= ~0x0004;
     }
     cfg |= (thys<<9);
     thermo8_writeReg(THERMO8_CONFIG,cfg);
}

void thermo8_tcritLock()
{
     uint16_t tmp;
     tmp=thermo8_readReg(THERMO8_CONFIG);
     tmp |= 1<<7;
     thermo8_writeReg(THERMO8_CONFIG,tmp);
}

void thermo8_tcritUnlock()
{
     uint16_t tmp;
     tmp=thermo8_readReg(THERMO8_CONFIG);
     tmp &= ~(1<<7);
     thermo8_writeReg(THERMO8_CONFIG,tmp);
}

void thermo8_winLock()
{
     uint16_t tmp;
     tmp=thermo8_readReg(THERMO8_CONFIG);
     tmp |= 1<<6;
     thermo8_writeReg(THERMO8_CONFIG,tmp);
}

void thermo8_winUnlock()
{
     uint16_t tmp;
     tmp=thermo8_readReg(THERMO8_CONFIG);
     tmp &= ~(1<<6);
     thermo8_writeReg(THERMO8_CONFIG,tmp);
}


/* -------------------------------------------------------------------------- */
/*
  __thermo8_driver.c

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