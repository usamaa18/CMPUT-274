
#include <Arduino.h>

const int serverPin = 13;

/*
    Returns true if arduino is server, false if arduino is client
*/
bool isServer() {
    if (digitalRead(serverPin) == HIGH) {
        return true;
    } else {
        return false;
    }
}


bool is_prime(uint32_t test) {
    if (test%2==0)
        return false;
    else
        for (int i=3; i<sqrt(test); i+=2)
            if (test%i==0)
                return false; 
    return true;
}


uint32_t random_k(int k){
  uint32_t num=0;
  for (int i = 0; i < k; ++i) {
    int val = analogRead(A1);
    //Serial.print(val&1);
    num <<= 1;
    num = num | (val&1);
    delay(5);
  }
  return num;
}


uint32_t random_prime(int k) {
    uint32_t test = random_k(k) + pow(2,k);
    while (!is_prime(test)) {
        test = random_k(k) + pow(2,k);
    }
    return test;
}


// greatest common divisor (fast euclid method) 
uint32_t gcd(uint32_t a, uint32_t b) {
  while (b > 0) {
    a %= b;
    // now swap them
    uint32_t tmp = a;
    a = b;
    b = tmp;
  }
  return a; // b is 0
}


uint32_t reduce_mod(int32_t x, uint32_t m){
    if (x<0){
        x = x%int(m);
        if (x<0)
            x = x+m;
    }
    return x;
}


uint32_t generate_d(uint16_t e, uint32_t phi) {
    int32_t r[40];
    r[0] = e;
    r[1] = phi;

    int32_t s[40];
    //int32_t t[40];

    s[0] = 1;
    s[1] = 0;

    //t[0] = 0;
    //t[1] = 1;

    int i = 1;
    while (r[i]>0){
        int32_t q = r[i-1]/r[i];    
        r[i+1] = r[i-1] - q*r[i];
        s[i+1] = s[i-1] - q*s[i];
        //t[i+1] = t[i-1] - q*t[i];
        i = i+1;
    }
//  cout<<s[i-1];
    uint32_t d = reduce_mod(s[i-1], phi);
    return d;
}


/*
    Compute and return (a*b)%m
    Note: m must be less than 2^31
    Arguments:
        a (uint32_t): The first multiplicant
        b (uint32_t): The second multiplicant
        m (uint32_t): The mod value
    Returns:
        result (uint32_t): (a*b)%m
*/
uint32_t multMod(uint32_t a, uint32_t b, uint32_t m) {
    uint32_t result = 0;
    uint32_t dblVal = a%m;
    uint32_t newB = b;

    // This is the result of working through the worksheet.
    // Notice the extreme similarity with powmod.
    while (newB > 0) {
        if (newB & 1) {
            result = (result + dblVal) % m;
        }
        dblVal = (dblVal << 1) % m;
        newB = (newB >> 1);
    }

    return result;
}


