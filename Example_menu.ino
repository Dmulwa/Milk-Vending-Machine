/*
   Buda ndio namaliza kuandika hii menu. ni mwenda sana
   I have commented where the functions are to be inserted
   we will remain with GSM,SD card and Password functionalities
   angalia hapo chini venye ni efficient
   saa hii ni 1 nimeenda kula btw na niwatch episode moja ya Simpsons, halafu huyu dem ameshadoz
   BTW hii kitu inacompile so usicomplain though nimeichora nikiwa tipsy..uvivu maze
   Halafu kesho natokea late...make up something
   ...AM ARA HIA
*/

// Menu texts
const char top_menu_item00[] PROGMEM = " Sell:";
const char top_menu_item01[] PROGMEM = " User:";
const char top_menu_item02[] PROGMEM = " Admin:";
//const char top_menu_item03[] PROGMEM ="Show credit";

PGM_P const top_menu_items[] PROGMEM = {top_menu_item00, top_menu_item01, top_menu_item02};

const char sub_menu_1_item00[] PROGMEM = " Cash AMOUNT: ";

PGM_P const sub_menu_1_items[]PROGMEM = {sub_menu_1_item00};

//const char sub_menu_2_item00[] PROGMEM ="Temperature";
const char sub_menu_2_item00[] PROGMEM = " Calibrate";
const char sub_menu_2_item01[] PROGMEM = " Load product";
const char sub_menu_2_item02[] PROGMEM = " Reserve";
const char sub_menu_2_item03[] PROGMEM = " CLEAN";

PGM_P const sub_menu_2_items[]PROGMEM = {sub_menu_2_item00, sub_menu_2_item01, sub_menu_2_item02, sub_menu_2_item03};

//const char sub_menu_3_item00[] PROGMEM = " Calibrate";
//const char sub_menu_3_item01[] PROGMEM = " Load Product";
const char sub_menu_3_item00[] PROGMEM = " Set Unit Price";
const char sub_menu_3_item01[] PROGMEM = " Total Sales";
const char sub_menu_3_item02[] PROGMEM = " Reserve";
const char sub_menu_3_item03[] PROGMEM = " Enter number1";
const char sub_menu_3_item04[] PROGMEM = " Enter number2";
const char sub_menu_3_item05[] PROGMEM = " Change Password";
const char sub_menu_3_item06[] PROGMEM = " RESET";

PGM_P const sub_menu_3_items[]PROGMEM = {sub_menu_3_item00, sub_menu_3_item01, sub_menu_3_item02, sub_menu_3_item03, sub_menu_3_item04, sub_menu_3_item05, sub_menu_3_item06};


//This program is the main menu. It handles inputs from the keys, updates the menu or executes a certain menu function accordingly.
int global_style = 109; // This is the style of the menu


