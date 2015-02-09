/* This is sample code for finding
		the max and min of a flex sensor.
		Author Willem Wyndham
*/

const int flexPin = 0;

//initializes bounds
int maxRes = 0;
int minRes = 1024;

void setup(){
	Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop(){
	int flexReading= analogRead(flexPin);
	minRes =  min(flexReading, minRes); //Find new min
	maxRes =  max(flexReading, maxRes); //Find new max
	Serial.print(flexReading);
    Serial.print(" Max: ");
    Serial.print(maxRes);
    Serial.print(" Min: ");
    Serial.println(minRes);


	delay(2);//Pause for analog to digital to settle

}
