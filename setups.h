#ifndef INC_SETUPS_H_
#define INC_SETUPS_H_

void PA6_OUT_setup() {
	// 05/05/2025
	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);

	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
}

void PA67_OUT_setup() {
	// 05/05/2025
	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_7, OUTPUT);
}

void PA_OUT_setup() {
	// 05/05/2025
	// liga todos os pinos da porta em OUTPUT
	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_2, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_3, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_4, INPUT);
	GPIO_Pin_Mode(GPIOA, PIN_5, INPUT);
	GPIO_Pin_Mode(GPIOA, PIN_6, INPUT);
	GPIO_Pin_Mode(GPIOA, PIN_7, INPUT);

	GPIO_Resistor_Enable(GPIOA, PIN_4, PULL_UP);
	GPIO_Resistor_Enable(GPIOA, PIN_5, PULL_UP);
	GPIO_Resistor_Enable(GPIOA, PIN_6, PULL_UP);
	GPIO_Resistor_Enable(GPIOA, PIN_7, PULL_UP);
}


void PB0_OUT_setup() {
	GPIO_Clock_Enable(GPIOB);

	GPIO_Pin_Mode(GPIOB, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOB, PIN_1, OUTPUT);
}

int K0 = PIN_4, K1 = PIN_3;
void K0_K1_buttons_setup() {
	GPIO_Clock_Enable(GPIOE);

	GPIO_Pin_Mode(GPIOE, K0, INPUT);
	GPIO_Pin_Mode(GPIOE, K1, INPUT);

	GPIO_Resistor_Enable(GPIOE, K0, PULL_UP);
	GPIO_Resistor_Enable(GPIOE, K1, PULL_UP);
}

void PD_OUT_setup(){
	GPIO_Clock_Enable(GPIOD);

	GPIO_Pin_Mode(GPIOD, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOD, PIN_1, INPUT);
	GPIO_Pin_Mode(GPIOD, PIN_2, OUTPUT);
}

void PC_OUT_setup(){
	GPIO_Clock_Enable(GPIOC);

	GPIO_Pin_Mode(GPIOC, PIN_0, OUTPUT);

}

void PE_OUT_setup() {
	// 14/05/2025
	// liga todos os pinos da porta E em OUTPUT
	GPIO_Clock_Enable(GPIOE);

	GPIO_Pin_Mode(GPIOE, PIN_0, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_1, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_2, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_3, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_4, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_5, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_6, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_7, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_8, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_9, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_10, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_11, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_12, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_13, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_14, OUTPUT);
	GPIO_Pin_Mode(GPIOE, PIN_15, OUTPUT);
}

void teclado_display_gpio_init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// Ativa o clock do GPIOE (se ainda não estiver ativado)
	__HAL_RCC_GPIOE_CLK_ENABLE();

	// === Linhas: PE0 a PE3 ===
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;    // saída open-drain
	GPIO_InitStruct.Pull = GPIO_PULLUP;             // com pull-up
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	// === Colunas: PE4 a PE7 ===
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;         // entrada
	GPIO_InitStruct.Pull = GPIO_PULLUP;             // com pull-up
	// não precisa definir Speed para entrada
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	// === Display 7 segmentos: PE8 a PE14 ===
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
	                      GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     // saída push-pull normal
	GPIO_InitStruct.Pull = GPIO_NOPULL;             // sem pull-up/down
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	// Inicializa linhas em HIGH (inativas)
	for (int i = 0; i < 4; i++) {
		HAL_GPIO_WritePin(GPIOE, (1 << i), GPIO_PIN_SET);
	}

	// Inicializa display desligado (opcional)
	for (int i = 8; i <= 14; i++) {
		HAL_GPIO_WritePin(GPIOE, (1 << i), GPIO_PIN_RESET);
	}
}

void pA0_inter(){
	GPIO_Clock_Enable(GPIOE);

	GPIO_Pin_Mode(GPIOE, PIN_3, INPUT);
	GPIO_Pin_Mode(GPIOE, PIN_4, INPUT);


	GPIO_Resistor_Enable(GPIOE, PIN_3, PULL_UP);
	GPIO_Resistor_Enable(GPIOE, PIN_4, PULL_UP);


	EXTI_Config(EXTI3, GPIOE, FALLING_EDGE);
	EXTI_Config(EXTI4, GPIOE, FALLING_EDGE);

	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);

	NVIC_SetPriority(EXTI3_IRQn, 0);
	NVIC_SetPriority(EXTI4_IRQn, 1);
}

void aula_inter(){

	//clock na porta A
	GPIO_Clock_Enable(GPIOA);
	GPIO_Clock_Enable(GPIOE);

	//Deixar os leds apagados
	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
	GPIO_Write_Pin(GPIOA, PIN_7, HIGH);

	// Habilitar os pinos de entrada e saida
	GPIO_Pin_Mode(GPIOE, PIN_3, INPUT);
	GPIO_Pin_Mode(GPIOE, PIN_4, INPUT);
	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_7, OUTPUT);

	//Estabelecer como vai ser o modo de entrada
	GPIO_Resistor_Enable(GPIOE, PIN_3, PULL_UP);
	GPIO_Resistor_Enable(GPIOE, PIN_4, PULL_UP);

	//Habilitar as duas interrupções
	EXTI_Config(EXTI3, GPIOE, FALLING_EDGE);
	EXTI_Config(EXTI4, GPIOE, FALLING_EDGE);

	//Mandar as inyerrupções para o controlador NVIC

	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);

	//Prioridade EXTI0 em relação a EXTI1

	NVIC_SetPriority(EXTI3_IRQn, 1);
	NVIC_SetPriority(EXTI4_IRQn, 0);
}

void setup_timer(){

	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOE, PIN_2, ALTERNATE);
	GPIO_Alternate_Function(GPIOA, PIN_2, AF2);

}
#endif /* INC_SETUPS_H_ */
