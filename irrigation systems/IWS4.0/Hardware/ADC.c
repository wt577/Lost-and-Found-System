#include "stm32f10x.h"                  // Device header
#include "LED.h"  
#include "ADC.h"

uint16_t AD_Value[2];					//定义用于存放AD转换结果的全局数组

void Adc_DMA_Init(void )
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1,ENABLE );
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE );
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M	
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3 ;//|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init (GPIOA ,&GPIO_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);	//规则组序列1的位置，配置为通道0
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_55Cycles5);	//规则组序列2的位置，配置为通道1
	
//	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode =ADC_Mode_Independent;
	ADC_InitStructure.ADC_ContinuousConvMode =ENABLE;//DISABLE
	ADC_InitStructure.ADC_DataAlign =ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv =ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_NbrOfChannel =2;
	ADC_InitStructure.ADC_ScanConvMode =ENABLE;	//模数转换工作在单通道模式
	ADC_Init (ADC1,&ADC_InitStructure);

	DMA_InitTypeDef DMA_InitStructure;											//定义结构体变量
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;				//外设基地址，给定形参AddrA
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//外设数据宽度，选择半字，对应16为的ADC数据寄存器
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址自增，选择失能，始终以ADC数据寄存器为源
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;					//存储器基地址，给定存放AD转换结果的全局数组AD_Value
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//存储器数据宽度，选择半字，与源数据宽度对应
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//存储器地址自增，选择使能，每次转运后，数组移到下一个位置
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							//数据传输方向，选择由外设到存储器，ADC数据寄存器转到数组
	DMA_InitStructure.DMA_BufferSize = 2;										//转运的数据大小（转运次数），与ADC通道数一致
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//模式，选择循环模式，与ADC的连续转换一致
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//存储器到存储器，选择失能，数据由ADC外设触发转运到存储器
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//优先级，选择中等
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);	

	DMA_Cmd(DMA1_Channel1, ENABLE);	
	ADC_DMACmd(ADC1, ENABLE);	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准	
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);	
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);	//在每次转换前，根据函数形参灵活更改规则组的通道1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);					//软件触发AD转换一次
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//等待EOC标志位，即等待AD转换结束
	return ADC_GetConversionValue(ADC1);					//读数据寄存器，得到AD转换的结果
}
//ADC获取光照（未用DMA式）
uint16_t Light_Get(void)
{
	uint16_t AD3,Light;
	AD3=AD_GetValue(ADC_Channel_3);
//	AD3=AD_Value[0];
	Light=100-(AD3/4096.0*100);
	return Light; 
}

uint16_t DHT11_Get(void)
{
	uint16_t AD4,DHT;
//	AD4=AD_GetValue(ADC_Channel_4);
	AD4=AD_Value[1];
	DHT=(float)AD4*(3.3/4096);
	return DHT; 
}

void Light_LED(void)
{
//	AD3=Light_Get ();
	AD3=100-(AD_Value [0]/4096.0*100);
	if(AD3>25){
		LED2_ON();}else  {LED2_OFF ();}
	if(AD3>50){
		LED3_ON ();}else {LED3_OFF ();}
	if(AD3>75){
		LED4_ON ();}else {LED4_OFF ();}
	if(AD3>90){
		LED5_ON ();}else {LED5_OFF ();}
}
