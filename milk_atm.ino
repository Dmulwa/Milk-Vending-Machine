//#include <stdio.h>
//#include "LCD.h" // For LCD
#include "LiquidCrystal_I2C.h" // Added library*
#include <Wire.h>
#include <itoa.h>

#define disk1 0x54    //Address of 24LC256 eeprom chip
//
//char newStr[10] ;

//Global variables
int screen, Length, itemSelected, sub, x, y, subMenuSelected;
int menu_Pointer = 0;
String text = "Hewbe";
//Main Menu
String menu[3] = {"Sell:", "User:", "Admin:"};

//sub menu
String subMenu0[4] = {"Load Product:", "Calibrate:", "Reserve:", "Clean:"};

String subMenu1[4] = {"Set Price:", "Sales:", "Change Passwrd:", "Reset:"};


//LCD definitions
//--------------LCD DEFINITIONS-----------------
#define COLUMNS           16
#define ROWS             2

#define LCD_SPACE_SYMBOL 0x20  //space symbol from the lcd ROM, see p.9 of GDM2004D datasheet

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

int text_width = 3;
int lcd_column = 2;
int lcd_row = 1;
//keypad
const byte rows = 5;
const byte cols = 4;

char keys[rows][cols] = {
  {'a', 'b', '#', '*'},
  {'1', '2', '3', 'U'},
  {'4', '5', '6', 'D'},
  {'7', '8', '9', 'B'},
  {'L', '0', 'R', 'E'}
};

//byte rowPins[rows] = {2, 3, 4, 5, 6};
//byte colPins[cols] = {10, 9, 8, 7};

byte rowPins[rows] = {PA1, PA11, PA12,PA4,PA5};
byte colPins[cols] = {PB1,PB0,PA7,PA6};

//byte rowPins[rows] = {PB1,PB0,PA7,PA6,PA6};
//byte colPins[cols] = {PA11, PA12, PA4, PA5};

//functional variables
float m_drn, bmlk, r_dn, ltrs, mqty, m_std, m_rmng, m_tt, p_mtt;
char phone[] = {"716233831"};
long int   phone_nmber = 0;
float capacity = 100;
uint32_t m_clb;

int user_pass = 0;
int admin_pass = 4;

//Define Pins
#define PMP PB11


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.end();
  //pmp
  pinMode(PMP, OUTPUT);
  digitalWrite(PMP, LOW);
  //keypad
  for (int r = 0; r < rows; r++) {
    pinMode(rowPins[r], INPUT_PULLUP); //rowpins as input
    // digitalWrite(rowPins[r],HIGH);

  }
  for (int c = 0; c < cols; c++) {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], HIGH);
  }
  //initialise LCD
  lcd.begin(COLUMNS, ROWS, LCD_5x8DOTS);
  //lcd.clear();
 // Wire.begin();
  /* prints static text */
  lcd.setCursor(0, 1);
  lcd.print(F("Hewbe Automation"));
  delay(2000);
  // lcd.clear();
  /* prints static text */
  // int out1 = out;
  if (get_Key() == '#') {
    int old_pass = 1234;
    writeData(disk1, 4, old_pass);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("New PASS:");
    lcd.setCursor(11, 0);
    lcd.print(old_pass);
    delay(3000);
  }

  lcd.clear();
  phone_nmber = (readLong(disk1, 140));
  //lcd.print(phone_nmber);
  //writeData(disk1, 4, 1234);

  //delay(5000);
  // read data addresses
  mqty = readData(disk1, 20);
  m_std = readData(disk1, 60);
  m_rmng = readData(disk1, 30);
  m_clb = readData(disk1, 70);
  m_tt = readData(disk1, 90);

}

void loop() {
  // mqty = 50.00;
  //m_clb = 15000;
  //m_std = 65.00;
  lcd.clear();
  /* prints static text */
  //lcd.print(F("main program"));
  topMenu();
}

