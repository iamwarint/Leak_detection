/*vs1003  Prawit Chumchu*/
/* Can record ADC PCM using Plugin and save sound1.wav*/
/* for one VS1003 */

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>
unsigned int sleep(unsigned int seconds);
//PIN Define
#define CS_PIN_VS1 0
#define DCS_PIN_VS1 1
#define DREQ_PIN_VS1 2
#define RESET_PIN_VS1 3
#define CS_PIN_VS2 5
#define DCS_PIN_VS2 6
#define DREQ_PIN_VS2 21
#define RESET_PIN_VS2 22

#define CS_PIN_VS3 23
#define DCS_PIN_VS3 24
#define DREQ_PIN_VS3 25
#define RESET_PIN_VS3 26

// VS1003 SCI Write Command byte is 0x02
#define VS_WRITE_COMMAND 0x02

// VS1003 SCI Read COmmand byte is 0x03
#define VS_READ_COMMAND  0x03

// SCI Registers

const unsigned int SCI_MODE = 0x0;
const unsigned int SCI_STATUS = 0x1;
const unsigned int SCI_BASS = 0x2;
const unsigned int SCI_CLOCKF = 0x3;
const unsigned int SCI_DECODE_TIME = 0x4;
const unsigned int SCI_AUDATA = 0x5;
const unsigned int SCI_WRAM = 0x6;
const unsigned int SCI_WRAMADDR = 0x7;
const unsigned int SCI_HDAT0 = 0x8;
const unsigned int SCI_HDAT1 = 0x9;
const unsigned int SCI_AIADDR = 0xa;
const unsigned int SCI_VOL = 0xb;
const unsigned int SCI_AICTRL0 =  0xc;
const unsigned int SCI_AICTRL1 = 0xd;
const unsigned int SCI_AICTRL2 = 0xe;
const unsigned int SCI_AICTRL3 = 0xf;
const unsigned int SCI_num_registers = 0xf;

// SCI_MODE bits

const unsigned int SM_DIFF = 0;
const unsigned int SM_LAYER12 = 1;
const unsigned int SM_RESET = 2;
const unsigned int SM_OUTOFWAV = 3;
const unsigned int SM_EARSPEAKER_LO = 4;
const unsigned int SM_TESTS = 5;
const unsigned int SM_STREAM = 6;
const unsigned int SM_EARSPEAKER_HI = 7;
const unsigned int SM_DACT = 8;
const unsigned int SM_SDIORD = 9;
const unsigned int SM_SDISHARE = 10;
const unsigned int SM_SDINEW = 11;
const unsigned int SM_ADPCM = 12;
const unsigned int SM_ADCPM_HP = 13;
const unsigned int SM_LINE_IN = 14;



const unsigned char header[]={

0x52,0x49,0x46,0x46,0x34,0x2C,0x01,0x00,  //last 4 bytes, Size of the overall file - 8 bytes, in bytes (32-bit integer)
0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,
0x10,0x00,0x00,0x00,0x01,0x00,0x01,0x00, 
0x40,0x1f,0x00,0x00,0x80,0x3E,0x00,0x00,
0x02,0x00,0x10,0x00,0x64,0x61,0x74,0x61,
0xC4,0x2B,0x01,0x00                        //Size of the data section.
};
//52 49 46 46 64 9c 00 00 
//57 41 56 45 66 6d 74 20
//10 00 00 00 01 00 01 00
//40 1f 00 00 80 3e 00 00 
//02 00 10 00 64 61 74 61
//40 9c 00 00
 
//52 49 46 46 64 9c 00 00 
//57 41 56 45 66 6d 74 20 
//10 00 00 00 01 00 01 00
//40 1f 00 00 80 3e 00 00
//02 00 10 00 64 61 74 61
//40 9c 00 00 

