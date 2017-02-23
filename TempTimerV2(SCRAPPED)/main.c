/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// TemperatureSensor + Timer /////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// By T.A & D.S              /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Our Includes
#include <pic32mx.h>
#include <stdint.h>
#include <stdbool.h>


#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200
#define TEMP_SENSOR_ADDR 0x48   //<-- Here the address of the temperature sensor is defined


//////////////////////////////////// The Above Could Be In Its Own File ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void) {

	uint16_t temp; //Deklarerar temp (fixed to string)
	char buf[32], s*, *t; 							// Vad är detta? (fixed to string)
	
	/* Set up peripheral bus clock */
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	
	/* Clear SPIROV*/
	SPI2STATCLR &= ~0x40;
	/* Set CKP = 1, MSTEN = 1; */
        SPI2CON |= 0x60;
	
	/* Turn on SPI */
	SPI2CONSET = 0x8000;
	
	//sätter
	I2C1CON = 0x0;
	
	I2C1BRG = 0x0C2;
	I2C1STAT = 0x0;
	I2C1CONSET = 1 << 13; //SIDL = 1
	I2C1CONSET = 1 << 15; // ON = 1
	temp = I2C1RCV; //Clear receive buffer
	
	
	//Uppdaterar display med text
	display_init();
	display_string(0, "Temperature:");
	display_string(1, "");
	display_string(2, "");
	display_string(3, "");
	display_update();
	
	/* Send start condition and address of the temperature sensor with
	write mode (lowest bit = 0) until the temperature sensor sends
	acknowledge condition */
	do {
		i2c_start();
	} while(!i2c_send(TEMP_SENSOR_ADDR << 1));
	/* Send register number we want to access */
	i2c_send(TEMP_SENSOR_REG_CONF);
	/* Set the config register to 0 */
	i2c_send(0x0);
	/* Send stop condition */
	i2c_stop();
	
	for(;;) {
		/* Send start condition and address of the temperature sensor with
		write flag (lowest bit = 0) until the temperature sensor sends
		acknowledge condition */
		do {
			i2c_start();
		} while(!i2c_send(TEMP_SENSOR_ADDR << 1));
		/* Send register number we want to access */
		i2c_send(TEMP_SENSOR_REG_TEMP);
		
		/* Now send another start condition and address of the temperature sensor with
		read mode (lowest bit = 1) until the temperature sensor sends
		acknowledge condition */
		do {
			i2c_start();
		} while(!i2c_send((TEMP_SENSOR_ADDR << 1) | 1));
		
		/* Now we can start receiving data from the sensor data register */
		temp = i2c_recv() << 8;
		i2c_ack();
		temp |= i2c_recv();
		/* To stop receiving, send nack and stop */
		i2c_nack();
		i2c_stop();
		
		s = fixed_to_string(temp, buf);		//lägger datan i s med fixed to string
		t = s + strlen(s);
		*t++ = ' ';
		*t++ = 7;
		*t++ = 'C';
		*t++ = 0;
		
		display_string(1, s);
		display_update();
		delay(1000000);
	}
	
	return 0;











}