void topMenu() {

  //update variables
  //Serial.println("Price per L:");
  lcd.clear();
  lcd.begin(COLUMNS, ROWS, LCD_5x8DOTS);
  lcd.setCursor(0, 0);
  lcd.print("Price per L:");
  lcd.setCursor(8, 1);
  lcd.print(m_std);
  delay(2000);

  while (1) {
    text = ">>Hewbe ATM<<";
    screen = 1;
    x = 1;
    y = 1;
    text_width = 3;
    lcd_column = 1;
    lcd_row = 1;
    Length = 3;

    itemSelected = displayMenu(menu, Length);

    switch (itemSelected) {

      case 0: {
          //sell code
          sell();
          break;

        }
      case 1: {
          //user submen
          sub0();
          break;

        }

      case 2: {
          sub1();
          break;

        }

      case 3: {

          sub2();
          break;

        }


    }



    //Serial.println(itemSelected);

  }
}
void sub0() {
  //Length = 4;
  text = ">>User Menu<<";
  screen = 3;
  x = 1;
  y = 1;
  text_width = 3;
  lcd_column = 1;
  lcd_row = 1;
  Length = 4;
  bool user = true;

  while (user) {
    text = ">>User Menu<<";
    
    center_text(text);
    itemSelected = displayMenu(subMenu0, Length);
    switch (itemSelected) {
      case 0: {
          loadProduct();
          break;

        }

      case 1: {
          calib();
          break;
        }
      case 2: {
          reserve();
          break;
        }
      case 3: {

          clean();
          break;

        }

    }
    //user = false;
  }

}


void sub1() {
  Length = 4;
  text = ">>Admin Menu<<";
  screen = 3;


 // while (!passwrd(admin_pass));

  x = 1;
  y = 1;
  text_width = 3;
  lcd_column = 1;
  lcd_row = 1;
  bool admin = true;
  while (admin) {
    text = ">>Admin Menu<<";
    itemSelected = displayMenu(subMenu1, Length);

    switch (itemSelected) {
      case 0: {

          setPrice();
          break;

        }
      case 1: {
          sales();
          break;
        }

      case 2: {

          pass(admin_pass);
          break;

        }

      case 3: {
          Reset();
          break;
        }

    }
  }

}
void sub2() {
  Length = 4;
  //  subMenuSelected = displayMenu(subMenu2, Length);
  lcd.noCursor();
  switch (subMenuSelected) {
    case 0: {
        pump();
        break;

      }

    case 1: {
        phone_number();
        break;
      }
    case 2: {
        pass(admin_pass);
        break;
      }
    case 3: {

        Reset();
        break;

      }

  }



}

int displayMenu(String menuInput[], int menuLength) {
  bool display = true;
  char valKey = 0;
  lcd.begin(COLUMNS, ROWS, LCD_5x8DOTS);

  while (display) {
    for (int i = 0; i < menuLength; i++) {
      lcd.clear();
      
      center_text(text);
      int x = lcd_column;
      int y = lcd_row;

      int state = i;
      menu_Pointer = state;
      itemSelected = i;

      //Arrow
      lcd.setCursor(0, 1);
      lcd.print((char) 126);

      //y list
      //  int yl = 1;
      //print on screen
      //  for (int j = 0; j < 1; j++) {
      String k = menuInput[i];
      //Serial.println(k);
      lcd.setCursor(x, y);
      lcd.print(k);

      // listing
      //     if (j > 0) {
      //         lcd.setCursor(0, yl);
      //   lcd.print((char) 165);

      //   x++;
      //   yl++;
      // }
      //  y++;

      /// if (i + 2 > menuLength) {
      // j = 3;
      //   }
      //  i++;

      //  }
      //done printing
      bool input = true;
      uint32_t instant_t0 = millis();
      //input
      while (input) {
        valKey = get_Key();
        if (millis() - instant_t0 > 10000) {
          input = false;
          display = false;
          topMenu();
        }

        switch (valKey) {
          case 'E': {
              input = false;
              display = false;
              state = i;
              i = 5;
              break;

            }

          case 'B': {
              if (state > 0) {
                input = false;
                state = 0;
                state--;
              }
              else {
                topMenu();

              }
              break;

            }
          case 'U': {
              if (state - 1 < 0) {
                state = menuLength - 2;
              }
              else {
                state = state - 2;
              }
              input = false;
              break;
            }
          case 'D': {
              input = false;
              break;
            }

        }
      }


      // i = state;
      Serial.println(i);
      Serial.println("\n");
      Serial.println("\n");
      //delay(5000);

      //keypad


    }

  }
  lcd.setCursor(15, 1);
  //lcd.print(menu_Pointer);
  //delay(2000);
  return menu_Pointer;
}