#define CODE_SIZE 411
const unsigned char atab[411] = { /* Register addresses */
    0x7, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6,
    0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6
};
const unsigned short dtab[411] = { /* Data to write */
    0x8030, 0x0000, 0x800a, 0x2800, 0x0cc0, 0x0006, 0x2016, 0x0030,
    0x0692, 0x0006, 0x5011, 0x0030, 0x0310, 0x0006, 0x95d3, 0x3e05,
    0xb814, 0x3635, 0x0024, 0x0000, 0x800a, 0xb880, 0x0005, 0x0006,
    0x9590, 0x3900, 0x0024, 0x0000, 0x0cc0, 0x0006, 0x9511, 0x2919,
    0x8880, 0x3a00, 0x0024, 0x0001, 0x0000, 0x3613, 0x2c00, 0xb880,
    0x0024, 0x3009, 0x2000, 0x0004, 0x0010, 0x3009, 0x2410, 0x0006,
    0x94d1, 0x3009, 0x2410, 0x0006, 0x5490, 0x3009, 0x0000, 0x0006,
    0x54d0, 0x291c, 0xc640, 0x3009, 0x2000, 0x458a, 0x0024, 0x002e,
    0xe002, 0x2800, 0x14d5, 0x0006, 0x5850, 0x0000, 0x0305, 0x3613,
    0x0024, 0x3e00, 0x7800, 0x0006, 0x0000, 0x3009, 0x0003, 0x0006,
    0x5810, 0x3009, 0x0006, 0xffe6, 0x0024, 0x48be, 0x0024, 0xffac,
    0x0024, 0x48b6, 0x0024, 0x291d, 0x7b00, 0xb882, 0x0024, 0x4c86,
    0x1bcc, 0x291d, 0x7b00, 0xb182, 0x4140, 0x4c86, 0x184c, 0x4088,
    0x3840, 0x6890, 0x0024, 0x2911, 0x4880, 0x3e10, 0x0024, 0x000f,
    0xa000, 0x6400, 0x1b8c, 0x0030, 0x0790, 0x2800, 0x1b91, 0x4550,
    0x0024, 0x2800, 0x1bc0, 0x689a, 0x2000, 0xb88a, 0x2005, 0x0000,
    0x0ec0, 0x30c3, 0x108c, 0xb880, 0x2000, 0xb884, 0xb3c0, 0x3c10,
    0x0024, 0x2800, 0x2240, 0x3ce1, 0x0024, 0x2919, 0x8440, 0x3613,
    0x0024, 0x6012, 0x0024, 0x0000, 0x0081, 0x2800, 0x2118, 0x0000,
    0x0024, 0x2919, 0x3ac0, 0x3613, 0x0024, 0x2919, 0x8440, 0x3613,
    0x0024, 0x6012, 0x0024, 0x0000, 0x0081, 0x2800, 0x1f08, 0x0000,
    0x0024, 0x6890, 0x104c, 0xf400, 0x4510, 0x2913, 0xb500, 0x34f3,
    0x184c, 0x6294, 0x0024, 0x0004, 0x0001, 0xffc2, 0x0024, 0x48b2,
    0x0024, 0x6212, 0x0024, 0x0000, 0x0081, 0x2800, 0x1d91, 0x0000,
    0x0024, 0x0006, 0x5593, 0x0000, 0x0012, 0x0006, 0x9611, 0x0030,
    0x07d0, 0xb880, 0x0001, 0x0006, 0x5550, 0x3009, 0x2440, 0x0000,
    0x2000, 0x3009, 0x27c1, 0x3009, 0x2c12, 0xb880, 0xa000, 0x0005,
    0x89d0, 0xb884, 0x2000, 0x0000, 0x0081, 0x2919, 0x8440, 0x3613,
    0x0024, 0x6012, 0x0024, 0x0000, 0x0081, 0x2800, 0x2b18, 0x0000,
    0x0024, 0x2919, 0x3ac0, 0x3613, 0x0024, 0x2919, 0x8440, 0x3613,
    0x0024, 0x6012, 0x0024, 0x0000, 0x0081, 0x2800, 0x2908, 0x0000,
    0x0024, 0x6890, 0x104c, 0xf400, 0x4510, 0x2913, 0xb500, 0x34f3,
    0x184c, 0x458a, 0x0024, 0x0005, 0x7941, 0x2800, 0x2f55, 0x0005,
    0x89d0, 0x3413, 0x184c, 0xf400, 0x4510, 0x291c, 0x5e80, 0x34f3,
    0x0024, 0x4080, 0x0024, 0x0005, 0x7941, 0x2800, 0x2745, 0x0000,
    0x0024, 0x0005, 0x89d0, 0x6890, 0x184c, 0x3e10, 0x104c, 0xf400,
    0x4512, 0x291c, 0xb580, 0x34f3, 0x0024, 0x0006, 0x94d2, 0x3000,
    0xd04c, 0x36f3, 0x13c0, 0x6396, 0x8811, 0x4397, 0x8390, 0x0006,
    0x94d1, 0x4132, 0x8412, 0xf400, 0x4055, 0x0005, 0x0001, 0x3d00,
    0x084c, 0xf400, 0x4480, 0x6016, 0x0024, 0x0000, 0x0801, 0x2800,
    0x34d1, 0x0000, 0x0024, 0x003f, 0x0013, 0x3283, 0x0024, 0x291c,
    0xc640, 0x3613, 0x2412, 0x3000, 0x0024, 0x6012, 0x0024, 0x0000,
    0x0024, 0x2800, 0x2748, 0x0000, 0x0024, 0x2919, 0x8180, 0x3613,
    0x0024, 0xf106, 0x0024, 0x0000, 0x0400, 0x6300, 0x0024, 0x0000,
    0x0800, 0x2800, 0x3958, 0x0000, 0x0024, 0x4490, 0x108c, 0x2800,
    0x3b00, 0x3ce0, 0x0024, 0x6300, 0x108c, 0x0000, 0x0024, 0x2800,
    0x3ac9, 0x6490, 0x0024, 0x2800, 0x3b00, 0x3ce0, 0x0024, 0x3ce1,
    0x0024, 0x3423, 0x0024, 0x3400, 0x1381, 0x601c, 0x0024, 0x0000,
    0x0024, 0x2800, 0x3e05, 0x0000, 0x0024, 0x408c, 0x108c, 0x3613,
    0x3380, 0x6892, 0x3840, 0x2911, 0x4880, 0x3e10, 0x4024, 0x36e3,
    0x0024, 0x2914, 0x0c40, 0x3613, 0x0024, 0xb880, 0x0024, 0x2800,
    0x2740, 0x3800, 0x0024
};
unsigned char temp_buffer[1000];
static const int CHANNEL = 0;  //not used for CE0 or CE1 but use different pins
unsigned int num_of_samples=12000;
unsigned char db[200000];