void top_menu() {
  EEPROM.get(60, m_std);

  //-----------LCD init-----------------

  lcd.begin(lcd_columns, lcd_rows);
  lcd.clear();
  lcd.print(F("Price Per L:"));
  lcd.setCursor(2, 1);
  lcd.print(m_std);
  query_sms();
  instant_t7 = millis();
  while (millis()  - instant_t7 < 1000);
  uint32_t instant_t1, instant_t0;
  bool cleaning;
  bool calibrating;
  int menu_pointer_1 = 0; // This stores the menu choice the user made.
  phi_prompt_struct myMenu; // This structure stores the main menu.
  //  Serial.println(myMenu);

  // Initialize the top menu
  myMenu.ptr.list = (char**)&top_menu_items; // Assign the list to the pointer
  myMenu.low.i = 0; // Default item highlighted on the list
  myMenu.high.i = 2; // Last item of the list is size of the list - 1.
  myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.
  myMenu.step.c_arr[0] = lcd_rows - 1; // rows to auto fit entire screen
  myMenu.step.c_arr[1] = 1; // one col list
  myMenu.step.c_arr[2] = 0; // y for additional feature such as an index
  myMenu.step.c_arr[3] = lcd_columns - 4 - ((global_style & phi_prompt_index_list) != 0); // x for additional feature such as an index
  myMenu.col = 0; // Display menu at column 0
  myMenu.row = 1; // Display menu at row 1
  myMenu.option = global_style ; // Option 0, display classic list, option 1, display 2X2 list, option 2, display list with index, option 3, display list with index2.
    //Enter Paswword for User:
   /*if(!passwrd(user_pass)) {
    return;
    }*/ 
  while (1) { // This loops every time a menu item is selected.

    lcd.clear();  // Refresh menu if a button has been pushed

    //lock_check();
    center_text(" Hewbe ATM:");
    myMenu.option = global_style; // This makes sure the change of menu style is updated if there is a menu style option in this level of menu.
    myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // This makes sure the change of menu style is updated if there is a menu style option in this level of menu. Auto fit the size of the list to the screen. Length in characters of the longest list item.
    myMenu.step.c_arr[3] = lcd_columns - 4 - ((global_style & phi_prompt_index_list) != 0); // This makes sure the change of menu style is updated if there is a menu style option in this level of menu. x for additional feature such as an index
    if (select_list(&myMenu) == -1) { // Use the select_list to ask the user to select an item of the list, that is a menu item from your menu.
      return;
    }

    menu_pointer_1 = myMenu.low.i; // Get the selected item number and store it in the menu pointer.

    switch (menu_pointer_1) {// See which menu item is selected and execute that correspond function
      case 0:
        //buy milk
        sub_menu_1();
        break;

      case 1:
        sub_menu_2();// user sub menu's
        break;

      case 2:
        sub_menu_3();// Admin sub menu's
        break;
      /* case 3:
        show_credit();// If this were a sub level menu, this last one should return to previous level by using "return".
        delay(200);
        break; */

      default:
        break;
    }

  }
}

