#include "nanolibc/libc.h"

#define WASM_EXPORT __attribute__((visibility("default"))) extern "C"
 
WASM_EXPORT char* get_memory_for_string(int size) {
	return new char[size];
}


WASM_EXPORT void free_memory_for_string(char* str) {
	delete[] str;
}


WASM_EXPORT int string_to_int(const char* str) {
	int i = 0U;
	while ('0' <= *str && *str <= '9') {
		i = i * 10 + (int)((*str++) - '0');
	}
	return i;
}

WASM_EXPORT void debug_dump_memory() {
	dump_memory();
}


// This function prints the factorial of a number.
//
// Used to test malloc/realloc/free.
WASM_EXPORT void factorial(int factorial) {
	//This is the number of digits stored in each int
	//The formatting string must reflect this value
	int digitvalue = 1000;
	const char* digitformatstring = "%03d";

	unsigned int maxdigits = 5 + factorial / 30;
	unsigned int* digits = (unsigned int*) calloc(maxdigits,sizeof(int));
	unsigned int* result = (unsigned int*) malloc(maxdigits*sizeof(int));

	//We begin with digits==1 and then multiply number
	//by number to digits
	digits[0]=1;
	for (unsigned int f=2;f<=factorial;++f) {
		//Perform the multiplication and store the result in
		//the result array
		memset(result,0,maxdigits);
		unsigned int memory=0; //Amount to carry over to the next digit
		for (unsigned int pos = 0; pos < maxdigits; pos++)
	        {
	        	unsigned int digit=0;
	        	//For each digit in f
	        	unsigned int ftemp = f;
	        	for (int r=0; r<=pos; ++r) {
	        		digit += digits[pos-r] * (ftemp%digitvalue);
	        		ftemp/=digitvalue;
	        	}
	        	digit+=memory;
	        	result[pos] = digit %digitvalue;
            		memory = digit/digitvalue;
		}

		//Now copy the result to the digits
		memcpy(digits,result,maxdigits*sizeof(int));


		//Is it needed to increase the buffer?
		//How many digits are left?
		unsigned int digitsleft=0;
		unsigned int p=maxdigits-1;
		while (digits[p--]==0) 	digitsleft++;
		if (digitsleft < 10) {
			//Make the buffers larger
			maxdigits += 10 + factorial % 7;
			digits = (unsigned int*) realloc(digits,maxdigits*sizeof(int));
			//Set the new part of the buffer to zero
			for (int i=p+2;i<maxdigits;++i)
				digits[i]=0;

			free(result); //no need to realloc result
			result = (unsigned int*) malloc(maxdigits*sizeof(int));

			//printf("Buffer increased to %d.\n",maxdigits);
		}
	}

	printf("%d! = ",factorial);
	int prnt=0;
	for (int d=maxdigits-1;d>=0;--d) {
		if (digits[d]!=0 && !prnt) {
			//Print the first digit (without zero-padding)
			printf("%d",digits[d]);
			prnt=1;
		}
		else if (prnt) {
			//Print the following digits (with zero-padding)
			printf(digitformatstring,digits[d]);
		}
	}

	free(digits);
	free(result);
	printf("\n");
}
