  
/*	Code for writing important values like passwords in EEPROM for system init.

*/

#include <EEPROM.h>
#include <avr/wdt.h>

int pass1 = 1234;
int pass2 = 1234;
//int master = 2016;
bool lock_sys = false; 

uint32_t phone = 716393097;
uint32_t phone2 = 716393097;
float mqty	 = 60.00;
float m_std	 = 140.0;
float m_rmng = 0;
float m_rev = 10;
float m_steps = 200;
unsigned long m_clb	 = 35000;
float m_tt	 = 64650.0;


int result = 40;

int pass1_rd, pass2_rd, master_rd;
uint32_t phone_rd;
uint32_t phone2_rd;
unsigned long m_clb_rd;
float mqty_rd, m_std_rd, m_rmng_rd, m_tt_rd;
int lock_sys_rd, lck_rd;

int i = 0;

char mqty_str[10];
char _buffer[55];

void setup() {
	
	Serial.begin(9600);
	while(!Serial);
	
	//Writing to EEPROM
	EEPROM.put(0, pass1);
	EEPROM.put(4, pass2);
 // EEPROM.put(9, master);

  EEPROM.put(120,lock_sys);
//  EEPROM.put(21,lck);
  
	EEPROM.put(200, phone);
	EEPROM.put(140, phone2);
	EEPROM.put(20, mqty);
	EEPROM.put(60, m_rev);
	EEPROM.put(30, m_rmng);
	EEPROM.put(70, m_clb);
	EEPROM.put(90, m_tt);
  EEPROM.put(120, lock_sys);
 
	delay(1000);
	/* 
	EEPROM.put(20, mqty);
	delay(1000); */
	
	
}

void loop() {
	
	for(; i < 1; ++i) {
			
		//Reading from EEPROM
		EEPROM.get(0, pass1_rd);
		EEPROM.get(4, pass2_rd);
		EEPROM.get(9, master_rd);
   // EEPROM.get(16, lock);
		EEPROM.get(120,lock_sys_rd);
   // EEPROM.get(120,lck_rd);
		EEPROM.get(200, phone_rd);
		EEPROM.get(140, phone2_rd);
    
		EEPROM.get(20, mqty_rd);
		EEPROM.get(60, m_std_rd);
		EEPROM.get(30, m_rmng_rd);
		EEPROM.get(70, m_clb_rd);
		EEPROM.get(90, m_tt_rd);
		delay(1000);
		
		/* mqty = mqty + (float)result;
		dtostrf(mqty, -6, 2, mqty_str);
		sprintf(_buffer, "Product Loaded %d L\r\nTotal Qty %s L", result, mqty_str);
		print_str(_buffer); */
		
		if(pass1 == pass1_rd) {
			Serial.println("Pass1 writing SUCCESS");
		}
		else {
			Serial.println("Pass1 writing FAILED");
		}
		
		if(pass2 == pass2_rd) {
			Serial.println("Pass2 writing SUCCESS");
		}
		if(phone == phone_rd) {
			Serial.println("phone writing SUCCESS");
		}
		else {
			Serial.println("phone writing FAILED");		
		}
		
		if(mqty == mqty_rd) {
			Serial.println("mqty writing SUCCESS");
		}
		else {
			Serial.println("mqty writing FAILED");
		}
		
		if(m_std == m_std_rd) {
			Serial.println("m_std writing SUCCESS");
		}
		else {
			Serial.println("m_std writing FAILED");
		}
		
		if(m_rmng == m_rmng_rd) {
			Serial.println("m_rmng writing SUCCESS");
		}
		else {
			Serial.println("m_rmng writing FAILED");
		}
		
		if(m_clb == m_clb_rd) {
			Serial.println("m_clb writing SUCCESS");
		}
		else {
			Serial.println("m_clb writing FAILED");
		}
		
		if(m_tt == m_tt_rd) {
			Serial.println("m_tt writing SUCCESS");
		}
		else { 
			Serial.println("m_tt writing FAILED");
		}
		
	}
}

void print_str(char message[]) {
	Serial.println(message);
}

/* void WDT_setup() {
	cli();
	wdt_reset();
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR |= (1<<WDIE) | (1<<WDE) | (1<<WDP3) | (1<<WDP0);
	sei();
}
 */