// Menu functions go here. The functions are called when their menu items are selected. They are called only once so if you want to do something repeatedly, make sure you have a while loop.
void sub_menu_1() { // This is the sub_menu_1
  bool buying = true;
  //lock_check();
  int menu_pointer_1 = 0;
  long result = 0;

  uint32_t instant_t10 = 0;
  uint32_t instant_t11 = 0;
  uint32_t instant_t6 = 0;

  phi_prompt_struct myTextInput;
  char nmuber_buffer[] = "            "; // This is the buffer that will store the content of the text panel.
  myTextInput.ptr.msg = nmuber_buffer; // Assign the text buffer address
  myTextInput.width = 4; // Length of the input panel is 12 characters.
  myTextInput.col = 2; // Display input panel at column 2
  myTextInput.row = 1; // Display input panel at row 1



  while (buying) {
    wdt_reset();

    //query_sms();
    //send_report();

    //-----------LCD init-----------------
    lcd.begin(lcd_columns, lcd_rows);
    lcd.clear();  // Refresh menu if a button has been pushed
    center_text(" Sell: ");

    /* myMenu.option=global_style; // This makes sure the change of menu style is updated if there is a menu style option in this level of menu.
      myMenu.width=lcd_columns-((global_style&phi_prompt_arrow_dot)!=0)-((global_style&phi_prompt_scroll_bar)!=0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.

      select_list(&myMenu);
      menu_pointer_1=myMenu.low.i;
      switch (menu_pointer_1)  {
      case 0: { */
    //sale in cash

    lcd.clear();
    center_text("Enter Amount:");
    //phi_prompt_struct myTextInput; // This is the structure you will use to invoke library function input_panel.


    if (input_number(&myTextInput) != -1) { // input_panel stores user choice in file_name.

      lcd.clear();
      lcd.print("AMT:"); // Display the user choice one more time
      lcd.setCursor(0, 1); // Display the list at column 2 row 1.
      if (sscanf(nmuber_buffer, "%ld", &result) == 1) {
        lcd.print(result); ///< Turn the text entered into integer number. If a valid input is acquired, sscanf returns 1.
        bmlk = (float)result;
        ltrs = bmlk / m_std;
      }
      if (mqty > ltrs) {
        //switch on pump
        m_drn = ltrs * (float)m_clb;

        instant_t10 = millis();
        digitalWrite(PMP, HIGH);  //switch on pump
        //-----------LCD prob.------------
        //delay(30);
        //lcd.begin(lcd_columns, lcd_rows);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F(" VENDING..."));

        while (millis() - instant_t10 <= (uint32_t)m_drn) {
          if (millis() - instant_t6 >= (uint32_t)2000) {
            instant_t6 = millis();
            wdt_reset();
          }

          if (millis() - instant_t11 > (uint32_t)100) {
            instant_t11 = millis();
            lcd.setCursor(1, 1);
            r_dn = (float)(millis() - instant_t10) / m_clb;
            lcd.print(r_dn);
            lcd.setCursor(5, 1);
            lcd.print(" L");
            r_dn = r_dn * m_std;
            lcd.setCursor(1, 2);
            lcd.print(r_dn);
            lcd.setCursor(5, 2);
            lcd.print("Units");
          }
          if (my_btns.getKey() == 8) {
            uint32_t pause_inst1 = millis();
            digitalWrite(PMP, LOW);
            while (my_btns.getKey() != 8);
            uint32_t pause_inst2 = millis();
            digitalWrite(PMP, HIGH);
            instant_t10 += (pause_inst2 - pause_inst1);

            //lcd.begin(lcd_columns, lcd_rows);

          }

          if (my_btns.getKey() == 7) {

            digitalWrite(PMP, LOW);
            buying = false;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(F("  Cancelled: "));
            
          }
        }
        digitalWrite(PMP, LOW);
        //-----------LCD prob.------------
        instant_t7 = millis();
        while (millis()  - instant_t7 < 50);;
        lcd.begin(lcd_columns, lcd_rows);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Thank you"));
        lcd.setCursor(0, 1);
        lcd.print(bmlk);
        lcd.setCursor(10, 1);
        lcd.print(F("Units"));

        mqty = mqty - ltrs;
        m_rmng = mqty;
        EEPROM.put(30, m_rmng);
        EEPROM.put(20, mqty);
        m_tt = m_tt + bmlk;
        EEPROM.put(90, m_tt);

        instant_t7 = millis();
        while (millis()  - instant_t7 < 2000);
        return;
      }
      else if (mqty <= ltrs) {
        lcd.clear();
        lcd.setCursor(1, 2);
        lcd.print(F("Load Product.."));
        lcd.setCursor(1, 3);
        lcd.print(mqty);
        lcd.setCursor(6, 3);
        lcd.print(F("Remaining"));
        wait_on_escape(4000);
      }
      else {
        lcd.print(F("Invalid input!"));
        wait_on_escape(1000);
      }
    }
    else {
      lcd.clear();
      lcd.print(F("You pressed ESC"));
      buying = false;
      return;
    }
    //break;

    /* case 1:// returns to previous menu level.
      return;
      break;

      default:
      break;
      }

      }*/
  }
}


