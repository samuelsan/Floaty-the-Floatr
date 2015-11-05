extern "C" {
#include <delay.h>
#include <FillPat.h>
#include <I2CEEPROM.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
}

char	rgBMPExhst1[] = {
  0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF0,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x0F};
  
char floatyBMP[] = {0x0C, 0x1E, 0x3F, 0x3F};

int charX, charY;
int controlMode;

int startX = 2;
int startY = 63;
int maxWallCountdown = 50;
int maxWalls = 3;


int wallCountdown;
int wallIndexCounter;

int wallX[3];
int wallY[3];
int wallHeight[3];
int wallWidth = 2;


void setup()
{
  
    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_SYSDIV_4);
  
    GPIOPadConfigSet(SWTPort, SWT1 | SWT2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
  
    GPIOPadConfigSet(BTN1Port, BTN1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    GPIOPadConfigSet(BTN2Port, BTN2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
  
    GPIOPadConfigSet(LED1Port, LED1, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(LED2Port, LED2, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(LED3Port, LED3, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(LED4Port, LED4, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
  
    GPIOPinTypeGPIOInput(SWTPort, SWT1 | SWT2);
  
    GPIOPinTypeGPIOInput(BTN1Port, BTN1);
    GPIOPinTypeGPIOInput(BTN2Port, BTN2);
  
    GPIOPinTypeGPIOOutput(LED1Port, LED1);
    GPIOPinTypeGPIOOutput(LED2Port, LED2);
    GPIOPinTypeGPIOOutput(LED3Port, LED3);
    GPIOPinTypeGPIOOutput(LED4Port, LED4);
  
    /*
     * Enable ADC Periph
     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
  
    GPIOPinTypeADC(AINPort, AIN);
  
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
    ADCSequenceEnable(ADC0_BASE, 0);
    
    charX = startX;
    charY = startY;
    
    wallCountdown = 10;
    wallIndexCounter = 0;
    
    for(int i = 0; i < maxWalls; i++){
      wallX[i] = wallY[i] = wallHeight[i] = -1;
    }
  
    /*
     * Initialize the OLED
     */
    OrbitOledInit();
  
}

void loop()
{
   setCursor(0,128,30);
   OrbitOledPutBmp(5,5, floatyBMP);
//   wallX[0] = 30;
//   wallY[0] = 60;
//   wallHeight[0] = 50;
   //setCursor(wallX[0], wallY[0],2);
       //OrbitOledPutBmp(wallHeight[0], wallWidth, rgBMPExhst1);
   if(wallCountdown == 0){
       
       wallHeight[wallIndexCounter] = random(40,80);
       wallX[wallIndexCounter] = 38;
       wallY[wallIndexCounter] = random(0,128);
       wallCountdown = 50;
       wallIndexCounter++;
       if(wallIndexCounter == 3){
         wallIndexCounter = 0;
       }
   }
   int i;
   for(i = 0; i < maxWalls; i++){
     if(wallX[i] != -1){
       wallX[i]--;
       if(wallX[i] <= 0){
         wallX[i] = wallY[i] = wallHeight[i] = -1;
       } else {
         setCursor(wallX[i], wallY[i],wallWidth);
         OrbitOledPutBmp(wallHeight[i], wallWidth, rgBMPExhst1);
         
       }
       
       OrbitOledUpdate();
     }
   }
   
   
   
   //OrbitOledClear();
   wallCountdown--;
   OrbitOledUpdate();
   delay(100);
}

int getCharVelocity(){
  if(controlMode == 0){
  } else {
  }
  
}

void setCursor(int x, int y, int width){
  int actualX = 38-x-width;
  int actualY = 128-y;
  OrbitOledMoveTo(actualY, actualX);
}

