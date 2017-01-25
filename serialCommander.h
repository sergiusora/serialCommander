/*
serialCommander.h
Recive a coded comand and calls user defined function with the decoded data as atributes
*/

#ifndef serialCommander
#define serialComander


#include <Arduino.h>


#define CodeSize 3					//  can be changed
#define integerSize 6				//  maxim value for range -32768...32767
#define serialReciveBuffSize 20		//	maxim characters in one command 

class serialCommander
{
	public:
    serialCommander(void (*com)(String Code, int A1, int A2));
	void verificareSerial();

	private:
	void clearBuffer(char *bufferIn, int buffSize);
	void commandProcess();
	bool atrProc();
	int ascii_int();
    void (*CommandA1A2)(String Code, int A1, int A2);
	// variables
	int reciveCount;
	char serialBuffer[serialReciveBuffSize];
	char integerASCII[integerSize];
	bool err = false;
	char Code[CodeSize];
	char separator = ' ';
	int index, index1;
	int a1, a2;
	bool min = false;
	bool intErr = false;
	String CodeS;
		
};

#endif