void sub_menu_2() { // This is the sub_menu_2
  int menu_pointer_2 = 0;
  phi_prompt_struct myMenu; // This structure stores the sub menu.

  //  lock_check();
  //Enter Paswword for User:
  /* if(!passwrd(user_pass)) {
    return;
    } */

  // Initialize the top menu
  myMenu.ptr.list = (char**)&sub_menu_2_items; // Assign the list to the pointer
  myMenu.low.i = 0; // Default item highlighted on the list
  myMenu.high.i = 3; // Last item of the list is size of the list - 1.
  myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.
  myMenu.step.c_arr[0] = lcd_rows - 1; // rows to auto fit entire screen
  myMenu.step.c_arr[1] = 1; // one col list
  myMenu.step.c_arr[2] = 0; // y for additional feature such as an index
  myMenu.step.c_arr[3] = lcd_columns - 4; // x for additional feature such as an index
  myMenu.col = 0; // Display prompt at column 0
  myMenu.row = 1; // Display prompt at row 1
  myMenu.option = global_style;


  while (1) {
    //query_sms();
//    lock_check();
    //-----------LCD init-----------------
    lcd.clear();
    lcd.begin(lcd_columns, lcd_rows);
    lcd.clear();  // Refresh menu if a button has been pushed
    center_text("User Menu:");
    myMenu.option = global_style; // This makes sure the change of menu style is updated if there is a menu style option in this level of menu.
    myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.

    if (select_list(&myMenu) == -1) {
      return;
    }

    menu_pointer_2 = myMenu.low.i;
    switch (menu_pointer_2) {
      case 0: {
          //calibrate
          calib();
          break;

        }
      case 1: {
          //Load product;
          char _buffer[55];
          char mqty_str[10];

          EEPROM.get(20, mqty);

          lcd.clear();
          center_text("Load Product");
          long result = 0;
          phi_prompt_struct myTextInput; // This is the structure you will use to invoke library function input_panel.

          char nmuber_buffer[] = "            "; // This is the buffer that will store the content of the text panel.
          myTextInput.ptr.msg = nmuber_buffer; // Assign the text buffer address
          myTextInput.width = 3; // Length of the input panel is 12 characters.
          myTextInput.col = 2; // Display input panel at column 2
          myTextInput.row = 1; // Display input panel at row 1
          lcd.print(F("Enter Amount:")); // Prompt user for input

          if (input_number(&myTextInput) != -1) { // input_panel stores user choice in file_name.

            if (sscanf(nmuber_buffer, "%ld", &result) == 1) { ///< Turn the text entered into integer number. If a valid input is acquired, sscanf returns 1.
              lcd.clear();
              lcd.setCursor(0, 1);
              lcd.print("Product loaded: ");
              lcd.setCursor(3, 2);
              lcd.print(result);
              lcd.setCursor(8, 2);
              lcd.print(F("Ltrs"));
              mqty = mqty + (float)result;
              m_rmng = mqty;
              EEPROM.put(20, mqty);
              EEPROM.put(30, m_rmng);
              sprintf(_buffer, " Product Loaded: %d L \r\nTotal Qty of Product: ",  result);
              strcat(_buffer, dtostrf(mqty, -6, 2, mqty_str));
              strcat(_buffer, " L");
              send_sms(_buffer);
            }
            else {
              lcd.print(F("Invalid input!"));
            }

          }
          else {
            lcd.clear();
            lcd.print(F("You pressed ESC"));
            break;
          }
          instant_t7 = millis();
          while (millis()  - instant_t7 < 2000);
          lcd.begin(lcd_columns, lcd_rows);
          break;
        }

      case 2: {
          //reserve
          reserve();
          break;
        }
      case 3: {
          //clean
          clean();
          break;
        }

      /* case 6:{
        top_menu_setting(&myMenu); // This is the setting function that changes how menu is rendered
        break;} */

      case 4:// returns to previous menu level.
        return;
        break;

      default:
        break;
    }
  }
}