// input
//keypress function
char get_Key() {
  char k = 0;
  bool button = true;

  for (int c = 0; c < cols; c++) {
    digitalWrite(colPins[c], LOW);

    for (int r = 0; r < rows; r++) {
      int p = digitalRead(rowPins[r]);
      if (p == LOW) {

        while (button) {

          k = keys[r][c];
          button = false;

        }
      }

    }
    digitalWrite(colPins[c], HIGH);

  }
  //Serial.println("escaping:");
  delay(300);
  return k;

}
//Handle multiple matrix keypad input

int myInput(int c, int r, int width) {

  char keyVal = 0;
  char buff[width];

  //memset(buff; /0; width);
  int i = 0;
  bool inputState = true;
  //width = width-1;
  center_text(text);
  lcd.setCursor(c, r);
  //null terminator
  for (int i = 0; i < width + 1; i++ ) {
    buff[i] = 0;
  }
  while (inputState) {
    keyVal = get_Key();
    //lcd.clear();
    if (c + i < c + width) {
      lcd.setCursor(c + i, r);
      lcd.cursor();
    }
    else {
      lcd.setCursor(c + width, r);
      lcd.cursor();
    }

    switch (keyVal) {
      case 'E':
        {
          Serial.println("Enter");
          inputState = false;
          break;
        }

      case 'B':
        {
          for (int y = 0; y < width; y++) {
            buff[y] = 0;
          }
          Serial.println("Back");
          inputState = false;
          break;

        }

      case 'L':
        {
          Serial.println("LEFT");
          if (i > 0) {
            i--;
            //  i--;
            //  c--;




          }
          break;
        }

      case 'R':
        {
          Serial.println("RIGHT");
          if (i > 0 ) {
            i++;
            //c++;

          }
          break;
        }

      default:
        {
          if (keyVal >= '0' && keyVal <= '9') {
            if (i < width) {
              //setCursor
              buff[i] = +keyVal;
              //Serial.println(n);
              i++;
              //display here
              Serial.println(buff);
              lcd.setCursor(c, r);
              lcd.print("                 ");
              lcd.setCursor(c, r);

              lcd.print(buff);


              //Serial.println("in myInput");
              //c++;
              delay(200);

            }
            else {

              i--;
              //setCursor
              buff[i] = 0;
              buff[i] = +keyVal;
              i++;

              //display here
              lcd.setCursor(c, r);
              lcd.print("                 ");
              lcd.setCursor(c, r);
              Serial.println(buff);
              lcd.print(buff);
              //c++;
              //Serial.println("in myInput");
              delay(100);



            }

          }
          break;
        }




    }
  }
  int nmber = atoi(buff);
  lcd.noCursor();
  return nmber;
}



//FUNCTIONS

