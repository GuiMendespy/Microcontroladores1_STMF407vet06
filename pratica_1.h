#ifndef INC_PRATICA_1_H_
#define INC_PRATICA_1_H_


void pratica_1_1() {
	GPIO_Write_Pin(GPIOA, PIN_6, LOW);
	HAL_Delay(100);
	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
	HAL_Delay(100);
}

void pratica_1_2() {
	GPIO_Write_Pin(GPIOA, PIN_6, LOW);
	HAL_Delay(100);
	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
	HAL_Delay(1900);
}

void pratica_1_3() {
	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
	HAL_Delay(50);
	GPIO_Write_Pin(GPIOA, PIN_6, LOW);

	HAL_Delay(1000);

	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
	HAL_Delay(50);
	GPIO_Write_Pin(GPIOA, PIN_6, LOW);

	HAL_Delay(100);

	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
	HAL_Delay(50);
	GPIO_Write_Pin(GPIOA, PIN_6, LOW);

	HAL_Delay(2000);
}

void pratica_1_4() {
	for(int i = 0; i<500;i++){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);

		Delay_us(i);

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);

		Delay_us(500 - i);
	}
	for(int i = 0; i<500;i++){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);

		Delay_us(500 - i);

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);

		Delay_us(i);
	}
}

void pratica_1_5() {
	for (uint16_t i = 0; i < 256; i++) {
		GPIO_Write_Port(GPIOA, i);
		HAL_Delay(100);
	}
}

void pratica_1_6() {
	for (uint16_t i = 1; i <= 128; i = i << 1) {
		GPIO_Write_Port(GPIOA, i);
		HAL_Delay(250);
	}
	for (uint16_t i = 64; i > 1; i = i >> 1) {
		GPIO_Write_Port(GPIOA, i);
		HAL_Delay(250);
	}
}

void pratica_1_7() {
	// data: 07/05/2925
	// XXGGYYRR
	#define VERMELHO 1
	#define AMARELO 4
	#define VERDE 16

	void ligarVermelho(uint8_t semaforo) {
		GPIOA->ODR |= VERMELHO << semaforo;
		GPIOA->ODR &= ~(AMARELO << semaforo);
		GPIOA->ODR &= ~(VERDE << semaforo);
	}

	void ligarAmarelo(uint8_t semaforo) {
		GPIOA->ODR |= AMARELO << semaforo;
		GPIOA->ODR &= ~(VERMELHO << semaforo);
		GPIOA->ODR &= ~(VERDE << semaforo);
	}

	void ligarVerde(uint8_t semaforo) {
		GPIOA->ODR |= VERDE << semaforo;
		GPIOA->ODR &= ~(AMARELO << semaforo);
		GPIOA->ODR &= ~(VERMELHO << semaforo);
	}

	while (1) {
		ligarVerde(0);
		ligarVermelho(1);

		HAL_Delay(5000);

		ligarAmarelo(0);
		ligarVermelho(1);

		HAL_Delay(2000);

		ligarVermelho(0);
		ligarVerde(1);

		HAL_Delay(5000);

		ligarVermelho(0);
		ligarAmarelo(1);

		HAL_Delay(2000);
	}
}

void pratica_1_8(){
	uint8_t nums[] = {
			0b0111111, // 0
			0b0000110, // 1
			0b1011011, // 2
			0b1001111, // 3
			0b1100110, // 4
			0b1101101, // 5
			0b1111101, // 6
			0b0000111, // 7
			0b1111111, // 8
			0b1101111, // 9
			0b1110111, // A
			0b1111100, // B
			0b0111001, // C
			0b1011110, // D
			0b1111001, // E
			0b1110001  // F
	};

	for (int i = 0; i < 16; i++) {
		GPIO_Write_Port(GPIOA, nums[i]);
		HAL_Delay(1000);
	}
	for (int i = 14; i > 0; i--) {
		GPIO_Write_Port(GPIOA, nums[i]);
		HAL_Delay(1000);
	}
}

uint8_t n = 0;

