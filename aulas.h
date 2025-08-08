#ifndef AULAS_H
#define AULAS_H

#include <math.h>
#include <stdio.h>
#include "main.h"

void aula001() {
	// piscando os dois leds do stm32
	// data:

	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	HAL_Delay(1000);
}

void aula002() {
	// piscando o led do PA6 em tempos diferentes

	int tempoMaior = 1500, tempoMenor = 250;

	void pisca(int tempoPos) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_Delay(tempoPos);
	}

	pisca(tempoMaior);
	pisca(tempoMenor);
	pisca(tempoMaior);
}

void aula003() {
	// alternando leds, assim que desliga um, liga o outro (através de funções)
	// pisca de Vermelho - Amarelo - Verde - Amarelo - Vermelho - etc
	// conecte os pinos PA0 a um led vermelho, o PA1 ao amarelo e PA2 ao verde

	int time = 500;

	void delay() {
	HAL_Delay(time);
	}

	void ligarVerde() {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
	}

	void ligarAmarelo() {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	}

	void ligarVermelho() {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	}

	ligarVermelho();
	delay();
	ligarAmarelo();
	delay();
	ligarVerde();
	delay();
	ligarAmarelo();
	delay();
}

void aula004_alarme() {
	// um alarme que toca quatro bipes e depois espera um tempo
	// conecte um buzzer a saída PB6

	void bipe() {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_Delay(50);
	}

	for (int i = 0; i < 4; i++)
		bipe();

	HAL_Delay(1000);
}

void aula004_piscar_2_leds_insta(int forma) {
	// pisca dois leds instantaneamente, através de ponteiros
	// 3 formas diferentes de fazer que produzem o mesmo resultado
	// lógica invertida dos sinais, resetando PA6 e PA7 os leds da placa acendem.

	switch (forma) {
		case 1:
			// primeira forma: setando e resetando o ODR com and e or
			GPIOA->ODR &= ~(0b11 << 6); // resetando
			HAL_Delay(100);
			GPIOA->ODR |= 0b11 << 6; // setando
			HAL_Delay(100);
		break;

		case 2:
			// segunda forma: alternando os sinais de ODR com ou-exclusivo
			GPIOA->ODR ^= 0b11 << 6; // alternando os pinos
			HAL_Delay(100);
		break;

		case 3:
			// terceira forma: alterando o registrador BSRR
			GPIOA->BSRR |= 0b11 << 22; // resetando
			HAL_Delay(100);
			GPIOA->BSRR |= 0b11 << 6; // setando
			HAL_Delay(100);
		break;
	}
}

void aula005_ideia_pwd() {
	// mantendo o mesmo periodo/frequência, e alternando o período ativo
	// consegue-se aumentar ou diminuir a intensidade do led

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // período ativo
	Delay_us(100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); // período desativo
	Delay_us(1900);
}

void pwm(int i) {
	// 500 Hz, alternando a intensidade do led
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	Delay_us(i);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	Delay_us(2000 - i);
}

void aula005_variando_intensidade_led() {
	// implementando pwm para alterar a intensidade do led
	// o led varia de ativo para desativo, e para ativo, de forma suave

	for(int i = 0; i < 2000; i++) {
		pwm(i);
	}

	for(int i = 2000; i > 0; i--) {
		pwm(i);
	}
}

void aula005_pwm(int freq, float duty_cycle) {
	// informe a frequência e o duty cycle (%) para acender o led de PA6

	int periodo_total = 1000000 / freq;
	int periodo_ativo = round(periodo_total * duty_cycle);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	Delay_us(periodo_ativo);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	Delay_us(periodo_total - periodo_ativo);
}

void aula006_k1(){

	if(!GPIO_Read_Pin(GPIOE, PIN_3))
		GPIO_Write_Pin(GPIOA, PIN_6, LOW);
	else
		GPIO_Write_Pin(GPIOA, PIN_6, HIGH);
}

void aula006_k0(){

	if(!GPIO_Read_Pin(GPIOE, PIN_4))
		GPIO_Write_Pin(GPIOA, PIN_7, LOW);
	else
		GPIO_Write_Pin(GPIOA, PIN_7, HIGH);
}

int k0 = 0;

void aula006_k1_aumentando() {
	if (!GPIO_Read_Pin(GPIOE, PIN_3)) {
		k0++;
		k0 = ((k0 > 2000) ? 2000 : k0);
	} else {
		k0--;
		k0 = ((k0 < 0) ? 0 : k0);
	}
	pwm(k0);
}

int estado = 0;

void aula006_k1_k0_setup() {
	// data 28/04/2023
	GPIO_Clock_Enable(GPIOE);

	GPIO_Pin_Mode(GPIOE,PIN_3,INPUT);
	GPIO_Pin_Mode(GPIOE,PIN_4,INPUT);

	GPIO_Resistor_Enable(GPIOE,PIN_3,PULL_UP);
	GPIO_Resistor_Enable(GPIOE,PIN_4, PULL_UP);

	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA, PIN_6, OUTPUT);
	GPIO_Pin_Mode(GPIOA, PIN_7, OUTPUT);
}

void aula006_k1_k0() {
	// data 28/04/2023
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
		if (!k0 && k1)
			estado = 1;
		break;
	case 1: // estado 01
		desligar();
		if (k0 && k1)
			estado = 2;
		else if (!k1)
			estado = 0;
		break;
	case 2:
		ligar();
		if (!k1)
			estado = 0;
		else if (!k0)
			estado = 1;
		break;
	}
}

void aula007_contagem_com_botoes_setup() {
	// data 30/04/2023
	GPIO_Clock_Enable(GPIOE);

	GPIO_Pin_Mode(GPIOE, PIN_3, INPUT);
	GPIO_Resistor_Enable(GPIOE, PIN_3, PULL_UP);

	GPIO_Clock_Enable(GPIOA);

	GPIO_Pin_Mode(GPIOA,PIN_0,INPUT);
	GPIO_Resistor_Enable(GPIOA, PIN_0, PULL_DOWN);

	GPIO_Pin_Mode(GPIOA,PIN_6,OUTPUT);
	GPIO_Pin_Mode(GPIOA,PIN_7,OUTPUT);
}

void aula007_contagem_com_botoes_teste() {
	// data 30/04/2023

	if (GPIO_Read_Pin(GPIOA, PIN_0))
		GPIO_Write_Pin(GPIOA, PIN_6, LOW);
	else
		GPIO_Write_Pin(GPIOA, PIN_6, HIGH);

	if (!GPIO_Read_Pin(GPIOE, PIN_3))
		GPIO_Write_Pin(GPIOA, PIN_7, LOW);
	else
		GPIO_Write_Pin(GPIOA, PIN_7, HIGH);

	HAL_Delay(100);
}

int pressPA0 = 0, pressPE3 = 0;
uint8_t i = 0b00;

void aula007_contagem_com_botoes() {
	// data 30/04/2023

	if (GPIO_Read_Pin(GPIOA, PIN_0)) {
		if (!pressPA0 && i < 3) {
			pressPA0 = 1;
			i++;
		}
	} else
		pressPA0 = 0;

	if (!GPIO_Read_Pin(GPIOE, PIN_3)) {
		if (!pressPE3 && i > 0) {
			pressPE3 = 1;
			i--;
		}
	} else
		pressPE3 = 0;

	GPIO_Write_Pin(GPIOA, PIN_7, !((i >> 0) & 1));
	GPIO_Write_Pin(GPIOA, PIN_6, !((i >> 1) & 1));
	HAL_Delay(100);
}

#endif
