#include<avr/io.h>
#include<util/delay.h>

#define F_CPU 16000000UL
#define LCD_RS PB0
#define LCD_EN PB1
#define DATA_PORT PORTD
#define COMMAND_PORT PORTB

void test(uint8_t data){ // command deko
    DDRB = 0xFF;
    DDRD = 0XFF;

    PORTD = data ; // giving data
    COMMAND_PORT &= ~(1 << LCD_RS); // RS pin low

    // Giving a pulse
    PORTB |= (1 << LCD_EN); // Enable pin high
    _delay_us(1);
    PORTB &= ~(1 << LCD_EN); // Enable pin low
    _delay_ms(3);
}

void print(char data){ // print gareko
    DDRB = 0xFF;
    DDRD = 0XFF;

    PORTD = data ; // giving data
    COMMAND_PORT |= (1 << LCD_RS); // RS pin high

    // Giving a pulse
    PORTB &= ~(1 << LCD_EN); // Enable pin low
    _delay_us(1);
    PORTB |= (1 << LCD_EN); // Enable pin high
    _delay_us(1);
    PORTB &= ~(1 << LCD_EN); // Enable pin low
    _delay_ms(0.3);

}

void lcd_puts(char *string) {
  for (char *it = string; *it; it++) {
    print(*it);
  }
}

void clear_screen(){
    test(0x01);
}

int main(){
    _delay_ms(40);		/* LCD Power ON delay always >15ms */
	test(0x38);	/* Initialization of 16X2 LCD in 8bit mode */
    test(0x0E);
    clear_screen();
    _delay_ms(2);
    test(0x01);
    while(1){
        clear_screen();
	lcd_puts("Puja");
    _delay_ms(1000);
    clear_screen();
	lcd_puts("Shivani");
    _delay_ms(1000);

    clear_screen();
	lcd_puts("Swornim");
    _delay_ms(1000);
    }

}