void pratica_1_9(){
	uint8_t nums[] = {
			0b0111111, // 0
			0b0000110, // 1
			0b1011011, // 2
			0b1001111, // 3
			0b1100110, // 4
			0b1101101, // 5
			0b1111101, // 6
			0b0000111, // 7
			0b1111111, // 8
			0b1101111, // 9
			0b1110111, // A
			0b1111100, // B
			0b0111001, // C
			0b1011110, // D
			0b1111001, // E
			0b1110001  // F
	};


	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			for (int d = 30; d > 0; d--) {
				GPIO_Write_Pin(GPIOB, PIN_0, LOW);
				GPIO_Write_Pin(GPIOB, PIN_1, HIGH);
				GPIO_Write_Port(GPIOA, nums[j]);
				HAL_Delay(1);
				GPIO_Write_Pin(GPIOB, PIN_0, HIGH);
				GPIO_Write_Pin(GPIOB, PIN_1, LOW);
				GPIO_Write_Port(GPIOA, nums[i]);
				HAL_Delay(1);
			}
		}
	}
	for (int i = 14; i > 0; i--) {
		for (int j = 15; j > 0; j--) {
			for (int d = 30; d > 0; d--) {
				GPIO_Write_Pin(GPIOB, PIN_0, LOW);
				GPIO_Write_Pin(GPIOB, PIN_1, HIGH);
				GPIO_Write_Port(GPIOA, nums[j]);
				HAL_Delay(1);
				GPIO_Write_Pin(GPIOB, PIN_0, HIGH);
				GPIO_Write_Pin(GPIOB, PIN_1, LOW);
				GPIO_Write_Port(GPIOA, nums[i]);
				HAL_Delay(1);
			}
		}
	}
}

void int_to_str(int num, char* buffer, int buffer_size) {
    snprintf(buffer, buffer_size, "%d", num);
}

void pratica_1_10(){
	LCD_Write_String(1,1,"Guilherme");
	LCD_Write_String(2,1,"Mendes");
	LCD_Write_String(3,1,"Jeronimo");
	for(int i = 0; i<11; i++){
		char texto[3];
		int_to_str(10-i, texto, sizeof(texto));
		LCD_Write_String(4, 2, " ");
		LCD_Write_String(4,1, texto);
		HAL_Delay(1000);
	}
}

void pwm_motordc(int i){
	// 2000 Hz, alternando a intensidade do led
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
	Delay_us(i);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	Delay_us(500 - i);
}

void pratica_1_11() {

	GPIO_Write_Pin(GPIOA, PIN_0, LOW);
	GPIO_Write_Pin(GPIOA, PIN_1, HIGH);

	for (int j = 0; j<500; j++){
		pwm_motordc(j);
	}
	for (int j = 500; j>0; j--){
		pwm_motordc(j);
	}

	GPIO_Write_Pin(GPIOA, PIN_1, LOW);
	GPIO_Write_Pin(GPIOA, PIN_0, HIGH);
	for (int j = 0; j<500; j++){
		pwm_motordc(j);
	}
	for (int j = 500; j>0; j--){
		pwm_motordc(j);
	}
}

void pratica_1_13() {

	GPIO_Write_Pin(GPIOA, PIN_6, GPIO_Read_Pin(GPIOE, PIN_4));

	GPIO_Write_Pin(GPIOA, PIN_7, GPIO_Read_Pin(GPIOE, PIN_3));
}

int press = 0;
void pratica_1_14(){

	if (!(GPIO_Read_Pin(GPIOE, PIN_4)) && press == 0){
		GPIO_Toggle_Pin(GPIOA, PIN_6);
		press = 1;
	}else if(GPIO_Read_Pin(GPIOE, PIN_4) && press == 1){
		press = 0;
	}
}

void pratica_1_15() {
	void ligar() {
		GPIO_Write_Pin(GPIOA, PIN_6, LOW);
		GPIO_Write_Pin(GPIOA, PIN_7, LOW);
	}
	void desligar() {
		GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
		GPIO_Write_Pin(GPIOA, PIN_7, HIGH);
	}

	int k0 = !GPIO_Read_Pin(GPIOE, PIN_3);
	int k1 = !GPIO_Read_Pin(GPIOE, PIN_4);

	switch (estado) {
	case 0: // estado 00
		desligar();
		if (!k0 && k1)
			estado = 1;
		break;
	case 1: // estado 10
		desligar();
		if (k0 && k1)
			estado = 2;
		else if (!k1)
			estado = 0;
		break;
	case 2: // estado 11
		ligar();
		if (!k1)
			estado = 0;
		else if (!k0)
			estado = 1;
		break;
	}
}

