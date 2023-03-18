#include <avr/io.h>
double tempconverter (double celcius)
{
	return (celcius * (9/5)) + 32;
	
}
//extra function
char* mode(char* string, unsigned short frequent)
{
	char* holdstr = string;
	
	if(frequent == 700 || frequent == 600)
	holdstr = "high";
	else if (frequent == 500 || frequent == 400)
	holdstr = "medium";
	else if (frequent == 300 || frequent == 200)
	holdstr = "low";
	
	return holdstr;
}

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}


unsigned char lights(unsigned short frequent)
{
	unsigned char tmp = 0x00;
	if(frequent == 700 || frequent == 600)
	tmp = 0x01; //red light
	else if (frequent == 500 || frequent == 400)
	tmp = 0x02; //yellow light
	else if (frequent == 300 || frequent == 200)
	tmp = 0x04; //green light
	
}