void sell() {
  lcd.clear();
  int text_width = 3;
  int lcd_column = 13;
  int lcd_row = 1;
  long result = 0;
  lcd.begin(COLUMNS, ROWS, LCD_5x8DOTS);
  text = ">>SELL:<<";
  center_text(text);
  lcd.setCursor(0, 1);
  lcd.print("Enter Amount:");
  bool buying = true;
  m_clb = readData(disk1, 70);

  uint32_t instant_t0 = 0;
  uint32_t instant_t1 = 0;


  while (buying) {
    //enter amount
    result = myInput( lcd_column, lcd_row, text_width);
    lcd.clear();
    center_text(text);
    if (result > 0) {
      //convert to float
      bmlk = (float)result;
      //convert to ltrs
      ltrs = bmlk / m_std;

      if (mqty > ltrs) {
        //sell
        instant_t0 = millis();
        m_drn = ltrs * (float)m_clb;

        //PMP on;
        digitalWrite(PMP, HIGH);
        lcd.setCursor(COLUMNS - 2, ROWS);
        lcd.print("OK");
        while (millis() - instant_t0 < (uint32_t)m_drn) {
          //display
          if (millis() - instant_t1 > (uint32_t)3) {
            instant_t1 = millis();
            lcd.setCursor(2, 1);
            r_dn = (float)(millis() - instant_t0) / m_clb;
            // lcd.print(r_dn);
            //lcd.setCursor(8, 1);
            //lcd.print(" L");
            r_dn = r_dn * m_std;
            lcd.setCursor(0, 1);
            lcd.print(r_dn);
            lcd.setCursor(8, 1);
            lcd.print("$$");
          }
          if (get_Key() == '#') {
            uint32_t pause_inst1 = millis();
            digitalWrite(PMP, LOW);
            lcd.setCursor(COLUMNS - 2, ROWS);
            lcd.noCursor();
            lcd.print("P!");
            while (get_Key() != '#');
            uint32_t pause_inst2 = millis();
            digitalWrite(PMP, HIGH);
            lcd.setCursor(COLUMNS - 2, ROWS);
            lcd.print("OK");
            //lcd.cursor();
            instant_t0 += (pause_inst2 - pause_inst1);

            //lcd.begin(lcd_columns, lcd_rows);

          }

          if (get_Key() == '*') {

            digitalWrite(PMP, LOW);
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print(F("Cancelled: "));
            m_drn = 0;
            lcd.setCursor(2, 1);
            lcd.print(r_dn);
            lcd.setCursor(8, 1);
            lcd.print("$$");
            bmlk = r_dn;
            delay(3000);

          }

        }


        //pump off
        //save data
        digitalWrite(PMP, LOW);


        //-----------LCD prob.------------
        delay(50);
        lcd.begin(COLUMNS, ROWS);

        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print(F("Thank you"));
        lcd.setCursor(2, 1);
        lcd.print(bmlk);
        lcd.setCursor(10, 1);
        lcd.print(F("$$"));

        mqty = mqty - ltrs;
        m_rmng = mqty;
        m_tt = m_tt + bmlk;
        //save data
        writeData(disk1, 30, m_rmng);
        writeData(disk1, 20, mqty);
        writeData(disk1, 90, m_tt);


        buying = false;
        delay(2000);
      }
      else {
        //low on stock
        buying = false;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Low Level!!");
        lcd.setCursor(2, 1);
        lcd.print("Replenish..");
        delay(2000);

      }


    }
    else {
      //invalid input
      buying = false;
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.print("invalid Input");
      delay(2000);
    }
  }

}


//load product
void loadProduct() {
  lcd.clear();
  int text_width = 3;
  int lcd_column = 3;
  int lcd_row = 1;
  long result = 0;
  text = ">>Load Product<<";
  center_text(text);
  lcd.setCursor(0, 0);
  lcd.print("Enter Amount:");
  result = myInput( lcd_column, lcd_row, text_width);

  //display and save
  if (result > 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Product loaded: ");
    lcd.setCursor(3, 1);
    lcd.print(result);
    lcd.setCursor(8, 1);
    lcd.print(F("Ltrs"));
    mqty = mqty + (float)result;
    m_rmng = mqty;
    writeData(disk1, 30, m_rmng);
    writeData(disk1, 20, mqty);
    delay(2000);

    //save data
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Invalid Input: ");

  }



}