int cont = 0;
void pratica_1_16() {
	void ligar() {
		GPIO_Write_Pin(GPIOA, PIN_6, LOW);
		GPIO_Write_Pin(GPIOA, PIN_7, LOW);
	}
	void desligar() {
		GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
		GPIO_Write_Pin(GPIOA, PIN_7, HIGH);
	}

	int k0 = !GPIO_Read_Pin(GPIOE, PIN_4);
	int k1 = !GPIO_Read_Pin(GPIOE, PIN_3);

	switch (estado) {
	case 0: // estado 00
		desligar();
		if (k0 && !k1)
			estado = 1;
		break;
	case 1: // estado inicia contador
		desligar();
		cont = 1000;
		estado = 2;
		break;
	case 2: // estado decrementa contador
		desligar();
		HAL_Delay(1);
		if (cont <= 0)
			estado = 3;
		cont--;
		if (k0 && k1)
			estado = 4;
		if (!k0)
			estado = 0;
		break;
	case 3:
		desligar();
		if (!k0)
			estado = 0;
		break;
	case 4:
		ligar();
		if (!k0)
			estado = 0;
		if (k0 && !k1)
			estado = 1;
		break;
	}
}

#define MAX 10

int ordem[MAX];

void escolher_ordem() {
	for (int i = 0; i < MAX; i++) {
		ordem[i] = (Random_Number() & 0b11);  // gera número entre 0 e 3
	}
}

void mostrar_primeiros(int q) {
	for (int i = 0; i < q; i++) {
		GPIOA->ODR &= ~(0b1111); 	// desliga todos os LEDs
		GPIOA->ODR |= (1 << ordem[i]); 	// liga LED da sequência
		HAL_Delay(300); 			// tempo da piscada
		GPIOA->ODR &= ~(0b1111); 	// desliga todos
		HAL_Delay(200); 			// delay entre os LEDs
	}
	HAL_Delay(500);
}

// Espera pressionar um botão de PA4 a PA7 e retorna qual foi (0 a 3)
int esperar_botao() {
	while (1) {
		for (int i = 0; i < 4; i++) {
			if (GPIO_Read_Pin(GPIOA, i + 4)) {
				while (GPIO_Read_Pin(GPIOA, i + 4)); // aguarda soltar
				return i;
			}
		}
	}
}

void pratica_1_20() {
	escolher_ordem();

	for (int round = 0; round < MAX; round++) {
		mostrar_primeiros(round + 1);

		// Leitura do jogador
		for (int i = 0; i <= round; i++) {
			int botao = esperar_botao();

			// Feedback: acende LED correspondente ao botão pressionado
			GPIOA->ODR |= (1 << botao);
			HAL_Delay(100);
			GPIOA->ODR &= ~(0b1111);

			if (botao != ordem[i]) {
				// Errou! Mostra todos os LEDs piscando
				for (int j = 0; j < 3; j++) {
					GPIOA->ODR |= 0b1111;
					HAL_Delay(200);
					GPIOA->ODR &= ~(0b1111);
					HAL_Delay(200);
				}
				return; // fim do jogo
			}
		}
		HAL_Delay(100); // próxima rodada
	}

	// Se acertou tudo
	GPIOA->ODR |= 0b1111;
	HAL_Delay(1000);
	GPIOA->ODR &= ~(0b1111);
}

void pwm_motorBI_HOR(int i){
	// 3 Hz, alternando a intensidade do led
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
	Delay_us(i);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
	Delay_us(1200 - i);
}

void pwm_motorBI_ANTI(int i){
	// 3 Hz, alternando a intensidade do led
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
	Delay_us(i);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
	Delay_us(1200 - i);
}

#define a1 PIN_0
#define b1 PIN_1
#define a2 PIN_2
#define b2 PIN_3
#define pw1 PIN_4
#define pw2 PIN_5