void sub_menu_3() {// This is the sub_menu_3
  //-----------LCD init-----------------
  lcd.begin(lcd_columns, lcd_rows);
  lcd.clear();
  int menu_pointer_3 = 0;
  phi_prompt_struct myMenu; // This structure stores the sub menu.
  //  lock_check();
  //Enter Paswword for Admin:
  /*if (!passwrd(admin_pass)) {
    return;
  }*/

  // Initialize the top menu
  myMenu.ptr.list = (char**)&sub_menu_3_items; // Assign the list to the pointer
  myMenu.low.i = 0; // Default item highlighted on the list
  myMenu.high.i = 6; // Last item of the list is size of the list - 1.
  myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.
  myMenu.step.c_arr[0] = lcd_rows - 1; // rows to auto fit entire screen
  myMenu.step.c_arr[1] = 1; // one col list
  myMenu.step.c_arr[2] = 0; // y for additional feature such as an index
  myMenu.step.c_arr[3] = lcd_columns - 4; // x for additional feature such as an index
  myMenu.col = 0; // Display prompt at column 0
  myMenu.row = 1; // Display prompt at row 1
  myMenu.option = global_style;

  while (1) {
    //query_sms();
    //send_report();

    //-----------LCD init-----------------
    lcd.begin(lcd_columns, lcd_rows);
    lcd.clear();  // Refresh menu if a button has been pushed
    center_text(" Admin: ");
    myMenu.option = global_style; // This makes sure the change of menu style is updated if there is a menu style option in this level of menu.
    myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.

    if (select_list(&myMenu) == -1) {
      return;
    }
    menu_pointer_3 = myMenu.low.i;

    switch (menu_pointer_3) {

      case 0: {
          //Set unit price

          lcd.clear();
          center_text("Set Unit Price");
          long result = 0;
          phi_prompt_struct myTextInput; // This is the structure you will use to invoke library function input_panel.

          char nmuber_buffer[] = "            "; // This is the buffer that will store the content of the text panel.
          myTextInput.ptr.msg = nmuber_buffer; // Assign the text buffer address
          myTextInput.width = 4; // Length of the input panel is 12 characters.
          myTextInput.col = 2; // Display input panel at column 2
          myTextInput.row = 1; // Display input panel at row 1

          lcd.print(F("Unit price: ")); // Prompt user for input

          if (input_number(&myTextInput) != -1) { // input_panel stores user choice in file_name.
            lcd.clear();
            lcd.print(F("Unit Price:")); // Display the user choice one more time
            lcd.setCursor(0, 1); // Display the list at column 2 row 1.
            if (sscanf(nmuber_buffer, "%ld", &result) == 1) { ///< Turn the text entered into integer number. If a valid input is acquired, sscanf returns 1.
              lcd.print(result);
              EEPROM.put(60, (float)result);

            }
            else {
              lcd.print(F("Invalid input!"));
            }
          }
          else {
            lcd.clear();
            lcd.print(F("You pressed ESC"));
          }

          wait_on_escape(2000);
          break;
        }

      case 1: {
          //Total sales
          EEPROM.get(90, m_tt);
          lcd.clear();
          center_text("Total sales:");
          lcd.setCursor(0, 1);
          lcd.print(m_tt);
          wait_on_escape(4000);
          break;
        }
      case 2:
        //Reserve
        reserve();
        break;

      case 3:  {
          //phone number

          long result = 0;
          phi_prompt_struct myTextInput; // This is the structure you will use to invoke library function input_panel.
          char nmuber_buffer[] = "            "; // This is the buffer that will store the content of the text panel.
          myTextInput.ptr.msg = nmuber_buffer; // Assign the text buffer address
          myTextInput.width = 10; // Length of the input panel is 12 characters.
          myTextInput.col = 2; // Display input panel at column 2
          myTextInput.row = 1; // Display input panel at row 1
          lcd.clear(); // Clear the lcd
          lcd.print(F("Enter number:")); // Prompt user for input

          if (input_number(&myTextInput) != -1) { // input_panel stores user choice in file_name.

            lcd.clear();
            lcd.print(F("You entered:")); // Display the user choice one more time
            lcd.setCursor(0, 1); // Display the list at column 2 row 1.
            if (sscanf(nmuber_buffer, "%ld", &result) == 1) { ///< Turn the text entered into integer number. If a valid input is acquired, sscanf returns 1.
              lcd.print(result);
              EEPROM.put(200, result);
              instant_t7 = millis();
              while (millis()  - instant_t7 < 2000);

              /* for(int k = 0; k <= sizeof(nmuber_buffer) ; ++k) {
                EEPROM.write(200 + k , nmuber_buffer[k]);
                } */
            }
            else lcd.print(F("invalid input!"));

          }
          else {
            lcd.clear();
            lcd.print(F("You pressed ESC"));
          }
          wait_on_escape(4000);
          break;
        }

      case 4:  {
          //phone number 2

          long result = 0;
          phi_prompt_struct myTextInput; // This is the structure you will use to invoke library function input_panel.
          char nmuber_buffer[] = "            "; // This is the buffer that will store the content of the text panel.
          myTextInput.ptr.msg = nmuber_buffer; // Assign the text buffer address
          myTextInput.width = 10; // Length of the input panel is 12 characters.
          myTextInput.col = 2; // Display input panel at column 2
          myTextInput.row = 1; // Display input panel at row 1
          lcd.clear(); // Clear the lcd
          lcd.print(F("Enter number:")); // Prompt user for input

          if (input_number(&myTextInput) != -1) { // input_panel stores user choice in file_name.

            lcd.clear();
            lcd.print(F("You entered:")); // Display the user choice one more time
            lcd.setCursor(0, 1); // Display the list at column 2 row 1.
            if (sscanf(nmuber_buffer, "%ld", &result) == 1) { ///< Turn the text entered into integer number. If a valid input is acquired, sscanf returns 1.
              lcd.print(result);
              //EEPROM.put(140, result);
              instant_t7 = millis();
              while (millis()  - instant_t7 < 3000);

              /* for(int k = 0; k <= sizeof(nmuber_buffer) ; ++k) {
                EEPROM.write(200 + k , nmuber_buffer[k]);
                } */
            }
            else lcd.print(F("invalid input!"));

          }
          else {
            lcd.clear();
            lcd.print(F("You pressed ESC"));
          }
          wait_on_escape(4000);
          break;
        }

      case 5:
        //change password
        ch_passwrd(admin_pass);

        break;


      case 6:
        //Reset EEPROM
        lcd.clear();
        center_text("RESETTING..");
        mqty = 0.0;
        m_rmng = 0.0;
        m_tt = 0.0;
        EEPROM.put(20, mqty);
        EEPROM.put(30, m_rmng);
        EEPROM.put(90, m_tt);
        lcd.setCursor(0, 1);
        lcd.print(F("RESET COMPLETE"));
        instant_t7 = millis();
        while (millis()  - instant_t7 < 3000);
        break;

      case 9:
        return; // This is the setting function that changes how menu is rendered
        break;

      /* case 10:// returns to previous menu level.
        return;
        break; */

      default:
        break;
    }
  }
}

