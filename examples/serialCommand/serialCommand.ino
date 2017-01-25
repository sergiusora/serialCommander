
#include <serialCommander.h>
#define parammeterSize  10
int parameters[parammeterSize];
bool nop;
bool pb1, pb2;
void command (String code, int a1, int a2); // predefine 

serialCommander com(command);
void setup() {
  Serial.begin(9600);
  Serial.println("Starting up");
}

void loop() {
 com.verificareSerial();  // verify if any data on serial

}

void command(String code, int a1, int a2){      // cant use switch with string
// Serial.print("command: "); Serial.print(code); 
// Serial.print(";  a1: ");  Serial.print(a1);
// Serial.print(";  a2: ");  Serial.println(a2);    // echo mode

  // command: set 0 123\n | atr1 - paramenter nr; atr2 - parameter value
if(code == "set"){
    if (a1 >= parammeterSize) Serial.println("No parameter nr");
    else parameters[a1] = a2;
    goto ok;                                          // jump to end after good command
}
// command: get 0\n | atr1 - paramenter nr; atr2 - not used
 if(code == "get"){
    if (a1 >= parammeterSize) Serial.println("No parameter nr");
    else Serial.println(parameters[a1]);
    goto ok;                                          // jump to end after good command
 }
  
 // command: pb1 0\n | atr1 - 0=false, 1=true, 2 return value; atr2 - not used
 if(code == "pb1"){
     if (a1==0) pb1 = false;
      else if (a1==1) pb1 = true;    
        else if (a1==2) {
          Serial.print("PB1: ");
          Serial.println(pb1, BIN);
        } else Serial.println("0 -> set false; 1 -> set true; 2 -> read status");
     goto ok;                                          // jump to end after good command
 }
  
// command: sum 1 2\n | atr1 - first value; atr2 - second value
  if(code == "sum"){
    adder(a1, a2);
    goto ok;                                          // jump to end after good command
  }
    Serial.println("Wrong command code.");      // if no comand 
  ok:                                           // if comand ok
  nop = true;
}


void adder(int a1, int a2){
  int sum = a1+a2;
 Serial.print("sum:");
  Serial.println(sum, DEC);
}
  
