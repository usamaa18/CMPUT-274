#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    int n = 0;
    int vote[1001];
    int candidate[1001] = {0};

    // keep reading until we see 0
    while (cin >> vote[n] && vote[n] != 0) {
        ++n;
    }


    for (int i=0; i<n; i++){
    	candidate[vote[i]]++;
    }

    int win = *max_element(candidate,candidate+1001);

    int count=0;
    int pos;
    for (int i=0; i<1000; i++){
    	//cout<<candidate[i];
    	if(candidate[i] == win){
    		count++;
    		pos=i;
    	}
    }

    if (count==1)
    	cout<<pos<<"\n";
    else
    	cout<<"tie\n";
    // now n == # votes and vote[i] is the i'th vote
    // for 0 <= i <= n-1

    // solve the problem and print the answer
    // good luck! (and remember to vote next election) :)

    return 0;
}
