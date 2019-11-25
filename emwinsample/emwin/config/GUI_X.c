/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.46 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : GUI_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "global_includes.h"
#include "stm32f1xx_hal.h"
/*********************************************************************
*
*       Global data
*/
extern __IO uint32_t uwTick;

/*********************************************************************
*
*      Timing:
*                 GUI_X_GetTime()
*                 GUI_X_Delay(int)

  Some timing dependent routines require a GetTime
  and delay function. Default time unit (tick), normally is
  1 ms.
*/

GUI_TIMER_TIME GUI_X_GetTime(void) { 
  return uwTick; 
}

void GUI_X_Delay(int ms) { 
  int tEnd = uwTick + ms;
  while ((tEnd - uwTick) > 0);
}



#define GUI_NUMBYTES  0xc000
//
// Define the average block size
//
#define GUI_BLOCKSIZE 0x80

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/


/* 32 bit aligned memory area */
U32 extMem[GUI_NUMBYTES / 4];

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   available memory for the GUI.
*/
void GUI_X_Config(void)
{

  GUI_ALLOC_AssignMemory(extMem, GUI_NUMBYTES);
	
	GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE);

}



#define LCD_DATA_ADDRESS    *(volatile U16*) ((volatile U32)0x60020000) 
#define LCD_REG_ADDRESS     *(volatile U16*) ((volatile U32)0x60000000)	

void STM3210E_LCD_Init(void);

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
/********************************************************************
*
*       LcdWriteReg
*
* Function description:
*   Sets display register
*/
//U16 Datareg;
static void LcdWriteReg(U16 Data) {
  LCD_REG_ADDRESS = Data;
	//Datareg = Data;
}

/********************************************************************
*
*       LcdWriteData
*
* Function description:
*   Writes a value to a display register
*/
static void LcdWriteData(U16 Data) {
  LCD_DATA_ADDRESS = Data;
}

/********************************************************************
*
*       LcdWriteDataMultiple
*
* Function description:
*   Writes multiple values to a display register.
*/
static void LcdWriteDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    LCD_DATA_ADDRESS = *pData++;
  }
}

/********************************************************************
*
*       LcdReadDataMultiple
*
* Function description:
*   Reads multiple values from a display register.
*/
static void LcdReadDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    *pData++ = LCD_DATA_ADDRESS;
  }
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
#define LCD_SIZE 7

	#if (LCD_SIZE == 7)
		#define XSIZE_PHYS  800
		#define YSIZE_PHYS  480
  #endif		
	
	#if (LCD_SIZE == 4)
		#define XSIZE_PHYS  480
		#define YSIZE_PHYS  280
	#endif

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif
	
//#define LCD_MIRROR_X 1	
	#define GUI_SWAP_XY_ 1

void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  //
  // Set display driver and color conversion
  //
  pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
  LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
  //
  // Orientation
  //
  //Config.Orientation = GUI_SWAP_XY_ ;
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set controller and operation mode
  //
  PortAPI.pfWrite16_A0  = LcdWriteReg;
  PortAPI.pfWrite16_A1  = LcdWriteData;
  PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple;
  PortAPI.pfReadM16_A1  = LcdReadDataMultiple;
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FlexColor_SetFunc66720, GUIDRV_FLEXCOLOR_M16C0B16);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;
  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    STM3210E_LCD_Init();
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}



unsigned int  HDP=XSIZE_PHYS - 1; //Horizontal Display Period
unsigned int  HT=1000; //Horizontal Total
unsigned int  HPS=51;  //LLINE Pulse Start Position
unsigned int  LPS=3;   //	Horizontal Display Period Start Position
unsigned char HPW=8;   //	LLINE Pulse Width


unsigned int  VDP=YSIZE_PHYS - 1;	//Vertical Display Period
unsigned int  VT=530;	//Vertical Total
unsigned int  VPS=24;	//	LFRAME Pulse Start Position
unsigned int  FPS=23;	//Vertical Display Period Start Positio
unsigned char   VPW=3;	// LFRAME Pulse Width


void LCD_clear(void)
{
    unsigned int l=800,w;

	LcdWriteReg(0x002A);	
	LcdWriteData(0);	    
	LcdWriteData(0);
	LcdWriteData(HDP>>8);	    
	LcdWriteData(HDP&0x00ff);
  LcdWriteReg(0x002b);	
	LcdWriteData(0);	    
	LcdWriteData(0);
	LcdWriteData(VDP>>8);	    
	LcdWriteData(VDP&0x00ff);
	LcdWriteReg(0x002c);
	
	
	while(l--)
	{
	    for(w=0;w<480;w++)
		{    
          	LcdWriteData(0xf800);
		}
	}
}



