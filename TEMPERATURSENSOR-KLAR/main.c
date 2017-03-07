
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

//Adressen för tempereratursensorn på I2C-bussen
#define TEMP_SENSOR_ADDR 0x48


// GLobala variabler
int tid = 0x00000000;	//Tiden som timern börjar från
char textstring[] = "asdkladklaskdlasdklasdklaskdlaskdl"; //Godtyckligt lång sträng
const uint8_t const font[128*8];
char textbuffer[4][16];
int timeoutcount = 0;


//Temperatursensorns interna register
typedef enum TempSensorReg TempSensorReg;
enum TempSensorReg {
	TEMP_SENSOR_REG_TEMP,
	TEMP_SENSOR_REG_CONF,
	TEMP_SENSOR_REG_HYST,
	TEMP_SENSOR_REG_LIMIT,
};

 
//Skapar bokstäverna koden skriver med på displayen
 const uint8_t const font[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 94, 0, 0, 0, 0,
	0, 0, 4, 3, 4, 3, 0, 0,
	0, 36, 126, 36, 36, 126, 36, 0,
	0, 36, 74, 255, 82, 36, 0, 0,
	0, 70, 38, 16, 8, 100, 98, 0,
	0, 52, 74, 74, 52, 32, 80, 0,
	0, 0, 0, 4, 3, 0, 0, 0,
	0, 0, 0, 126, 129, 0, 0, 0,
	0, 0, 0, 129, 126, 0, 0, 0,
	0, 42, 28, 62, 28, 42, 0, 0,
	0, 8, 8, 62, 8, 8, 0, 0,
	0, 0, 0, 128, 96, 0, 0, 0,
	0, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 96, 0, 0, 0,
	0, 64, 32, 16, 8, 4, 2, 0,
	0, 62, 65, 73, 65, 62, 0, 0,
	0, 0, 66, 127, 64, 0, 0, 0,
	0, 0, 98, 81, 73, 70, 0, 0,
	0, 0, 34, 73, 73, 54, 0, 0,
	0, 0, 14, 8, 127, 8, 0, 0,
	0, 0, 35, 69, 69, 57, 0, 0,
	0, 0, 62, 73, 73, 50, 0, 0,
	0, 0, 1, 97, 25, 7, 0, 0,
	0, 0, 54, 73, 73, 54, 0, 0,
	0, 0, 6, 9, 9, 126, 0, 0,
	0, 0, 0, 102, 0, 0, 0, 0,
	0, 0, 128, 102, 0, 0, 0, 0,
	0, 0, 8, 20, 34, 65, 0, 0,
	0, 0, 20, 20, 20, 20, 0, 0,
	0, 0, 65, 34, 20, 8, 0, 0,
	0, 2, 1, 81, 9, 6, 0, 0,
	0, 28, 34, 89, 89, 82, 12, 0,
	0, 0, 126, 9, 9, 126, 0, 0,
	0, 0, 127, 73, 73, 54, 0, 0,
	0, 0, 62, 65, 65, 34, 0, 0,
	0, 0, 127, 65, 65, 62, 0, 0,
	0, 0, 127, 73, 73, 65, 0, 0,
	0, 0, 127, 9, 9, 1, 0, 0,
	0, 0, 62, 65, 81, 50, 0, 0,
	0, 0, 127, 8, 8, 127, 0, 0,
	0, 0, 65, 127, 65, 0, 0, 0,
	0, 0, 32, 64, 64, 63, 0, 0,
	0, 0, 127, 8, 20, 99, 0, 0,
	0, 0, 127, 64, 64, 64, 0, 0,
	0, 127, 2, 4, 2, 127, 0, 0,
	0, 127, 6, 8, 48, 127, 0, 0,
	0, 0, 62, 65, 65, 62, 0, 0,
	0, 0, 127, 9, 9, 6, 0, 0,
	0, 0, 62, 65, 97, 126, 64, 0,
	0, 0, 127, 9, 9, 118, 0, 0,
	0, 0, 38, 73, 73, 50, 0, 0,
	0, 1, 1, 127, 1, 1, 0, 0,
	0, 0, 63, 64, 64, 63, 0, 0,
	0, 31, 32, 64, 32, 31, 0, 0,
	0, 63, 64, 48, 64, 63, 0, 0,
	0, 0, 119, 8, 8, 119, 0, 0,
	0, 3, 4, 120, 4, 3, 0, 0,
	0, 0, 113, 73, 73, 71, 0, 0,
	0, 0, 127, 65, 65, 0, 0, 0,
	0, 2, 4, 8, 16, 32, 64, 0,
	0, 0, 0, 65, 65, 127, 0, 0,
	0, 4, 2, 1, 2, 4, 0, 0,
	0, 64, 64, 64, 64, 64, 64, 0,
	0, 0, 1, 2, 4, 0, 0, 0,
	0, 0, 48, 72, 40, 120, 0, 0,
	0, 0, 127, 72, 72, 48, 0, 0,
	0, 0, 48, 72, 72, 0, 0, 0,
	0, 0, 48, 72, 72, 127, 0, 0,
	0, 0, 48, 88, 88, 16, 0, 0,
	0, 0, 126, 9, 1, 2, 0, 0,
	0, 0, 80, 152, 152, 112, 0, 0,
	0, 0, 127, 8, 8, 112, 0, 0,
	0, 0, 0, 122, 0, 0, 0, 0,
	0, 0, 64, 128, 128, 122, 0, 0,
	0, 0, 127, 16, 40, 72, 0, 0,
	0, 0, 0, 127, 0, 0, 0, 0,
	0, 120, 8, 16, 8, 112, 0, 0,
	0, 0, 120, 8, 8, 112, 0, 0,
	0, 0, 48, 72, 72, 48, 0, 0,
	0, 0, 248, 40, 40, 16, 0, 0,
	0, 0, 16, 40, 40, 248, 0, 0,
	0, 0, 112, 8, 8, 16, 0, 0,
	0, 0, 72, 84, 84, 36, 0, 0,
	0, 0, 8, 60, 72, 32, 0, 0,
	0, 0, 56, 64, 32, 120, 0, 0,
	0, 0, 56, 64, 56, 0, 0, 0,
	0, 56, 64, 32, 64, 56, 0, 0,
	0, 0, 72, 48, 48, 72, 0, 0,
	0, 0, 24, 160, 160, 120, 0, 0,
	0, 0, 100, 84, 84, 76, 0, 0,
	0, 0, 8, 28, 34, 65, 0, 0,
	0, 0, 0, 126, 0, 0, 0, 0,
	0, 0, 65, 34, 28, 8, 0, 0,
	0, 0, 4, 2, 4, 2, 0, 0,
	0, 120, 68, 66, 68, 120, 0, 0,
};

