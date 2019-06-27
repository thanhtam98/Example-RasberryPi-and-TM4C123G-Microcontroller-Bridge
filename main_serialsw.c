//******************************************************************************************************
//Defination
//******************************************************************************************************
#define PART_TM4C123GH6PM

//******************************************************************************************************
//Including
//******************************************************************************************************
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "stdbool.h"
#include "stdint.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "crc16.h"
#include "afproto.h"

#define RX_BUF_SIZE 100
#define SeverPort 	UART0_BASE
#define SerialPort0 UART6_BASE
#define SerialPort1 UART1_BASE
#define SerialPort2 UART2_BASE
#define SerialPort3 UART5_BASE
#define SerialPort4 UART7_BASE
#define SerialPort5 UART3_BASE
#define SerialPort6 UART4_BASE

//******************************************************************************************************
//Variables
//******************************************************************************************************
bool IsSending_sever;
bool IsSending[7];

bool BufferFlag_sever;
bool BufferFlag[7];

volatile int Count_sever;
volatile int Count[7];

volatile int BufferLength_sever;
volatile int BufferLength[7];

volatile uint8_t ReadByte_sever;
volatile uint8_t ReadByte[7];

char RawBuffer_sever[RX_BUF_SIZE];
char RawBuffer[7][RX_BUF_SIZE];

char PayLoad[7][RX_BUF_SIZE];

int a,b;
int buffernum;
//
unsigned int write_len;
int size;
char BufferReply[7][2*RX_BUF_SIZE];
int len;
int i,j;
int temp;
//******************************************************************************************************
//Functions header
//******************************************************************************************************
void setup(void);
void SYSClockSet(void);
void UART0_Config(void);
void UART1_Config(void);
void UART2_Config(void);
void UART3_Config(void);
void UART4_Config(void);
void UART5_Config(void);
void UART6_Config(void);
void UART7_Config(void);
void SeverIntHandler(void);
void Port0IntHandler(void);
void Port1IntHandler(void);
void Port2IntHandler(void);
void Port3IntHandler(void);
void Port4IntHandler(void);
void Port5IntHandler(void);
void Port6IntHandler(void);
void GPIO_Config(void);
void ClearRXBuffer_sever(void);
void ClearRXBuffer(int buffernumber);
uint8_t Checksum_sever(void);
uint8_t Checksum(uint8_t buffernumber);
void DELAYms(int n);

//******************************************************************************************************
//Functions
//******************************************************************************************************
//Setup all
void setup(void)
{
	SYSClockSet();
	UART0_Config();
	UART1_Config();
	UART2_Config();
	UART3_Config();
	UART4_Config();
	UART5_Config();
	UART6_Config();
	UART7_Config();
	GPIO_Config();
}

//Setup Clock
void SYSClockSet(void)
{
	//
	// Configure clock 40 MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}


//******************************************************************************************************
//Function: Configurate UART0
//Input:
//Output:
//******************************************************************************************************
void UART0_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(SeverPort, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	IntMasterEnable();
	IntEnable(INT_UART0);
	UARTIntEnable(SeverPort, UART_INT_RX | UART_INT_RT);
}

//******************************************************************************************************
//Function: Configurate UART1
//Input:
//Output:
//******************************************************************************************************
void UART1_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	GPIOPinConfigure(GPIO_PB0_U1RX);
  GPIOPinConfigure(GPIO_PB1_U1TX);
	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(SerialPort1, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	IntMasterEnable();
	IntEnable(INT_UART1);
	UARTIntEnable(SerialPort1, UART_INT_RX | UART_INT_RT);
}

//******************************************************************************************************
//Function: Configurate UART2
//Input:
//Output:
//******************************************************************************************************
void UART2_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
	HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) |= 0x80;
	HWREG(GPIO_PORTD_BASE + GPIO_O_DEN) |= 0x80;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);	
	GPIOPinConfigure(GPIO_PD6_U2RX);
  GPIOPinConfigure(GPIO_PD7_U2TX);
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	UARTConfigSetExpClk(SerialPort2, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	IntMasterEnable();
	IntEnable(INT_UART2);
	UARTIntEnable(SerialPort2, UART_INT_RX | UART_INT_RT);
}
//******************************************************************************************************
//Function: Configurate UART3
//Input:
//Output:
//******************************************************************************************************
void UART3_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
	GPIOPinConfigure(GPIO_PC6_U3RX);
  GPIOPinConfigure(GPIO_PC7_U3TX);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	UARTConfigSetExpClk(SerialPort5, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	IntMasterEnable();
	IntEnable(INT_UART3);
	UARTIntEnable(SerialPort5, UART_INT_RX | UART_INT_RT);
}

