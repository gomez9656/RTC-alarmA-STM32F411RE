/*
 * This project is to use the RTC alarm A and trigger an interrupt
*/
#include "stm32f4xx.h"
#include "main.h"
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

/*
 * Functions prototypes
 */
void SystemCoreClockConfig(uint8_t clock_freq);
void Error_handler(void);
void GPIO_Init(void);
void UART2_Init(void);
void RTC_Init(void);
void RTC_CalendarConfig(void);
void HAL_GPIO_EXTI_Callback(uint16_t Gpio_pin);
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);
void RTC_AlarmConfig(void);

UART_HandleTypeDef huart2; //Handle of UART 2
RTC_HandleTypeDef hrtc;

void printmsg(char *format, ...)
 {

	char str[80];

	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, format);
	vsprintf(str, format,args);
	HAL_UART_Transmit(&huart2,(uint8_t *)str, strlen(str),HAL_MAX_DELAY);
	va_end(args);

 }

int main(){


	/* Basic initialization  */
	HAL_Init();

	//SystemCoreClockConfig(SYS_CLOCK_FREQ_50_MHZ);

	GPIO_Init();

	UART2_Init();

	RTC_Init();

	while(1){
	}

	return 0;
}

void RTC_Init(void){

	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
	hrtc.Init.AsynchPrediv = 0x7F;
	hrtc.Init.SynchPrediv = 0xFF;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	if( HAL_RTC_Init(&hrtc) != HAL_OK){
		Error_handler();
	}

}

/*
 * This function configures the calendar for time 12:11:10 PM Date
 * 22 January 2021 Monday. The RTC needs BCD values
 */

void RTC_CalendarConfig(void){

	RTC_TimeTypeDef RTC_TimeInit;
	RTC_DateTypeDef RTC_DateInit;

	//This are not BCD values, the HAL_RTC_SetTime will change it for us
	RTC_TimeInit.Hours = 12;
	RTC_TimeInit.Minutes = 45;
	RTC_TimeInit.Seconds = 00;
	RTC_TimeInit.TimeFormat = RTC_HOURFORMAT12_PM;

	HAL_RTC_SetTime(&hrtc, &RTC_TimeInit, RTC_FORMAT_BIN);

	RTC_DateInit.Date = 22;
	RTC_DateInit.Month = RTC_MONTH_JANUARY;
	RTC_DateInit.Year = 21;
	RTC_DateInit.WeekDay = RTC_WEEKDAY_MONDAY;

	HAL_RTC_SetDate(&hrtc, &RTC_DateInit, RTC_FORMAT_BIN);

}

/*
 * You can use it when you need an specific clock
 * By default will be internal clock
 */
void SystemCoreClockConfig(uint8_t clock_freq){

	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSE;
	Osc_Init.HSIState = RCC_HSI_ON;
	Osc_Init.LSEState = RCC_LSE_ON;
	Osc_Init.HSEState = RCC_HSE_ON;
	Osc_Init.HSICalibrationValue = 16;
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq){
	  case SYS_CLOCK_FREQ_50_MHZ:
			  Osc_Init.PLL.PLLM = 8;
			  Osc_Init.PLL.PLLN = 50;
			  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
			  Osc_Init.PLL.PLLQ = 2;
			  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
			  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
			  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
		     break;

		  case SYS_CLOCK_FREQ_84_MHZ:
			  Osc_Init.PLL.PLLM = 8;
			  Osc_Init.PLL.PLLN = 84;
			  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
			  Osc_Init.PLL.PLLQ = 2;
			  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
			  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
			  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
		     break;

		  case SYS_CLOCK_FREQ_120_MHZ:
			  Osc_Init.PLL.PLLM = 8;
			  Osc_Init.PLL.PLLN = 120;
			  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
			  Osc_Init.PLL.PLLQ = 2;
			  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
			  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
			  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
		     break;

		  default:
		   return ;
		 }

			if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
		{
				Error_handler();
		}



		if (HAL_RCC_ClockConfig(&Clock_Init, FLASH_LATENCY_2) != HAL_OK)
		{
			Error_handler();
		}


		/*Configure the systick timer interrupt frequency (for every 1 ms) */
		uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
		HAL_SYSTICK_Config(hclk_freq/1000);

		/**Configure the Systick
		*/
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

		/* SysTick_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/*
 *	This function helps us reduce the current consumption by establishing the
 *	other pins we are not using, in Analog mode. It is not mandatory
 */

void UART2_Init(void){

	//1. Linking handle struct to base address
	huart2.Instance = USART2;

	//2. Init the handle variable
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;

	//3. Use the API to initialize
	if(HAL_UART_Init(&huart2) != HAL_OK){

		//There is a problem
		Error_handler();
	}
}


void GPIO_Init(void){

	__HAL_RCC_GPIOC_CLK_ENABLE();


	GPIO_InitTypeDef buttongpio;

	buttongpio.Pin = GPIO_PIN_13;//This pin will wake up the processor from sleep mode
	buttongpio.Mode = GPIO_MODE_IT_FALLING;
	buttongpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &buttongpio);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

char* getDayofWeek(uint8_t number){
	char *weekday[] = {	"Monday",
						"Tuesday",
						"Wednesday",
						"Thursday",
						"Friday",
						"Saturday",
						"Sunday"
	};

	return weekday[number - 1];
}

void HAL_GPIO_EXTI_Callback(uint16_t Gpio_pin){

	RTC_TimeTypeDef RTC_TimeRead;
	RTC_DateTypeDef RTC_DateRead;

	RTC_CalendarConfig();

	HAL_RTC_GetTime(&hrtc, &RTC_TimeRead, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTC_DateRead, RTC_FORMAT_BIN);

	printmsg("Current time is: %02d:%02d:%02d\r\n", RTC_TimeRead.Hours, \
			RTC_TimeRead.Minutes, RTC_TimeRead.Seconds);

	printmsg("Current data is :%02d-%02d-%02 <%s> d\r\n", RTC_DateRead.Month, \
			RTC_DateRead.Date, RTC_DateRead.Year, getDayofWeek(RTC_DateRead.Date));

	RTC_AlarmConfig();
}

void RTC_AlarmConfig(void){

	RTC_AlarmTypeDef AlarmA_Set;

	memset(&AlarmA_Set, 0, sizeof(AlarmA_Set)); //Avoid garbage values in automatic variables

	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	//xx.45:09
	AlarmA_Set.Alarm = RTC_ALARM_A;
	AlarmA_Set.AlarmTime.Minutes = 45;
	AlarmA_Set.AlarmTime.Seconds = 9;
	AlarmA_Set.AlarmMask = RTC_ALARMMASK_HOURS | RTC_ALARMMASK_DATEWEEKDAY;
	AlarmA_Set.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;

	if(HAL_RTC_SetAlarm_IT(&hrtc, &AlarmA_Set, RTC_FORMAT_BIN) != HAL_OK){
		Error_handler();
	}
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){

	printmsg("Alarm triggered \r\n");

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}

void Error_handler(void){
	while(1);
}