//Gör om tidens värde till string (timern)
void time2string(char* textStr, int timeVal)
{

int val; 
	
	//Maskar fram siffran för rätt plats
	//shiftar den så att den blir de fyra minst signifikanta bitarna
	//Adderar siffran med 0x30 för att få rätt ascii-symbol
	//Lägger symbolen på rätt plats i texStr arrayen
	//Lägger även till kolon var tredje siffra
	val = (timeVal & 0xf0000000) >> 28;
	textStr[0] = (char) val + 0x30;
	
	val = (timeVal & 0x0f000000) >> 24;
	textStr[1] = (char) val + 0x30;
	
	textStr[2] = ':';
	
	val = (timeVal & 0x00f00000) >> 20;
	textStr[3] = (char) val + 0x30;
	
	val = (timeVal & 0x000f0000) >> 16;
	textStr[4] = (char) val + 0x30;
	
	textStr[5] = ':';
	
	val = (timeVal & 0x0000f000) >> 12;
	textStr[6] = (char) val + 0x30;
	
	val = (timeVal & 0x00000f00) >> 8;
	textStr[7] = (char) val + 0x30;
	
	textStr[8] = ':';
	
	val = (timeVal & 0x000000f0) >> 4;
	textStr[9] = (char) val + 0x30;
	
	val = timeVal & 0x0000000f;
	textStr[10] = (char) val + 0x30;
	
	textStr[11] = '\0';
	
}

//Används som delay
void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

//Godtycklig Labbkod
uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 0x01));
	return SPI2BUF;
}