//******************************************************************************************************
//Function: Configurate UART4
//Input:
//Output:
//******************************************************************************************************
void UART4_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
	GPIOPinConfigure(GPIO_PC4_U4RX);
  GPIOPinConfigure(GPIO_PC5_U4TX );//GPIO_PB1_U1TX);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	UARTConfigSetExpClk(SerialPort6, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	IntMasterEnable();
	IntEnable(INT_UART4);
	UARTIntEnable(SerialPort6, UART_INT_RX | UART_INT_RT);
}

//******************************************************************************************************
//Function: Configurate UART5
//Input:
//Output:
//******************************************************************************************************
void UART5_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
	GPIOPinConfigure(GPIO_PE4_U5RX);
  GPIOPinConfigure(GPIO_PE5_U5TX);
	GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	UARTConfigSetExpClk(SerialPort3, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	IntMasterEnable();
	IntEnable(INT_UART5);
	UARTIntEnable(SerialPort3, UART_INT_RX | UART_INT_RT);
}

//******************************************************************************************************
//Function: Configurate UART6
//Input:
//Output:
//******************************************************************************************************
void UART6_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);
	GPIOPinConfigure(GPIO_PD4_U6RX);
  GPIOPinConfigure(GPIO_PD5_U6TX);
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	UARTConfigSetExpClk(SerialPort0, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	IntMasterEnable();
	IntEnable(INT_UART6);
	UARTIntEnable(SerialPort0, UART_INT_RX | UART_INT_RT);
}

//******************************************************************************************************
//Function: Configurate UART7
//Input:
//Output:
//******************************************************************************************************
void UART7_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
	GPIOPinConfigure(GPIO_PE0_U7RX);
  GPIOPinConfigure(GPIO_PE1_U7TX);
	GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(SerialPort4, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	IntMasterEnable();
	IntEnable(INT_UART7);
	UARTIntEnable(SerialPort4, UART_INT_RX | UART_INT_RT);
}

//******************************************************************************************************
//Function: UART0 Handler
//Input:
//Output:
//******************************************************************************************************
void SeverIntHandler(void) //Uart0
{

	unsigned long ulStatus;
	ulStatus = UARTIntStatus(SeverPort, true); //get interrupt status
	UARTIntClear(SeverPort, ulStatus); //clear the asserted interrupts
	while(UARTCharsAvail(SeverPort)) //loop while there are chars
	{
		if (Count_sever>100) Count_sever = 0;
		ReadByte_sever = UARTCharGet(SeverPort);
		if(IsSending_sever==true)
		{
			if(ReadByte_sever==AFPROTO_END_BYTE)
			{
				RawBuffer_sever[Count_sever]=ReadByte_sever;
				Count_sever++;
				BufferLength_sever = Count_sever;			
				Count_sever = 0;
				IsSending_sever = false;
				BufferFlag_sever = true;			
			}
			else
			{
				RawBuffer_sever[Count_sever]=ReadByte_sever;
				Count_sever++;
			}
		}
		else
		{
			if(ReadByte_sever==AFPROTO_START_BYTE)
			{
				IsSending_sever = true;
				RawBuffer_sever[Count_sever]=ReadByte_sever;
				Count_sever++;
			}
		}
	}
}

