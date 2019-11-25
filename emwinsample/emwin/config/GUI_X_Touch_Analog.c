/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
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
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/
//#include "stm32f4xx.h"
#include "GUI.h"
//#include "touch.h"
//#include "TouchCalibrate.h"


#define TOUCH_INT             GPIO_Pin_7
#define TOUCH_INT_GPIO        GPIOD


unsigned int GlobalX = 0;
unsigned int GlobalY = 0;

extern unsigned int TotalX;
extern unsigned int TotalY;
extern int ax_Phys[5],ay_Phys[5];


//void GUI_TOUCH_X_ActivateX(void) 
//{
//}

void GUI_TOUCH_X_ActivateY(void)
{

}

int  GUI_TOUCH_X_MeasureX(void) 
{
//	unsigned int x;
//	//GPIO_INT_INIT();	
//	if(GPIO_ReadInputDataBit(TOUCH_INT_GPIO,TOUCH_INT)== 0) 
//	{
//		//x=(( TPReadX() - ax_Phys[0] ) * LCD_XSIZE) / TotalX;
//		x=(( TPReadX()));
//		return x;
//	}
//	else
//		return 0;
	
}

int  GUI_TOUCH_X_MeasureY(void)
{

	unsigned int y;

//   //GPIO_INT_INIT();	
//	if(GPIO_ReadInputDataBit(TOUCH_INT_GPIO,TOUCH_INT)== 0) 
//	{
//		//y= (( TPReadY() - ay_Phys[0] ) * LCD_YSIZE ) / TotlY;
//		y= (( TPReadY()));
//		return y;
//	}
//	else
//		 return 0;
}

//void CalcTouchValue(void)
//{
//	if(GPIO_ReadInputDataBit(TOUCH_INT_GPIO,TOUCH_INT)== 0) 
//	{
//		GlobalX = TPReadX();
//		GlobalY = TPReadY();
//	}
//	else 
//	{
//		GlobalX = 0;
//		GlobalY = 0;
//	}
//}
