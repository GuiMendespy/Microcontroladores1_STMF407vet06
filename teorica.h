#ifndef INC_TEORICA_H_
#define INC_TEORICA_H_

// ADC Exemplo da aula 13
void setup_ADC(){
	GPIO_Clock_Enable(GPIOA);
	GPIO_Pin_Mode(GPIOA, PIN_0, ANALOG);
}

void config_ADC(){
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   //liga o clock do adc1
	ADC->CCR |= 0b01 << 16; //prescaler /4 (fADC  =84/4 = 21MHZ)
	ADC1->SQR1 &= ~(0xF << 20); // conversão de apenas um canal
	ADC1->SQR3 &= ~(0x1F); //seleção do canal a ser convertido (IN_0)
	ADC1->SMPR2 |= 0b111; //seleção do tempo de amostragem
	ADC1->CR2 |= ADC_CR2_ADON; //liga o conversor AD
}

void loop_ADC(){

	ADC1->CR2 |= ADC_CR2_SWSTART; //inicia a conversão
	while(!(ADC1->SR & ADC_SR_EOC)); //aguarda o fim da conversão

	uint16_t leitura = ADC1->DR; // faz a leitura do valor convertido

	printf("Valor convertido = %d\n", leitura);
	Delay_ms(1000);
}

//ADC exemplo da aula 13 com biblioteca utility

void aula_13_utility(){
	ADC_Init(ADC1, SINGLE_CHANNEL, DAC_RES_12BITS);
	ADC_SingleChannel(ADC1, ADC_IN0);
}

void loop_aula_13_utility(){
	uint16_t leitura = ADC_GetSingleConversion(ADC1);
	printf("Valor Convertido = %d\n", leitura);
	Delay_ms(1000);
}

// ADC exemplo da aula 14
void setup_ADC_2canais(){
	GPIO_Clock_Enable(GPIOA);
	GPIO_Pin_Mode(GPIOA, PIN_0, ANALOG);
	GPIO_Pin_Mode(GPIOA, PIN_1, ANALOG);
}

void config_ADC_2canais(){

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   //liga o clock do adc1

	ADC->CCR |= 0b01 << 16; //prescaler /4 (fADC  =84/4 = 21MHZ)
	ADC1->SQR1 = 1 << 20; // conversão de mais de um canal
	ADC1->SQR3 = (1 << 5 ) | (0 << 0); //seleção do canal a ser convertido (IN_0) e (IN_1) para PA0 e PA1
	ADC1->CR1 |= ADC_CR1_SCAN; // habilita o modo scan
	ADC1->CR2 |= ADC_CR2_EOCS; // sinal de EOC ao final de cada conversão
	ADC1->CR2 |= ADC_CR2_ADON; //liga o conversor AD
}
void loop_ADC_2canais(){

	ADC1->CR2 |= ADC_CR2_SWSTART; //inicia a conversão
	while(!(ADC1->SR & ADC_SR_EOC)); //aguarda o fim da conversão

	uint16_t leitura = ADC1->DR; // faz a leitura do valor convertido

	while(!(ADC1->SR & ADC_SR_EOC));

	uint16_t leitura2 = ADC1->DR; // faz a leitura do valor convertido

	printf("Valor convertido do 1 canal = %d\nValor convertido do 2 canal = %d\n\n", leitura, leitura2);
	Delay_ms(1000);
}

// ADC exemplo da aula 14 com a biblioteca utility

void aula_14_utility(){
	ADC_Init(ADC1, MULTI_CHANNEL, ADC_RES_12BITS);

	uint8_t  canais[2] = {ADC_IN0, ADC_IN1};
	ADC_MultiChannel (ADC1,2,canais);
}

void loop_aula_14_utility(){
	uint16_t leitura[2];
	ADC_GetMultiConversions(ADC1, 2, leitura);

	printf("Valor PA0 = %d\nValor PA1 = %d\n\n", leitura[0], leitura[1]);
	Delay_ms(500);
}

//interrupção no ADC

void inter_aula_14(){
	ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
	ADC_SingleChannel(ADC1, ADC_IN0);

	ADC1->CR1 |= ADC_CR1_EOCIE;
	NVIC_EnableIRQ(ADC_IRQn);
}

void loop_inter_aula_14(){
	ADC1->CR2 |= ADC_CR2_SWSTART;
	Delay_ms(500);
}

//Aula 15 sensor de temperatura interno