//******************************************************************************************************
//Function: UART0 Handler
//Input:
//Output:
//******************************************************************************************************
void Port0IntHandler(void) //UART6
{
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(SerialPort0, true); //get interrupt status
	UARTIntClear(SerialPort0, ulStatus); //clear the asserted interrupts
	while(UARTCharsAvail(SerialPort0)) //loop while there are chars
	{
		if (Count[0]>100) Count[0] = 0;
		ReadByte[0] = UARTCharGet(SerialPort0);
		if(IsSending[0]==true)
		{
			if(ReadByte[0]==AFPROTO_END_BYTE)
			{
				RawBuffer[0][Count[0]]=ReadByte[0];
				Count[0]++;
				BufferLength[0] = Count[0];
				Count[0] = 0;
				IsSending[0] = false;
				BufferFlag[0] = true;			
			}
			else
			{
				RawBuffer[0][Count[0]]=ReadByte[0];
				Count[0]++;
			}
		}
		else
		{
			if(ReadByte[0]==AFPROTO_START_BYTE)
			{
				IsSending[0] = true;
				RawBuffer[0][Count[0]]=ReadByte[0];
				Count[0]++;
			}
		}
	}
}

//******************************************************************************************************
//Function: UART0 Handler
//Input:
//Output:
//******************************************************************************************************
void Port1IntHandler(void) //UART1
{
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(SerialPort1, true); //get interrupt status
	UARTIntClear(SerialPort1, ulStatus); //clear the asserted interrupts
	while(UARTCharsAvail(SerialPort1)) //loop while there are chars
	{
		if (Count[1]>100) Count[1] = 0;
		ReadByte[1] = UARTCharGet(SerialPort1);
		if(IsSending[1]==true)
		{
			if(ReadByte[1]==AFPROTO_END_BYTE)
			{
				RawBuffer[1][Count[1]]=ReadByte[1];
				Count[1]++;
				BufferLength[1] = Count[1];
				Count[1] = 0;
				IsSending[1] = false;
				BufferFlag[1] = true;			
			}
			else
			{
				RawBuffer[1][Count[1]]=ReadByte[1];
				Count[1]++;
			}
		}
		else
		{
			if(ReadByte[1]==AFPROTO_START_BYTE)
			{
				IsSending[1] = true;
				RawBuffer[1][Count[1]]=ReadByte[1];
				Count[1]++;
			}
		}
	}
}

//******************************************************************************************************
//Function: UART0 Handler
//Input:
//Output:
//******************************************************************************************************
void Port2IntHandler(void) //UART2
{
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(SerialPort2, true); //get interrupt status
	UARTIntClear(SerialPort2, ulStatus); //clear the asserted interrupts
	while(UARTCharsAvail(SerialPort2)) //loop while there are chars
	{
		if (Count[2]>100) Count[2] = 0;
		ReadByte[2] = UARTCharGet(SerialPort2);
		if(IsSending[2]==true)
		{
			if(ReadByte[2]==AFPROTO_END_BYTE)
			{
				RawBuffer[2][Count[2]]=ReadByte[2];
				Count[2]++;
				BufferLength[2] = Count[2];
				Count[2] = 0;
				IsSending[2] = false;
				BufferFlag[2] = true;			
			}
			else
			{
				RawBuffer[2][Count[2]]=ReadByte[2];
				Count[2]++;
			}
		}
		else
		{
			if(ReadByte[2]==AFPROTO_START_BYTE)
			{
				IsSending[2] = true;
				RawBuffer[2][Count[2]]=ReadByte[2];
				Count[2]++;
			}
		}
	}
}

//******************************************************************************************************
//Function: UART0 Handler
//Input:
//Output:
//******************************************************************************************************
void Port3IntHandler(void) //UART5
{
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(SerialPort3, true); //get interrupt status
	UARTIntClear(SerialPort3, ulStatus); //clear the asserted interrupts
	while(UARTCharsAvail(SerialPort3)) //loop while there are chars
	{
		if (Count[3]>100) Count[3] = 0;
		ReadByte[3] = UARTCharGet(SerialPort3);
		if(IsSending[3]==true)
		{
			if(ReadByte[3]==AFPROTO_END_BYTE)
			{
				RawBuffer[3][Count[3]]=ReadByte[3];
				Count[3]++;
				BufferLength[3] = Count[3];
				Count[3] = 0;
				IsSending[3] = false;
				BufferFlag[3] = true;			
			}
			else
			{
				RawBuffer[3][Count[3]]=ReadByte[3];
				Count[3]++;
			}
		}
		else
		{
			if(ReadByte[3]==AFPROTO_START_BYTE)
			{
				IsSending[3] = true;
				RawBuffer[3][Count[3]]=ReadByte[3];
				Count[3]++;
			}
		}
	}
}

