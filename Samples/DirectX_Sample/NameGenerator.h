#include <iostream>
#include <string>
#include <sstream> 
#include <math.h>
using namespace std;

char* consonants = "tnrshdlfcmgypwbvkjxqz";
int numc = 21;
char* vowels = "eaoiu";
int numv = 5;

int weightedRand(int max)
{
	float percent = (rand() / (float) RAND_MAX);
	//weight the number twoards more common letters
	percent = pow(percent, 1.8f);
	return (int)(max * percent);
}

char getCons()
{
	return consonants[weightedRand(numc)];
}
char getVowel()
{
	return vowels[weightedRand(numv)];
}
wstring genName(int length)
{
	wstring ret;
	wstringstream finalName;
	if(length > 8)
	{
		wstring join = L".";
		int sideA, sideB;
		length -= 1;
		sideA = rand() % (length/2) + length/4;
		sideB = length - sideA;
		finalName << genName(sideA)<< join <<genName(sideB);
	}
	else
	{
		while(length > 1)
		{
			finalName<<getCons()<<getVowel();
			length -=2;
		}
		if(length ==1)
			finalName<<getVowel();
	}
	return finalName.str();
}