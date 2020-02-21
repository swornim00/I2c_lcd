#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

#define F_CPU 16000000UL
#define LCD_RS PB0
#define LCD_EN PB1
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

uint8_t data;


void send_command(uint8_t data){ // command deko
    DDRB = 0xFF;
    DDRD = 0XFF;

    PORTD = data ; // giving data
    CONTROL_PORT &= ~(1 << LCD_RS); // RS pin low

    // Giving a pulse
    PORTB |= (1 << LCD_EN); // Enable pin high
    _delay_us(1);
    PORTB &= ~(1 << LCD_EN); // Enable pin low
    _delay_ms(3);
}

void send_8bit(char data){ // print gareko
    PORTD = data ; // giving data
    CONTROL_PORT |= (1 << LCD_RS); // RS pin high

    // Giving a pulse
    PORTB &= ~(1 << LCD_EN); // Enable pin low
    _delay_us(1);
    PORTB |= (1 << LCD_EN); // Enable pin high
    _delay_us(1);
    PORTB &= ~(1 << LCD_EN); // Enable pin low
    _delay_ms(0.3);

}

void init_lcd(){  
    CONTROL_PORT = (1 << LCD_RS) | (1 << LCD_EN);
    DATA_PORT = 0XFF;

    _delay_ms(40);		/* LCD Power ON delay always >15ms */
	send_command(LCD_8BITMODE);	// Initialization in 8bit mode
    send_command(LCD_CURSORON);
    send_command(LCD_CLEARDISPLAY);
    _delay_ms(2);
}

void lcd_string(char *string) {
  for (char *it = string; *it; it++) {
    send_8bit(*it);
  }
}

void init_i2c(uint8_t addr){
    TWAR = addr << 1 ;// Loadd address into TWAR
    TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT); // Setting up the Control register 
    sei();
}


ISR(TW_Vect){
    switch((TWSR & 0xF8)){
        case TW_SR_SLA_ACK: // Slave has been acknowledged

            TWCR |= (1<<TWINT) | (1<<TWEN) | (1 << TWEA);

        case TW_SR_DATA_ACK: // Data has been recieved by the slave
            data = TWDR;
            
        case TW_ST_DATA_ACK: // Data has been requested

        default:
        
    }
}

int main(){
    DDRB = 0xff;
    PORTB |= (1 << PB0);
    return 0;
}







