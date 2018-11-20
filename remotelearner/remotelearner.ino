#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <EEPROM.h>
#include <IRremote.h>

#define KP_CLK 4
#define KP_DATA 5

IRrecv ir_receiver(2);
IRsend ir_send; //automatically  on pin 3

decode_results results;
bool captured = false;
long timer;
unsigned short last_keypad = 0 ;

Adafruit_PCD8544 display = Adafruit_PCD8544(6, 8, 7);

void display_results(decode_results *results);
unsigned short keypad_value = 0;

unsigned short result_type = 0;

void setup() {
  // put your setup code here, to run once:

  display.begin();
  display.setContrast(50);
  display.setRotation(2); //we have an upside down screen
  display.clearDisplay();

  display.setCursor(0,0);
  display.println("Jaycar");
  display.setTextSize(2);
  display.println("Remote Learner");
  display.display();

  Serial.begin(9600);
  ir_receiver.enableIRIn();

  pinMode(KP_CLK,OUTPUT);
  pinMode(KP_DATA,INPUT);
  
  while(!keypad_read(0)) //keypress to remove screen
    ;
   
}

/* Most remote codes are 32-bit codes. this requires 4 bytes to store. We'll be storing 16 ( one each button keypad) 
 *  so this means we need atleast 64bytes of memory.
 */

void loop() {
  // put your main code here, to run repeatedly:

    //received:
    if (ir_receiver.decode(&results)) {
       Serial.println(results.value, HEX);
        display_results(&results);
        captured = true;
        timer = millis();
        ir_receiver.resume(); // Receive the next value
    }


    keypad_value = keypad_read(last_keypad); //block the last keypad value
    
    if (keypad_value) {
      last_keypad = keypad_value;
      timer = millis();
      Serial.println(keypad_value,DEC);
      if(captured){
        captured = false;
        store_results( keypad_value , results.value, results.decode_type);
        display.print("store -> ");
        display.println(keypad_value);
        display.print("type:");
        display.println(results.decode_type,HEX);
      } else {
        //not captured, just a regular press
        byte type = 0;
        long var = retreive_results(keypad_value, type);
        display.clearDisplay();
        display.setTextSize(1);
        display.print("fire: ");
        display.println(keypad_value);
        display.print("value: (");
        display.print(type,HEX);
        display.println(")");
        display.println(var,HEX);
        last_keypad = 0; //just incase we want to keep pressing

       fire_ir(type,var);
        
      }
      display.display();
    }


    if(timer+3000 < millis()){
      captured = false;
      last_keypad = 0;
      display.clearDisplay();
      display.display();
    }
}

void fire_ir(byte type, long value){
  if (type == UNKNOWN) {
    display.println("unknown type! can't do!");
    display.display();
  } else if (type == NEC) {
    ir_send.sendNEC(value, 32);
  }  else if (type == SONY) {
    ir_send.sendSony(value,32);
  }  else if (type == RC5) {
    ir_send.sendRC5(value,32);
  }  else if (type == RC6) {
    ir_send.sendRC6(value,32);
  }  else if (type == PANASONIC) {
    display.println("Panasonic needs addresses, submit a patch!");
    display.display();
  }  else if (type == LG) {
    ir_send.sendLG(value,32);
  }  else if (type == JVC) {
    ir_send.sendJVC(value,32,false);
  }  else if (type == AIWA_RC_T501) {
    ir_send.sendAiwaRCT501(value);
  }  else if (type == WHYNTER) {
    ir_send.sendWhynter(value,32);
  }
}
short keypad_read(unsigned short block_value) {
  digitalWrite(KP_CLK, HIGH);
  delay(2);
  for( unsigned short i = 0; i < 16; ++i){
    digitalWrite(KP_CLK,LOW);
    digitalWrite(KP_CLK,HIGH);

    if (!digitalRead(KP_DATA) && (i+1 != block_value))
      return i+1;
  }
  return 0;
}
void store_results(unsigned short key, unsigned long value, byte r_type){
  //assuming 32bit data, each space in memory is 8 bits
  //so we space it out by 4. thus each "key" takes 4 bytes in memory
  //so the spacing is 4 wide, or 4*key = address;

  //1+ because we need the type aswel
  unsigned short base_address = 5 * key;
  
  byte b1 = (value    )& 0xFF;
  byte b2 = (value>>8 )& 0xFF;
  byte b3 = (value>>16)& 0xFF;
  byte b4 = (value>>24)& 0xFF;
  
  EEPROM.update(base_address,   b4); //update is nicer method, as it checks first.
  EEPROM.update(base_address+1, b3);
  EEPROM.update(base_address+2, b2);
  EEPROM.update(base_address+3, b1);
  EEPROM.update(base_address+4, r_type); //global result_type;
}
unsigned long retreive_results(unsigned short key, byte &type){
  unsigned short base_address = 5 * key;
  long b4 = EEPROM.read(base_address);
  long b3 = EEPROM.read(base_address+1);
  long b2 = EEPROM.read(base_address+2);
  long b1 = EEPROM.read(base_address+3);

  type = EEPROM.read(base_address+4);

  return (
    ((b4<<24)& 0xFF000000) | 
    ((b3<<16)& 0x00FF0000) | 
    ((b2<<8 )& 0x0000FF00) | 
    ( b1     & 0x000000FF)
    );
}
void display_results(decode_results *results){
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  if (results->decode_type == UNKNOWN) {
    display.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC) {
    display.print("Decoded NEC: ");
  }
  else if (results->decode_type == SONY) {
    display.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5) {
    display.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6) {
    display.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {
    display.print("Decoded PANASONIC - Address: ");
    display.print(results->address, HEX);
    display.print(" Value: ");
  }
  else if (results->decode_type == LG) {
    display.print("Decoded LG: ");
  }
  else if (results->decode_type == JVC) {
    display.print("Decoded JVC: ");
  }
  else if (results->decode_type == AIWA_RC_T501) {
    display.print("Decoded AIWA RC T501: ");
  }
  else if (results->decode_type == WHYNTER) {
    display.print("Decoded Whynter: ");
  }
    display.println();
  display.println(results->value, HEX);
  display.print("(");
  display.print(results->bits, DEC);
  display.println(" bits)");
    display.print("(type: ");
  display.print(results->decode_type);
  display.println(")");
  display.display();
}

