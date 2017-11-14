#include <esp_log.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "sdkconfig.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
//#define GPIO_BIT_MASK  ((1ULL<<GPIO_NUM_34) | (1ULL<<GPIO_NUM_35) | (1ULL<<GPIO_NUM_23) | (1ULL<<GPIO_NUM_22)| (1ULL<<GPIO_NUM_19)) 
#define PIN1_RED1 GPIO_NUM_16
#define PIN1_RED2 GPIO_NUM_17
#define PIN2_RED1 GPIO_NUM_32
#define PIN2_RED2 GPIO_NUM_33
#define PIN3_RED1 GPIO_NUM_25
#define PIN3_RED2 GPIO_NUM_26
#define PIN4_RED1 GPIO_NUM_27
#define PIN4_RED2 GPIO_NUM_21
#define PIN_BLK GPIO_NUM_23
#define PIN_STR GPIO_NUM_22
#define PIN_CLK GPIO_NUM_19
#define Mat_Alt 8
#define Mat_Larg 32
byte Matriz[]={
B00000000000000000000000000000000,
B00000000000000000000000000000000,
B00000000000000000000000000000000,
B00000000000000000000000000000000,
B00000000000000000000000000000000,
B00000000000000000000000000000000,
B00000000000000000000000000000000,
B00000000000000000000000000000000
};
const byte CaracteresArray[][8]={
{
B00000010,
B00001010,
B00010010,
B00000010,
B00000010,
B00000010,
B00000010,
B00000010
},
{
B00111000,
B01001100,
B01010010,
B00000100,
B00001100,
B00011000,
B00110000,
B01111110
},
{
B00111000,
B01000100,
B00000010,
B00011110,
B00000010,
B00000010,
B01000100,
B00111000
},
{
B00001010,
B00010010,
B00100010,
B01111110,
B00000010,
B00000010,
B00000010,
B00000010
},
{
B00000010,
B00001010,
B00010010,
B00000010,
B00000010,
B00000010,
B00000010,
B00000010
},
{
B00000010,
B00001010,
B00010010,
B00000010,
B00000010,
B00000010,
B00000010,
B00000010
},
{
B00000010,
B00001010,
B00010010,
B00000010,
B00000010,
B00000010,
B00000010,
B00000010
},
{
B00000010,
B00001010,
B00010010,
B00000010,
B00000010,
B00000010,
B00000010,
B00000010
},
{
B00000010,
B00001010,
B00010010,
B00000010,
B00000010,
B00000010,
B00000010,
B00000010
},
{
B00000010,
B00001010,
B00010010,
B00000010,
B00000010,
B00000010,
B00000010,
B00000010
},
void Grafica_Mat(byte *Matriz,int largo){
	int val;
	for (int i=0 ; i<largo, i++){
		val=(Matriz[0]>> i) and 1;
		printf("Val %d.\n",val);
		gpio_set_level(PIN1_RED1, val);
		val=(Matriz[1]>> i) and 1;
		printf("Val %d.\n",val);
                gpio_set_level(PIN1_RED2, val);
		val=(Matriz[2]>> i) and 1;
		printf("Val %d.\n",val);
                gpio_set_level(PIN2_RED1, val);
		val=(Matriz[3]>> i) and 1;
		printf("Val %d.\n",val);
                gpio_set_level(PIN2_RED2, val);
		val=(Matriz[4]>> i) and 1;
		printf("Val %d.\n",val);
                gpio_set_level(PIN3_RED1, val);
		val=(Matriz[5]>> i) and 1;
		printf("Val %d.\n",val);
                gpio_set_level(PIN3_RED2, val);
		val=(Matriz[6]>> i) and 1;
		printf("Val %d.\n",val);
                gpio_set_level(PIN4_RED1, val);
		val=(Matriz[7]>> i) and 1;
		printf("Val %d.\n",val);
                gpio_set_level(PIN4_RED2, val);
		vTaskDelay(50 / portTICK_PERIOD_MS);
                gpio_set_level(PIN_CLK, 1);
                printf("Clock.\n");
                vTaskDelay(50 / portTICK_PERIOD_MS);
                gpio_set_level(PIN_CLK, 0);
	}
        vTaskDelay(50 / portTICK_PERIOD_MS);
        gpio_set_level(PIN_STR, 1);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        gpio_set_level(PIN_STR, 0);
        gpio_set_level(PIN_BLK, 0);
}
		

static void run() {
	//gpio_config_t io_conf;
	//io_conf.intr_type = GPIO_INTR_DISABLE;
	//io_conf.mode = GPIO_MODE_OUTPUT;
	//io_conf.pin_bit_mask = GPIO_BIT_MASK;
	//io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	//io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	//gpio_config(&io_conf);
    	gpio_set_direction(PIN1_RED1, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN1_RED2, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN2_RED1, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN2_RED2, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN3_RED1, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN3_RED2, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN4_RED1, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN4_RED2, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN_BLK, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN_STR, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN_CLK, GPIO_MODE_OUTPUT);
        gpio_set_level(PIN_BLK, 1);
        gpio_set_level(PIN_CLK, 0);
        printf("BLK 1.\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
        printf("BLK 0.\n");
       	vTaskDelay(200 / portTICK_PERIOD_MS);
	//esp_deep_sleep(10000);
	//esp_deep_sleep(10000);
	int val=0;
	while (1){
		val= !val;
	        printf("RED 1.\n");
		gpio_set_level(PIN1_RED1, val);
		gpio_set_level(PIN1_RED2, val);
		gpio_set_level(PIN2_RED1, val);
		gpio_set_level(PIN2_RED2, val);
		gpio_set_level(PIN3_RED1, val);
		gpio_set_level(PIN3_RED2, val);
		gpio_set_level(PIN4_RED1, val);
		gpio_set_level(PIN4_RED2, val);
	        printf("RED 2.\n");
        	vTaskDelay(50 / portTICK_PERIOD_MS);
		for (int i=0 ; i < 16 ; i++){
			gpio_set_level(PIN_CLK, 1);
	        	printf("Clock.\n");
			//esp_deep_sleep(10000);
	        	vTaskDelay(50 / portTICK_PERIOD_MS);
			gpio_set_level(PIN_CLK, 0);
        		vTaskDelay(50 / portTICK_PERIOD_MS);
			gpio_set_level(PIN_STR, 1);
		        vTaskDelay(50 / portTICK_PERIOD_MS);
			gpio_set_level(PIN_STR, 0);
	//		esp_deep_sleep(10000);
	        	gpio_set_level(PIN_BLK, 0);
		}
		vTaskDelay(50 / portTICK_PERIOD_MS);
        	printf("Fin.\n");
	}
	
}

void MatrixLed(void)
{
	run();
} // app_main


