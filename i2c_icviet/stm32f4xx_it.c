/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    13-November-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
uint32_t	tick_count;
uint32_t	tick_flag;

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
  {
  }
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
  {
  }
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
  {
  }
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
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    tick_flag = 1;
    tick_count++;
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
void I2C2_ER_IRQHandler(void)
{
  /* Check on I2C2 AF flag and clear it */
  if (I2C_GetITStatus(I2C2, I2C_IT_AF)) 
  {
    I2C_ClearITPendingBit(I2C2, I2C_IT_AF);
  }
}

/**
  * @brief  This function handles I2Cx event interrupt request.
  * @param  None
  * @retval None
  */
volatile uint32_t Event = 0;
volatile uint8_t Tx_Idx;
volatile uint8_t Rx_Idx;
extern uint8_t SlaveTxBuffer[];
extern uint8_t SlaveRxBuffer[];
void I2C2_EV_IRQHandler(void)
{
  /* Get Last I2C Event */
  Event = I2C_GetLastEvent(I2C2);
  switch (Event)
  { 
  /* ****************************************************************************/
  /*                          Slave Transmitter Events                          */
  /*                                                                            */
  /* ****************************************************************************/  
    
  /* Check on EV1 */
  case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:  
    Tx_Idx = 0;
    I2C_ITConfig(I2C2, I2C_IT_BUF , ENABLE);
    break;
  /* Check on EV3 */
  case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:
  case I2C_EVENT_SLAVE_BYTE_TRANSMITTED:    
    I2C_SendData(I2C2, SlaveTxBuffer[Tx_Idx++]);
    break;
    
  /* ****************************************************************************/
  /*                              Slave Receiver Events                         */
  /*                                                                            */
  /* ****************************************************************************/ 
    
  /* check on EV1*/
  case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
    Rx_Idx = 0;
    break;
    
  /* Check on EV2*/
  case I2C_EVENT_SLAVE_BYTE_RECEIVED:  
  case (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_SR1_BTF):  
    SlaveRxBuffer[Rx_Idx++] = I2C_ReceiveData(I2C2);
    break;
 
  /* Check on EV4 */
  case I2C_EVENT_SLAVE_STOP_DETECTED:             
    I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF);
    I2C_Cmd(I2C2, ENABLE);
    break;
    
  default:
    break;
  } 
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