void aula_15_sensorTempInter(){
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //ligar o clock do ADC1

	ADC->CCR |= ADC_CCR_TSVREFE | (0b01 << 16); // Liga sensor de temperatura + prescaler /4
	ADC1->SQR1 &= ~(0xF << 20); // 1 conversão regular
	ADC1->SQR3 = 16;            // Canal 16 (sensor temp)
	ADC1->SMPR1 |= (7 << 18);   // 480 ciclos de amostragem
	ADC1->CR2 |= ADC_CR2_ADON;  // Liga o ADC
}

void loop_aula_15_sensorTempInter(){
	// Leitura correta dos valores de calibração
	uint16_t TS_CAL1 = *(uint16_t*)0x1FFF7A2C;
	uint16_t TS_CAL2 = *(uint16_t*)0x1FFF7A2E;

	// Inicia a conversão
	ADC1->CR2 |= ADC_CR2_SWSTART;
	while(!(ADC1->SR & ADC_SR_EOC));

	uint16_t adc_val = ADC1->DR;

	// Converte para temperatura usando fórmula de calibração
	float temp = ((80.0f * (adc_val - TS_CAL1)) / (TS_CAL2 - TS_CAL1)) + 30.0f;

	printf("Temperatura = %.2f°C | ADC = %u\n", temp, adc_val);
	Delay_ms(500);
}


//codigo sensor de temperatura interno aula 15 pelo chat

void sensor_TempCHAT(){
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC->CCR |= 0b01 << 16;                    // Prescaler /4
	ADC1->SQR1 &= ~(0xF << 20);               // Uma conversão
	ADC1->SQR3 = 16;                          // Canal 16 = sensor temperatura
	ADC1->SMPR1 |= (7 << 18);                 // 480 ciclos para canal 16
	ADC->CCR |= ADC_CCR_TSVREFE;              // Liga sensor de temperatura
	ADC1->CR2 |= ADC_CR2_ADON;

	// Espera estabilização do sensor de temperatura
	Delay_ms(10);

	// Lê calibração correta
	uint16_t TS_CAL1 = *(uint16_t*)0x1FFF7A2C;  // 30 ºC
	uint16_t TS_CAL2 = *(uint16_t*)0x1FFF7A2E;  // 110 ºC

	while (1) {
	    ADC1->CR2 |= ADC_CR2_SWSTART;
	    while (!(ADC1->SR & ADC_SR_EOC));
	    uint16_t raw = ADC1->DR;

	    // Fórmula oficial do datasheet
	    float temp = ((110.0f - 30.0f) * (raw - TS_CAL1)) / (TS_CAL2 - TS_CAL1) + 30.0f;

	    printf("Temperatura = %.2f °C\n", temp);
	    Delay_ms(500);
	}
}

//watchdog analogico aula 15 com potenciômetro

void aula_15_watchdog(){
	ADC_Init(ADC1, SINGLE_CHANNEL, ADC_RES_12BITS);
	ADC_SingleChannel(ADC1, ADC_IN0);

	ADC1->LTR = 1000; //limiar inferior
	ADC1->HTR = 3000; //limiar superior
	ADC1->CR1 |= ADC_CR1_AWDSGL; //habilita watchdog no canal simples
	ADC1->CR1 &= ~ADC_CR1_AWDCH; //seleção do canal monitorado (IN_0)
	ADC1->CR1 |= ADC_CR1_AWDEN; // habilita o watchdog
}

void loop_aula_15_watchdog(){
	ADC1->CR2 |= ADC_CR2_SWSTART;
	while(!(ADC1->SR & ADC_SR_EOC));
	printf("LEITURA = %d\n", (int) ADC1->DR);

	if(ADC1->SR & ADC_SR_AWD)
		printf("SAIDA DA REGIAO MONITORADA\n");

	ADC1->SR &= ~ADC_SR_AWD;

	Delay_ms(500);
}

// aula 16 timers, usando systick e delay_us para alernar led

void aula_16_timers(){

	GPIO_Clock_Enable(GPIOA);
	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_7, OUTPUT);

	SysTick->LOAD = 6.3e6 - 1; // carrega o registrador Reload Value
	SysTick->CTRL = 0b011; // liga o systick, habilita interrupção e seleciona a fonte de clock

}
void loop_aula_16_timers(){
	GPIO_Toggle_Pin(GPIOA, PIN_6);
	Delay_ms(1000);
}

//aula 17 usando o timer 5

void aula_17_timer5(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; // liga o clock do Timer5
	TIM5->PSC = 41999;	// prescaler ajeitado para pulsos de 500us
	TIM5->EGR |= TIM_EGR_UG; // update event para escrever o valor do prescaler
	TIM5->CR1 &= ~TIM_CR1_DIR; // contagem crescente
	TIM5->CR1 |= TIM_CR1_CEN; // habilita a contagem
}

