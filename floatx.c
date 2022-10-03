#include "floatx.h"
#include "bitFields.h" // Remove this if you don't use bitFields
#include <assert.h>
#include <limits.h> // for CHAR_BIT - number of bits per byte
#include <math.h> // for isinf and isnan
#include <stdio.h>

floatx doubleToFloatx(double val,int totBits,int expBits) {

/*-----------------------------------------------------------------------------------------------
    From the README:     At a high level, doing the conversion requires several manipulations:
    1. Extracting the sign bit from the double value, and inserting it into the floatx value at
        the correct position.
    2. Handle special cases, such as 0.0, infinity, or not-a-number.
    3. Extract the biased exponent from the double value.
            Check to see if the double value is sub-normal. If so, handle it.
            Check to make sure the floatx exponent won't overflow or underflow).
                    If so, handle as a special case.
            If not, rebias the exponent using the floatx bias (which depends on the number of exponent bits)
            and write the result to the correct location in the floatx result.
    4. Extract the fraction bits from the double value.
            Determine how many bits are available for the fraction in the floatx value,
            and truncate or extend the original value,
            and write the resulting bits to the floatx result.
    6. Return the floatx result.
----------------------------------------------------------------------------------------------------*/

    // First, make some assertions to ensure the totBits and expBits parameters are OK
    // Then, implement the algorithm

   if( !((1<totBits) && (totBits<=64)))
   {
       printf("invalid param");
       return 0;
   }
    if( !((1<=expBits) && (expBits<=totBits-2)))
   {
       printf("invalid param");
       return 0;
   }

    floatx finalNum = 0;
    floatx * finalNumPtr = &finalNum;

    union hexDouble {
   	    double dbl;
   	    long lng;
    } unionVal;
    unionVal.dbl = val;

    //do sign bit 
    int signBit = getBit(63, unionVal.lng); 
    if(signBit==1)
    {
        setBit(totBits-1, 1, finalNumPtr);
    }
    else
    {
        setBit(totBits-1, 0, finalNumPtr);
    }

    //special case: 0
    if (val == 0)
    {
        val = 0;
        return finalNum;
    }


    //special case: infinity --> exp bits are all one and fraction bits are all 0; NAN --> exp bits are all one and fractions bits are all not zero
    //go through all of the exponent bits and check if they are all one, then branch off from there
    //to check if exp bits are all one, loop through the exp bits, and if a bit is ever 0 then break out of the loop; if that does not happen
    //then that means that all of the bits are ones; so after that now needs to check if fraction bits are either all one or all zero
    //loop through bits and if a bit is ever one then floatX=NAN and return, and if that condition is never met, then the number is infinity and return that
    int checkIfExpAllOnes = 1;

    for(int i=totBits-2; i>=(totBits-1-expBits); i--)  //range might be wrong; forgot to do getBitFld! do if this doesn't work
    {
        int currentExpBitVal = getBit(i, unionVal.lng);
        if(currentExpBitVal==0)
        {
            checkIfExpAllOnes = 0;
            break;
        }
    }
    if(checkIfExpAllOnes==1)
    {
        for(int i=(totBits-1-expBits); i>=0; i--)
        {
            int currentFracBitVal = getBit(i, unionVal.lng);
            if(currentFracBitVal==1)
            {
                return (floatx) NAN; 
            }
        }
        return (floatx) INFINITY;
    }


    //biased exponent --> have not done subnormal; if there is overflow, then there are too many exp bits
    unsigned long expBitFld = getBitFldU(62, 11, unionVal.lng); 
    long unbiasedVal = expBitFld - 1023;
    unsigned long biasedExpForFloatx = unbiasedVal + (1<<(expBits-1)) - 1;
    if(biasedExpForFloatx > ((1L<<expBits)-1) ) //not sure if correct
    {
        for(int i= totBits-2; i>=totBits-1-expBits;i--)
        {
            setBit(i,1,finalNumPtr);
        }
        return finalNum;
    }
	setBitFld(totBits-2, expBits, biasedExpForFloatx, finalNumPtr); //setting might be wrong


   //fraction portion
	unsigned long fracBitField = getBitFldU(51, 52 ,unionVal.lng); 
    int numFracBitsInFloatx = totBits-1-expBits; 

//if the values in the bit field extend past or are less than the number of bits available
    if( fracBitField > ((1L<<numFracBitsInFloatx)-1) )
    {
        //need to shift fracBitField to the right
        int amtToShift = 52-numFracBitsInFloatx;
        int newFracBitField = fracBitField>>amtToShift;
        setBitFld(totBits-expBits-2, numFracBitsInFloatx, newFracBitField,finalNumPtr);
    }
    if(fracBitField <= ((1L<<numFracBitsInFloatx)-1) ) 
    {
        setBitFld(totBits-expBits-2, numFracBitsInFloatx, fracBitField,finalNumPtr);
    }

    return finalNum; 
}