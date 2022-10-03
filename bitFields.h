/* Utility to get/set bits and bit fields
	Conventions:
		vals are 64 bit integers
		Bit positions are 0<=pos<64
		lengths must fit in value size: 1<=len<=bs+1
		setBit uses the truth value of "new" to set the bit
		setBitFld will use bits (len-1) to 0 in the "new" parameter
		getBitFld will return a value with the sign bit of the sub-field propagated
		getBitFldU will return a value padded to the left with zeros
*/

int getBit(int bn,unsigned long val);
void setBit(int bn,int new,unsigned long *val);
long getBitFld(int bs,int len,unsigned long val);
unsigned long getBitFldU(int bs,int len,unsigned long val);
void setBitFld(int bs,int len,unsigned long new,unsigned long *val);