FILE *filePointer;
unsigned int read_register_VS1(unsigned _reg);
void write_register_VS1(unsigned int  _reg,unsigned int _value);

unsigned int SpiLoadImageInto1003() {
   unsigned int idx;
   for (idx=0; idx < sizeof(atab); idx++) { 
  	 write_register_VS1(atab[idx],dtab[idx]);
    }
  
  return 0xFFFF;
}

void control_mode_on_VS1(void)
{
    digitalWrite(DCS_PIN_VS1,HIGH);
    digitalWrite(CS_PIN_VS1,LOW);
}

void control_mode_off_VS1(void)
{
    digitalWrite(CS_PIN_VS1,HIGH);
}
void data_mode_on_VS1(void)
{
    digitalWrite(CS_PIN_VS1,HIGH);
    digitalWrite(DCS_PIN_VS1,LOW);
}

void data_mode_off_VS1(void)
{
    digitalWrite(CS_PIN_VS1,HIGH);
}

void await_data_request_VS1(void)
{
    while ( !digitalRead(DREQ_PIN_VS1) );
}

unsigned int read_register_VS1(unsigned _reg)
{
  unsigned int result;

  /*control_mode_on_VS1();
  //delayMicroseconds(1); // tXCSS
 // SPI.transfer(VS_READ_COMMAND); // Read operation  
  temp_buffer[0]=VS_READ_COMMAND;
  result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 1);
  temp_buffer[0]=_reg;
  result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 1);
  result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 2);
 //result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 1);
  // delayMicroseconds(1); // tXCSH
  await_data_request_VS1();
  control_mode_off_VS1();
  result= temp_buffer[1]|temp_buffer[0]<<8;
  return result;*/

  control_mode_on_VS1();
  //delayMicroseconds(1); // tXCSS
 // SPI.transfer(VS_READ_COMMAND); // Read operation  
  temp_buffer[0]=VS_READ_COMMAND;
  temp_buffer[1]=_reg;
  temp_buffer[2]=0x00;
  temp_buffer[3]=0x00;
  result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 4);
 //result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 1);
  // delayMicroseconds(1); // tXCSH
  await_data_request_VS1();
  control_mode_off_VS1();
  result= temp_buffer[3]|temp_buffer[2]<<8;
  return result;

}

