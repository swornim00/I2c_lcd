#include<avr/io.h>
#define F_CPU 16000000UL      // Define CPU Frequency here it 16MHz 
#include<util/delay.h>
#include<avr/interrupt.h>
#include <util/twi.h>


#define LCD_RS PB0
#define LCD_EN PB1

#define DATA_DIR DDRD
#define CONTROL_DIR DDRB
#define DATA_PORT PORTD
#define CONTROL_PORT PORTB

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

void send_command(uint8_t data){ // command deko
    CONTROL_PORT &= ~(1 << LCD_RS); // RS pin high
    send_4bit(data);
    send_4bit(data << 4);

}

void send_data(uint8_t data){
    CONTROL_PORT |= (1 << LCD_RS); // RS pin high
    send_4bit(data);
    send_4bit(data << 4);
}

void send_4bit(char data){ // print gareko
    DATA_PORT = (DATA_PORT & 0x0F) | (data & 0xF0); // giving data
    // Giving a pulse

    CONTROL_PORT |= (1 << LCD_EN); // Enable pin high
	_delay_us(50);
    CONTROL_PORT &= ~(1 << LCD_EN); // Enable pin low
	_delay_us(200);

}

void init_lcd(){  
    CONTROL_DIR = 0xFF;
    DATA_DIR = 0XFF;

	_delay_ms(200);
    send_command(0x33);
	_delay_ms(20);
    send_command(0x32);
	_delay_ms(20);
    send_command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
	_delay_ms(20);
	send_command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKOFF );
	_delay_ms(20);
	send_command(LCD_CLEARDISPLAY);
	_delay_ms(20);


}

void lcd_string(char *str) {
	int i;
	for(i=0;str[i]!=0;i++)				/* Send each char of string till the NULL */
	{
		send_data(str[i]);
	}
}

void init_i2c(uint8_t addr){
    TWAR = addr << 1 ;// Load address into TWAR
	TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
    sei();

    _delay_ms(1000);
    send_command(LCD_CLEARDISPLAY);
    _delay_ms(1);
    lcd_string("I2C Initialized");
}


ISR(TWI_vect){
    send_command(LCD_CLEARDISPLAY);
    _delay_ms(1);
    lcd_string("Ack");
    
    uint8_t data;
    switch((TWSR & 0xF8)){
        case TW_SR_SLA_ACK: // Slave has been acknowledged
        case TW_SR_DATA_ACK: // Data has been recieved by the slave
            data = TWDR;
            send_data(data);
            TWCR |= (1<<TWINT) | (1<<TWEN) | (1 << TWEA);
        default:
          TWCR |= (1<<TWINT) | (1<<TWEN) | (1 << TWEA);
    }
}

int main(){
    init_lcd();
    init_i2c(0x47);
    _delay_ms(2000);

    send_command(LCD_CLEARDISPLAY);
    _delay_ms(2);

    lcd_string("Hello! People");
    _delay_ms(2000);
    send_command(0xC0);
    lcd_string("I'm I2C LCD");
    _delay_ms(2000);
}







