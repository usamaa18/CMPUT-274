#include <Arduino.h>
#include <string.h>


uint32_t serverPublicKey = 7;
uint32_t serverPrivateKey = 27103;
uint32_t serverModulus = 95477;
uint32_t clientPublicKey = 11;
uint32_t clientPrivateKey = 38291;
uint32_t clientModulus = 84823;

uint32_t d;
uint32_t n;
uint32_t e;
uint32_t m;


// Determines whether the arduino is a server or not.
bool Server(){
    if (digitalRead(13) == HIGH)
    	return true;
    else
    	return false;
}

void setup(){
	init();
	Serial.begin(9600);
	Serial3.begin(9600);
	pinMode(13, INPUT); 

	// assigns d, n, e, m, based on whether it is configured in server or client mode
	if (Server() == true){
		d = serverPrivateKey;
		n = serverModulus;
		e = clientPublicKey;
		m = clientModulus;
    }

    else{
		d = clientPrivateKey;
		n = clientModulus;
		e = serverPublicKey;
		m = serverModulus;
    }
}

/** Writes an uint32_t to Serial3, starting from the least-significant
 * and finishing with the most significant byte. 
 */
void uint32_to_serial3(uint32_t num) {
	Serial3.write((char) (num >> 0));
	Serial3.write((char) (num >> 8));
	Serial3.write((char) (num >> 16));
	Serial3.write((char) (num >> 24));
}


/** Reads an uint32_t from Serial3, starting from the least-significant
 * and finishing with the most significant byte. 
 */
uint32_t uint32_from_serial3() {
	uint32_t num = 0;
	num = num | ((uint32_t) Serial3.read()) << 0;
	num = num | ((uint32_t) Serial3.read()) << 8;
	num = num | ((uint32_t) Serial3.read()) << 16;
	num = num | ((uint32_t) Serial3.read()) << 24;
	return num;
}


// to use during mulmod method, converts dec to bin, returns reverse binary string
String NumberToBinary(uint32_t number){ 
	String x = "\0";

	if (number == 0)
    	return "0";

  	while (number!=0){
    	x = x + (String(number % 2));
    	number /= 2;
  	}

  	return x;
}

// computues the value (a*b) mod m using only uint32_t, while preventing overflow
uint32_t mulmod(uint32_t a, uint32_t b, uint32_t m){
	String a_bin = NumberToBinary(a); 

	uint32_t term, ans;
	ans = 0;
	term = b % m;

	// special case for LSB
	if (a_bin[0]=='1'){
			ans += term;
			ans = ans % m;
		}

	// note that NumberToBinary() returns a reverse binary string, so we start with low index
	for (int i=1; a_bin[i]!='\0'; i++){

		// compute each term to represent 2^i mod m
		term *= 2;
		term = term % m;

		// add term to ans and mod m, if i'th poition in binary string is 1
		if (a_bin[i]=='1'){
			ans += term;
			ans = ans % m;
		}
	}
	return ans;
}


// computes the value x^pow mod m ("x to the power of pow" mod m)
uint32_t powmod(uint32_t x, uint32_t pow, uint32_t m) {

  uint32_t ans = 1;
  uint32_t pow_x = x;

  while (pow > 0) {
  	// if the LSB of pow is 1
    if (pow&1 == 1)
    	ans = mulmod(ans,pow_x,m);

    pow_x = mulmod(pow_x,pow_x,m);

    pow >>= 1; // divides by 2
  }

  return ans;
}


void chat(){
	//encrypting
	if (Serial.available()){

		int character = Serial.read();

		// if inputted character is a carraige return, transmit '\r\n' to create a newline
		if (character == '\r'){
			Serial.write('\r');
			Serial.write('\n');

			uint32_t encrypted_char = powmod(uint32_t('\n'), e, m);
			uint32_to_serial3(encrypted_char);
				
			encrypted_char = powmod(uint32_t('\r'), e, m);
			uint32_to_serial3(encrypted_char);
		}

		// write character to serial-mon, encrypt it, and transmit it
		else{
			Serial.write(char(character));
			uint32_t encrypted_char = powmod(uint32_t(character), e, m);
		 	uint32_to_serial3(encrypted_char);
		}
	}

	//decrypting

	// recieve encrypted character, decrypt it, and write it to serial-mon
	// but only start decryption when 4 bytes (32 bits) are received
    if (Serial3.available()==4){
    	uint32_t encrypted_char = uint32_from_serial3();
    	uint32_t character = powmod(encrypted_char, d, n);
    	Serial.write(char(character));
    }
}



int main(){
	setup();
	Serial.println("Welcome to Arduino chat!");
	while(true)
		chat();
}