unsigned int read_register_N_VS1(unsigned _reg,unsigned words)
{
  unsigned int result;

  /*control_mode_on_VS1();
  //delayMicroseconds(1); // tXCSS
 // SPI.transfer(VS_READ_COMMAND); // Read operation  
  temp_buffer[0]=VS_READ_COMMAND;
  result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 1);
  temp_buffer[0]=_reg;
  result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 1);
  result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 2);
 //result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 1);
  // delayMicroseconds(1); // tXCSH
  await_data_request_VS1();
  control_mode_off_VS1();
  result= temp_buffer[1]|temp_buffer[0]<<8;
  return result;*/

  control_mode_on_VS1();
  //delayMicroseconds(1); // tXCSS
 // SPI.transfer(VS_READ_COMMAND); // Read operation  
  int idx=0;
 /* for(idx=0;idx<words*2+2;idx++){
          temp_buffer[idx]=0x00;
  }*/
  temp_buffer[0]=VS_READ_COMMAND;
  temp_buffer[1]=_reg;
  
  result = wiringPiSPIDataRW(CHANNEL, temp_buffer, words*2+2);
 //result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 1);
  // delayMicroseconds(1); // tXCSH
  await_data_request_VS1();
  control_mode_off_VS1();
  result= 0x00;
  return result;

}


void print_byte_register(unsigned int reg)
{
       unsigned value;
	value=read_register_VS1(reg);
	printf("Reg:  %d  value:  %d \n",reg,value);
  
}

void printDetails(void)
{
  printf("VS1003 Configuration:\n");
  int i = 0;
  while ( i <= SCI_num_registers )
    print_byte_register(i++);
}
void write_register_VS1(unsigned int  _reg,unsigned int _value)
{
  int result;
 
 /* control_mode_on_VS1();
//  delayMicroseconds(1); // tXCSS
 // SPI.transfer(VS_WRITE_COMMAND); // Write operation
  temp_buffer[0]=VS_WRITE_COMMAND;
  result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 1);

 // SPI.transfer(_reg); // Which register
   temp_buffer[0]=_reg;
  result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 1);
  temp_buffer[0]=_value >> 8;
  temp_buffer[1]=_value & 0xff;
 // SPI.transfer(_value >> 8); // Send hi byte
  result = wiringPiSPIDataRW(CHANNEL,  temp_buffer, 2);
 //SPI.transfer(_value & 0xff); // Send lo byte
 //  delayMicroseconds(1); // tXCSH
  await_data_request_VS1();
  control_mode_off_VS1(); */

   control_mode_on_VS1();
   delayMicroseconds(1);
   temp_buffer[0]=VS_WRITE_COMMAND;
   temp_buffer[1]=_reg;
   temp_buffer[2]=_value >> 8;
   temp_buffer[3]=_value & 0xff;
   result = wiringPiSPIDataRW(CHANNEL, temp_buffer, 4);
   await_data_request_VS1();
  control_mode_off_VS1(); 

}


void SPIBegin(void)
{
   int fd;
  // Keep the chip in reset until we are ready
  pinMode(RESET_PIN_VS1,OUTPUT);
  digitalWrite(RESET_PIN_VS1,LOW);

  // The SCI and SDI will start deselected
  pinMode(CS_PIN_VS1,OUTPUT);
  digitalWrite(CS_PIN_VS1,HIGH);
  pinMode(DCS_PIN_VS1,OUTPUT);
  digitalWrite(DCS_PIN_VS1,HIGH);

  // DREQ is an input
  pinMode(DREQ_PIN_VS1,INPUT);

  // Boot VS1003
  printf("Booting VS1003...\n");

  delay(1);

  // init SPI slow mode
  fd = wiringPiSPISetup(CHANNEL, 100000);
  // release from reset
  digitalWrite(RESET_PIN_VS1,HIGH);
  // Declick: Immediately switch analog off
  write_register_VS1(SCI_VOL,0xffff); // VOL
  /* Declick: Slow sample rate for slow analog part startup */
  write_register_VS1(SCI_AUDATA,10);
  delay(100);


  /* Switch on the analog parts */
  //write_register(SCI_VOL,0xfefe); // VOL
  
  printf("VS1003 still booting\n");

  write_register_VS1(SCI_AUDATA,8001); // 44.1kHz stereo

  write_register_VS1(SCI_VOL,0x2000); // VOL
  
  // soft reset
  write_register_VS1(SCI_MODE, 1<<SM_SDINEW | 1<<SM_RESET);
  delay(1);
  await_data_request_VS1();
  write_register_VS1(SCI_CLOCKF,0xE000);//0x4430); // Experimenting with higher clock settings
  delay(1);
  await_data_request_VS1();

  // Now you can set high speed SPI clock
  fd = wiringPiSPISetup(CHANNEL, 2000000);

  printf("VS1003 Set\n");
  printf("VS1003 OK\n");

  
}

