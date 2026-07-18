/* USER CODE BEGIN Header */
/**
 * @file main.c
 * @brief Main Function Implementation
 * @details This function is responsible for initializing every peripheral and the kernel.
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "fdcan.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <string.h>
#include "fdcan.h"
#include "tim.h"
#include "usart.h"

//#include <csp/csp.h>
//#include <csp/csp_conn.h>
//#include <csp/csp_promisc.h>
//#include "../../CAN Driver/include/can_stm32.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
extern void MX_USB_DEVICE_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief Peripheral and RTOS Initialization.
 * @details
 * 1. Every peripheral is initialized.
 * 2. RTOS Kernel is initialized.
 * 3. MX_FREERTOS_Init() is called to initialize the Root Task
 * 4. The kernel is started. From now on there is no serial execution and the scheduler has control.
 * @note 1. There is a 5 sec delay before initializing the OS to make sure all peripherals are brought up fully.
 * @note 2. TIM7 is used to keep track of FreeRTOS Statistics (FreeRTOS Task List, Timers etc) for debugging purposes.
 * @note 3. Look into Flash and Debug Manual for more info on how Cortex-M7 handles flashing.
 * @warning Control should never reach main's inifinite loop because that would mean the RTOS has failed.
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
/* USER CODE BEGIN Boot_Mode_Sequence_0 */
  int32_t timeout;
/* USER CODE END Boot_Mode_Sequence_0 */

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
  Error_Handler();
  }
/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
/* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
HSEM notification */
/*HW semaphore Clock enable*/
__HAL_RCC_HSEM_CLK_ENABLE();
/*Take HSEM */
HAL_HSEM_FastTake(HSEM_ID_0);
/*Release HSEM in order to notify the CPU2(CM4)*/
HAL_HSEM_Release(HSEM_ID_0,0);
/* wait until CPU2 wakes up from stop mode */
timeout = 0xFFFF;
while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
if ( timeout < 0 )
{
Error_Handler();
}
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_TIM7_Init();
  MX_FDCAN1_Init();
//  MX_IWDG1_Init();
  /* USER CODE BEGIN 2 */

  MX_USB_DEVICE_Init();
  HAL_Delay(5000);
  /* Setting the debug level. Only debugging statements up to 'debug_level' will be printed to the debug terminal. */
//  csp_debug_level_t debug_level = CSP_INFO;
//  csp_debug_level_t debug_level = CSP_PACKET;
//  for (csp_debug_level_t i = 0; i <= CSP_LOCK; ++i) {
//	  csp_debug_set_level(i, (i <= debug_level) ? true: false);
//  }
//
//  /* Initialising CSP */
//  csp_log_info("Initialising CSP");
//  csp_conf_t csp_conf;
//  csp_conf_get_defaults(&csp_conf);
//  uint8_t csp_address = 255;
//  csp_conf.address = csp_address;
//  int error = csp_init(&csp_conf);
//  if (error != CSP_ERR_NONE) {
//	  csp_log_error("csp_init() failed, error: %d", error);
//  }
//  /* Add interface(s) */
//  csp_iface_t CSP_IF_CAN = {
//		  .name = "CSP IF CAN",
//		  .driver_data = &hfdcan1,
////		  .nexthop = csp_can_tx_stm32,
//		  .mtu = 64,
//  };
//  csp_iface_t *can_iface = &CSP_IF_CAN;
//  error = csp_can_stm32_open_and_add_interface(CSP_IF_CAN.name, &can_iface, 0);
//  if (error != CSP_ERR_NONE) {
//	  csp_log_error("csp_can_stm32_open_and_add_interface() failed, error: %d", error);
//  }
//  /* Setting route table */
//  if (can_iface) {
////	  csp_rtable_set(CSP_DEFAULT_ROUTE, 0, can_iface, CSP_NO_VIA_ADDRESS);
//	  csp_route_set(CSP_DEFAULT_ROUTE, can_iface, CSP_NO_VIA_ADDRESS);
//  } else {
//	  csp_log_info("Couldn't set route table");
////	  server_address = csp_address;
//  }
//  /* Start router task with 1000 bytes of stack (priority is only supported on FreeRTOS) */
//  if(csp_route_start_task(1000, 1) != CSP_ERR_NONE){
//	  csp_log_warn("Failed to start router!");
//  }
//  MX_IWDG1_Init();
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_CRSInitTypeDef RCC_CRSInitStruct = {0};

  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI
                              |RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 9;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 6;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOMEDIUM;
  RCC_OscInitStruct.PLL.PLLFRACN = 3072;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_RCC_CRS_CLK_ENABLE();

  RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;
  RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB2;
  RCC_CRSInitStruct.Polarity = RCC_CRS_SYNC_POLARITY_RISING;
  RCC_CRSInitStruct.ReloadValue = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000,1000);
  RCC_CRSInitStruct.ErrorLimitValue = 34;
  RCC_CRSInitStruct.HSI48CalibrationValue = 32;

  HAL_RCCEx_CRSConfig(&RCC_CRSInitStruct);
}

/* USER CODE BEGIN 4 */
//int _write(int file, char *ptr, int len) {
//    uint8_t result = CDC_Transmit_FS((uint8_t*)ptr, len);
//
//    // If the USB is busy, wait until it's ready to send again
//    while (result == USBD_BUSY) {
//        result = CDC_Transmit_FS((uint8_t*)ptr, len);
//    }
//    return len;
//}
int _write(int file, char *ptr, int len) {
    // 1. Check if the USB is actually ready
//    if (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) {
//        return len; // Drop data silently if USB is not connected
//    }

    // 2. Try to transmit
    uint8_t result = CDC_Transmit_FS((uint8_t*)ptr, len);

    // 3. Instead of a hard 'while', wait for a small amount of time
    // If it's still busy after a short time, just exit to keep the system alive
    uint32_t timeout = 50;
    while (result == USBD_BUSY && timeout > 0) {
        osDelay(1); // Yield to the scheduler!
        result = CDC_Transmit_FS((uint8_t*)ptr, len);
        timeout--;
    }

    return len;
}
/* USER CODE END 4 */

/**
 * @brief TIM6 callback function.
 * @details Whenever TIM6 overflows, the callback increments the global variable "uwTick" used as application time base for Cortex-M7.
 * @warning
 * To prevent interrupt priority inversion and deadlocks, we separate the
 * RTOS timebase from the HAL timebase using TIM6 as the HAL timebase and SysTick for RTOS
 *
 *
 * 1. SysTick is dedicated exclusively to the RTOS and is
 *    assigned the LOWEST priority so it doesn't block time-critical ISRs.
 *
 * 2. A dedicated hardware timer (TIM6) is used for the HAL timebase
 *    (HAL_Delay) and is assigned a HIGH priority. This ensures HAL timeouts
 *    can safely execute even when called from within peripheral interrupts.
 *
 *
 * Using SysTick for both would cause a deadlock if a HAL function with a
 * timeout were ever called from a high-priority interrupt handler.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
