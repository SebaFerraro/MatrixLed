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
#include "Veml6070.h"


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
#define PIN_RED_STR GPIO_NUM_22
#define PIN_RED_CLK GPIO_NUM_19
#define PIN_GRN_STR GPIO_NUM_0
#define PIN_GRN_CLK GPIO_NUM_4
#define PIN_DHT11 GPIO_NUM_18
#define SDA_PIN GPIO_NUM_5
#define SCL_PIN GPIO_NUM_14
#define Mat_Alt 8
#define Mat_Larg 32
#define SDELAY 1
#define T IT_2
#define DELAY_INFO 10000
#define DELAY_BANNER 7000
#define CaracteresArray CaracteresArray2

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

uint32_t MatrizROSARIO[]={
0b00000000000000000000000000000000,
0b11100011000110001100111001001100,
0b10010100101000010010100101010010,
0b11100100100110011110111001010010,
0b10100100100001010010101001010010,
0b10010011000110010010100101001100,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000
};
uint32_t MatrizSMART[]={
0b00000000000000000000000000000000,
0b00000110011011001100111001110000,
0b01101000010101010010100100100000,
0b00000110010001011110111000100000,
0b01100001010001010010101000100000,
0b00000110010001010010100100100000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000
};
uint32_t MatrizCIOR[]={
0b00000000000000000000000000000000,
0b00000001100100110001110000000000,
0b00111010010101001001001000000000,
0b00000010000101001001110000000000,
0b00111010010101001001010000000000,
0b00000001100100110001001000000000,
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
void Grafica_Mat(uint32_t Matriz[],int largo,int color,int borra){
	int val;
	for (int i=0;i<largo;i++){
		val=(Matriz[0]>> i) & 1;
		gpio_set_level(PIN1_RED1, val);
		val=(Matriz[1]>> i) & 1;
                gpio_set_level(PIN1_RED2, val);
		val=(Matriz[2]>> i) & 1;
                gpio_set_level(PIN2_RED1, val);
		val=(Matriz[3]>> i) & 1;
                gpio_set_level(PIN2_RED2, val);
		val=(Matriz[4]>> i) & 1;
                gpio_set_level(PIN3_RED1, val);
		val=(Matriz[5]>> i) & 1;
                gpio_set_level(PIN3_RED2, val);
		val=(Matriz[6]>> i) & 1;
                gpio_set_level(PIN4_RED1, val);
		val=(Matriz[7]>> i) & 1;
                gpio_set_level(PIN4_RED2, val);
		if(SDELAY>0)
		   vTaskDelay(SDELAY / portTICK_PERIOD_MS);
		switch (color){
                case 0:
			gpio_set_level(PIN_RED_CLK, 1);
                	printf("Clock RED.\n");
			if(SDELAY>0)
                	  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
                	gpio_set_level(PIN_RED_CLK, 0);
			if(SDELAY>0)
			  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
			if(borra==1){
				gpio_set_level(PIN1_RED1, 0);
		                gpio_set_level(PIN1_RED2, 0);
		                gpio_set_level(PIN2_RED1, 0);
		                gpio_set_level(PIN2_RED2, 0);
		                gpio_set_level(PIN3_RED1, 0);
		                gpio_set_level(PIN3_RED2, 0);
		                gpio_set_level(PIN4_RED1, 0);
		                gpio_set_level(PIN4_RED2, 0);
        	        	if(SDELAY>0)
				  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
				gpio_set_level(PIN_GRN_CLK, 1);
	                	printf("Clock GREEN BLANQUEO.\n");
        	        	if(SDELAY>0)
        	        	  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
                		gpio_set_level(PIN_GRN_CLK, 0);
        	        	if(SDELAY>0)
				  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
			}
			break;
		case 1:
			gpio_set_level(PIN_GRN_CLK, 1);
                	printf("Clock GREEN.\n");
        	        if(SDELAY>0)
                	  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
                	gpio_set_level(PIN_GRN_CLK, 0);
        	        if(SDELAY>0)
			  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
			if(borra==1){
				gpio_set_level(PIN1_RED1, 0);
		                gpio_set_level(PIN1_RED2, 0);
		                gpio_set_level(PIN2_RED1, 0);
		                gpio_set_level(PIN2_RED2, 0);
		                gpio_set_level(PIN3_RED1, 0);
		                gpio_set_level(PIN3_RED2, 0);
		                gpio_set_level(PIN4_RED1, 0);
		                gpio_set_level(PIN4_RED2, 0);
        	        	if(SDELAY>0)
        	        	  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
				gpio_set_level(PIN_RED_CLK, 1);
	                	printf("Clock RED BLANQUEO.\n");
        	        	if(SDELAY>0)
        	        	  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
                		gpio_set_level(PIN_RED_CLK, 0);
        	        	if(SDELAY>0)
				  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
			}
			break;
		case 2:
			gpio_set_level(PIN_RED_CLK, 1);
			gpio_set_level(PIN_GRN_CLK, 1);
                	printf("Clock RED-GREEN.\n");
        	        if(SDELAY>0)
                	  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
                	gpio_set_level(PIN_RED_CLK, 0);
                	gpio_set_level(PIN_GRN_CLK, 0);
			break;
		default:
			gpio_set_level(PIN_RED_CLK, 1);
                	printf("Clock RED.\n");
        	        if(SDELAY>0)
                	  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
                	gpio_set_level(PIN_RED_CLK, 0);
		}		
			
			
	}
	switch (color){
                case 0:
        	        if(SDELAY>0)
        		  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
		        gpio_set_level(PIN_RED_STR, 1);
                	printf("STROB RED.\n");
        	        if(SDELAY>0)
		          vTaskDelay(SDELAY / portTICK_PERIOD_MS);
		        gpio_set_level(PIN_RED_STR, 0);
			if(borra==1){
        	        	if(SDELAY>0)
				  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
	                        gpio_set_level(PIN_GRN_STR, 1);
        	                printf("STROB GREEN BORRA.\n");
        	        	if(SDELAY>0)
                	          vTaskDelay(SDELAY / portTICK_PERIOD_MS);
        	                gpio_set_level(PIN_GRN_STR, 0);
			}
			break;
		case 1:
        	        if(SDELAY>0)
        		  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
		        gpio_set_level(PIN_GRN_STR, 1);
                	printf("STROB GREEN.\n");
        	        if(SDELAY>0)
		          vTaskDelay(SDELAY / portTICK_PERIOD_MS);
		        gpio_set_level(PIN_GRN_STR, 0);
			if(borra==1){
        	        	if(SDELAY>0)
				  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
	                        gpio_set_level(PIN_RED_STR, 1);
        	                printf("STROB RED BORRA.\n");
        	        	if(SDELAY>0)
                	          vTaskDelay(SDELAY / portTICK_PERIOD_MS);
        	                gpio_set_level(PIN_RED_STR, 0);
			}
			break;
		case 2:
        	        if(SDELAY>0)
        		  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
		        gpio_set_level(PIN_RED_STR, 1);
		        gpio_set_level(PIN_GRN_STR, 1);
                	printf("STROB RED-GREEN.\n");
        	        if(SDELAY>0)
		          vTaskDelay(SDELAY / portTICK_PERIOD_MS);
		        gpio_set_level(PIN_RED_STR, 0);
		        gpio_set_level(PIN_GRN_STR, 0);
			break;
		default:
        	        if(SDELAY>0)
        		  vTaskDelay(SDELAY / portTICK_PERIOD_MS);
		        gpio_set_level(PIN_RED_STR, 1);
                	printf("STROB RED.\n");
        	        if(SDELAY>0)
		          vTaskDelay(SDELAY / portTICK_PERIOD_MS);
		        gpio_set_level(PIN_RED_STR, 0);
        }
	printf("BLK .\n");
        gpio_set_level(PIN_BLK, 0);
}
		
void Grafica_Matriz_Desplaza(uint32_t Matriz[],int largo,int color,int borra,int direccion){
	
uint32_t MatrizTemp[]={
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000,
0b00000000000000000000000000000000
};
	for(int i=0;i<=largo;i++){
		if(direccion==0){
		MatrizTemp[0]=(uint32_t)((Matriz[0]<< (largo - i)) & 0b11111111111111111111111111111111);
		MatrizTemp[1]=(uint32_t)((Matriz[1]<< (largo - i)) & 0b11111111111111111111111111111111);
		MatrizTemp[2]=(uint32_t)((Matriz[2]<< (largo - i)) & 0b11111111111111111111111111111111);
		MatrizTemp[3]=(uint32_t)((Matriz[3]<< (largo - i)) & 0b11111111111111111111111111111111);
		MatrizTemp[4]=(uint32_t)((Matriz[4]<< (largo - i)) & 0b11111111111111111111111111111111);
		MatrizTemp[5]=(uint32_t)((Matriz[5]<< (largo - i)) & 0b11111111111111111111111111111111);
		MatrizTemp[6]=(uint32_t)((Matriz[6]<< (largo - i)) & 0b11111111111111111111111111111111);
		MatrizTemp[7]=(uint32_t)((Matriz[7]<< (largo - i)) & 0b11111111111111111111111111111111);
		}else{
		MatrizTemp[0]=(uint32_t)((Matriz[0]>>i) & 0b11111111111111111111111111111111);
		MatrizTemp[1]=(uint32_t)((Matriz[1]>>i) & 0b11111111111111111111111111111111);
		MatrizTemp[2]=(uint32_t)((Matriz[2]>>i) & 0b11111111111111111111111111111111);
		MatrizTemp[3]=(uint32_t)((Matriz[3]>>i) & 0b11111111111111111111111111111111);
		MatrizTemp[4]=(uint32_t)((Matriz[4]>>i) & 0b11111111111111111111111111111111);
		MatrizTemp[5]=(uint32_t)((Matriz[5]>>i) & 0b11111111111111111111111111111111);
		MatrizTemp[6]=(uint32_t)((Matriz[6]>>i) & 0b11111111111111111111111111111111);
		MatrizTemp[7]=(uint32_t)((Matriz[7]<<i) & 0b11111111111111111111111111111111);
		}
		Imprime_Mat(MatrizTemp);
		Grafica_Mat(MatrizTemp,largo,color,borra);
	}
}

void Grafica_Banner(void){
	Blanc_Mat(Matriz);
	Grafica_Mat(Matriz,32,2,0);
	Grafica_Matriz_Desplaza(MatrizROSARIO,32,2,0,0);
        if(DELAY_BANNER>0)
          vTaskDelay(DELAY_BANNER / portTICK_RATE_MS);
	Grafica_Matriz_Desplaza(MatrizSMART,32,2,0,1);
        if(DELAY_BANNER>0)
          vTaskDelay(DELAY_BANNER / portTICK_RATE_MS);
	Blanc_Mat(Matriz);
	Grafica_Mat(Matriz,32,2,0);
}


static void run() {
	nvs_flash_init();
  	
	setDHTPin(PIN_DHT11);
	setPinsVeml6070(SDA_PIN,SCL_PIN);
	
	gpio_set_direction(PIN1_RED1, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN1_RED2, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN2_RED1, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN2_RED2, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN3_RED1, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN3_RED2, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN4_RED1, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN4_RED2, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN_BLK, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN_RED_STR, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN_RED_CLK, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN_GRN_STR, GPIO_MODE_OUTPUT);
    	gpio_set_direction(PIN_GRN_CLK, GPIO_MODE_OUTPUT);
        gpio_set_level(PIN_BLK, 0);
        gpio_set_level(PIN_RED_CLK, 0);
        gpio_set_level(PIN_GRN_CLK, 0);
       
	i2c_master_init();
	
	printf("BLK 1.\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
        printf("BLK 0.\n");
       	vTaskDelay(200 / portTICK_PERIOD_MS);
	int val=0;
	Grafica_Banner();
	//Blanc_Mat(Matriz);
	//Imprime_Mat(MatrizROSARIO);
	//Grafica_Mat(MatrizROSARIO,32,2,0);
	//vTaskDelay(DELAY_BANNER / portTICK_RATE_MS);
	//Blanc_Mat(Matriz);
	//Imprime_Mat(MatrizCIOR);
	//Grafica_Mat(MatrizCIOR,32,2,0);
	//vTaskDelay(DELAY_BANNER / portTICK_RATE_MS);
	
	//Pone_Car_Mat(CaracteresArray[1],0,Matriz);
	//Pone_Car_Mat(CaracteresArray[0],8,Matriz);
	//Imprime_Mat(Matriz);
	//Grafica_Mat(Matriz,32,2,1);
	//GenBarra_Mat(Matriz,20);
	//Imprime_Mat(Matriz);
	//Grafica_Mat(Matriz,32,1,1);
	//double sali[2]={0};
	//Blanc_Mat(Matriz);
	//Pone_Car_Mat(CaracteresArray[0],0,Matriz);
	//Pone_Car_Mat(CaracteresArray[1],8,Matriz);
	//Imprime_Mat(Matriz);
	//Grafica_Mat(Matriz,32,1,1);
	//vTaskDelay(1000 / portTICK_RATE_MS);
	//Blanc_Mat(Matriz);
	//Pone_Car_Mat(CaracteresArray[2],0,Matriz);
	//Pone_Car_Mat(CaracteresArray[3],8,Matriz);
	//Imprime_Mat(Matriz);
	//Grafica_Mat(Matriz,32,1,1);
	//vTaskDelay(1000 / portTICK_RATE_MS);
	//Blanc_Mat(Matriz);
	//Pone_Car_Mat(CaracteresArray[4],0,Matriz);
	//Pone_Car_Mat(CaracteresArray[5],8,Matriz);
	//Imprime_Mat(Matriz);
	//Grafica_Mat(Matriz,32,1,1);
	//vTaskDelay(1000 / portTICK_RATE_MS);
	//Blanc_Mat(Matriz);
	//Pone_Car_Mat(CaracteresArray[6],0,Matriz);
	//Pone_Car_Mat(CaracteresArray[7],8,Matriz);
	//Imprime_Mat(Matriz);
	//Grafica_Mat(Matriz,32,1,1);
	//vTaskDelay(1000 / portTICK_RATE_MS);
	//Blanc_Mat(Matriz);
	//Pone_Car_Mat(CaracteresArray[8],0,Matriz);
	//Pone_Car_Mat(CaracteresArray[9],8,Matriz);
	//Imprime_Mat(Matriz);
	//Grafica_Mat(Matriz,32,1,1);
	//vTaskDelay(1000 / portTICK_RATE_MS);
	//Blanc_Mat(Matriz);
	//Pone_Car_Mat(CaracteresArray[10],0,Matriz);
	//Pone_Car_Mat(CaracteresArray[11],8,Matriz);
	//Imprime_Mat(Matriz);
	//Grafica_Mat(Matriz,32,1,1);
	//vTaskDelay(1000 / portTICK_RATE_MS);
	Blanc_Mat(Matriz);
	
	while(1){
		val=getTemp();
	    	printf("Temperatura %d\n",val);
		int itemp = (int)roundf(val);
	        printf("iTemp %d\n",itemp);
		int k=0;
		Blanc_Mat(Matriz);
		Pone_Car_Mat(CaracteresArray[10],k,Matriz);
		k=k+8;
		while (itemp > 0)
		{	
    			int digit = itemp%10;
	    		itemp /= 10;
			Pone_Car_Mat(CaracteresArray[digit],k,Matriz);
			k=k+8;
	    		printf("Digit : %d  Posicion : %d", digit,k);
		}
		Imprime_Mat(Matriz);
		Grafica_Mat(Matriz,32,2,1);
        	if(DELAY_INFO>0)
		  vTaskDelay(DELAY_INFO / portTICK_RATE_MS);
		
		val=getHumidity();
	    	printf("Humedad %d\n",val);
		itemp = (int)roundf(val);
	        printf("iHumed %d\n",itemp);
		k=0;
		Blanc_Mat(Matriz);
		Pone_Car_Mat(CaracteresArray[11],k,Matriz);
		k=k+8;
		while (itemp > 0)
		{	
    			int digit = itemp%10;
	    		itemp /= 10;
			Pone_Car_Mat(CaracteresArray[digit],k,Matriz);
			k=k+8;
	    		printf("Digit : %d  Posicion : %d", digit,k);
		}
		Imprime_Mat(Matriz);
		Grafica_Mat(Matriz,32,2,1);
        	if(DELAY_INFO>0)
		  vTaskDelay(DELAY_INFO / portTICK_RATE_MS);
		
		int col=1;
		int l=0;
		//int l = rand() % 32;
		uint8_t dat=((T<<2) | 0x02);
		uint16_t uv=0;
		uint8_t uvi=0;
		uv=i2c_veml6070_uv(dat);
		uvi=i2c_veml6070_indexuv(uv,T);
		l= (int)roundf(uvi*32/11);
		printf("UV: %d UVI: %d IBAR: %d\n",uv,uvi,l);
		if(l==0)
		  l=1;
		GenBarra_Mat(Matriz,l);
	        Imprime_Mat(Matriz);
		if(l>6){
			col=2;
		}
		if(l>15){
			col=0;
		}
	      	Grafica_Mat(Matriz,32,col,1);
        	if(DELAY_INFO>0)
		  vTaskDelay(DELAY_INFO / portTICK_RATE_MS);
		Blanc_Mat(Matriz);
		Grafica_Banner();
	}
	
}

void MatrixLed(void)
{
	run();
}