void loop_aula_17_timer5(){
	TIM5->CNT = 0;
	Delay_ms(1000);
	printf("TIM5 = %u\n", (unsigned int)TIM5->CNT);
}

//aula 17 usando o timer 5 com interrupção

void aula_17_timer5_inter(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; // liga o clock do Timer5
	TIM5->PSC = 41999;	// prescaler ajeitado para pulsos de 500us
	TIM5->ARR = 1999;
	TIM5->EGR |= TIM_EGR_UG; // update event para escrever o valor do prescaler
	TIM5->CR1 &= ~TIM_CR1_DIR; // contagem crescente

	TIM5->DIER |= TIM_DIER_UIE; // habilita interrupção por update
	NVIC_EnableIRQ(TIM5_IRQn); // habilita interrupção no NVIC

	TIM5->CR1 |= TIM_CR1_CEN; // habilita a contagem
	 //não precisa de loop mediante ser interrução, que está na main.c em TIM5_IRQHandler();
}

//aula 18, usando timer para poder gerar sinal PWM

void aula_18_timer_pwm(){

	GPIO_Clock_Enable(GPIOA);
	GPIO_Pin_Mode(GPIOA,PIN_2,ALTERNATE);
	GPIO_Alternate_Function(GPIOA,PIN_2,AF2);

	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;  //LIGA O CLOCK DO TIMER 5
	TIM5->CR1 &= ~TIM_CR1_DIR;  //CONTADOR CRESCENTE
	TIM5->ARR = 99; // RELOAD DE 99
	TIM5->PSC = 419; //PRESCALER PARA FREQUENCIA DE 2KHZ
	TIM5->CCMR2 |= 0B110 << 4; // SELECIONA O PWM 1
	TIM5->CCMR2 |= TIM_CCMR2_OC3PE; // HABILITA O PRE CARREGAMENTO DO CCR3
	TIM5->CCER |= TIM_CCER_CC3E; //HABILITA A SAÍDA
	TIM5->EGR = TIM_EGR_UG; // EVENTO DE UPDATE NO PRESCALER
	TIM5->CR1 |= TIM_CR1_CEN;  //HABILITAR O TIMER

	TIM5->CCR3 = 30; //ATUALIZA O DUTY CYCLE;
}

void aula_18_timer_pwm_dutycyclevariado(){
	while(1){
		for(int duty = 0; duty<100; ++duty){
			TIM5->CCR3 = duty;
			Delay_ms(20);
		}
		for(int duty = 99; duty>=0; --duty){
			TIM5->CCR3 = duty;
			Delay_ms(20);
		}

	}
}

//AULA 18, SAÍDAS DE DISPARO

void aula_18_timer_saidadisparo(){

	//Configuração de GPIO
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //habilita o clock do GPIOA
	GPIOA->MODER |= 0b11; //pino PA0 como entrada analógica

	//Configuração do TIMER3
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //liga o clock do Timer3
	TIM3->CR1 &= ~TIM_CR1_DIR; //contador crescente
	TIM3->PSC = 419; //prescaler para pulsos a cada 5uS
	TIM3->ARR = 39999; //estouros a cada 200ms
	TIM3->CR2 = 0b010 << 4; //master mode (TRGO a cada estouro do timer)
	TIM3->CR1 |= TIM_CR1_URS; //overflow gera um gatilho em TRGO
	TIM3->EGR = 1; //update para escrever o valor do prescaler
	TIM3->CR1 |= TIM_CR1_CEN; //habilita o timer

	//Configuração do ADC1
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //liga o clock da interface digital do ADC1
	ADC->CCR |= 0b01 << 16; //prescaler /4
	ADC1->SQR1 &= ~(0xF << 20); //conversão de apenas um canal
	ADC1->SQR3 &= ~(0x1F); //seleção do canal a ser convertido (IN_0)
	ADC1->CR2 |= 0b1000 << 24; //TIM3_TRGO como fonte de gatilho do ADC1
	ADC1->CR2 |= ADC_CR2_EXTEN_0; //disparo externo em rising edge no ADC1
	ADC1->CR1 |= 1 << 5; //habilita interrupção de EOC
	NVIC_EnableIRQ(ADC_IRQn); //habilita a interrupção do ADC no NVIC
	ADC1->CR2 |= ADC_CR2_ADON; //liga o conversor ADC1
}
#endif