//calib
void calib() {
  // uint32_t instant_t6 = 0;
  uint32_t instant_t0 = 0;
  uint32_t instant_t1 = 0;
  m_clb = readData(disk1, 70);
  lcd.clear();
  text = ">>CALIBRATE:<<";
  center_text(text);
  lcd.setCursor(0, 1);
  bool calibrating = true;
  lcd.print(m_clb);
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Press * to start"));
  lcd.noCursor();
  while (calibrating) {


    switch (get_Key()) {
      case '*': {
          instant_t0 = millis();
          digitalWrite(PMP, HIGH);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("Calibrating..."));
          // lcd.setCursor(1, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(F("Press # to stop.."));
          break;
        }

      case '#': {
          //stop m_drning product
          instant_t1 = millis();

          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print(F("Done: "));
          lcd.setCursor(8, 1);

          calibrating = false;
          m_clb = instant_t1 - instant_t0;
          lcd.print(m_clb);
          writeData(disk1, 70, m_clb);


          digitalWrite(PMP, LOW);


          delay(2000);
          lcd.begin(COLUMNS, ROWS, LCD_5x8DOTS);
          lcd.clear();
          /*  if (!eeprom.eeprom_write(70, m_clb)) {
              Serial.println("Failed to store FLOAT");
            }*/


          break;
        }
      case 4:
        return;
        break;
    }
  }


}
//reserve
void reserve() {
  //Get data from EEPROM
  mqty = readData(disk1, 20);

  lcd.clear();
  lcd.noCursor();
  text = ">>RESERVE<<";
  center_text(text);
  lcd.setCursor(6, 1);
  lcd.print(mqty);
  lcd.setCursor(12 , 1);
  lcd.print("L");

  delay(4000);


}

//clean
void clean() {

  // uint32_t instant_t0 = 0;
  uint32_t instant_t1 = millis();
  bool cleaning = true;
  lcd.clear();
  //center_text("Clean");
  lcd.clear();
  text = ">>CLEAN:<<";
  center_text(text);
  lcd.setCursor(1, 1);
  lcd.print(F("Press * to start"));

  while (cleaning) {

    if (millis() - instant_t1 >= (uint32_t)60000) {
      lcd.clear();

      lcd.begin(COLUMNS, ROWS);
      lcd.setCursor(1, 1);
      lcd.print(F("Ceaning DONE!"));
      digitalWrite(PMP, LOW);

      cleaning = false;
    }


    switch (get_Key()) {
      case '*': {
          digitalWrite(PMP, HIGH);
          //-----------LCD prob.------------
          //delay(250);
          //lcd.begin(COLUMNS, ROWS);
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print(F("Cleaning..."));
          //lcd.setCursor(1, 2);
          //lcd.print(F("Press # to stop.. "));
          instant_t1 = millis();
          break;
        }
      case '#': {
          //stop cleaning product
          digitalWrite(PMP, LOW);
          //-----------LCD prob.------------
          // delay(250);

          lcd.begin(COLUMNS, ROWS);
          lcd.setCursor(1, 1);
          lcd.print(F("DONE!"));

          cleaning = false;
          break;
        }
      case 3:
        return;
        break;
    }

  }
}

//test pump
void pump() {

  // uint32_t instant_t0 = 0;
  uint32_t instant_t1 = millis();
  bool cleaning = true;
  lcd.clear();
  //center_text("Clean");
  lcd.clear();
  text = ">pump test:<";
  center_text(text);
  lcd.setCursor(1, 1);
  lcd.print(F("Press * to start"));

  while (cleaning) {

    switch (get_Key()) {
      case '*': {
          digitalWrite(PMP, HIGH);
          //-----------LCD prob.------------
          //delay(250);
          //lcd.begin(COLUMNS, ROWS);
          lcd.setCursor(1, 1);
          lcd.print(F("pump run..."));
          lcd.setCursor(1, 2);
          lcd.print(F("Press # to stop.. "));

          break;
        }
      case '#': {
          //stop cleaning product
          digitalWrite(PMP, LOW);
          //-----------LCD prob.------------
          // delay(250);
          lcd.begin(COLUMNS, ROWS);
          lcd.setCursor(1, 2);
          lcd.print(F("DONE!"));

          cleaning = false;
          break;
        }
      case 3:
        return;
        break;
    }

  }
}
//set price
void setPrice() {
  int text_width = 3;
  int lcd_column = 13;
  int lcd_row = 1;
  long result = 0;
  lcd.clear();
  text = ">>Set Price<<";
  // center_text(text);

  lcd.setCursor(0, 1);
  lcd.print("Enter Price:");
  result = myInput( lcd_column, lcd_row, text_width);
  //display and save
  if (result > 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unit Price:");
    lcd.setCursor(13, 0);
    lcd.print(result);
    m_std = (float)result;


    //save data
    writeData(disk1, 60, m_std);


    delay(3000);
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Invalid Input: ");
    delay(1000);

  }


  //display and save
}

