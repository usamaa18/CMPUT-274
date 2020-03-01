#include <Arduino.h>

const int led[5] = {13, 12, 11, 10, 9};

// this bool array stores the current LED states
bool LED_states[8] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};


void setup(){

	init();
	Serial.begin(9600);

	for (int i=0; i<5; i++)
		pinMode(led[i], OUTPUT);
}


int8_t getHexValue(char digit){

	int8_t x;

	if (digit >= '0' && digit <= '9')
		x = digit - '0';
	else if (digit >= 'A' && digit <= 'F')
		x = digit - 'A' + 10;
	else if (digit >= 'a' && digit <= 'f')
		x = digit - 'a' + 10;

	// 13 in ASCII is carriage return and 32 is space
	// -2 tells main() to reset total
	else if (digit == 13 || digit == 32)
		x = -2;

	// -1 tells main() to ignore the character
	else
		x = -1;

	return x;
}


void update_LEDs(uint8_t total){

	// reset LED_states to LOW
	for (int i=0; i<8; i++)
		LED_states[i] = LOW;

	// increment LED_states 'total' times
	for (int j=0; j<total; j++)
		for (int i=7; i>=0; i--)
			// keep decrementing i till a LOW state LED is found or out-of-index
			if (LED_states[i] == HIGH)
				LED_states[i] = LOW;
			else{
				LED_states[i] = HIGH;
				i=-1;
			}

	// change LED power based on 5 LSBs of LED_states
	for (int i=3; i<8; i++)
		digitalWrite(led[i-3], LED_states[i]);
}


int main(){

	setup();

	// bool value to ensure that LEDs are only updated when some button is pressed
	bool update = true;
	uint8_t total = 0;
	
	while(true){

		if (Serial.available() > 0){
			char digit = Serial.read();
			int8_t x = getHexValue(digit);

			// indicate LEDs need to be updated
			update = true;

			// LEDs don't need to be updated if character entered was invalid
			if (x == -1)
				update = false;
			// reset total to 0 if space or enter key is pressed
			else if (x == -2)
				total = 0;
			else
				total += x;
		}
		
		// updates LEDs only if a button was pressed 
		if (update == true){
			update_LEDs(total);
			update = false;
		}
	}

	return 0;
}