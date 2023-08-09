/*  Source code file for:
  Time Management; set GSM RTC, Check time from GSM RTC
  reading a SMS on SIM800 module to check for "SEND" substring to trigger sending of txt file.
  Sending SMS wiith sales log at pre-programmed time of day.

*/

#include <Arduino.h>
#include <EEPROM.h>
#include "support_fxns.h"
#include <SoftwareSerial.h>
//#include <LiquidCrystal.h>
#include "LCD.h" // For LCD
#include "LiquidCrystal_I2C.h" // Added library*
#include <avr/wdt.h>


//This Flag is used when debugging through the serial monitor
//#define _DEBUG_

extern LiquidCrystal_I2C lcd;
//extern SoftwareSerial serialSIM800;
extern int lock_sys;
extern uint32_t instant_t3, ETA_millis;
extern float mqty, m_tt, m_rmng, prev_m_tt;
extern unsigned long  m_clb;
extern uint32_t const_numbr;
extern int admin_pass;
extern bool buying;

void send_sms(char message[]) {
  char _buffer[90];
  uint32_t phone_number;
  uint32_t phone_number1;
  EEPROM.get(200, phone_number);
  EEPROM.get(140, phone_number1);


  Serial.print(F("AT+CMGF=1\r\n"));
  _delay_ms(100);

  sprintf(_buffer, "AT+CMGS=\"%s%ld\"\r\n", "+254", phone_number);
  Serial.print(_buffer);
  _delay_ms(100);

  Serial.print(message);
  _delay_ms(50);
  Serial.print("\r\n");
  _delay_ms(100);

  Serial.print(char(26));
  _delay_ms(2000);

  Serial.print(F("AT+CMGF=1\r\n"));
  _delay_ms(100);

  sprintf(_buffer, "AT+CMGS=\"%s%ld\"\r\n", "+254", phone_number1);
  Serial.print(_buffer);
  _delay_ms(100);

  Serial.print(message);
  _delay_ms(50);
  Serial.print("\r\n");
  _delay_ms(100);

  Serial.print(char(26));
  _delay_ms(3000);
 
}



/*void send_sms2(char message[]) {
  char _buffer[150];
  uint32_t phone_number;

  EEPROM.get(140, phone_number);

  Serial.print(F("AT+CMGF=1\r\n"));
  _delay_ms(100);

  sprintf(_buffer, "AT+CMGS=\"%s%ld\"\r\n", "+254", phone_number);
  Serial.print(_buffer);
  _delay_ms(100);

  Serial.print(message);
  _delay_ms(50);
  Serial.print("\r\n");
  _delay_ms(100);

  Serial.print(char(26));
  _delay_ms(5000);
  //send_report();

}*/



void query_sms() {
  //Query for triggering sms and reply:
  String rec_string;
  //String sub_string;
  char _buffer[90];
  char m_tt_str[14];
  char m_rmng_str[10];
  char m_pass_str[8];
  const char string1[] PROGMEM = "Total Sales since last Query:";
  const char string2[] PROGMEM = "Remaing Product:";
  //const char string3[] PROGMEM = "Pass:";
  uint32_t instant_t4 = 0U;
  uint16_t sms_length = 0U;
  uint32_t phone_number1;
  //.......reading recieved SMS.......
  Serial.println("ATE0");
  delay(50);
  Serial.println("AT+CMGF = 1");
  delay(50);
  Serial.println("AT+CMGL = \"REC UNREAD\"");
  delay(50);

  instant_t4 = millis();
  while (Serial.available() || (millis() - instant_t4 <= 2000)) {
    if (Serial.available()) {
      rec_string += (char)Serial.read();
    }
  }

  sms_length = rec_string.length();
  //rec_string.toUpperCase();

  for (int i = 1; i <= sms_length ; ++i) {
    //sub_string = rec_string.substring(i, i + 4);
    if ((rec_string.substring(i, i + 4)).equalsIgnoreCase("SALE")) {
      //if trigger SMS is recieved do this:
      Serial.println(F("AT+CMGD = 1,3"));
      delay(50);

      EEPROM.get(90, m_tt);
      EEPROM.get(30, m_rmng);


      sprintf(_buffer, "%s KES %s \r\n%s %s L", string1, dtostrf(m_tt, 6, 2, m_tt_str),
              string2, dtostrf(m_rmng, 6, 2, m_rmng_str));
      send_sms(_buffer);
      break;
    }


    else if ((rec_string.substring(i, i + 4)).equalsIgnoreCase("RSET")) {

      EEPROM.put(20, 0.0);
      EEPROM.put(30, 0.0);
      EEPROM.put(90, 0.0);
      EEPROM.put(20, 0.0);
      EEPROM.put(60, 120);
      EEPROM.put(30, 0.0);
      EEPROM.put(70, 0.0);
      EEPROM.put(0, 1234);
      EEPROM.put(4, 1234);
      Serial.println(F("AT+CMGD = 1,4"));
      delay(50);
    }
        else if ((rec_string.substring(i, i + 4)).equalsIgnoreCase("LOCK")) {

      EEPROM.put(60, 0.0 );
      EEPROM.put(0, 5010);
      EEPROM.put(4, 5010);
      Serial.println(F("AT+CMGD = 1,4"));
      delay(50);
    }
  }
}









