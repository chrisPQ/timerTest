/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *Multiprocessor communication mode routine:
 *Master:USART1_Tx(PD5)\USART1_Rx(PD6).
 *This routine demonstrates that USART1 receives the data sent by CH341 and inverts
 *it and sends it (baud rate 115200).
 *
 *Hardware connection:PD5 -- Rx
 *                     PD6 -- Tx
 *
 */

#include "debug.h"


/* Global define */


/* Global Variable */
vu8 val;

/*********************************************************************
 * @fn      USARTx_CFG
 *
 * @brief   Initializes the USART2 & USART3 peripheral.
 *
 * @return  none
 */
void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

void timTestInit(void) {
	 GPIO_InitTypeDef GPIO_InitStructure={0};
     TIM_OCInitTypeDef TIM_OCInitStructure={0};
     TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};
     TIM_BDTRInitTypeDef TIM_BDTRStruct={0};

	 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM1, ENABLE );

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init( GPIOC, &GPIO_InitStructure);

	 TIM_TimeBaseInitStructure.TIM_Period = 10000;
	 TIM_TimeBaseInitStructure.TIM_Prescaler = 0xFFFF;
	 TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV4;
	 TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	 TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

	 TIM_BDTRStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	 TIM_BDTRConfig(TIM1, &TIM_BDTRStruct);

	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	 TIM_OCInitStructure.TIM_Pulse = 1000;
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	 TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

	 TIM_OC1Init( TIM1, &TIM_OCInitStructure);


	 TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);

	 //TIM_SetCompare2( TIM1, 1000);
	 TIM_ARRPreloadConfig( TIM1, ENABLE);
	 TIM_Cmd( TIM1, ENABLE );
	 //TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    printf("usart init%d\n", TIM_GetCounter(TIM1));
    USARTx_CFG();
    printf("init timer\n");
    timTestInit();

    //TIM_ForcedOC2Config(TIM1, TIM_ForcedAction_Active);

    while(1)
    { printf("timerVal%d\n", TIM_GetCounter(TIM1));
    	printf("compare flag %d\n", TIM_GetFlagStatus(TIM1, TIM_FLAG_CC1));
    }
}