void pratica_1_12(){

		for(int i = 0; i<100; i++){
		//1º estado

			GPIO_Write_Pin(GPIOD, pw2, HIGH);
			GPIO_Write_Pin(GPIOD, pw1, LOW);

			GPIO_Write_Pin(GPIOD, a1, HIGH);
			GPIO_Write_Pin(GPIOD, b1, LOW);

			GPIO_Write_Pin(GPIOD, a2, HIGH);
			GPIO_Write_Pin(GPIOD, b2, LOW);
			HAL_Delay(5);

		 //2º estado

			GPIO_Write_Pin(GPIOD, PIN_4, LOW);
			GPIO_Write_Pin(GPIOD, PIN_5, HIGH);

			GPIO_Write_Pin(GPIOD, a1, LOW);
			GPIO_Write_Pin(GPIOD, b1, HIGH);

			GPIO_Write_Pin(GPIOD, a2, HIGH);
			GPIO_Write_Pin(GPIOD, b2, LOW);
			HAL_Delay(5);


		 //3º estado

			GPIO_Write_Pin(GPIOD, PIN_4, HIGH);
			GPIO_Write_Pin(GPIOD, PIN_5, LOW);

			GPIO_Write_Pin(GPIOD, a1, LOW);
			GPIO_Write_Pin(GPIOD, b1, HIGH);

			GPIO_Write_Pin(GPIOD, a2, LOW);
			GPIO_Write_Pin(GPIOD, b2, HIGH);
			HAL_Delay(5);


			// 4º estado
			GPIO_Write_Pin(GPIOD, PIN_4, LOW);
			GPIO_Write_Pin(GPIOD, PIN_5, HIGH);

			GPIO_Write_Pin(GPIOD, a1, HIGH);
			GPIO_Write_Pin(GPIOD, b1, LOW);

			GPIO_Write_Pin(GPIOD, a2, LOW);
			GPIO_Write_Pin(GPIOD, b2, HIGH);
			HAL_Delay(5);

		}



		for(int i = 0; i<100; i++){
			//4º estado
			GPIO_Write_Pin(GPIOD, PIN_4, HIGH);
			GPIO_Write_Pin(GPIOD, PIN_5, LOW);

			GPIO_Write_Pin(GPIOD, a1, HIGH);
			GPIO_Write_Pin(GPIOD, b1, LOW);

			GPIO_Write_Pin(GPIOD, a2, LOW);
			GPIO_Write_Pin(GPIOD, b2, HIGH);
			HAL_Delay(5);


			//3º estado
			GPIO_Write_Pin(GPIOD, PIN_4, LOW);
			GPIO_Write_Pin(GPIOD, PIN_5, HIGH);

			GPIO_Write_Pin(GPIOD, a1, LOW);
			GPIO_Write_Pin(GPIOD, b1, HIGH);

			GPIO_Write_Pin(GPIOD, a2, LOW);
			GPIO_Write_Pin(GPIOD, b2, HIGH);
			HAL_Delay(5);


			//2º estado
			GPIO_Write_Pin(GPIOD, PIN_4, HIGH);
			GPIO_Write_Pin(GPIOD, PIN_5, LOW);

			GPIO_Write_Pin(GPIOD, a1, LOW);
			GPIO_Write_Pin(GPIOD, b1, HIGH);

			GPIO_Write_Pin(GPIOD, a2, HIGH);
			GPIO_Write_Pin(GPIOD, b2, LOW);
			HAL_Delay(5);


			//1º estado
			GPIO_Write_Pin(GPIOD, PIN_4, LOW);
			GPIO_Write_Pin(GPIOD, PIN_5, HIGH);

			GPIO_Write_Pin(GPIOD, a1, HIGH);
			GPIO_Write_Pin(GPIOD, b1, LOW);

			GPIO_Write_Pin(GPIOD, a2, HIGH);
			GPIO_Write_Pin(GPIOD, b2, LOW);
			HAL_Delay(5);

		}

}

void pratica_1_17() {
	// Setups: K0_K1_buttons_setup() e PC0_OUT_setup()
	// Conexões:
	// - PC0 conectado à entrada PWM (fio laranja/amarelo) do servomotor
	// - +5V ao fio vermelho
	// - GND ao fio preto

	void servomotor_para(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t angulo) {
		// Enviar o servo para o ângulo desejado
		int pulso_us = 500 + (2000 * angulo) / 180;
		int periodo_us = 20000;

		GPIO_Write_Pin(GPIOx, pin, HIGH);
		Delay_us(pulso_us);
		GPIO_Write_Pin(GPIOx, pin, LOW);
		Delay_us(periodo_us - pulso_us);
	}

	const int VELOCIDADE = 5;
	int angulo = 90; // Começa no meio
	while (1) {
		if (!GPIO_Read_Pin(GPIOE, K0)) { // reduzir o ângulo
			angulo = (angulo - 1 < 0 ? 0 : angulo - 1);
			HAL_Delay(VELOCIDADE);
		}

		if (!GPIO_Read_Pin(GPIOE, K1)) { // aumentar o ângulo
			angulo = (angulo + 1 > 180 ? 180 : angulo + 1);
			HAL_Delay(VELOCIDADE);
		}

		// Enviar sinal PWM ao servo com o novo ângulo
		servomotor_para(GPIOC, PIN_0, angulo);
	}
}