void RecordPcm1003(void) {
	unsigned int pluginStartAddr=0x0030;
 	unsigned int w = 0, idx = 0,value=0,line_in=0,num_of_block=100;
	/* Set VS1053 clock to 4.5x = 55.3 MHz */
 	
        write_register_VS1(SCI_CLOCKF, 0x4000); 
	write_register_VS1(SCI_BASS,0);
  
       // Disable all interrupts except SCI
  	//Write1053Sci(SCI_WRAMADDR, VS1053_INT_ENABLE);
  	//Write1053Sci(SCI_WRAM, 0x2);

	write_register_VS1(SCI_AICTRL0, 0x000c);
	write_register_VS1(SCI_AICTRL1,0); //gain=1,1024, gain=auto,0
        printf("AAAAAAAAAAAA1111111\n");	
	SpiLoadImageInto1003();
  	printf("AAAAAAAAAAAA222222222222\n");
        


	for (idx=0; idx < sizeof(header); idx++) { /* Save header first */
		db[idx] = header[idx];
		
	}


       //File Size, 
	unsigned int temp=num_of_samples*2+44-8;
	db[4]=temp&0xFF;
  	db[5]=(temp>>8);
	db[6]=(temp>>16);;
	db[7]=(temp>>24);
       //----
       //Data Size, 
	temp=temp=num_of_samples*2;
	db[40]=temp&0xFF;
  	db[41]=(temp>>8);
	db[42]=(temp>>16);
	db[43]=(temp>>24);
       //----
       
       //----
	if (line_in) {
		//write_register_VS1(SCI_MODE, 0x5804); /* Normal SW reset + other bits */
	} else {
		//write_register_VS1(SCI_MODE,0x2052);// 0x1804); /* Normal SW reset + other bits */
	}

        //from plugin
	write_register_VS1(SCI_MODE, 0x1800);
      
	int num_round=0;
        
 	
	printDetails();


      //  idx=0;
        int temp_idx=0,temp_idx1=0;
        pluginStartAddr=0x0030;
	write_register_VS1(SCI_AIADDR, pluginStartAddr);
        printf("Start Rec....................... %u \n",pluginStartAddr);
	unsigned int wordsWaiting;
        while(1){ 
		wordsWaiting = read_register_VS1(SCI_HDAT1);
		//printf("wordsWaiting= %u\n",wordsWaiting);
		//delayMicroseconds(20840);
		if(wordsWaiting==256){
			printf("wordsWaiting= %u   %d  sizeof uint = %d\n",wordsWaiting,temp_idx1,sizeof(w));
			//read_register_N_VS1(SCI_HDAT0,256);
			for(temp_idx=0;temp_idx<wordsWaiting;temp_idx++){
				
				w =read_register_VS1(SCI_HDAT0);
				w=w;
				db[idx++] =w&0xFF; ;;;//temp_buffer[(temp_idx+1)*2]; ///w&0xFF; ;;;/
   				db[idx++] = w>>8;;;//temp_buffer[(temp_idx+1)*2+1];//w>>8;;;//
				temp_idx1=temp_idx1+1;
				if(temp_idx1==num_of_samples){
					break;
				}
				
			}
			
		}
		if(temp_idx1==num_of_samples){
					break;
		}	
		
	}
        
	
	printf("Stop Rec....................... \n");
	unsigned int  temp1;
	unsigned int  temp2,temp3;
	/*for (idx=0;idx<num_of_samples;idx=idx+2){
		temp1=db[idx]<<8;
		temp2=db[idx+1];
		temp3=temp1+temp2;
		//fprintf(filePointer, "%x  %x %u\n",db[idx],db[idx+1],temp3);
		fprintf(filePointer, "%u\n",temp3);

	}*/
        for (idx=0;idx<sizeof(header);idx++){
		//fprintf(filePointer, "%c",db[idx]);

	}
	for (idx=0;idx<num_of_samples*2+44;idx=idx+1){

		fprintf(filePointer, "%c",db[idx]);
	}
    
}
int main (void)
{
  int result;
   unsigned char buffer[100];
  printf ("Raspberry Pi SPI\n") ;

  if (wiringPiSetup () == -1)
    return 1 ;
   
   SPIBegin();
   
   sleep(1);
  int i;
  //for debug
  pinMode(CS_PIN_VS3,OUTPUT);
  //  

  filePointer = fopen("sound1.wav", "w");
  RecordPcm1003();
  fclose(filePointer);
  
  return 0 ;
}
