#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c.h>
#include <math.h>
#include "esp_wifi.h"
#include "caracteres.h"
#include "esp_system.h"
#include "sdkconfig.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "rom/ets_sys.h"
#include "driver/gpio.h"
#include "dht11.h"
//#include "DHT11.h"
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
//#define GPIO_BIT_MASK  ((1ULL<<GPIO_NUM_34) | (1ULL<<GPIO_NUM_35) | (1ULL<<GPIO_NUM_23) | (1ULL<<GPIO_NUM_22)| (1ULL<<GPIO_NUM_19)) 
#define PIN1_RED1 GPIO_NUM_17
#define PIN1_RED2 GPIO_NUM_16
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

uint32_t Matriz[]={
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000
};

//static uint8_t temprature_sens_read(); 
//static uint32_t hall_sens_read();
//void TempPres(double sali[]);

void binary(uint32_t numero){
	for (int i = 31; i >= 0; i--)
    		printf("%u",((numero >> i) & 1));
	printf("\n");
}
void GenBarra_Mat(uint32_t Mat[],int indice){
	uint32_t val=0;
	for (int j=0;j<indice;j++){
		val=val | (1<<j);
	}
	binary(val);
	for (int i=0;i<Mat_Alt;i++){
		Mat[i]=val;
	}
}
void Blanc_Mat(uint32_t Mat[]){
	for (int i=0;i<Mat_Alt;i++){
		Mat[i]=0b00000000000000000000000000000000;
	}
}
void Pone_Car_Mat(const uint8_t Caract[], int posicion,uint32_t Mat[]){
	for (int i=0;i<Mat_Alt;i++){
		Mat[i]=Mat[i] | (uint32_t)(Caract[i] << posicion);		
	}
}
void Imprime_Mat(uint32_t Mat[]){
	printf("Matriz :\n");
	for (int i=0;i<Mat_Alt;i++){
		binary(Mat[i]);
	}
}
void Grafica_Mat(uint32_t Matriz[],int largo){
	int val;
	for (int i=0;i<largo;i++){
		val=(Matriz[0]>> i) & 1;
		//printf("Val %d.\n",val);
		gpio_set_level(PIN1_RED1, val);
		val=(Matriz[1]>> i) & 1;
		//printf("Val %d.\n",val);
                gpio_set_level(PIN1_RED2, val);
		val=(Matriz[2]>> i) & 1;
		//printf("Val %d.\n",val);
                gpio_set_level(PIN2_RED1, val);
		val=(Matriz[3]>> i) & 1;
		//printf("Val %d.\n",val);
                gpio_set_level(PIN2_RED2, val);
		val=(Matriz[4]>> i) & 1;
		//printf("Val %d.\n",val);
                gpio_set_level(PIN3_RED1, val);
		val=(Matriz[5]>> i) & 1;
		//printf("Val %d.\n",val);
                gpio_set_level(PIN3_RED2, val);
		val=(Matriz[6]>> i) & 1;
		//printf("Val %d.\n",val);
                gpio_set_level(PIN4_RED1, val);
		val=(Matriz[7]>> i) & 1;
		//printf("Val %d.\n",val);
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
	nvs_flash_init();
    	//system_init();
	//gpio_config_t io_conf;
	//io_conf.intr_type = GPIO_INTR_DISABLE;
	//io_conf.mode = GPIO_MODE_OUTPUT;
	//io_conf.pin_bit_mask = GPIO_BIT_MASK;
	//io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	//io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	//gpio_config(&io_conf);
    	
  	setDHTPin(18);
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
        gpio_set_level(PIN_BLK, 0);
        gpio_set_level(PIN_CLK, 0);
       
	printf("BLK 1.\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
        printf("BLK 0.\n");
       	vTaskDelay(200 / portTICK_PERIOD_MS);
	//esp_deep_sleep(10000);
	//esp_deep_sleep(10000);
	int val=0;
	Blanc_Mat(Matriz);
	Imprime_Mat(Matriz);
	Pone_Car_Mat(CaracteresArray[1],0,Matriz);
	Pone_Car_Mat(CaracteresArray[0],8,Matriz);
	Imprime_Mat(Matriz);
	Grafica_Mat(Matriz,16);
	Blanc_Mat(Matriz);
	GenBarra_Mat(Matriz,20);
	Imprime_Mat(Matriz);
	Grafica_Mat(Matriz,16);
	//double sali[2]={0};
	while(1){
		//printf("Starting DHT measurement!\n");
		val=getTemp();
		//val=23.5;
	    	printf("Temperatura %d\n",val);
		//printf("F temperature is %d\n", getFtemp());
		//printf("Humidity reading %d\n",getHumidity());
		//vTaskDelay(1000 / portTICK_RATE_MS);
        	//TempPres(sali);
		//float temp=( temprature_sens_read() - 32) / 1.8 ;
	        //printf("Temp %f\n",sali[0]);
		//int itemp = static_cast<int>(val);
		int itemp = (int)roundf(val);
	        printf("iTemp %d\n",itemp);
		int k=0;
		Blanc_Mat(Matriz);
		while (itemp > 0)
		{	
    			int digit = itemp%10;
	    		itemp /= 10;
			Pone_Car_Mat(CaracteresArray[digit],k,Matriz);
			k=k+8;
	    		printf("Digit : %d  Posicion : %d", digit,k);
		}
		Imprime_Mat(Matriz);
		Grafica_Mat(Matriz,16);
		vTaskDelay(10000 / portTICK_RATE_MS);
		
		val=getHumidity();
	    	printf("Humedad %d\n",val);
		itemp = (int)roundf(val);
	        printf("iHumed %d\n",itemp);
		k=0;
		Blanc_Mat(Matriz);
		while (itemp > 0)
		{	
    			int digit = itemp%10;
	    		itemp /= 10;
			Pone_Car_Mat(CaracteresArray[digit],k,Matriz);
			k=k+8;
	    		printf("Digit : %d  Posicion : %d", digit,k);
		}
		Imprime_Mat(Matriz);
		Grafica_Mat(Matriz,16);
		vTaskDelay(10000 / portTICK_RATE_MS);
		
	}
	//while (1){
	//	val= !val;
	 //       printf("RED 1.\n");
	//	gpio_set_level(PIN1_RED1, val);
	//	gpio_set_level(PIN1_RED2, val);
	//	gpio_set_level(PIN2_RED1, val);
	//	gpio_set_level(PIN2_RED2, val);
	//	gpio_set_level(PIN3_RED1, val);
	//	gpio_set_level(PIN3_RED2, val);
	//	gpio_set_level(PIN4_RED1, val);
	//	gpio_set_level(PIN4_RED2, val);
	 //       printf("RED 2.\n");
        //	vTaskDelay(50 / portTICK_PERIOD_MS);
	//	for (int i=0 ; i < 16 ; i++){
	//		gpio_set_level(PIN_CLK, 1);
	 //       	printf("Clock.\n");
	//		//esp_deep_sleep(10000);
	 //       	vTaskDelay(50 / portTICK_PERIOD_MS);
	//		gpio_set_level(PIN_CLK, 0);
        //		vTaskDelay(50 / portTICK_PERIOD_MS);
	//		gpio_set_level(PIN_STR, 1);
	//	        vTaskDelay(50 / portTICK_PERIOD_MS);
	//		gpio_set_level(PIN_STR, 0);
	////		esp_deep_sleep(10000);
	  //      	gpio_set_level(PIN_BLK, 0);
	//	}
	//	vTaskDelay(50 / portTICK_PERIOD_MS);
        //	printf("Fin.\n");
	//}
	
}

void MatrixLed(void)
{
//	i2c_config_t conf;
 //   	conf.mode = I2C_MODE_MASTER;
  //  	conf.sda_io_num = GPIO_NUM_18;
   // 	conf.scl_io_num = GPIO_NUM_19;
    //	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    //	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    //	conf.master.clk_speed = 100000;
    //	i2c_param_config(I2C_NUM_0, &conf);
    //	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
	run();
} // app_main