void pratica_1_18() {
	// Mapeamento para 7 segmentos (A a G em PE8 a PE14)
	uint8_t nums[] = {
		0b0111111, // 0
		0b0000110, // 1
		0b1011011, // 2
		0b1001111, // 3
		0b1100110, // 4
		0b1101101, // 5
		0b1111101, // 6
		0b0000111, // 7
		0b1111111, // 8
		0b1101111, // 9
		0b1110111, // A
		0b1111100, // B
		0b0111001, // C
		0b1011110, // D
		0b1111001, // E
		0b1110001  // F
	};

	// Função para escrever nos segmentos PE8–PE14
	void escrever_hex_7seg(uint8_t n) {
		for (int i = 0; i < 7; i++) {
			uint8_t estado = (nums[n] >> i) & 0x01;
			GPIO_Write_Pin(GPIOE, i + 8, estado);  // PE8 a PE14
		}
	}

	// Função para ativar (LOW) uma linha por vez
	void ativar_linha(uint8_t linha) {
		for (int i = 0; i < 4; i++) {
			GPIO_Write_Pin(GPIOE, i, i == linha ? LOW : HIGH);  // PE0 a PE3
		}
	}

	// Matriz do teclado
	uint8_t teclado[4][4] = {
		{ 1,  2,  3, 10},  // linha 0
		{ 4,  5,  6, 11},  // linha 1
		{ 7,  8,  9, 12},  // linha 2
		{15,  0, 14, 13}   // linha 3
	};

	while (1) {
		for (int linha = 0; linha < 4; linha++) {
			ativar_linha(linha);  // ativa uma linha por vez
			HAL_Delay(1);  // tempo mínimo para evitar ruído

			// Verifica colunas (PE4–PE7)
			for (int coluna = 0; coluna < 4; coluna++) {
				if (!GPIO_Read_Pin(GPIOE, coluna + 4)) {  // coluna + 4 → PE4 a PE7
					escrever_hex_7seg(teclado[linha][coluna]);
					HAL_Delay(200);  // debounce
					// Espera soltar o botão
					while (!GPIO_Read_Pin(GPIOE, coluna + 4));
				}
			}
		}
	}
}

void pratica_1_19(){

	Delay_ms(10);
	GPIO_Write_Pin(GPIOD, PIN_0, HIGH);
	Delay_us(10);
	GPIO_Write_Pin(GPIOD, PIN_0, LOW);

	GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
	while(!GPIO_Read_Pin(GPIOD, PIN_1));
	GPIO_Write_Pin(GPIOA, PIN_6, LOW);

	int cont = 0;

	GPIO_Write_Pin(GPIOA, PIN_7, HIGH);
	while(GPIO_Read_Pin(GPIOD, PIN_1) && cont < 23200){
		Delay_us(1);
		cont++;
	}
	GPIO_Write_Pin(GPIOA, PIN_7, LOW);

	unsigned int distancia = cont/58;

	if(distancia>50){
		GPIO_Write_Pin(GPIOD, PIN_2, LOW);
	}else if(distancia>30){
		GPIO_Write_Pin(GPIOD, PIN_2, HIGH);
		Delay_ms(250);
		GPIO_Write_Pin(GPIOD, PIN_2, LOW);
		Delay_ms(250);
	}else if(distancia>10){
		GPIO_Write_Pin(GPIOD, PIN_2, HIGH);
		Delay_ms(100);
		GPIO_Write_Pin(GPIOD, PIN_2, LOW);
		Delay_ms(100);
	}else{
		GPIO_Write_Pin(GPIOD, PIN_2, HIGH);
		Delay_ms(50);
	}

}
#endif /* INC_PRATICA_1_H_ */