/* //render setting function
  void top_menu_setting(phi_prompt_struct * para) {//Settings

  char buffer[32];
  byte opt=global_style, yn;

  lcd.clear();
  sprintf(buffer, "%s%d","Current style:", global_style);
  lcd.print(buffer);
  Serial.println(buffer);
  wait_on_escape(200);
  for (byte i=0;i<7;i++)
  {
    lcd.clear();
    strcpy_P(buffer,(char*)pgm_read_word(menu_styles+i));
    yn=yn_dialog(buffer);
    bitWrite(opt,i,yn);
  }

  lcd.clear();
  sprintf(buffer, "%s%d","New style:", global_style=opt);
  lcd.print(buffer);
  Serial.println(buffer);
  wait_on_escape(200);
  lcd.clear();
  } */

//calibrating:

void calib() {
  uint32_t instant_t6 = 0;
  
    calibrating = true;
    lcd.clear();
    center_text("Calibrate"); 
    lcd.setCursor(0,1);
    lcd.print(F("Press * to start"));

    while(calibrating) {
    if(millis() - instant_t6 >= (uint32_t)4000) {
      instant_t6 = millis();
      wdt_reset();
    }
    
      switch (my_btns.getKey()) {         
      case 7: {
        instant_t0 = millis();
        digitalWrite(PMP, HIGH);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Calibrating..."));
        lcd.setCursor(0, 1);
        lcd.print(F("Press # to stop.."));
        break;
    }
  
      case 8: {
        //stop m_drning product
        instant_t1 = millis();
        digitalWrite(PMP, LOW);
        m_clb = instant_t1 - instant_t0;
        EEPROM.put(70, m_clb);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Done:"));
        lcd.setCursor(0, 1);
        lcd.print(m_clb);
        calibrating = false; 
        wait_on_escape(2000);
        break;
      } 
      case 4:
      return;
      break;       
    }
    }
}

