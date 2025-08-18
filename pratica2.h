#include "apresentacao.h"

#ifndef INC_PRATICA2_H_
#define INC_PRATICA2_H_

#include <math.h>

void questao_1(){

	DAC_Init(DAC_CHANNEL1);

	while(1){
		for(int i=0; i<4096; i++){
			Delay_ms(1);
			DAC_SetValue(DAC_CHANNEL1,i,DAC_RES_12BITS);
		}

		for(int i=0; i<4096; i++){
			Delay_ms(1);
			DAC_SetValue(DAC_CHANNEL1,4095 - i,DAC_RES_12BITS);
		}
	}

}

void questao_2() {
  /* ENUNCIADO:
    2. Gerar no pino PA5 um sinal analógico senoidal, conforme mostrado na figura abaixo, com resolução de 12 bits, com uma frequência de ______ Hz, amplitude de ______ V de pico-a-pico (Vpp) e valor médio (Vm) de _______ V. Utilize 500 amostras de 12 bits para representar um ciclo do sinal senoidal. Utilize uma interrupção de timer para controlar os instantes de atualização do conversor DAC.
  */

  /* MONTAGEM:
    PA5 (DAC2) -> OSCILOSCÓPIO
  */

  /* CONFIGURAÇÃO */
  int freq = 700;
  int amostra = 500;
  int periodo_us = 1000000 / freq;
  double Vpp = 1.5;
  double Vm = 1.8;
  double passo_rad = 2 * M_PI / amostra;
  uint16_t amostras[amostra];

  for (int i = 0; i < amostra; i++)
    amostras[i] = (sin(i * passo_rad) * Vpp / 2 + Vm) / 3.3 * 4095;

  Utility_Init();

  DAC_Init(DAC_CHANNEL2);

  /* LOOP PRINCIPAL */
  while (1) {
    for (int i = 0; i < amostra; i++) {
      DAC_SetValue(DAC_CHANNEL2, amostras[i], DAC_RES_12BITS);
      Delay_us(periodo_us/500);
    }
  }
}

