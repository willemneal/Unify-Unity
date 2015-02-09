/* This is sample code for two way communication
 	with computer using a flex sensor.
	Author: Willem Wyndham
*/


//Sensor inputs
const int NUMBER_OF_INPUT_PINS = 2;
const int flexPins[NUMBER_OF_INPUT_PINS] = {0,1};
//Add pins to array, don't have to be in order.

//AnalogOutput
const int analogOutPin = 9;


const int waitTimeAfterStartup = 2000; // 2 seconds

//data from computer to be read by arduino outputs
const int NUM_OF_COMP_INPUTS = 2;
int inputFromComputer[NUM_OF_COMP_INPUTS] = {100,100}; //Default inputs

//for reading computer input and decoding it
const char DELIMITER = ',';
String inputString = ""; //String to hold input Data

//Things for synchronization
bool mutex = true; //A binary Semaphore
uint8_t InterruptFlag;

bool isInput(){
	return Serial.available()>0;
}

int readPin(int pinNumber){
	return min(359,max(0,map(analogRead(pinNumber),401,766,0,359)));//Map to degrees
}


void println(){
	/* The structure of the output message is:
	   flexPin:value,value,value
	   the first part tells us what the output is and we know the values
	   follow in order.
	*/
	Serial.print("flexPin:");
	for (int pin = 0; pin< NUMBER_OF_INPUT_PINS; pin++){
		Serial.print(readPin(pin));
		if (pin + 1 == NUMBER_OF_INPUT_PINS) //its the last one
			break; 			//break before the last ','
		Serial.print(",");
	}
	Serial.print('\n');//endline char
}

void flushOutput(){
	//read until you can't
	while(isInput()){
		char nextChar = (char) Serial.read();
	}
}

void readInput(){
	while (isInput()){
		char nextChar = (char) Serial.read();
		if (nextChar == '\n')//break out of the loop if we read the end of the line.
				break;
		inputString += nextChar;
		delay(3);
	}
	flushOutput();//In case the other program included more after the new line
				 //This should not happen other side communicates correctly.
	decodeInput(inputString);
	inputString = "";
}



void decodeInput(String input){
	//here is the logic to parse the input
	/* We expect the following input
		value,value,value etc.

		All values will be stored in inputFromComputer array.
	*/
	int startIndex = 0;
	int delimiterIndex;
	Serial.println(input);
	for (int index=0; index < NUM_OF_COMP_INPUTS; index++){
		delimiterIndex = input.indexOf(DELIMITER,startIndex);
		//index where the first delimiter is found after startIndex

		if (delimiterIndex > 0)//There is a delimiter found so print from start to just before
			inputFromComputer[index] = input.substring(startIndex,delimiterIndex-1).toInt();
		else
			inputFromComputer[index] = input.substring(startIndex).toInt();
			//Otherwise we are at the end of input and we want to read from start to end.

		startIndex = delimiterIndex + 1;//now start is just after the delimiter
	}
}

void disableInterrupts(){
	InterruptFlag = SREG;   // save interrupt flag
	cli();   // disable interrupts
}
void enableInterrupts(){
	SREG = InterruptFlag;
}


bool down(bool sem){
	/*
	* Uses busy waiting if the semaphore is in use
	**************************************************
	*  1. disable interrupts and check the value of sem
	*  2. If false enable and yeild to other threads
	*  3. Repeat 1-2 until sem == true, set it to false and leave with
		Interrupts enabled.  Since the sem is now held, no other thread
		can be past the down call.
	*/
	disableInterrupts();
	bool waited = false;
	while (!sem){
		enableInterrupts();
		waited = true;
		Serial.println("I waited on another process");
		delay(3);//yield();			// the thread yields itself so another program can run
		disableInterrupts();
	}
	sem = false;//acquire the semaphore
	enableInterrupts();
	return waited;
}

void up(bool sem){
	//This sets the semaphore as available, thus anyone waiting can continue
	sem = true;
}

void sendOutput(){
	int ledIntensity = map(inputFromComputer[0],0,359,0,255);
	// Convert the String to an int and map to the output value range
	//Could add more outputs here
	analogWrite(analogOutPin,ledIntensity);//output to LED
}


void serialEvent(){
		bool waited = down(mutex);
		//acquire Mutex so the next section is only read by one thread at a time
		if (waited){
			Serial.println("I waited on another process");
		}
		if (isInput()){//Only read if there is input. else release the mutex
			readInput();
			println();
		}
		up(mutex);//Release the mutex so other threads can continue
}

void setup(){
	Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
	inputString.reserve(200); // Reserve 200 bytes for the input
	delay(waitTimeAfterStartup); // Pause for X seconds
	println();//print line of Flex Values
	//this starts the back and forth communication
}

void loop(){

	serialEvent();
	//This handles a serial event
	//Could do other things here

	delay(2);//Pause for analog to digital to settle

}