void reserve() {
  EEPROM.get(30, m_rmng);
  lcd.clear();
  center_text("Reserve:");
  lcd.setCursor(0, 1);
  lcd.print(m_rmng);
  lcd.setCursor(8 , 1);
  lcd.print("L");
  wait_on_escape(2000);

}

void clean() {
  uint32_t instant_t6 = 0;
  // uint32_t instant_t0 = 0;
  uint32_t instant_t1 = millis();
  cleaning = true;
  lcd.clear();
  center_text("Clean");
  lcd.setCursor(0, 1);
  lcd.print(F("Press * to start"));

  while (cleaning) {
    if (millis() - instant_t6 >= (uint32_t)4000) {
      instant_t6 = millis();
      wdt_reset();
    }
    if (millis() - instant_t1 >= (uint32_t)30000) {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(F("Press * to start"));
      lcd.begin(lcd_columns, lcd_rows);
      lcd.setCursor(0, 1);
      lcd.print(F("Ceaning DONE!"));
      wait_on_escape(2000);
      cleaning = false;
    }


    switch (my_btns.getKey()) {
      case 7: {
          digitalWrite(PMP, HIGH);
          //-----------LCD prob.------------
          //delay(250);
          lcd.begin(lcd_columns, lcd_rows);
          lcd.setCursor(0, 1);
          lcd.print(F("Press # to stop.. "));
          instant_t1 = millis();
          break;
        }
      case 8: {
          //stop cleaning product
          digitalWrite(PMP, LOW);
          //-----------LCD prob.------------
          // delay(250);
          lcd.begin(lcd_columns, lcd_rows);
          lcd.setCursor(0, 1);
          lcd.print(F("DONE!"));
          wait_on_escape(2000);
          cleaning = false;
          //wait_on_escape(2000);
          break;
        }
      case 3:
        return;
        break;
    }

  }
}


bool passwrd(int pass_addr) {
  //bool acc = false;
  //long pass = 5678;
  int pass;
  EEPROM.get(pass_addr, pass);

  while (1) {
    long result = 0;
    phi_prompt_struct myTextInput; // This is the structure you will use to invoke library function input_panel.
    char nmuber_buffer[] = "            "; // This is the buffer that will store the content of the text panel.
    myTextInput.ptr.msg = nmuber_buffer; // Assign the text buffer address
    myTextInput.width = 4; // Length of the input panel is 12 characters.
    myTextInput.col = 2; // Display input panel at column 2
    myTextInput.row = 1; // Display input panel at row 1
    myTextInput.option = 1;

    //-----------LCD init-----------------
    lcd.begin(lcd_columns, lcd_rows);
    lcd.clear(); // Clear the lcd
    lcd.print(F(" Enter password:")); // Prompt user for input

    if (input_number(&myTextInput) != -1) { // input_panel stores user choice in file_name.
      lcd.clear();
      //lcd.print(F("You entered:")); // Display the user choice one more time
      lcd.setCursor(0, 1); // Display the list at column 2 row 1.
      if (sscanf(nmuber_buffer, "%ld", &result) == 1) { ///< Turn the text entered into integer number. If a valid input is acquired, sscanf returns 1.
        //Password Comparison happens here

        if (result == (long)pass) {
          lcd.print(F("Pass Correct"));
          //acc = true;
          return true;
        }
      }
      else {
        lcd.print(F("Invalid input!!"));
      }

    }
    else {
      lcd.clear();
      lcd.print(F("You pressed ESC"));
      return false;
    }
  }
}