//******************************************************************************************************
//Function: UART0 Handler
//Input:
//Output:
//******************************************************************************************************
void Port4IntHandler(void) //UART7
{
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(SerialPort4, true); //get interrupt status
	UARTIntClear(SerialPort4, ulStatus); //clear the asserted interrupts
	while(UARTCharsAvail(SerialPort4)) //loop while there are chars
	{
		if (Count[4]>100) Count[4] = 0;
		ReadByte[4] = UARTCharGet(SerialPort4);
		if(IsSending[4]==true)
		{
			if(ReadByte[4]==AFPROTO_END_BYTE)
			{
				RawBuffer[4][Count[4]]=ReadByte[4];
				Count[4]++;
				BufferLength[4] = Count[4];
				Count[4] = 0;
				IsSending[4] = false;
				BufferFlag[4] = true;			
			}
			else
			{
				RawBuffer[4][Count[4]]=ReadByte[4];
				Count[4]++;
			}
		}
		else
		{
			if(ReadByte[4]==AFPROTO_START_BYTE)
			{
				IsSending[4] = true;
				RawBuffer[4][Count[4]]=ReadByte[4];
				Count[4]++;
			}
		}
	}
}

//******************************************************************************************************
//Function: UART0 Handler
//Input:
//Output:
//******************************************************************************************************
void Port5IntHandler(void) //UART3
{
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(SerialPort5, true); //get interrupt status
	UARTIntClear(SerialPort5, ulStatus); //clear the asserted interrupts
	while(UARTCharsAvail(SerialPort5)) //loop while there are chars
	{
		if (Count[5]>100) Count[5] = 0;
		ReadByte[5] = UARTCharGet(SerialPort5);
		if(IsSending[5]==true)
		{
			if(ReadByte[5]==AFPROTO_END_BYTE)
			{
				RawBuffer[5][Count[5]]=ReadByte[5];
				Count[5]++;
				BufferLength[5] = Count[5];
				Count[5] = 0;
				IsSending[5] = false;
				BufferFlag[5] = true;			
			}
			else
			{
				RawBuffer[5][Count[5]]=ReadByte[5];
				Count[5]++;
			}
		}
		else
		{
			if(ReadByte[5]==AFPROTO_START_BYTE)
			{
				IsSending[5] = true;
				RawBuffer[5][Count[5]]=ReadByte[5];
				Count[5]++;
			}
		}
	}
}

//******************************************************************************************************
//Function: UART0 Handler
//Input:
//Output:
//******************************************************************************************************
void Port6IntHandler(void) //UART4
{
	unsigned long ulStatus;
	ulStatus = UARTIntStatus(SerialPort6, true); //get interrupt status
	UARTIntClear(SerialPort6, ulStatus); //clear the asserted interrupts
	while(UARTCharsAvail(SerialPort6)) //loop while there are chars
	{
		if (Count[6]>100) Count[6] = 0;
		ReadByte[6] = UARTCharGet(SerialPort6);
		if(IsSending[6]==true)
		{
			if(ReadByte[6]==AFPROTO_END_BYTE)
			{
				RawBuffer[6][Count[6]]=ReadByte[6];
				Count[6]++;
				BufferLength[6] = Count[6];
				Count[6] = 0;
				IsSending[6] = false;
				BufferFlag[6] = true;			
			}
			else
			{
				RawBuffer[6][Count[6]]=ReadByte[6];
				Count[6]++;
			}
		}
		else
		{
			if(ReadByte[6]==AFPROTO_START_BYTE)
			{
				IsSending[6] = true;
				RawBuffer[6][Count[6]]=ReadByte[6];
				Count[6]++;
			}
		}
	}
}
//GPIO Config
void GPIO_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, 	GPIO_PIN_2);		
	GPIODirModeSet(GPIO_PORTA_BASE,		GPIO_PIN_2, GPIO_DIR_MODE_OUT);
	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
}

void ClearRXBuffer_sever(void)
{
	int i;
	for (i=0; i<RX_BUF_SIZE; i++)
	{
		RawBuffer_sever[i]=0;
	}
}
void ClearRXBuffer(int buffernumber)
{
	int i;
	for (i=0; i<RX_BUF_SIZE; i++)
	{
		RawBuffer[buffernumber][i]=0;
	}
}

