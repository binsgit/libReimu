#include "StrStack.hpp"

#include <stdio.h>
#include <string.h>


namespace Alice {

	// resets the object to it's original state
	void StrStack::clear(){

		for (size_t i = 0; i < stackDepth; i++)
			free(*(buffer--));

	  	// set buffer to it's original size
		stackHead = buffer = (char**) realloc(buffer, 256);

		stackDepth = 0;

	}

	// doubles the size of the buffer
	void StrStack::grow(){

	  	// don't copy the contents of an empty buffer (shouldn't get called in this case...)
		if (stackDepth == 0)
			stackHead = buffer = (char**) realloc(buffer, ((1 << ++sizeFactor) * 256) * sizeof(char*));

		else {

			char** oldHead = stackHead;

		  	// make a new buffer twice as big as the old one
			char** buffer2 = stackHead = (char**) malloc(((1 << ++sizeFactor) * 256) * sizeof(char*));

		  	// copy all the strings into their new locations
		  	for (ssize_t i = 0; i < buffer - oldHead; i++)
				*(buffer2 + i) = *(oldHead + i);

			buffer2 += buffer - oldHead ;

			// delete the old buffer
		  	free(oldHead);

		  	// replace buffer with buffer2
		  	buffer = buffer2;

		}

	}


	// pushes a line to the top of the stack
	void StrStack::push(const char* str){
		if (!str)
			return;

		*buffer = (char*) malloc(strlen(str) + 1);

		// write the string to the buffer
		strcpy(*buffer, str);

		// go to next empty space
		buffer++;

		// if the size needs to be doubled after adding a new element
	  	if (stackDepth++ == (1u <<sizeFactor) * 256)
			grow();
	}



	// deletes the string at the top of the stack.
	void StrStack::pop(){
		if (stackDepth != 0) {
			// deallocate the line
			// & continue to point to the top
			free(*(--buffer));

			// decrease stack depth
			stackDepth--;

		}
	}


	// modify the top element
	void StrStack::changeTop(const char* str){
		if (stackDepth) {

			char*& topStr = *(buffer - 1);

			// change the size to fit the new string
			topStr = (char*) realloc(topStr, strlen(str) + 1);

			strcpy(topStr, str);

		}

	}

	void StrStack::toString(char** dest, size_t* space){

		// reallocate memory to fit data
		*space = totalLength();
		*dest = (char*) realloc((void*) *dest, *space);
		//memset(*dest, '-', *space);
		// begin copying in data
		char** buff = stackHead; // start from bottom of stack

		char* cpyto = *dest;

		// copy each string into *dest
		do {
			char* line = *buff;
			size_t len = strlen(line); // len of str + \n
			strcpy(cpyto, line);
			*(cpyto + len + 1) = '\n';
			cpyto += len;
		} while (++buff < buffer);

	}
}
