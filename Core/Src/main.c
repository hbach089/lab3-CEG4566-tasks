#include "main.h"
#include "cmsis_os.h"
#include "Keypad4X4.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fonts.h"
#include "ssd1306.h"
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osMessageQId myQueue01Handle;
extern char key;
char hold[4];
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
void StartTask02(void const * argument);
void StartTask03(void const * argument);
int main(void)
{
HAL_Init();
SystemClock_Config();
MX_GPIO_Init();
MX_USART2_UART_Init();
MX_I2C1_Init();
SSD1306_Init();

SSD1306_GotoXY(0, 0);
SSD1306_Puts("Not Armed!", &Font_11x18, 1);
SSD1306_GotoXY(0, 30);
SSD1306_Puts("Code:", &Font_11x18, 1);
SSD1306_UpdateScreen();
SSD1306_UpdateScreen();
HAL_Delay(500);
osMessageQDef(myQueue01, 16, char);
myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), NULL);
osThreadDef(myTask02, StartTask02, osPriorityAboveNormal, 0, 128);
myTask02Handle = osThreadCreate(osThread(myTask02), NULL);
osThreadDef(myTask03, StartTask03, osPriorityNormal, 0, 128);
myTask03Handle = osThreadCreate(osThread(myTask03), NULL);
osKernelStart();
while (1)
{
}
}
void SystemClock_Config(void)
{
RCC_OscInitTypeDef RCC_OscInitStruct = {0};
RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
__HAL_RCC_PWR_CLK_ENABLE();
__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
RCC_OscInitStruct.HSIState = RCC_HSI_ON;
RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
RCC_OscInitStruct.PLL.PLLM = 16;
RCC_OscInitStruct.PLL.PLLN = 336;
RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
RCC_OscInitStruct.PLL.PLLQ = 2;
RCC_OscInitStruct.PLL.PLLR = 2;
if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
{
Error_Handler();
}
RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)

}
}
static void MX_I2C1_Init(void)
{
hi2c1.Instance = I2C1;
hi2c1.Init.ClockSpeed = 400000;
hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
hi2c1.Init.OwnAddress1 = 0;
hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
hi2c1.Init.OwnAddress2 = 0;
hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
if (HAL_I2C_Init(&hi2c1) != HAL_OK)
{
Error_Handler();
}
}
static void MX_USART2_UART_Init(void)
{
huart2.Instance = USART2;
huart2.Init.BaudRate = 9600;
huart2.Init.WordLength = UART_WORDLENGTH_8B;
huart2.Init.StopBits = UART_STOPBITS_1;
huart2.Init.Parity = UART_PARITY_NONE;
huart2.Init.Mode = UART_MODE_TX_RX;
huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
huart2.Init.OverSampling = UART_OVERSAMPLING_16;
if (HAL_UART_Init(&huart2) != HAL_OK)
{
Error_Handler();
}
}
static void MX_GPIO_Init(void)
{
GPIO_InitTypeDef GPIO_InitStruct = {0};
__HAL_RCC_GPIOC_CLK_ENABLE();
__HAL_RCC_GPIOH_CLK_ENABLE();
__HAL_RCC_GPIOA_CLK_ENABLE();
__HAL_RCC_GPIOB_CLK_ENABLE();
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, KC0_Pin | KC3_Pin | KC1_Pin | KC2_Pin,
GPIO_PIN_RESET);
GPIO_InitStruct.Pin = GPIO_PIN_5;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
GPIO_InitStruct.Pin = GPIO_PIN_6;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
GPIO_InitStruct.Pin = GPIO_PIN_7;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
GPIO_InitStruct.Pin = KC0_Pin | KC3_Pin | KC1_Pin | KC2_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
GPIO_InitStruct.Pin = KR1_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLDOWN;
HAL_GPIO_Init(KR1_GPIO_Port, &GPIO_InitStruct);
GPIO_InitStruct.Pin = KR3_Pin | KR2_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLDOWN;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
GPIO_InitStruct.Pin = KR0_Pin;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLDOWN;
HAL_GPIO_Init(KR0_GPIO_Port, &GPIO_InitStruct);
}
void StartTask02(void const * argument)
{
char receivedKey;
char enteredCode[7] = {0};
uint8_t codeLength = 0;
const uint8_t maxCodeLength = 6;
const char correctCode[7] = "000000";
int armed = 0;
for (;;)
{
if (armed)
{
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, SET);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
osDelay(2000);
}
else
{
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, SET);

HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, RESET);
}
if (xQueueReceive(myQueue01Handle, &receivedKey, portMAX_DELAY) == pdPASS)
{
if (receivedKey == '#' && codeLength == maxCodeLength)
{
if (strncmp(enteredCode, correctCode, maxCodeLength) == 0)
{
SSD1306_Clear();
SSD1306_GotoXY(0, 0);
SSD1306_Puts("Success!", &Font_11x18, 1);
if (armed)
{
armed = 0;
}
else
{
armed = 1;
}
}
else
{
SSD1306_Clear();
SSD1306_GotoXY(0, 0);
SSD1306_Puts("Failed!", &Font_11x18, 1);
}
SSD1306_UpdateScreen();
HAL_Delay(2000);
SSD1306_Clear();
SSD1306_GotoXY(0, 0);
if (armed)
{
SSD1306_Puts("Armed!", &Font_11x18, 1);
}
else
{
SSD1306_Puts("Not Armed!", &Font_11x18, 1);
}
SSD1306_GotoXY(0, 30);
SSD1306_Puts("Code:", &Font_11x18, 1);
SSD1306_UpdateScreen();
memset(enteredCode, 0, sizeof(enteredCode));
codeLength = 0;
}
else
{
if (codeLength < maxCodeLength)
{
enteredCode[codeLength] = receivedKey;
SSD1306_Puts("*", &Font_11x18, 1);
SSD1306_UpdateScreen();
codeLength++;
}
else
{

  printf("Error: Max Code Length Reached!\r\n");
}
}
}
}
}
void StartTask03(void const * argument)
{
const TickType_t xDelay = 20 / portTICK_PERIOD_MS;
char keyToSend;
for (;;)
{
keyToSend = Get_Key();
if (keyToSend != '\0')
{
if (xQueueSend(myQueue01Handle, &keyToSend, portMAX_DELAY) != pdPASS)
{
printf("Error: Data couldn't be sent from task 2\r\n");
}
}
vTaskDelay(xDelay);
}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
if (htim->Instance == TIM6)
{
HAL_IncTick();
}
}
void Error_Handler(void)
{
__disable_irq();
while (1)
{
}
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */