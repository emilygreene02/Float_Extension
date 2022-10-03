# proj02 - Extending Floating Point Numbers

## Introduction

The goal of this assignment is to:
- Learn about IEEE floating point numbers
- Learn about manipulating bits (“bit twiddling”) in C
- Understand number type conversions

(Note - former students have described this project when interviewing for jobs or internships and the interviewers have been very impressed!)

We will discuss the internal representation of IEEE floating point numbers (float and double) in class. We will learn how to use the IEEE standard to represent floating point numbers in 32 or 64 bits worth of data. Some applications need the flexibility of floating point numbers, but may not need the full precision offered by the standard float (32 bit) or double (64 bit) implementations of floating point numbers. Or, applications may need higher precision, and are willing to sacrifice the range by decreasing the number of exponent bits. This project asks you implement a generalized floating point number capability – one which uses an arbitrary total number of bits, broken into a sign bit, an arbitrary number of exponent bits, and an arbitrary number of fraction bits to represent a floating point number.  We will call this generalized floating point number a “floatx” number. 

Other than changing the number of bits for the sub-fields, all the rest of the IEEE conventions for floating point numbers must be followed:
- The left-most (most significant) bit is a sign bit – 0 for positive, 1 for negative
- The next expBits bits represent an exponent field, biased by 2<sup>(expBits-1)</sup> – 1
- Exponent bits of all ones represents special values like infinity (if fraction bits are all zero), or “Not a Number” (Nan) (if fraction bits are not all zero.)
- The remaining bits represent a fraction field, “F”.  For most numbers, there is an implied “1.” in front of the fraction so we interpret it as “1.F”.  
- If the exponent field and the fraction field are all zeroes, that represents the number 0.0.
- If the exponent field is all zeroes, but the fraction field is not zero, then we treat the number as a “sub-normal” (or denormalized) number, and assume it has the value of 0.F x 2<sup>-bias</sup>

Obviously, creating a floatx number will require a lot of bit manipulation. In order to organize the bit manipulation, I have provided suggestions for a library of bit manipulation functions that allow you to extract and insert individual bits and fields of bits into and out of a long integer. However, I have not provided complete implementations of these libraries. You are not required to use these libraries, but I would recommend completing the libraries and using them. Whether you use the libraries or not, you still have to perform the bit manipulation, and I found that using these libraries made the project much easier to understand.

We will use the standard double-precision floating point specification to provide values to convert to the floatx format. The Wikipedia [Double-precision floating-point](https://en.wikipedia.org/wiki/Double-precision_floating-point_format) article does a good job of describing this format. For the project, you will need to implement a single function, the `doubleToFloatx` function that takes a double precision floating point input argument, along with the total number of bits and the number of exponent bits for the floatx version of the value. The function needs to return the floatx version of the number. We define a floatx number as a 64 bit unsigned integer (an unsigned long). However, we will only be using the rightmost bits (bit positions totBits-1 to zero) for our floatx value.

For example, a floatx number with 12 total bits and 5 exponent bits would use the following bits in a long unsigned value...

![floatx_12_5](https://github.com/BinghamtonUniversityCS140/images/blob/master/floatx_12_5.png)

At a high level, doing the conversion requires several manipulations:
1. Extracting the sign bit from the double value, and inserting it into the floatx value at the correct position.
2. Handle special cases, such as 0.0, infinity, or not-a-number.
3. Extract the biased exponent from the double value. Check to see if the double value is sub-normal. If so, handle it. Check to make sure the floatx exponent won't overflow (or underflow). If so, handle as a special case. If not, rebias the exponent using the floatx bias (which depends on the number of exponent bits) and write the result to the correct location in the floatx result.
4. Extract the fraction bits from the double value. Determine how many bits are available for the fraction in the floatx value, and truncate or extend the original value, and write the resulting bits to the floatx result.
6. Return the floatx result.

## Provided to you

The proj02 repository already contains some infrastructure to get you started, as follows:

### bitFields.h and bitFields.c

These files are the optional but suggesting bit-twiddling library that you will find useful for completing this project. The bitFields.h file declares five functions whose definitions appear in bitFields.c. These functions include a get and set function for a single bit, as well as get and set functions for bit fields that contain multiple bits. The get bit field function has two different flavors - the standard `getBitFld` function returns a signed value with the sign bit of the sub-field propagated. The `getBitFldU` function returns an unsigned version of the bit field, that is padded on the left with zeros.

The definition of these functions is not complete - you must finish the code in bitFields.c in order to use these functions.

### testBitFields.c

This is some code I have provided for you to unit test the bitFields functions. It uses the functions in the way they were intended, and prints results based on that usage. If there are bugs in the bitFields functions, running this code will print out obvious miscompares in a way that should make it easy to debug.

### floatx.h and floatx.c

The floatx.h file declares the single floatx function, `doubleToFloatx`, we will be working on for this project. It would also make sense to work on a `floatxToDouble` function if this were a real library, but doing so adds more work to an already difficult project, and doesn't contribute much more to the educational value of the project, so I've left that out. The floatx.h file also contains a typedef statement to define floatx as an unsigned long (64 bit) integer. You should not have to edit floatx.h.

Your main job will be to complete the definition of the `doubelToFloatx` function in floatx.c. This is the function that takes a double precision floating point value, and returns the floatx representation of that value.

### testFloatx.c

This is a program that uses command line arguments to specify 1) the total number of bits, and 2) the number of exponent bits in a floatx format. It then reads standard input, looking for a double precision floating point value. For each floating point value found in standard input, the testFloatx program invokes the `doubleToFloatx` with the value read from standard input, and the total bits and exponent size specified in the command line arguments. Then the testFloatx program prints the original value, the floatx specification, and the floatx value in hexadecimal. Any non-whitespace characters from standard input that cannot be translated to a double will cause the program to stop.

We will use UNIX redirection to read and write from files (see the Wikipedia [Redirection(computing)](https://en.wikipedia.org/wiki/Redirection_(computing)) article for more details.) Please do not edit the code in testFloatx.c. There is no need to use file I/O functions to read or write files for this project.

### test1.txt

A text file which contains some example floating point values to test with. You can use redirection, running a command like `./testFloatx 32 8 <test1.txt` to see how all the values in test1.txt would be represented in floatx with 32 bits and 8 exponent bits.

### Makefile

I have provided a Makefile with several targets:

- `clean` Removes all the files Makefile can create
- `testBitFields` - creates the testBitFields executable from testBitFields.c and bitFields.c - Remove this if you don't want to use the bitFields library
- `bfgdb` - A target to run gdb on the testBitFields executable - Remove this if you don't want to use the bitFields library
- `testFloatx` - creates the testFloatx executable from testFloatx.c, floatx.c, and bitFields.c. Remove bitFields.c (and bitFields.h) from this rule if you don't want to use the bitFields library
- `gdb` - A target to run gdb on the testFloatx executable. This will automatically set a breakpoint at the first instruciton in `doubleToFloatx`, and run with command line arguments 16 and 4. You will still need to type in values to convert on stdin, which is the keybaord in this case.
- `test` - Runs the testFloatx code with total bits of 16, of which 4 are exponent bits. You will still need to type values to convert. Type something like "q" and hit enter to quit.


## Coding the project

After reading through this readme, I would recommend first working on the bitFields functions. To do so, complete the function definitions for all five bitFields functions in bitFields.c, and then `make testBitFields` to see if you have everything correct.

Once you have the bitFields library working correctly (or if you decide not to use the bitFields library), start working on floatx.c, implementing the `doubleToFloatx` function. Then use `make test` to check your results. Type in several values, including special cases, to make sure your conversion code works correctly. You will have to manually verify the results. Once you have things working for a simple 16 4 floatx specification, try some others... your code needs to work for any valid specification.

Here is an example of the output produced when your program is coded correctly:

```
>cat test1.txt
1.0
100.0
1.3333333333333333333333333333
1.27563e45
6.23e22
7.9e-39
3.4028235e+38
3.4028236e+38
q
>./testFloatx 32 8 <test1.txt
             1 = floatx(32,8) : 000000003f800000
           100 = floatx(32,8) : 0000000042c80000
     1.3333333 = floatx(32,8) : 000000003faaaaaa
   1.27563e+45 = floatx(32,8) : 000000007f800000 
      6.23e+22 = floatx(32,8) : 00000000655314a2
       7.9e-39 = floatx(32,8) : 00000000002c0bf8
 3.4028235e+38 = floatx(32,8) : 000000007f7fffff
 3.4028236e+38 = floatx(32,8) : 000000007f7fffff
Input : q not recognized as a double...         quitting

```

## Hints and Suggestions

1. This project is almost impossible without the use of the shift left (<<) and shift right (>>) C operators. Remember that in C, the right shift operator, >>, behaves differently on signed and unsigned data types.

2. When bit twiddling, remember that a literal value is treaded as a 32 bit integer UNLESS it has an "L" suffix! I have spent hours and hours debugging code such as `(1<<pos & value)`, where `value` is a 64 bit (long) value, and `pos` is a position between 0 and 63. The bug is that `1<<pos` says shift a **32** bit value by `pos` bits. If `pos` is greater than 31, the result is undefined! The correct code is `(1L<<pos & value)`.

3. To get a bit field that contains `n` bits, right justified, you can use the bit-twiddle: `(1L<<n)-1`.

4. It's hard to get the bit values that make up a double. If you try to assign a double to a long, C will convert the double from floating point value to an integer, and put that integer in the long result instead of just copying the bits. The easiest way to get the bit values is to use a "union" in C. I used the following union...

   ```
   union hexDouble {
   	double dbl;
   	long lng;
   } unionVal;
   ```
   
   Then, you can take the parameter input value and copy it to unionVal.dbl, and use unionVal.lng to inspect the bits. 

4. When the C scanf function reads standard input and converts to a double, it accepts many different formats. Several different formats are represented in the example above. Scan also recognizes "inf" for infinity, and on many machines "nan" for "not a number". However, not all versions of the C library recognize "nan" as not a number, including the C library on the LDAP machines. Therefore, we will not use "nan" as a test case when grading.

5. Use the online floating point calculators referenced in the Brightspace/Content/Reference section to double check your work, at least for common specifications such as "32 8" for single-precision floating point numbers.

6. One of the hardest special cases to deal with are "sub-normal" numbers... numbers in which the biased exponent is 0, which indicates that the fraction should be preceded with 0. instead of 1. This gets even more complicated because it is possible for numbers which are not sub-normal when represented as a double can become sub-normal when represented as a floatx. There are even cases where numbers which *are* subnormal when represented as a double may *not be* subnormal when represented in floatx (for instance, when floatx has more than the 11 bits reserved for exponents in the double format.) Since sub-normal numbers are often complicated, no deduction will occur if you get sub-normal numbers incorrect. However, if you get them correct, you can earn some extra credit.


## Submitting the Project

When you have finished coding and testing you testFloatx program, make sure you do a final git commit and push to update your main git repository. Then, retreive the  digit git hash code either from browsing your updated repository on the web, or using the command `git rev-parse HEAD` on the command line. Paste the git hash code in the Brightspace PROJ02 assignment. You may resubmit as many times as you would like to. Only the last submission will be graded.

## Grading Criteria

Your repository will be downloaded onto an LDAP machine at the level corresponding to the hash code retrieved from Brightspace, and compiled and run using the Makefile. Your code will be tested with the same inputs as the example above, and with three other sets of input values, and the results will be compared to correct results. The project is worth 100 points, and the following deductions will occur:

- -100 if your code matches or is significantly similar to any other student's code.
- -80 If your code does not attempt to address the problem.
- -80 if testFloatx does not compile
- Up to -30 if there are mismatches on the published test case
- Up to -15 points per test if there are incorrect results on each unpublished test case (for a maximum of -45)
- -10 if testFloatx produces warning  messages when compiled
- -10 for each 24 hours (or part of 24 hours) the submission is late After 5 days, there will be a -100 deduction

If the correct results are not produced for the published test case, the TA will try to fix your code. For each fix, there will be a deduction of -15. If the TA cannot fix your code, or if you code requires more than 4 fixes, you will get a -60 deduction.

There will be no deductions for mismatches where either the input is "subnormal" (has a biased exponent of 0), or the resulting floatx would be "subnormal" (have a biased exponent of 0). However, if you get all the subnormal test cases correct, you will get an extra 10 points added to your grade (up to but not exceeding 100).

