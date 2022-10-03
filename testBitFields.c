#include "bitFields.h"
#include <stdio.h>

int main(int argc, char **argv) {

	// Unit test for the bitFields functions

	unsigned long val=0xbeefdeaddeadbeef;

	printf("testing deadbeef bits...");
	for(int i=63;i>=0;i--) {
		printf("%c",getBit(i,val)?'1':'0');
		if (0==i%4) printf(" ");
	}

	// a=1010 b=1011 d=1101 e=1110 f=1111
	//                                                    b     e      e      f     d      e     a      d      d      e     a      d        b      e      e      f
	printf("\nexpected deadbeef bits..1011 1110 1110 1111 1101 1110 1010 1101 1101 1110 1010 1101 1011 1110 1110 1111\n");

	unsigned long newBeef=0;
	for(int i=0;i<64;i++) setBit(i,getBit(i,val),&newBeef);
	printf("Setting deadbeef bits: %lx\n",newBeef);

	// Testing get/set fields using nibbles

	printf("setting deadbeef by nibbles...");
	for (int i=63;i>=0;i-=4) { printf("%lx ",getBitFldU(i,4,val)); }
	printf("\n");

	printf("getting signed deadbeef by nibbles...");
	for (int i=63;i>=0;i-=4) { printf("%x ",(unsigned char)getBitFld(i,4,val)); }
	printf("\n");

	newBeef=0;
	for (int s=63;s>=0;s-=4) setBitFld(s,4,getBitFldU(s,4,val),&newBeef);
	printf("deadbeef set by nibbles: %lx\n",newBeef);

	newBeef=0;
	for (int s=63;s>=0;s-=4) setBitFld(s,4,getBitFld(s,4,val),&newBeef);
	printf("deadbeef set by signed nibbles: %lx\n",newBeef);

	return 0;
}