/*
Trimiterea unui mesaj prin intermediul modemului GSM in momentul in care motorul a fost activat prin apasarea butonului P15.13, 
de asemenea acest eveniment este insotit de aprinderea ledurilor.

Interfata seriala UART_0 prevazuta cu driver RS3232 se configureaza cu transmisie
directa , respectiv:

TX - P2.14
Rx - P2.15

*/

#include <DAVE.h>
int s,i;


void delay()
{
	 for(i = 0;i <0xfffff;i++);
}


int main(void)
{

  uint8_t send_data1[] = "AT\r\n";
  uint8_t send_data2[] = "AT+CMGF=1\r\n";
  uint8_t send_data3[] = "AT+CMGS=\"0720861550\"\r\n";
  uint8_t send_data4[] = "Motorul a fost pus in functiune";


  DAVE_Init();		/* Initializare  DAVE APPs  */
  BUS_IO_Write (&byte, 0x1); //sting ledurile


  PWM_Init (&PWM_0);         // initializare modul PWM
  PWM_SetFreq(&PWM_0,50);     // perioada 20 milisecunde
  PWM_SetDutyCycle(&PWM_0,0); // nu genereaza semnal
  PWM_Start(&PWM_0);          // porneste pwm cu factor zero


  /* Transmisie mesaj de inceput */
  while(1U)
  {
	s= DIGITAL_IO_GetInput(&k); //lectura buton
	if(s==0)   // eventual testare valoare sensor
	{
	  PWM_SetDutyCycle(&PWM_0,810);  // limita stanga
	  delay(); delay(); delay();delay();delay();


	  PWM_SetDutyCycle(&PWM_0,480);  // limita drepata
	  delay();delay();delay();delay();delay();


	  BUS_IO_Write (&byte,0x0);
	  UART_Transmit(&UART_0, send_data1, sizeof(send_data1));//"AT\r\n"
	  
	  
	  for( i=0;i<0xfffff;i++);

	  UART_Transmit(&UART_0, send_data2, sizeof(send_data2));//"AT+CMGF=1\r\n"
	  for( i=0;i<0xfffff;i++);
	  

	  UART_Transmit(&UART_0, send_data3, sizeof(send_data3)); 
	  for( i=0;i<0xfffff;i++);
	  

	  UART_Transmit(&UART_0, send_data4, sizeof(send_data4)); //"Semnificatie data "
	  for( i=0;i<0xfffff;i++);
	  

      UART_TransmitWord(&UART_0, 26);   // ctrl z
      for( i=0;i<0xfffff;i++);          // evitare transmisii multiple
	 }
  }
  return 0;
}