void STM3210E_LCD_Init(void){
	
	


	 LcdWriteReg(0x00E2);	
	LcdWriteData(0x0023);
	// Set PLL with OSC = 10MHz (hardware)
    // Multiplier N = 35, VCO (>250MHz)= OSC*(N+1), VCO = 360MHz	   
	LcdWriteData(0x0002);
	// Divider M = 2, PLL = 360/(M+1) = 120MHz
	LcdWriteData(0x0004);
	// Validate M and N values

	LcdWriteReg(0x00E0);  // PLL enable
	LcdWriteData(0x0001);
	HAL_Delay(1);
	LcdWriteReg(0x00E0);
	LcdWriteData(0x0003);
	HAL_Delay(5);
	LcdWriteReg(0x0001);  // software reset
	HAL_Delay(5);
	LcdWriteReg(0x00E6);	//PLL setting for PCLK, depends on resolution
	
	
	//Set LSHIFT freq, i.e. the DCLK with PLL freq 120MHz set previously
	//Typical DCLK for AT070TN92 is 34MHz
	//34MHz = 120MHz*(LCDC_FPR+1)/2^20
	//LCDC_FPR = 300000 (0x0493E0)
	
	LcdWriteData(0x0004);
	LcdWriteData(0x0093);
	LcdWriteData(0x00e0);

	LcdWriteReg(0x00B0);	//LCD SPECIFICATION
	LcdWriteData(0x0000);
	LcdWriteData(0x0000);
	LcdWriteData((HDP>>8)&0X00FF);  //Set HDP
	LcdWriteData(HDP&0X00FF);
  LcdWriteData((VDP>>8)&0X00FF);  //Set VDP
	LcdWriteData(VDP&0X00FF);
  LcdWriteData(0x0000);

	LcdWriteReg(0x00B4);	//HSYNC
	LcdWriteData((HT>>8)&0X00FF);  //Set HT
	LcdWriteData(HT&0X00FF);
	LcdWriteData((HPS>>8)&0X00FF);  //Set HPS
	LcdWriteData(HPS&0X00FF);
	LcdWriteData(HPW);			   //Set HPW
	LcdWriteData((LPS>>8)&0X00FF);  //Set HPS
	LcdWriteData(LPS&0X00FF);
	LcdWriteData(0x0000);

	LcdWriteReg(0x00B6);	//VSYNC
	LcdWriteData((VT>>8)&0X00FF);   //Set VT
	LcdWriteData(VT&0X00FF);
	LcdWriteData((VPS>>8)&0X00FF);  //Set VPS
	LcdWriteData(VPS&0X00FF);
	LcdWriteData(VPW);			   //Set VPW
	LcdWriteData((FPS>>8)&0X00FF);  //Set FPS
	LcdWriteData(FPS&0X00FF);

	LcdWriteReg(0x00BA);
	LcdWriteData(0x0005);//0x000F);    //GPIO[3:0] out 1

	LcdWriteReg(0x00B8);
	LcdWriteData(0x0007);    //GPIO3=input, GPIO[2:0]=output
	LcdWriteData(0x0001);    //GPIO0 normal

	LcdWriteReg(0x0036); //rotation
	LcdWriteData(0x0000);
//	LcdWriteData(0x0060);


	LcdWriteReg(0x00F0); //pixel data interface
	LcdWriteData(0x0003);


	HAL_Delay(5);

	LCD_clear();
	LcdWriteReg(0x0029); //display on

	LcdWriteReg(0x00BE); //set PWM for B/L
	LcdWriteData(0x0006);
	//LcdWriteData(0x0008);
	LcdWriteData(0x0080);
	//LcdWriteData(0x00f0);
	
	LcdWriteData(0x0001);
	LcdWriteData(0x00f0);
	LcdWriteData(0x0000);
	LcdWriteData(0x0000);

	LcdWriteReg(0x00d0);//设置动态背光控制配置 
	LcdWriteData(0x000d);
	
}



void GUI_TOUCH_X_ActivateX(void) 
{
	
	
}

void GUI_TOUCH_X_ActivateY(void)
{

}

int  GUI_TOUCH_X_MeasureX(void) 
{
	
}

int  GUI_TOUCH_X_MeasureY(void)
{


}
/*********************************************************************
*
*       GUI_X_Init()
*
* Note:
*     GUI_X_Init() is called from GUI_Init is a possibility to init
*     some hardware which needs to be up and running before the GUI.
*     If not required, leave this routine blank.
*/

void GUI_X_Init(void) {}


/*********************************************************************
*
*       GUI_X_ExecIdle
*
* Note:
*  Called if WM is in idle state
*/

void GUI_X_ExecIdle(void) {}

/*********************************************************************
*
*      Logging: OS dependent

Note:
  Logging is used in higher debug levels only. The typical target
  build does not use logging and does therefor not require any of
  the logging routines below. For a release build without logging
  the routines below may be eliminated to save some space.
  (If the linker is not function aware and eliminates unreferenced
  functions automatically)

*/

void GUI_X_Log     (const char *s) { GUI_USE_PARA(s); }
void GUI_X_Warn    (const char *s) { GUI_USE_PARA(s); }
void GUI_X_ErrorOut(const char *s) { GUI_USE_PARA(s); }


void GUI_X_InitOS(void)    {  }
void GUI_X_Unlock(void)    {  }
void GUI_X_Lock(void)      {  }
U32  GUI_X_GetTaskId(void) {  }

/*************************** End of file ****************************/