void questao_3(){
''''
	ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
	ADC_SingleChannel(ADC1, ADC_IN0);

	ADC1->LTR = 1116; //limiar inferior
	ADC1->HTR = 1985; //limiar superior
	ADC1->CR1 |= ADC_CR1_AWDSGL; //habilita watchdog no canal simples
	ADC1->CR1 &= ~ADC_CR1_AWDCH; //seleção do canal monitorado (IN_0)
	ADC1->CR1 |= ADC_CR1_AWDEN; // habilita o watchdog
}
void loop_questao_3(){
	ADC1->CR2 |= ADC_CR2_SWSTART;
	while(!(ADC1->SR & ADC_SR_EOC));
	printf("LEITURA = %.2f\n", ((float)ADC1->DR *3.3)/4095);

	if(ADC1->SR & ADC_SR_AWD)
		printf("SAIDA DA REGIAO MONITORADA\n");

	ADC1->SR &= ~ADC_SR_AWD;

	Delay_ms(500);
}

void questao_4(){

	void Setup_Timer5_21us() {
	  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;   // Habilita clock do TIM5
	  TIM5->PSC = 83;     // 84MHz / (71 + 1) = 1MHz
	  TIM5->ARR = 20;       // 1MHz / (20+1) = 47619Hz → 21us período
	  TIM5->CCR2 = 50;      // 50% duty cycle (opcional, se usar PWM no canal 2)
	  TIM5->CCMR1 &= ~(TIM_CCMR1_OC2M);        // Limpa OC2M
	  TIM5->CCMR1 |= (0b110 << 12);            // PWM mode 1 no CH2
	  TIM5->CCMR1 |= TIM_CCMR1_OC2PE;          // Preload enable
	  TIM5->CCER |= TIM_CCER_CC2E;             // Habilita saída do canal 2
	  TIM5->EGR |= TIM_EGR_UG;                 // Gera atualização
	  TIM5->CR1 |= TIM_CR1_CEN;                // Inicia o timer

	  // INTERRUPÇÃO TIMER 5
	  TIM5->DIER |= TIM_DIER_UIE;       // Habilita interrupção de atualização
	  NVIC_EnableIRQ(TIM5_IRQn);        // Habilita no NVIC
	}

	DAC_Init(DAC_CHANNEL1);

	Setup_Timer5_21us();
}
void questao_5() {
	// 09/06/2025 - joystick para mover o servomotor
	// funcões
	void servomotor_para(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t angulo) {
		// enviar o micro servomotor para um determinado ângulo
		int pulso_us = 500 + (2000 * angulo) / 180;
		int periodo_us = 20000;

		GPIO_Write_Pin(GPIOx, pin, HIGH);
		Delay_us(pulso_us);
		GPIO_Write_Pin(GPIOx, pin, LOW);
		Delay_us(periodo_us - pulso_us);
	}

	// setup
	ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
	ADC_Init(ADC2, SINGLE_CHANNEL, ADC_RES_12BITS);
	ADC_SingleChannel(ADC1, ADC_IN0);
	ADC_SingleChannel(ADC2, ADC_IN1);

	GPIO_Pin_Mode(GPIOA, PIN_2, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_3, OUTPUT);

	uint16_t leituraX, leituraY, SPEED = 10;
	int anguloX, anguloY, limite_esquerda = 2047, limite_direita = 2049;

	// loop principal
	while (1) {
		leituraX = ADC_GetSingleConversion(ADC1);
		leituraY = ADC_GetSingleConversion(ADC2);

		servomotor_para(GPIOA, PIN_2, (leituraX * 180) / 4095);
		servomotor_para(GPIOA, PIN_3, (leituraY * 180) / 4095);
	}
}

void questao_6(){
	void Setup_Timer5_20ms() {
		 // 1. Habilita o clock do TIM5 (APB1)
		    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

		    TIM5->CR1 &= ~(TIM_CR1_DIR);
		    // 2. Configura o prescaler e o auto-reload para gerar interrupção a cada 10ms
		    // Timer clock = 84MHz (APB1 * 2) -> PSC = 8399 → 10kHz
		    // ARR = 99 → 10kHz / (99 + 1) = 100Hz → 10ms
		    TIM5->PSC = 8399;  // 84MHz / (8399 + 1) = 10kHz
		    TIM5->ARR = 199;    // 10kHz / (99 + 1) = 100Hz → 10ms

		    // 3. Gera um evento de atualização para carregar os registros
		    TIM5->EGR = TIM_EGR_UG;

		    // 4. Habilita interrupção de atualização (quando contador reinicia)
		    TIM5->DIER |= TIM_DIER_UIE;

		    // 5. Habilita a interrupção no NVIC
		    NVIC_EnableIRQ(TIM5_IRQn);

		    // 6. Inicia o Timer
		    TIM5->CR1 |= TIM_CR1_CEN;
	}
	void Setup_Timer3_1000ms() {
			 // 1. Habilita o clock do TIM5 (APB1)
			    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

			    TIM3->CR1 &= ~(TIM_CR1_DIR);
			    // 2. Configura o prescaler e o auto-reload para gerar interrupção a cada 10ms
			    // Timer clock = 84MHz (APB1 * 2) -> PSC = 8399 → 10kHz
			    // ARR = 99 → 10kHz / (99 + 1) = 100Hz → 10ms
			    TIM3->PSC = 8399;  // 84MHz / (8399 + 1) = 10kHz
			    TIM3->ARR = 2499;    // 10kHz / (9999 + 1) = 1Hz → 1000ms

			    // 3. Gera um evento de atualização para carregar os registros
			    TIM3->EGR = TIM_EGR_UG;

			    // 4. Habilita interrupção de atualização (quando contador reinicia)
			    TIM3->DIER |= TIM_DIER_UIE;

			    // 5. Habilita a interrupção no NVIC
			    NVIC_EnableIRQ(TIM3_IRQn);

			    // 6. Inicia o Timer
			    TIM3->CR1 |= TIM_CR1_CEN;
		}

	GPIO_Clock_Enable(GPIOA);
	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_7, OUTPUT);
	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
	GPIO_Write_Pin(GPIOA, PIN_7, HIGH);

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // Habilita clock ADC1

	ADC->CCR |= ADC_CCR_TSVREFE | (0b01 << 16); // Ativa sensor de temp e prescaler /4

	ADC1->SQR1 &= ~(0xF << 20); // 1 conversão
	ADC1->SQR3 = 16;            // Canal 16 (sensor temp)
	ADC1->SMPR1 |= (7 << 18);   // 480 ciclos

	ADC1->CR2 |= ADC_CR2_ADON;  // Liga ADC

	Setup_Timer5_20ms();
	Setup_Timer3_1000ms();
}
void questao_7(){
	void setup_Timer5_pwm(){
		GPIO_Clock_Enable(GPIOA);
		GPIO_Pin_Mode(GPIOA, PIN_2, ALTERNATE);
		GPIO_Alternate_Function(GPIOA, PIN_2, AF2);

		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; //liga o clock do Timer5
		TIM5->CR1 &= ~TIM_CR1_DIR; //contador crescente
		TIM5->ARR = 99; //auto reload=99
		TIM5->PSC = 419; //prescaler para pulsos a cada 5uS
		TIM5->CCMR2 |= 0b110 << 4; //seleciona PWM modo 1
		TIM5->CCMR2 |= TIM_CCMR2_OC3PE; //habilita o pré-carregamento de CCR3
		TIM5->CCER |= TIM_CCER_CC3E; //habilita a saída
		TIM5->EGR = TIM_EGR_UG; //update event para escrever PSC e ARR
		TIM5->CR1 |= TIM_CR1_CEN; //habilita o timer
	}

	void setup_Timer3_delay_100ms(){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Ativa Clock Timer 3
		TIM3->CR1 &= ~(TIM_CR1_DIR); // Direção Crescente
		TIM3->PSC = 8399;  // 84MHz / (8399 + 1) = 10kHz
		TIM3->ARR = 999;    // 10kHz / (999 + 1) = 10Hz → 100ms
		// 3. Gera um evento de atualização para carregar os registros
		TIM3->EGR = TIM_EGR_UG;
		// 4. Habilita interrupção de atualização (quando contador reinicia)
		TIM3->DIER |= TIM_DIER_UIE;
		// 5. Habilita a interrupção no NVIC
		NVIC_EnableIRQ(TIM3_IRQn);
		// 6. Inicia o Timer
		TIM3->CR1 |= TIM_CR1_CEN;
	}

	GPIO_Clock_Enable(GPIOA);
	GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_3, OUTPUT);

	ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
	ADC_SingleChannel(ADC1, ADC_IN0);

	setup_Timer5_pwm();
	setup_Timer3_delay_100ms();
}
void questao_8_9_10() {


    // 1. Habilita clocks dos GPIOs
    GPIO_Clock_Enable(GPIOA);
    GPIO_Clock_Enable(GPIOB);
    GPIO_Clock_Enable(GPIOE);

    // 2. Configura PB0 e PB1 como entrada com pull-up
    GPIO_Pin_Mode(GPIOB, PIN_0, INPUT);
    GPIO_Pin_Mode(GPIOB, PIN_1, INPUT);
    GPIO_Resistor_Enable(GPIOB, PIN_0, PULL_UP);
    GPIO_Resistor_Enable(GPIOB, PIN_1, PULL_UP);

    // 3. Configura PE3 (K1) e PE4 (K0) como entrada com pull-up
    GPIO_Pin_Mode(GPIOE, PIN_3, INPUT);
    GPIO_Pin_Mode(GPIOE, PIN_4, INPUT);
    GPIO_Resistor_Enable(GPIOE, PIN_3, PULL_UP);
    GPIO_Resistor_Enable(GPIOE, PIN_4, PULL_UP);

    // 4. Configura PA6 e PA7 como saída (LEDs D2 e D3)
    GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
    GPIO_Pin_Mode(GPIOA, PIN_7, OUTPUT);
    GPIO_Write_Pin(GPIOA, PIN_6, HIGH);    // Garante que LED D2 inicie desligado
    GPIO_Write_Pin(GPIOA, PIN_7, HIGH);    // Garante que LED D3 inicie desligado

    // 5. Configura PA1 como saída alternativa para PWM (TIM5_CH2)
    GPIO_Pin_Mode(GPIOA, PIN_1, ALTERNATE);
    GPIO_Alternate_Function(GPIOA, PIN_1, AF2); // AF2 = TIM5_CH2

    // 6. Configura Timer 5 para PWM de 1 Hz (50% duty)
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    TIM5->PSC = 7199;     // 72MHz / (7199+1) = 10kHz
    TIM5->ARR = 9999;     // 10kHz / (9999+1) = 1Hz
    TIM5->CCR2 = 5000;    // 50% duty
    TIM5->CCMR1 |= 0b110 << 12;      // PWM mode 1 (CH2)
    TIM5->CCMR1 |= TIM_CCMR1_OC2PE;
    TIM5->CCER |= TIM_CCER_CC2E;
    TIM5->EGR = TIM_EGR_UG;
    TIM5->CR1 |= TIM_CR1_CEN;


    // Timer2 para LED D2 (2 segundos)
	  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	  TIM2->PSC = 7199;
	  TIM2->ARR = 19999;  // 2s
	  TIM2->DIER |= TIM_DIER_UIE;
	  NVIC_EnableIRQ(TIM2_IRQn);

	  // Timer3 para LED D3 (4 segundos)
	  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	  TIM3->PSC = 7199;
	  TIM3->ARR = 39999;  // 4s
	  TIM3->DIER |= TIM_DIER_UIE;
	  NVIC_EnableIRQ(TIM3_IRQn);


    // 8. Habilita interrupções de borda de subida nos botões
    EXTI_Config(EXTI0, GPIOB, RISING_EDGE); // PB0
    EXTI_Config(EXTI1, GPIOB, RISING_EDGE); // PB1
    EXTI_Config(EXTI3, GPIOE, FALLING_EDGE); // PE3 (K1)
    EXTI_Config(EXTI4, GPIOE, FALLING_EDGE); // PE4 (K0)

    // 9. Habilita NVIC para interrupções externas
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_EnableIRQ(EXTI3_IRQn);
    NVIC_EnableIRQ(EXTI4_IRQn);

    NVIC_SetPriority(EXTI0_IRQn, 0);
    NVIC_SetPriority(EXTI1_IRQn, 0);
    NVIC_SetPriority(EXTI3_IRQn, 0);
    NVIC_SetPriority(EXTI4_IRQn, 0);
}

#endif /* INC_PRATICA2_H_ */
