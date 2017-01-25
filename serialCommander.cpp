/*
serialCommander.c
*/

#include <Arduino.h>
#include "serialCommander.h"

//_________________________________________________________________initializer
serialCommander::serialCommander(void (*com)(String Code, int A1, int A2)){
    CommandA1A2 = com;
}     

//_________________________________________________________________process the recived data
void serialCommander:: commandProcess(){
	index = 0;                                                     
	index1 = 0;
	a1 = 0;
	a2 = 0;
	for (int i = 0; i<CodeSize;i++) Code[i] = ' ';                 // clear Code buffer
	for (int i = 0; i<integerSize;i++) integerASCII[i] = ' ';      // clear ASCII buffer	
//--------- read command code -------------
	while((serialBuffer[index]!=' ')&&(serialBuffer[index] != 10)){                  // loop for getting the code char
		Code[index1] = serialBuffer[index];                                 //
		index+=1;                                                  //
		index1+=1;                                                 //
        if (index1 == CodeSize+1){                                 // if code size is to big
           Serial.println("Error Code to long!!");                 // error report
           goto err;                                               // exit on error
        }		
	}
    CodeS = (String(Code[0])+String(Code[1])+String(Code[2]));    // retrive the command code
	if (serialBuffer[index] == 10) goto ok;                       // exit on end off data
	index+=1;		                                              // skip curent index com = SPACE 
//--------- read first atribute -----------	
    index1 = integerSize-1;                                       // set index to corect value
	min = false;                                                  // clear the sign
	if(atrProc()) a1 = ascii_int();                               // save atribute as integer 
	else goto err;
	if (intErr == true) goto err;                                 // exit on error
    for (int i = 0; i<integerSize;i++) integerASCII[i] = ' ';     // clear ASCII buffer
    if (serialBuffer[index] == 10) goto ok;                       // exit on end off data
	index+=1;                                                     // skip curent index com = SPACE 
//--------- read second atribute -------------	
    index1 = integerSize-1;                                       // set index to corect value
    min = false;                                                  // clear the sign
	if(atrProc()) a2 = ascii_int();                               // save atribute as integer  
	else goto err;
	if (intErr == true) goto err;                                 // exit on error
    for (int i = 0; i<integerSize;i++) integerASCII[i] = ' ';     // clear ASCII buffer
    
	goto ok;
	err:
	intErr = false;
	Serial.println("wrong data format");
	goto done;
	ok:
	CommandA1A2(CodeS, a1, a2);
	done:
	index = 0;	
	
	
}
//________________________________________________________________atribute processing
bool serialCommander::atrProc(){
     while((serialBuffer[index]!=' ')&&(serialBuffer[index] != 10)){                // loop for getting the atribute                     
		if ((serialBuffer[index]>47)&&(serialBuffer[index]<58)||(serialBuffer[index]==45)){  // if numeric or '-'                 
		   integerASCII[index1] = serialBuffer[index];                     // add to array
		   index1-=1;                                             // decrement array index (logic :D)
        }
		index+=1;                                                 // increment buffer index
        if (index1<-1){                                           // out of array bounds
           Serial.println("Error atribute to long!!");            // error report
           return false;                                          // error exit
        }	
	}                                                             // retrive second atribute
    if (integerASCII[integerSize-1]=='-'){
        min = true;                                               // verify if negative nr
        integerASCII[integerSize-1] = 48;                         // pass the sign after the 
    }                                                             // rotate 
    while(index1>-1){
        integerASCII[0] = integerASCII[1];
        integerASCII[1] = integerASCII[2];
        integerASCII[2] = integerASCII[3];
        integerASCII[3] = integerASCII[4];
        integerASCII[4] = integerASCII[5];
        integerASCII[5] = 48;                                     // rotate array ele. to left
        index1-=1;                                                // to get the unit nr in the 
    }                                                             // 0 index                               
	if (min == true) integerASCII[5] = '-';                       // replace the sign if negativ  
     return true;                                                 // all ok    
}

//__________________________________________________________process the recived data for commands
void serialCommander:: verificareSerial(){
	if (Serial.available() > 0) {
		serialBuffer[reciveCount] = Serial.read();               // read data in serial buffer
		reciveCount+=1;                                          // index for next char in data
		if (serialBuffer[reciveCount-1] == 10){                  // "Line Feed" is end of command
			if (err == true) {
				clearBuffer(serialBuffer,serialReciveBuffSize);  // in error clear buffer
				err = false;                                     // clear error status
			}
		else {
			commandProcess();                         // process recived data
			clearBuffer(serialBuffer,serialReciveBuffSize);      // clear buffer	
			}
	    }
		if (reciveCount > serialReciveBuffSize){                 // data exceede buffer size
			err = true;                                          // set error status
			clearBuffer(serialBuffer, serialReciveBuffSize);     // clear buffer
			Serial.println("Data exceed buffer size");           // report error 
	    }    
  } 
};

//__________________________________________________________________clear comand buffer
void serialCommander::clearBuffer(char *bufferIn, int buffSize){
	for (int i = 0; i<buffSize;i++) bufferIn[i] = 0;
    reciveCount = 0;
};

//_________________________________________________________________convert char string in integer
int serialCommander::ascii_int(){
	int res = 0;
	int sign = integerASCII[5];
	int M = integerASCII[4]-48;
	int m = integerASCII[3]-48;
	int s = integerASCII[2]-48;
	int z = integerASCII[1]-48;
	int u = integerASCII[0]-48;
	
	bool c1 = (M==3)&&(m>2);                                           // 
	bool c2 = (M==3)&&(m==2)&&(s>7);                                   //
	bool c3 = (M==3)&&(m==2)&&(s==7)&&(z>6);                           //
	bool c4 = (M==3)&&(m==2)&&(s==7)&&(z=6)&&(u>7);                    // condition for safe conversion
    
    if ((sign == 48)||(sign == 45)){                                   // sign char must be ' ' or '-'
       if ((M>3)||c1||c2||c3||c4){                                     // 
          Serial.println("Error value not in range -32768...32767");   //
          intErr = true;
          return 0;                                                    //       
       }
       res = (M*10000)+(m*1000)+(s*100)+(z*10)+ u;                     // generate the result
    }
	if (sign == 45)return res*-1;                                      // return with sign 
	else return res;
}