//Godtycklig Labbkod
void display_init() {
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	delay(10);
	DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
	delay(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
	delay(10);
	DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
	delay(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
	delay(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

//Godtycklig Labbkod
void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

//Godtycklig Labbkod
void display_update() {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

//Godtycklig Labbkod
void tick(unsigned int * timep)
{
  //Tar det nuvarande värdet av tiden och lagrar det lokalt
  register unsigned int t = * timep;
  t += 1; //Ökar räknaren i tick med 1
  
  //Här ändrar man hur klockans egenskapaer och hur den beter sig

  if( (t & 0x0000000f) >= 0x0000000a ) t += 0x00000006;
  if( (t & 0x000000f0) >= 0x00000060 ) t += 0x000000a0;
  //Sekunder

  if( (t & 0x00000f00) >= 0x00000a00 ) t += 0x00000600;
  if( (t & 0x0000f000) >= 0x00006000 ) t += 0x0000a000;
  //Minuter

  if( (t & 0x000f0000) >= 0x000a0000 ) t += 0x00060000;
  if( (t & 0x00ff0000) >= 0x00240000 ) t += 0x00dc0000;
  //Timmar

  if( (t & 0x0f000000) >= 0x0a000000 ) t += 0x06000000;
  if( (t & 0xf0000000) >= 0xa0000000 ) t = 0;
  //Dagar

  * timep = t; //Lagrar t till ett nytt värde
}

//Väntar på att I2C-bussen ska bli inaktiv
void i2c_idle() {
	while(I2C1CON & 0x1F || I2C1STAT & (1 << 14)); 
	//Medan alla "starting bits" i I2C1CON är enablade, eller,  när TRSTAT(Transmit status bit) är lika med 1
}


//Skickar en byte via I2C bussen, returnar en ack/nack ,"status of transaction"
bool i2c_send(uint8_t data) {
	i2c_idle();
	I2C1TRN = data;
	i2c_idle();
	return !(I2C1STAT & (1 << 15)); //ACKSTAT
}

//Får in en byte från I2C bussen
uint8_t i2c_recv() {
	i2c_idle();
	I2C1CONSET = 1 << 3; //RCEN = 1
	i2c_idle();
	I2C1STATCLR = 1 << 6; //I2COV = 0
	return I2C1RCV;	     //I2C1RCV är ett register som innehåller "read data" (byte)
}

//skicakr ack via I2C-bussen
void i2c_ack() {
	i2c_idle();
	I2C1CONCLR = 1 << 5; //ACKDT = 0
	I2C1CONSET = 1 << 4; //ACKEN = 1
}

//skicakr nack via I2C-bussen
void i2c_nack() {
	i2c_idle();
	I2C1CONSET = 1 << 5; //ACKDT = 1
	I2C1CONSET = 1 << 4; //ACKEN = 1
}

//skicakr "start" via I2C-bussen
void i2c_start() {
	i2c_idle();
	I2C1CONSET = 1 << 0; //SEN
	i2c_idle();
}

//skicakr "restart" via I2C-bussen
void i2c_restart() {
	i2c_idle();
	I2C1CONSET = 1 << 1; //RSEN
	i2c_idle();
}

//skicakr "stop" via I2C-bussen
void i2c_stop() {
	i2c_idle();
	I2C1CONSET = 1 << 2; //PEN
	i2c_idle();
}


/* Convert 8.8 bit fixed point to string representation*/
char *fixed_to_string(uint16_t num, char *buf) // unsigned int av storleken 16, charpekare
{
	bool neg = false;
	uint32_t n;
	char *tmp;
	
	if(num & 0x8000) {
		num = ~num + 1;
		neg = true;
	}
	
	buf += 4; //RIP
	n = num >> 8;
	tmp = buf;
	do {
		*--tmp = (n  % 10) + '0';
		n /= 10;
	} while(n);
	if(neg)
		*--tmp = '-';
	
	n = num;
	if(!(n & 0xFF)) {
		*buf = 0;
		return tmp;
	}
	*buf++ = '.';
	while((n &= 0xFF)) {
		n *= 10;
		*buf++ = (n >> 8) + '0';
	}
	*buf = 0;
	
	return tmp;
}

uint32_t strlen(char *str) // unsigned int av storleken 32, charpekare
{ 
	uint32_t n = 0;
	while(*str++)
		n++;
	return n;
}

//Hämter värdet för Switcharna (1-4)
int getsw(void)
{
	int sw; 
	sw = ((PORTD >> 8) & 0x0000F);
	return sw;
	
}

//Kollar interrupts, skriver ut timern
void user_isr( void )
{
	if(IFS(0) & 0x100)
	{	
		timeoutcount++;
		if(timeoutcount == 10)
		{
			time2string(textstring, tid);
			display_string(3, textstring);			
			display_update();
			tick(&tid);
			timeoutcount = 0;
		}	
		IFSCLR(0) = 0x100;
	}
}


int main(void) {
	
	uint16_t temp = 0;
	uint16_t mintemp = 0;
	uint16_t maxtemp = 0;
	uint16_t avgtemp = 0;
	char buf[32], *s, *t;


	//Sätter upp peripheral bus clock
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;
	
	// Output pins för display signalerna
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	//Gör SPI till master
	SPI2CON = 0;
	SPI2BRG = 4;
	
	//Nollställer SPIROV
	SPI2STATCLR &= ~0x40;
	//Gör Set CKP = 1 och MSTEN = 1
        SPI2CON |= 0x60;
	
	//Sätter upp SPI
	SPI2CONSET = 0x8000;
	
	//Sätter upp I2C
	I2C1CON = 0x0;
	// I2C "Baud rate" bör vara mindre än 400 kHz, vilket genereras när man dividerar pbclock med 40 MHz
	I2C1BRG = 0x0C2;
	I2C1STAT = 0x0;
	I2C1CONSET = 1 << 13; //SIDL = 1
	I2C1CONSET = 1 << 15; // ON = 1
	temp = I2C1RCV; //nollställer "receive buffer"

	//Initialiserar displayen att visa information
	//ger varje rad tomt
	display_init();
	display_string(0, "");
	display_string(1, "");
	display_string(2, "");
	display_string(3, "");
	
	//Ställer in timer2
	PR2 = 31250; // 100ms / (80 000 000Hz / 256prescale )
	T2CONSET = 0x70;
	TMR2 = 0;
	TRISD = TRISD | 0xfe0; // [1111,1110,0000]
	TRISESET = 0x00;
	T2CONSET = 0x8000;
	IEC(0) = 0x100; //0xbf881060
	IPC(2) = 0x1f;  //0xbf8810b0
	enable_interrupt();
	
	//Skickar statkondition och temperatursensorns adress med
	//"write mode" (lägsta bit = 0) tills temperatursensorn skickar en ack
	do {
		i2c_start();
	} while(!i2c_send(TEMP_SENSOR_ADDR << 1));
	//Skickar register numret vi vill ha tillgång till
	i2c_send(TEMP_SENSOR_REG_CONF);
	//Sätter configregistret till 0
	i2c_send(0x0);
	//Skickar stoppkondition
	i2c_stop();
	

	for(;;) {
		//Skickar statkondition och temperatursensorns adress i
		//"write flag" (lägsta bit = 0) tills temperatursensorn skickar en ack
		do {
			i2c_start();
		} while(!i2c_send(TEMP_SENSOR_ADDR << 1));
		
		//Skickar register numret vi vill ha tillgång till
		i2c_send(TEMP_SENSOR_REG_TEMP);
		
		//Skickar en annan startkondition och temperatursensorns adress
		//i "read mode" (lägsta bit = 1) tills temperatursensorn skickar en ack
		do {
			i2c_start();
		} while(!i2c_send((TEMP_SENSOR_ADDR << 1) | 1));
	
		//Nu kan vi börja ta emot data från sensorns dataregister
		temp = i2c_recv() << 8;
		i2c_ack();
		temp |= i2c_recv();
		
		//För att sluta ta emot, skicka nack och stopp
		i2c_nack();
		i2c_stop();
		
		//Lägger till det första temp-värdet i maxtemp
		//Om ett annat temp värde är större än nuvarande maxtemp, byter den ut
		if ((maxtemp == 0) || (temp > maxtemp))
			maxtemp = temp;
		
		//Lägger till det första temp-värdet i mintemp
		//Om ett annat temp värde är mindre än nuvarande mintemp, byter den ut
		if ((mintemp == 0) || (temp < mintemp))
			mintemp = temp;
		
		//Lägger till det första  temp-värdet i avgtemp
		if (avgtemp == 0)
			avgtemp = temp;
		
		//Tar nuvarande avgtemp och adderar med nya temp, sedan delar på två. 
		//Lägger det nya värdet i avgtemp
		else 
			avgtemp = ((avgtemp + temp)/2);
		
		
		//Gör om värdet temp till en string
		s = fixed_to_string(temp, buf);
		
		//t används för att skriva ut enheten
		t = s + strlen(s);
		*t++ = ' ';
		*t++ = 'C';
		*t++ = 0;
		
		//Visar temp på rad noll
		//Uppdaterar displayn
		display_string(0, s);
		display_update();
		
		//Tar värden från switcharna
		int switchez = getsw();
		
		//Om ingen switch är på
		//Skriv ut inget
		if(switchez == 0)
			{
				display_string(1, "");
				display_string(2, "");
			}
		//Om switch ett är på
		//Skriv ut maxtemp
		if(switchez == 1)
			{
				s = fixed_to_string(maxtemp, buf);
				display_string(1, "Max Temp");
				display_string(2, s);

			}	
		//Om switch två är på
		//Skriv ut mintemp
		if(switchez == 2)
			{
				s = fixed_to_string(mintemp, buf);
				display_string(1, "Min Temp");
				display_string(2, s);
			}
		//Om switch tre är på
		//Skriv ut avgtemp
		if(switchez == 4)
			{
				s = fixed_to_string(avgtemp, buf);
				display_string(1, "Avg Temp"); 
				display_string(2, s);
			}
			
		
		//t används för att skriva ut enheten
		t = s + strlen(s);
		*t++ = ' ';
		*t++ = 'C';
		*t++ = 0;
		
		//Uppdatera displayn		
		display_update();
		
		// saktar ner for loopen.
		delay(1000000);
		
	}
	
	return 0;
}