uint8_t Checksum(uint8_t buffernumber)
{
	uint8_t x=0;
	uint8_t i;
	for (i=0; i<RawBuffer[buffernumber][1]-1;i++)
	{
		x+=RawBuffer[buffernumber][i];
	}
	return x;
}

uint8_t Checksum_sever(void)
{
	uint8_t x=0;
	uint8_t i;
	for (i=0; i<RawBuffer_sever[1]-1;i++)
	{
		x+=RawBuffer_sever[i];
	}
	return x;
}

void DELAYms(int n)
{
	int i;
	for (i = 0; i <n; i++)		SysCtlDelay(SysCtlClockGet()/3000);	
}
void channel(unsigned long ulBas)
{
	for (b=0;b<BufferLength_sever;b++)
	{
		UARTCharPut(ulBas, RawBuffer_sever[b]);	
	}
}


//########################################################################################
int main(void)
{
	setup();
	while(1)
	{
		if(BufferFlag_sever==1)
		{
			//Truong hop START ESC _ _
			if (RawBuffer_sever[1]==AFPROTO_ESC_BYTE)
			{	
				//START ESC DATA^0x20 ESC ID^0x20
				if (RawBuffer_sever[3]==AFPROTO_ESC_BYTE)		
				{
						switch(RawBuffer_sever[4]^0x20)
					{
						case 0:
							channel(SerialPort0);
							break;
						case 1:
							channel(SerialPort1);
							break;
						case 2:
							channel(SerialPort2);
							break;
						case 3:
							channel(SerialPort3);
							break;
						case 4:
							channel(SerialPort4);
							break;
						case 5:
							channel(SerialPort5);
							break;
						case 6:
							channel(SerialPort6);
							break;
						default:;
					}	
				}					
				//SART ESC DATA^0x20 ID
				else
				{
						switch(RawBuffer_sever[3])
					{
						case 0:
							channel(SerialPort0);
							break;
						case 1:
							channel(SerialPort1);
							break;
						case 2:
							channel(SerialPort2);
							break;
						case 3:
							channel(SerialPort3);
							break;
						case 4:
							channel(SerialPort4);
							break;
						case 5:
							channel(SerialPort5);
							break;
						case 6:
							channel(SerialPort6);
							break;
						default:;
					}		
				}			
			}
			//Truong hop START Data ID
			else
			{				
				switch(RawBuffer_sever[2])
				{
					case 0:
						channel(SerialPort0);
						break;
					case 1:
						channel(SerialPort1);
						break;
					case 2:
						channel(SerialPort2);
						break;
					case 3:
						channel(SerialPort3);
						break;
					case 4:
						channel(SerialPort4);
						break;
					case 5:
						channel(SerialPort5);
						break;
					case 6:
						channel(SerialPort6);
						break;
					default:;
				}		
			}	
			BufferFlag_sever=0;
			ClearRXBuffer_sever();	
		}
	
		for (buffernum =0; buffernum<7; buffernum++)
		{
			if(BufferFlag[buffernum]==1)
			{
				//Encode Raw thanh Payload
				
			for (size =0; size < BufferLength[buffernum]; size++)
			{
				temp = afproto_get_data(RawBuffer[buffernum], size, PayLoad[buffernum], &write_len);
			}
			if (temp>0)
			{
				len = write_len;
		//		PayLoad[buffernum][1] = buffernum;  //numbered ID of packet
				for (i = 0; i < len; i++)
				{
					afproto_frame_data(PayLoad[buffernum], len, BufferReply[buffernum], &write_len);	
				}
				len = write_len+1;
				for (j = 0; j <len ; j++)
				{
					UARTCharPut(UART1_BASE,BufferReply[buffernum][j]);
					UARTCharPut(UART0_BASE,BufferReply[buffernum][j]);
				}
			}			
			else 
			{
				UARTCharPut(UART0_BASE,'F');
			}
			
			SysCtlDelay(SysCtlClockGet()/300);
				
				//Chen ID vao PayLoad
				
				//Clear RawBuffer
				
				//Goi lai RawBuffer va gui di
				
				//xoa co buffer
				BufferFlag[buffernum]=0;
				ClearRXBuffer(buffernum);			
			}
		}
	}
}
