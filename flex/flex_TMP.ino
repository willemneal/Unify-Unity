/* This is sample code for using a flex sensor.

*/
final int NUM_OF_COMP_INPUTS = 2;


String inputString = ""; //String to hold input Data
boolean sendData = false;


int [NUM_OF_COMP_INPUTS] inputFromComputer = {100,100} //Default inputs
char delimiter = ';'

const int flexPins[] = {0,1};//Add pins here
const int analogOutPin = 9; //Analog output pin for LED

const int waitTimeAfterStartup = 5000 // 5 seconds

void setup(){
	Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
	inputString.reserve(200); // Reserve 200 bytes for the input
	delay(waitTimeAfterStartup); // Pause for ten seconds

}

void loop(){
	int flexReading =
	//
	if (sendData && Serial.available()==0{
			println();
			inputString = "";//Reset the data
			sendData = false;
	}
	analogWrite(analogOutPin,ledIntensity);//output to LED

	delay(2);//Pause for analog to digital to settle

}

int readPin(pinNumber){
	return min(359,max(0,map(analogRead(flexPin),401,766,0,359)));//Map to degrees
}

void printLine(){

	foreach (pin in flexPins){
		Serial.print("flexPin:")
		Serial.print(readPin(pin))
	}
}
void readInput(){
	while (Serial.available() > 0 ){
		char nextChar = (char) Serial.read();
		if (nextChar == '\n')//break out of the loop if we read the end of the line.
				break;
		inputString += nextChar;
	}

}
int decodeInput(string input){
	\\here is the logic to parse the input
	int startIndex = 0;
	while ()

	inputFromComputer
}

void serialEvent(){
		readInput()
		ledIntensity = map(inputData.toInt(),0,359,0,255);// Convert the String to an int and map to the output value range
		sendData = true;
}