void ch_passwrd(int pass_addr) {
  int old_pass;
  int new_pass;
  EEPROM.get(pass_addr, old_pass);

  while (1) {
    long result = 0;
    phi_prompt_struct myTextInput; // This is the structure you will use to invoke library function input_panel.
    char nmuber_buffer[] = "            "; // This is the buffer that will store the content of the text panel.
    myTextInput.ptr.msg = nmuber_buffer; // Assign the text buffer address
    myTextInput.width = 4; // Length of the input panel is 12 characters.
    myTextInput.col = 2; // Display input panel at column 2
    myTextInput.row = 1; // Display input panel at row 1
    myTextInput.option = 1;
    //-----------LCD init-----------------
    lcd.begin(lcd_columns, lcd_rows);
    lcd.clear(); // Clear the lcd
    lcd.print(F("Enter old pass:")); // Prompt user for input

    if (input_number(&myTextInput) != -1) { // input_panel stores user choice in file_name.
      lcd.clear();
      //lcd.print(F("You entered:")); // Display the user choice one more time
      lcd.setCursor(0, 1); // Display the list at column 2 row 1.
      if (sscanf(nmuber_buffer, "%ld", &result) == 1) { ///< Turn the text entered into integer number. If a valid input is acquired, sscanf returns 1.
        //Password Comparison happens here

        if (result == old_pass) {
          lcd.print(F("Pass Correct"));
          instant_t7 = millis();
          while (millis()  - instant_t7 < 500);
          new_pass = newpass_prompt();
          if (new_pass) {
            EEPROM.put(pass_addr, new_pass);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(F("Pass Changed"));
          }
          return;

        }
        else {
          lcd.print(F("Pass Wrong!"));

        }
      }
      else {
        lcd.print(F("Invalid input!!"));
      }

    }
    else {
      lcd.clear();
      lcd.print(F("You pressed ESC"));
      return;
    }
  }
}

int newpass_prompt() {

  while (1) {
    long tries[2] = {0, 0};
    phi_prompt_struct myTextInput; // This is the structure you will use to invoke library function input_panel.
    char nmuber_buffer[] = "            "; // This is the buffer that will store the content of the text panel.
    myTextInput.ptr.msg = nmuber_buffer; // Assign the text buffer address
    myTextInput.width = 4; // Length of the input panel is 12 characters.
    myTextInput.col = 2; // Display input panel at column 2
    myTextInput.row = 1; // Display input panel at row 1
    myTextInput.option = 1;

    //-----------LCD init-----------------
    lcd.begin(lcd_columns, lcd_rows);
    lcd.clear(); // Clear the lcd
    lcd.print(F("Enter new pass:")); // Prompt user for input

    if (input_number(&myTextInput) != -1) { // input_panel stores user choice in file_name.
      lcd.clear();
      lcd.setCursor(0, 1); // Display the list at column 2 row 1.
      if (sscanf(nmuber_buffer, "%ld", &tries[0]) == 1) { ///< Turn the text entered into integer number. If a valid input is acquired, sscanf returns 1.
        //Password Comparison happens here
        //lcd.setCursor(0,0);
        lcd.clear();
        lcd.print(F("Enter again:"));
        nmuber_buffer[0] = '\0';
        //lcd.setCursor(0,1);
        if (input_number(&myTextInput) != -1) {
          lcd.clear();
          lcd.setCursor(0, 1);
          if (sscanf(nmuber_buffer, "%ld", &tries[1]) == 1) {
            lcd.clear();
            if (tries[0] == tries[1]) {
              lcd.print(F("Pass Match"));
              return tries[0];
            }
          }
        }
        else {
          lcd.print(F("Invalid input!!"));

        }


        /* if(result == old_pass) {
          lcd.print(F("Pass Correct"));

          EEPROM.put(pass_addr, result);
          return;

          }
          else {
          lcd.print(F("Pass Wrong!"));

          } */
      }
      else {
        lcd.print(F("Invalid input!!"));
      }

    }
    else {
      lcd.clear();
      lcd.print(F("You pressed ESC"));
      return 0;
    }
  }
}






