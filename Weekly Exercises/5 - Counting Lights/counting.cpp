#include <Arduino.h>

const int led[5] = {13, 12, 11, 10, 9};

const int btn_down = 7;
const int btn_up = 6;
const int btn_save = 5;

void setup(){

	init();
	Serial.begin(9600);

	for (int i=0; i<5; i++){
		pinMode(led[i], OUTPUT);
		digitalWrite(led[i], LOW);
	}

	pinMode(btn_up, INPUT);
	pinMode(btn_down, INPUT);
	pinMode(btn_save, INPUT);

	digitalWrite(btn_up, HIGH);
	digitalWrite(btn_down, HIGH);
	digitalWrite(btn_save, HIGH);

}

// these bool arrays store the current and saved version of LED states
bool leds[5] = {LOW, LOW, LOW, LOW, LOW};
bool save[5];

// these bools are the new and old states of each btn. We compare them to ensure
// that time is not wasted on changing LED states when we don't need to
bool new_up = HIGH;
bool old_up = HIGH;
bool new_down = HIGH;
bool old_down = HIGH;
bool new_save = HIGH;
bool old_save = HIGH;

// toggle between HIGH (save) and LOW (load)
bool status_save = HIGH;

// bool value to ensure that LEDs are only updated when some button is pressed
bool update = false;

void loop(){

	// this block increments the LEDs when btn_up is pressed
	new_up = digitalRead(btn_up);
	if (new_up != old_up){
		update = true;

		// if statement prevent double incrementation of LEDs
		if (new_up == HIGH)
			for (int i=4; i>=0; i--)
				// keep decrementing i till a LOW state LED is found or out-of-index
				if (leds[i] == HIGH)
					leds[i] = LOW;
				else{
					leds[i] = HIGH;
					i=-1;
				}

		old_up = new_up;
	}

	// this block decrements the LEDs when btn_down is pressed
	new_down = digitalRead(btn_down);
	if (new_down != old_down){
		update = true;

		if (new_down == HIGH)
			for (int i=4; i>=0; i--)
				// keep decrementing i till a HIGH state LED is found or out-of-index
				if (leds[i] == HIGH){
					leds[i] = LOW;
					i=-1;
				}
				else
					leds[i] = HIGH;

		old_down = new_down;
	}

	// this block saves or loads the LEDs when btn_down is pressed based on status_save
	new_save = digitalRead(btn_save);
	if (new_save != old_save){
		update = true;

		if (new_save == HIGH)
			// save LEDs state and turn them all off
			if (status_save == HIGH){
				for (int i=0; i<5; i++){
					save[i] = leds[i];
					leds[i] = LOW;
				}
				status_save = LOW;
			}

			// load LEDs state
			else{
				for (int i=0; i<5; i++)
					leds[i] = save[i];
				status_save = HIGH;
			}

		old_save = new_save;
	}

	// updates LEDs only if a button was pressed 
	if (update == true)
		for (int i=0; i<5; i++){
			digitalWrite(led[i], leds[i]);

		update = false;
	}

	// minimizes button bounce
	delay(50);
}

int main(){

	setup();

	while(true){
		loop();
	}

	return 0;
}