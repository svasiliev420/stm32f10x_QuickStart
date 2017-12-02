#include "main.h"

void SystemClock_Config(void);
void delay(int val)
{
    for(int i=0; i<val;i++){}
}



int main(void)
{
    SysTick_Config(SystemCoreClock / 100);      // Конфигурируем таймер SysTick на срабатывание 100 раз в секунду 
    SystemClock_Config();
    
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    
   
    // Настройка выводов согласно выполняемым функциям: 
    // Вывод управления NSS: выход двухтактный, общего назначения, 50 МГц 
    GPIOA->CRL   |=  GPIO_CRL_MODE4; 
    GPIOA->CRL   &= ~GPIO_CRL_CNF4; 
    GPIOA->BSRR   =  GPIO_BSRR_BS4; 
    // Вывод SCK: выход двухтактный, альтернативная функция, 50 МГц 
    GPIOA->CRL   |=  GPIO_CRL_MODE5; 
    GPIOA->CRL   &= ~GPIO_CRL_CNF5; 
    GPIOA->CRL   |=  GPIO_CRL_CNF5_1; // Вывод MISO: вход цифровой с подтягивающим резистором, подтяжка к плюсу 
    GPIOA->CRL   &= ~GPIO_CRL_MODE6; 
    GPIOA->CRL   &= ~GPIO_CRL_CNF6; 
    GPIOA->CRL   |=  GPIO_CRL_CNF6_1; 
    GPIOA->BSRR   =  GPIO_BSRR_BS6; // Вывод MOSI: выход двухтактный, альтернативная функция, 50 МГц 
    GPIOA->CRL   |=  GPIO_CRL_MODE7; 
    GPIOA->CRL   &= ~GPIO_CRL_CNF7; 
    GPIOA->CRL   |=  GPIO_CRL_CNF7_1;
    
    // Настройка SPI 
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
    SPI1->CR1 = 0x0000;                 // Очистить первый управляющий регистр 
    SPI1->CR1 |= LL_SPI_DATAWIDTH_8BIT; // Бит11 SPI_CR1_DFF Формат данных 0-8бит 1-16бит 
    SPI1->CR1 |= LL_SPI_NSS_SOFT;       // Бит9 SPI_CR1_SSM SSM – выбирает источник сигнала NSS (0 — с внешнего вывода, 1 — программно); 
    SPI1->CR1 |= SPI_CR1_SSI;           // Бит8 SSI – если SSM = 1 определяет значение NSS; 
    SPI1->CR1 |= SPI_CR1_LSBFIRST;      // Бит7 LSBFIRST – задаёт способ передачи (0 - старшим, 1 — младшим разрядом вперёд); 
    SPI1->CR1 |= SPI_CR1_SPE;           // Бит6 SPE - работа SPI (1 – вкл. 0 – откл.) Бит3-5 BR[2:0] — делитель скорости обмена fPCLK/x (000:2, 001:4, 010:8, 011:16, 100:32, 101:64, 110:128, 111:256) 
    SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2; // Задать скорость fPCLK/x 
    SPI1->CR1 |= LL_SPI_MODE_MASTER;        // SPI_CR1_MSTR;      - делает модуль ведущим(1)/ведомым(0); 
    SPI1->CR1 |= LL_SPI_POLARITY_LOW;       // SPI_CR1_CPOL;      - задаёт полярность тактового сигнала; 
    SPI1->CR1 |= SPI_CR1_CPHA;              // CPHA - задаёт фазу тактового сигнала 0-\ 1-/; 
    SPI1->CR2 = 0x0000;                     // Очистить второй управляющий регистр 
    
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_9,    LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_9,    LL_GPIO_PULL_UP);
    LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_9,   LL_GPIO_MODE_OUTPUT_2MHz);
    
    loop:while (1)
    {
        delay(500*2048);
        LL_GPIO_TogglePin(GPIOC,LL_GPIO_PIN_9);
    }
}

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 24000000
  *            HCLK(Hz)                       = 24000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 2
  *            PLLMUL                         = 6
  *            Flash Latency(WS)              = 0
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Enable HSE oscillator */
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_2, LL_RCC_PLL_MUL_6);
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms */
  SysTick_Config(24000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 24000000;
}