/*
    NOTE: This was modified using our multMod function, but is otherwise the
    function powModFast provided in the lectures.

    Compute and return (a to the power of b) mod m.
	  Example: powMod(2, 5, 13) should return 6.
*/
uint32_t powMod(uint32_t a, uint32_t b, uint32_t m) {
    uint32_t result = 1 % m;
    uint32_t sqrVal = a % m;  // stores a^{2^i} values, initially 2^{2^0}
    uint32_t newB = b;

    // See the lecture notes for a description of why this works.
    while (newB > 0) {
        if (newB & 1) {  // evalutates to true iff i'th bit of b is 1 in the i'th iteration
            result = multMod(result, sqrVal, m);
        }
        sqrVal = multMod(sqrVal, sqrVal, m);
        newB = (newB >> 1);
    }

    return result;
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


/*
    Encrypts using RSA encryption.

    Arguments:
        c (char): The character to be encrypted
        e (uint32_t): The partner's public key
        m (uint32_t): The partner's modulus

    Return:
        The encrypted character (uint32_t)
*/
uint32_t encrypt(char c, uint32_t e, uint32_t m) {
    return powMod(c, e, m);
}


/*
    Decrypts using RSA encryption.

    Arguments:
        x (uint32_t): The communicated integer
        d (uint32_t): The Arduino's private key
        n (uint32_t): The Arduino's modulus

    Returns:
        The decrypted character (char)
*/
char decrypt(uint32_t x, uint32_t d, uint32_t n) {
    return (char) powMod(x, d, n);
}

/*
    Core communication loop
    d, n, e, and m are according to the assignment spec
*/
void communication(uint32_t d, uint32_t n, uint32_t e, uint32_t m) {
    // Consume all early content from Serial3 to prevent garbage communication
    while (Serial3.available()) {
        Serial3.read();
    }

    // Enter the communication loop
    while (true) {
        // Check if the other Arduino sent an encrypted message.
        if (Serial3.available() >= 4) {
            // Read in the next character, decrypt it, and display it
            uint32_t read = uint32_from_serial3();
            Serial.print(decrypt(read, d, n));
        }

        // Check if the user entered a character.
        if (Serial.available() >= 1) {
            char byteRead = Serial.read();
            // Read the character that was typed, echo it to the serial monitor,
            // and then encrypt and transmit it.
            if ((int) byteRead == '\r') {
                // If the user pressed enter, we send both '\r' and '\n'
                Serial.print('\r');
                uint32_to_serial3(encrypt('\r', e, m));
                Serial.print('\n');
                uint32_to_serial3(encrypt('\n', e, m));
            } else {
                Serial.print(byteRead);
                uint32_to_serial3(encrypt(byteRead, e, m));
            }
        }
    }
}


/*
    Performs basic Arduino setup tasks.
*/
void setup() {
    init();
    Serial.begin(9600);
    Serial3.begin(9600);

    Serial.println("Welcome to Arduino Chat!");
}

/** Code from assignment description
* Waits for a certain number of bytes on Serial3 or timeout
* @param nbytes : the number of bytes we want
* @param timeout : timeout period (ms); specifying a negative number
* turns off timeouts (the function waits indefinitely
* if timeouts are turned off).
* @return True if the required number of bytes have arrived .
*/
bool wait_on_serial3 ( uint8_t nbytes , long timeout ) {
    unsigned long deadline = millis () + timeout ;// wraparound not a problem
    while ( Serial3.available () < nbytes && ( timeout <0 || millis () < deadline ) ){
        delay (1) ; // be nice , no busy loop
    }
    return Serial3.available () >= nbytes ;
}



void server_states(uint32_t d, uint32_t n, uint32_t e, uint32_t m, uint32_t myPublicKey){

    enum states{Listen, WaitingForKey, WaitForAck, DataExchange};
    states cur_state = Listen;

    while(true){

        if(cur_state == Listen)
            if( wait_on_serial3(1, 1000))
                if(Serial3.read() == 'C'){
                    cur_state = WaitingForKey;            
        }

        else if(cur_state == WaitingForKey){
            if(wait_on_serial3(8, 1000)){
                cur_state = WaitForAck;
                e = uint32_from_serial3();
                m = uint32_from_serial3();
                //Serial.write("state1")
                uint32_to_serial3(myPublicKey);
                uint32_to_serial3(n);
                Serial3.write('A');
            }

            else
                cur_state = Listen;
                //Serial.write("state2")
        }

        else if(cur_state == WaitForAck){
            if(wait_on_serial3(1, 1000)){
                if(Serial3.read() == 'C')
                    cur_state = WaitingForKey;
                    //Serial.write("state4")
                else if(Serial3.read() == 'A')
                    cur_state = DataExchange;
                    //Serial.write("state3")
                else
                    cur_state = Listen;
                    //Serial.write("state5")
            }

            else
                cur_state = Listen;
                //Serial.write("state6")
        }

        else if(cur_state == DataExchange)

            communication(d, n, e, m);
            //Serial.write("state7")

        Serial3.flush();
    }
}


void client_states(uint32_t d, uint32_t n, uint32_t e, uint32_t m, uint32_t myPublicKey){
    enum states{Start, WaitForAck, DataExchange};
    states cur_state = Start;

    while(true){

        if(cur_state == Start){
            uint32_to_serial3(myPublicKey);
            uint32_to_serial3(n);
            cur_state = WaitForAck;
            Serial3.write('C');
            //Serial.write("state1")
        }

        else if(cur_state == WaitForAck){

            if(wait_on_serial3(9, 1000)){
                if(Serial3.read() == 'A'){
                    e = uint32_from_serial3();
                    m = uint32_from_serial3();
                    cur_state = DataExchange;
                    Serial3.write('A');
                    //Serial.write("state2")
                }
                else
                    cur_state = Start;
                    //Serial.write("state3")  
            }

            else
                cur_state = Start;
                //Serial.write("state4")
        }

        else if(cur_state == DataExchange)
            communication(d, n, e, m);
            //Serial.write("state5")

        Serial3.flush();
    }
}

/*
    The entry point to our program.
*/
int main() {
    setup();
    uint32_t d, n, e, m, myPublicKey;

    uint32_t p =random_prime(14);
    uint32_t q =random_prime(15);



    n = p*q;
    uint32_t phi = (p-1)*(q-1);

    myPublicKey = random_k(15);

    while (!(myPublicKey>1 && myPublicKey<phi && gcd(myPublicKey,phi)==1))
        myPublicKey= random_k(15);


    d = generate_d(e,phi);

    // Determine our role and the encryption keys.
    if (isServer()) {
        Serial.println("Server");
        server_states(d,n,e,m,myPublicKey);
    } else {
        Serial.println("Client");
        client_states(d,n,e,m,myPublicKey);
        
    }

    //Serial.println("");

    // Now enter the communication phase.

    // Should never get this far (communication has an infite loop).

    return 0;
}
