#include "bitFields.h"
#include <assert.h>
#include <stdio.h>

int getBit(int bn,unsigned long val) {
	// TODO:
	//		check to make sure bn is a valid bit position in long (you may assert this is true)
	//		If the bit in val at position bn is 1, return 1
	//		otherwise return 0.


	assert((0<=bn) && (bn<64));
	int bit = (val>>bn)&1L;  
	if (bit==1)
	{
		return 1;
	}
	else
	{
		return 0;
	}

	printf("Not valid");
	return 0;


}

void setBit(int bn,int new,unsigned long *val) {
	// TODO:
	//		check to make sure bn is a valid bit position in long (you may assert this is true)
	//		if new is true, set the bit in val at bn to 1
	//     otherwise, set the bit in val at bn to 0

	assert((0<=bn) && (bn<64));
	if(new)
	{
		//here want to set specificed bit to 1 --> if current bit is a 1 need to OR it with 0
		//if current bit is 0 need to OR it with a 1
		if (getBit(bn, *val)==0)
		{
			*val = (*val) | (1L<<bn);
			return;
		}
		else
		{
			return;
		}   
	}
	//here want to set specified bit to 0 --> if current bit is a 1 need to ^ with 1
	//if current bit is 0 need to ^ it with 0
	else
	{
		if(getBit(bn, *val)==1)
		{
			*val = *val ^ (1L<<bn);
			return;
		}
		else
		{
			return;
		}
	}
	

	
}


long getBitFld(int bs,int len,unsigned long val) {
	// TODO:
	//		check to make sure bs is a valid bit position in long (you may assert this is true)
	//		check to make sure len is a valid length for a subfield that starts at bs (you may assert this is true)
	// 		perform an unsigned getBitFldU to extract the unsigned version of the field
	//		get the high order bit of the result... that's the "sign" bit
	//		if the sign bit not zero, replace the 0 padding with 1 padding

	assert((0<=bs) && (bs<64) && (1<=len) && (len<=bs+1));

	unsigned long sheUnsigned = getBitFldU(bs, len, val);
	int signBit = getBit(len, sheUnsigned);
	unsigned long *sheUnsignedPtr = &sheUnsigned;
	if (signBit!=0)
	{
		//start at bit len and pad left with 1s
		for(int i=len; i<64 ;i++)
		{
			setBit(i, 1, sheUnsignedPtr);
		}
	}
	long sheSigned = sheUnsigned;
	return sheSigned; 

}

unsigned long getBitFldU(int bs,int len,unsigned long val) {
	// TODO:
	//		check to make sure bs is a valid bit position in long (you may assert this is true)
	//		check to make sure len is a valid length for a subfield that starts at bs (you may assert this is true)
	// 		make a mask that has len 1's
	// 		Shift that mask left so it's leftmost 1 starts at bs
	//      bitwise and the mask and val to isolate the bit field bits
	//		Shift the result to the right so that the rightmost bit of the bit field is bit 0
	//			Note that shift right pads to the left with the sign bit if the field is signed, but
	//			for unsigned, it will pad to the left with 0

	assert((0<=bs) && (bs<64) && (1<=len) && (len<=bs+1));
	
	unsigned long maskVal = 0;
	unsigned long * mask = &maskVal;
	int bsCopy = bs;
	for(int i=0; i<len; i++)
	{
		setBit(bsCopy, 1, mask);
		bsCopy = bsCopy - 1;
	}
	unsigned long newThingIdk = val & *mask;
	newThingIdk = newThingIdk>>(bsCopy+1);
	return newThingIdk; 
}

void setBitFld(int bs,int len,unsigned long new,unsigned long *val) {
	// TODO:
	//		check to make sure bs is a valid bit position in long (you may assert this is true)
	//		check to make sure len is a valid length for a subfield that starts at bs (you may assert this is true)
	//      Make a mask that has len bits, right justified
	//		Use mask to turn off any bits to the left of the bitField values in new
	// 		Shift both the new value and the mask to the correct bit position
	//		Turn off all bits in *val under the mask by bitwise anding with ~mask
	//		Or *val with new.  (new has zeros for all non bit field positions, so won't affect val)

	assert( (0<=bs) && (bs<64) && (1<=len) && (len<=bs+1));
	
	unsigned long mask = (1L<<len)-1;
	new = new & mask;
	new = new<<(bs-len+1);
	mask = mask<<(bs-len+1);
	*val = (*val)&(~mask);
	*val |= new;
}
