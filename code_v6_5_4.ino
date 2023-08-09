//#include <LiquidCrystal.h>
//#include "LCD.h" // For LCD
#include "LiquidCrystal_I2C.h" // Added library*
#include <EEPROM.h>
#include <avr/wdt.h>
#include "phi_interfaces.h"
#include "phi_prompt.h"
//#include <phi_prompt.h>
#include "support_fxns.h"
#include <Wire.h>
//#include <twi.h>
#include <SoftwareSerial.h>

#define lcd_rows 4
#define lcd_columns 20

// LCD pin setting
//Phi-panel LCD pin setting

//H/w interconnect pins:
#define PMP 16
//#define TEMP_PIN 9

//SIM800 TX is connected to Arduino D8, RX connected to Arduino D7
#define SIM800_TX_PIN 	1
#define SIM800_RX_PIN 	0


//const char phone_number[] = "+254723043353";
//const char phone_number[] = "+254701598405";


uint32_t instant_t3 = 0;

//Create software serial object to communicate with SIM800
//SoftwareSerial serialSIM800(SIM800_TX_PIN, SIM800_RX_PIN);

//	AltSoftSerial serialSIM800;

//Application Global Variables:
byte addr[8];
byte data[12];
byte present = 0;
//float celsius, fahrenheit;
bool type_s = false;
float m_drn, bmlk, r_dn, ltrs;
//This variables should be read from EEPROM at setup()
float mqty;
float m_std;
float m_rmng;
unsigned long  m_clb;
float m_tt;
float prev_m_tt;
float Capacity = 250;
bool state;
bool lock_sys;
bool buying;
uint32_t const_numbr = 716233831;

//char phone = EEPROMReadlong(200);
//char check = EEPROMReadlong(15);	//what is this!?
int user_pass = 0;
int admin_pass  = 4;

uint32_t instant_t1, instant_t0;
bool cleaning;
bool calibrating;
//One wire object & lcd object
//OneWire  ds(TEMP_PIN);
//LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7);
//LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
// This is for a analog keypad:

// This is for a matrix keypad:
//#define buttons_per_column 	4
//#define buttons_per_row 	4
char mapping[] = {7, 8, 9,10,1, '3', '2', '1', 2, '6', '5', '4', 3, '9', '8', '7', 4, 6, '0', 5}; // This is a matrix keypad. 5*4
//char mapping[] = {4, 6, '0', 5, 3, '9', '8', '7', 2, '6', '5', '4', 1, '3', '2', '1'}; // This is a matrix keypad.4*4
//byte pins[]={16, 17, 18, 19, 20, 21, 22, 23}; // Both 3*4 and 4*4 matrix keypads can use this definition
byte pins[]={7,8,9,10,11,12,13,14,15};//5*4
//byte pins[] = {7, 8, 9, 10, 11, 12, 13, 14}; // Both 3*4 and 4*4 matrix keypads can use this definition
//byte pins[]={15,14,13,12,11,10,9,8,7};//5*4
phi_matrix_keypads my_btns(mapping, pins, 5, 4);
//phi_matrix_keypads my_btns(mapping, pins, 4, 4);
phi_serial_keypads debug_keypad(&Serial, 9600);

multiple_button_input * keypads[] = {&my_btns, &debug_keypad, 0};
char up_keys[]     = {1, 0}; ///< All keys that act as the up key are listed here.
char down_keys[]   = {2, 0}; ///< All keys that act as the down key are listed here.
char left_keys[]   = {5, 0}; ///< All keys that act as the left key are listed here.
char right_keys[]  = {6, 0}; ///< All keys that act as the right key are listed here.
char enter_keys[]  = {4, 0}; ///< All keys that act as the enter key are listed here.
char escape_keys[] = {3, 0}; ///< All keys that act as the escape key are listed here.
char * function_keys[] = {up_keys, down_keys, left_keys, right_keys, enter_keys, escape_keys}; ///< All function key names are gathered here for phi_prompt.

/*
multiple_button_input * keypads[]={&my_btns,&debug_keypad,0};
char up_keys[]     = {2,0}; ///< All keys that act as the up key are listed here.
char down_keys[]   = {3,0}; ///< All keys that act as the down key are listed here.
char left_keys[]   = {5,0}; ///< All keys that act as the left key are listed here.
char right_keys[]  = {6,0}; ///< All keys that act as the right key are listed here.
char enter_keys[]  = {1,0}; ///< All keys that act as the enter key are listed here.
char escape_keys[] = {4,0}; ///< All keys that act as the escape key are listed here.
char * function_keys[]={up_keys,down_keys,left_keys,right_keys,enter_keys,escape_keys}; ///< All function key names are gathered here for phi_prompt.
*/
const char msg_00[] PROGMEM = "Hewbe";
// Setting up two menus top and sub_1

//
 uint32_t instant_t7 = millis();

void setup() {
  //-----------LCD init-----------------
  lcd.begin(lcd_columns, lcd_rows);

  init_phi_prompt(&lcd, keypads, function_keys, lcd_columns, lcd_rows, '~'); // Supply the liquid crystal object, input keypads, and function key names. Also supply the column and row of the lcd, and indicator as '>'. You can also use '\x7e', which is a right arrow.

  analogWrite(3, 80);
  lcd.setBacklight(HIGH);
  Serial.begin(9600);
  while (!Serial);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("  powered by"));
  lcd.setCursor(2, 2);
  lcd.print(F(" Hewbe"));
  //DDRB |= (1<<PB5);
  //PORTB &= ~(1<<PB5);
  pinMode(PMP, OUTPUT);
  digitalWrite(PMP, LOW);


  EEPROM.get(20, mqty);
  EEPROM.get(60, m_std);
  EEPROM.get(30, m_rmng);
  EEPROM.get(70, m_clb);
  EEPROM.get(90, m_tt);
  //EEPROM.get(120, lock_sys);
  //Serial.begin(9600);
  instant_t7 = millis();
  while (millis()  - instant_t7 < 500);
//lock_check();
  //Watchdog timer setup
  //cli();
  //wdt_reset();
  //WDTCSR |= (1U<<WDCE) | (1U<<WDE);
  //WDTCSR  = (1U<<WDIE) | (1U<<WDE) | (1U<<WDP3) | (1U<<WDP0); //timeout of 8s
  //sei();
}

void loop() {
  // A two-level menu using two lists, one for top menu and one for a sub menu. Create any level of menu you like.

  top_menu();

  /*  static uint32_t previous_millis = 0;
    if((uint32_t)millis() - previous_millis >= (uint32_t)2*1000) {
  	previous_millis = millis();
  	query_sms();

    } */

}



ISR(WDT_vect) {
  EEPROM.put(20, mqty);
  EEPROM.put(30, m_rmng);
  EEPROM.put(90, m_tt);

}