//sales

void sales() {
  //Total sales
  // EEPROM.get(90, m_tt);
  m_tt = readData(disk1, 90);
  mqty = readData(disk1, 20);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("sales:");
  lcd.setCursor(10, 0);
  lcd.print(m_tt);
  lcd.setCursor(1, 1);
  lcd.print("Stock:");
  lcd.setCursor(10, 1);
  lcd.print(mqty);
  delay(4000);
}

//number
void phone_number() {
  int text_width = 10;
  int lcd_column = 2;
  int lcd_row = 3;
  long result = 0;
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("Enter number: ");
  result = myInput(lcd_column, lcd_row,  text_width);

  //display and save
  if (result > 0) {
    lcd.clear();
    lcd.setCursor(1, 1);
    lcd.print("Enter number: ");
    lcd.setCursor(2, 3);
    lcd.print(result);
    //convert to char and save data
    char cstr[12];
    itoa(result, cstr, 10);

    //save data
    writeLong(disk1, 140, cstr);
    delay(5000);
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Invalid Input: ");

  }
  //save

}
bool passwrd(int pass_addr) {
  int pass = readData(disk1, pass_addr);

  lcd.clear();
  int text_width = 4;
  int lcd_column = 11;
  int lcd_row = 1;
  long result = 0;
  lcd.setCursor(0, 1);
  lcd.print("Enter pass:");

  result = passInput( lcd_column, lcd_row, text_width);

  if (result == (long)pass) {
    return true;
  }
  else {

    return false;
  }

}
//pass
void pass(int pass_addr) {
  int old_pass = readData(disk1, pass_addr);
  int new_pass;
  //get pass

  int text_width = 4;
  int lcd_column = 5;
  int lcd_row = 3;
  int result = 0;
  lcd.clear();
  text = ">>password<<";
  center_text(text);
  lcd.setCursor(1, 2);
  lcd.print("Enter old pass:");
  result = passInput( lcd_column, lcd_row, text_width);
  //compare
  if (result == old_pass) {
    //enter newpass
    result = 0;
    lcd.clear();
    center_text(text);
    lcd.setCursor(1, 2);
    lcd.print("Enter new pass:");
    result = myInput( lcd_column, lcd_row, text_width);
    new_pass = result;
    //enter new pass again
    result = 0;
    lcd.clear();
    center_text(text);
    lcd.setCursor(1, 2);
    lcd.print("Confirm pass:");
    result = myInput( lcd_column, lcd_row, text_width);

    if (result == new_pass) {
      //save new pass
      lcd.clear();
      center_text(text);
      lcd.setCursor(4, 2);
      lcd.print("Success!!");
      writeData(disk1, pass_addr, result);
      delay(2000);

    }
    else {
      //try again
      lcd.clear();
      center_text(text);
      lcd.setCursor(1, 2);
      lcd.print("TRY AGAIN:");
      delay(1000);


    }


  }
  else {
    lcd.clear();
    center_text(text);
    lcd.setCursor(1, 2);
    lcd.print("Wrong Pass!!");
    delay(2000);

  }
  //compare
}

//reset
void Reset() {
  lcd.clear();
  center_text("RESETTING..");
  //reset data to 0
  writeData(disk1, 20, 0.00);
  writeData(disk1, 30, 0.00);
  writeData(disk1, 60, 0.00);
  writeData(disk1, 70, 0.00);
  writeData(disk1, 90, 0.00);
  lcd.setCursor(0, 1);
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print(F("RESET COMPLETE"));
  delay(2000);
}


//Heading display
void center_text(String dis) {

  //int disp= atoi(dis.c_str());
  int string_length = dis.length();

  int x = (COLUMNS - string_length) / 2;
  lcd.setCursor(x, 0);
  lcd.print(dis);
}


