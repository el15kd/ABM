#include <stdio.h>
int thresh = 0, state = 0, input = 0;
/* Threshold Func Test */
const char* treshold(int thresh) {
// 
    if(thresh <= 7) 			{state = 3; return "using a car or public transport";} // agent chooses to use (drive/ride in) a motorized vehicle
    else if (thresh = 8 || thresh < 11)	{state = 2; return "cycling";} // agent chooses to cycle
    else 				{state = 1; return "on-foot";} // agent chooses to walk
} // ^ lower state corresponds to transportation mode creating the least amount of traffic (1 - least, 3 - most); thresh range e [4 (4*1), 12 (4*3)] since an agent has 4 neighbours
int a = 10, b = 10, c = 10, testSum = 0, z = 0;
int sum(int a,int b, int c) {return z = a+b+c;}
int main(int argc, char** argv){
    input = 13; testSum = sum(a,b,c);
    /* Threshold Func Test */ printf("Agent is %s\n", treshold(input));
    printf("Sum is %d\n", testSum);
}
    

