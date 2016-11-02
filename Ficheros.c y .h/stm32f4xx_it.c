/**
  ******************************************************************************
  * @file    TIM_PWM_Output/stm32f4xx_it.c */
  // @author  Juan Domingo Jimenez Jerez
  //           Joaquin Bonet Mira
  /* @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx.h"
#include <stdio.h>


extern  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
extern  TIM_OCInitTypeDef  TIM_OCInitStructure;
extern  uint16_t CCR4_Val ;
extern  uint16_t PrescalerValue ;
extern  uint32_t TimingDelay; 

extern void Delay(__IO uint32_t nTime);   
extern uint8_t semaforo ; 
/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup TIM_PWM_Output
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern __IO uint8_t UserButtonPressed; //Interrupcion externa boton

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{}


void EXTI0_IRQHandler(void)  //Interrupcion externa boton
{
  UserButtonPressed = 0x01;
   while (1)
  {
  
   while(semaforo==0&&STM_EVAL_PBGetState(BUTTON_USER) != Bit_SET){ //lo hace solo la primera vez
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED5);
      STM_EVAL_LEDOff(LED6);
            Delay(1000000); 
          STM_EVAL_LEDToggle(LED4);
            Delay(1000000);
          STM_EVAL_LEDToggle(LED3);
            Delay(1000000);
          STM_EVAL_LEDToggle(LED5);
            Delay(1000000);
          STM_EVAL_LEDToggle(LED6);
            Delay(1000000);
        }
    semaforo=1;
   
     if (STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET)
       {
     STM_EVAL_LEDOff(LED4), STM_EVAL_LEDOff(LED3), STM_EVAL_LEDOff(LED5), STM_EVAL_LEDOff(LED6);
          
       
        
         while (STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET)  //Espera a soltar boton
      
          if (CCR4_Val>3){
            STM_EVAL_LEDOn(LED4);
          }if (CCR4_Val>83){
            STM_EVAL_LEDOn(LED3);
          }if (CCR4_Val>166){ 
            STM_EVAL_LEDOn(LED5);
          }if (CCR4_Val>332){
            STM_EVAL_LEDOn(LED6); 
          }
         //ACTUALIZAMOS PWM
        TIM_Cmd(TIM3, DISABLE); //Paramos timmer
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   
        TIM_OCInitStructure.TIM_Pulse = CCR4_Val; //actualizamos valor
        TIM_OC3Init(TIM3, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
        TIM_Cmd(TIM3, ENABLE); //Arrancamos de nuevo el timmer
        
        if (CCR4_Val<664)
          {
              CCR4_Val= CCR4_Val+CCR4_Val;  // CCR4=CCR4*2
          }
        else
          {
              CCR4_Val=83;
          }
            Delay(1000000); //MUY IMPORTANTE!!, sin este delay tiene algunos fallos                                                            
      } //fin IF  
  }
  /* Clear the EXTI line pending bit */
  EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE); //No salimos de la interrupcion hasta pulsar el otro boton (Reset)
  //return CCR4_Val;
}