int passInput(int c, int r, int width) {

  char keyVal = 0;
  char buff[width];
  char dbuff[width];

  //memset(buff; /0; width);
  int i = 0;
  bool inputState = true;
  //width = width-1;
  center_text(text);
  lcd.setCursor(c, r);
  //null terminator
  for (int i = 0; i < width + 1; i++ ) {
    buff[i] = 0;
    dbuff[i] = 0;
  }
  while (inputState) {
    keyVal = get_Key();
    //lcd.clear();
    if (c + i < c + width) {
      lcd.setCursor(c + i, r);
      lcd.cursor();
    }
    else {
      lcd.setCursor(c + width, r);
      lcd.cursor();
    }

    switch (keyVal) {
      case 'E':
        {
          Serial.println("Enter");
          inputState = false;
          break;
        }

      case 'B':
        {
          for (int y = 0; y < width; y++) {
            buff[y] = 0;
            dbuff[y] = 0;
          }
          Serial.println("Back");
          inputState = false;
          break;

        }

      case 'L':
        {
          Serial.println("LEFT");
          if (i > 0) {
            i--;
            //  i--;
            //  c--;




          }
          break;
        }

      case 'R':
        {
          Serial.println("RIGHT");
          if (i > 0 ) {
            i++;
            //c++;

          }
          break;
        }

      default:
        {
          if (keyVal >= '0' && keyVal <= '9') {
            if (i < width) {
              //setCursor
              buff[i] = +keyVal;
              dbuff[i] = +'*';
              //Serial.println(n);
              i++;
              //display here
              Serial.println(buff);
              lcd.setCursor(c, r);
              lcd.print("                 ");
              lcd.setCursor(c, r);

              lcd.print(dbuff);


              //Serial.println("in myInput");
              //c++;
              delay(200);

            }
            else {

              i--;
              //setCursor
              buff[i] = 0;
              dbuff[i] = 0;
              buff[i] = +keyVal;
              dbuff[i] = +'*';
              i++;

              //display here
              lcd.setCursor(c, r);
              lcd.print("                 ");
              lcd.setCursor(c, r);
              //Serial.println(buff);
              lcd.print(dbuff);
              //c++;
              //Serial.println("in myInput");
              delay(200);



            }

          }
          break;
        }




    }
  }
  int nmber = atoi(buff);
  lcd.noCursor();
  return nmber;
}

//External EEProm functions


byte readEEPROM(int deviceaddress, unsigned int eeaddress )
{
  byte rdata = 0xFF;

  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(deviceaddress, 1);

  if (Wire.available()) rdata = Wire.read();

  return rdata;
}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data )
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB

  Wire.write(data);
  Wire.endTransmission();

  delay(5);
}

float readData(int devAddr, unsigned int eeaddress) {
  //read
  char chrFloat[] = "";
  char newStr[20];
  for (byte i = 0; i <= 10; i++) {
    newStr[i] = readEEPROM(disk1, eeaddress + i);
    newStr[i + 1] = 0;
    //convert to float

  }
  float out = atof(newStr);
  return out;

}

void writeData(int deviceaddress, unsigned int eeaddress, float data) {
  float t = data;
  char chrFloat[] = "";
  char chBuff[10];
  dtostrf(t, 6, 4, chBuff);
  strcat(chrFloat, chBuff);

  //write
  for (byte i = 0; i < 10; i++) {
    writeEEPROM(disk1, eeaddress + i, chrFloat[i]);


  }
}

void writeLong(int deviceaddress, unsigned int eeaddress, char* data) {
  ///char t = data;
  // char chrFloat[] = "";
  // char chBuff[10];
  //dtostrf(t, 6, 4, chBuff);
  // strcat(chrFloat, chBuff);

  //write
  for (byte i = 0; i < 10; i++) {
    writeEEPROM(disk1, eeaddress + i, data[i]);


  }
}

long int readLong(int devAddr, unsigned int eeaddress) {
  //read
  char chrFloat[] = "";
  char newStr[20];
  for (byte i = 0; i <= 10; i++) {
    newStr[i] = readEEPROM(disk1, eeaddress + i);
    newStr[i + 1] = 0;
    //convert to float

  }
  long int out = atoi(newStr);
  return out;

}
