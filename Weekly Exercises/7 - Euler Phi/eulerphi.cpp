#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

unsigned int phi(unsigned int n) {

	// index of primes[]
	int curr = 0;
	// condition of while loop 1
	bool loop1 = true;
	uint32_t primes[10] = {0};
	// holds last used prime number
	uint32_t prev = 1;
	uint32_t phi = n;

	while (loop1){
		// prevents retarting the loop at 2 after n' is computed
		int p=prev+1;
		// condition of while loop 2
		bool loop2 = true;

		// check every integer from 2 to sqrt(n) if it is a factor of n
		while (p*p<=n && loop2){
			if (n%p == 0){
				primes[curr] = p;
				loop2 = false;
			}
			p++;
		}

		// if no factor was found, n is a prime number
		if (primes[curr] == 0){
			primes[curr] = n;
			loop1 = false;
		}
		
		// else, divide n by the factor till no longer divisible
		else {
			while (n % primes[curr] == 0)
				n = n / primes[curr];

			if (n == 1)
				loop1 = false;

			prev = primes[curr];
		}

		curr++;
	}

	// avoid edge case where user inputs n = 1
	if (primes[0]!=1)
		// calculate phi using all primes that were found
		for (int i=0; i<curr; i++) {
			double prime = double(primes[i]);
			double term = (prime-1)/prime;
			phi = phi * term;
		}
	return phi;	
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


int main() {
	uint32_t n;
	cin>>n;

	//for (n=1; n<=4294967295;n++){

	uint32_t aphi = phi(n);
	
	cout << "n" << setw(13) << "= " << n << "\n";
	cout << "phi(n)" << setw(8) << "= " << aphi << "\n";
	cout << "phi(n)/n" << setw(6) << "= " << fixed << setprecision(5) << double(double(aphi)/n) << "\n";
	
	// do 15-bit test only if n>(2^15)
	if (n>=32768){
		int x=0;
		int low = pow(2,14);
		int high = pow(2,15);
		// test all possible integer values of k between in [2^14,2^15)
		for (int k=low; k<high; k++){
			if (gcd(n,k)==1){
				x++;
			}
		}
		cout << "15-bit test = " << fixed << setprecision(5) << double(double(x)/low)<<"\n";
	}
	//}
}
