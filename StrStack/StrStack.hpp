/*
    This file is part of libReimu.
    Copyright (C) 2016-2017  Dustin Van Tate Testa <toast27@gmail.com>

    libReimu is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libReimu is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libReimu.  If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef STRING_STACK_H
#define STRING_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

namespace Alice {

	// like std::stack<char*> but better
	class StrStack {

	public:
		// how many times to double the number of lines
		uint8_t sizeFactor;

		// points to element above top of stack
		char** buffer;
		// number of elements in the stack
		size_t stackDepth;

		// the first elements in the stack
		char** stackHead;


		StrStack():
			sizeFactor(0),
			buffer((char**) malloc(256 * sizeof(char*))),
			stackDepth(0),
			stackHead(buffer)
		{
			*buffer = NULL;
		}

		// copy constructor
		StrStack(const StrStack& cpy):
			sizeFactor(cpy.sizeFactor),
			buffer((char**) malloc((1 << cpy.sizeFactor) * 256 * sizeof(char*))),
			stackDepth(cpy.stackDepth),
			stackHead(buffer)
		{
			*buffer = NULL;
			char** sh = cpy.stackHead;
			while (sh != cpy.buffer) {
				*buffer = (char*) malloc ( strlen(*sh) + 1 );
				strcpy(*buffer++, *sh++);
			}

		}

		~StrStack(){
			for (; stackDepth > 0; stackDepth--)
				free(*(--buffer));

			free(stackHead);
		}

		// copy
		StrStack& operator=(const StrStack& cpy){
			sizeFactor = cpy.sizeFactor;
			buffer = (char**) malloc((1 << cpy.sizeFactor) * 256 * sizeof(char*));
			stackDepth = cpy.stackDepth;
			stackHead = buffer;

			char** sh = cpy.stackHead;
			while (sh != cpy.buffer) {
				*buffer = (char*) malloc ( strlen(*sh) + 1 );
				strcpy(*buffer++, *sh++);
			}

			return *this;
		}

		// 0 = bottom of stack
		char* at(const ssize_t index) {
			if ((size_t)abs(index) >= stackDepth)
				throw "StrStack[] index out of bounds";

			return index >= 0 ? *(stackHead + index) : *(buffer + index - 1);
		}
		char* operator[](const ssize_t index) {
			return at(index);
		}

		// deletes all strings
		void clear();

		/// doubles the size of the buffer
		void grow();

		/// pushes a line to the top of the stack
		void push(const char* str);

		/// deletes the string at the top of the stack.
		void pop();

		/// the string at the top of the stack
		char* top()
			{ return stackDepth ? *(buffer - 1) : NULL; }

		/// the number of strings being stored
		size_t size()
			{ return stackDepth; }

		// modify the top element
		void changeTop(const char* str);
		void top(const char* str)
			{ return changeTop(str);}

		/// the number of characters in all the strings stored in the stack
		size_t totalLength(){
			char** buff = buffer;

			// start with one to count the '\0' char at the end
			size_t ret = 1;

			while (buff-- > stackHead)
				ret += strlen(*buff);

			return ret;

		}

		// converts the block to a string
		void toString(char** dest, size_t* size);

		/// prints the contents of a string stack
		static inline void printStrStack(const StrStack& stack){
			// start from top element (this one is empty)
			char** buff = stack.buffer;

			unsigned num = 0;
			// skip the first element (empty) and print the rest of the stack
			while (buff-- > stack.stackHead)
				printf("%d: %s\n", num++, *buff);

		}

		/// concatenates 2 stacks together
		static inline void appendToStack(StrStack& out, const StrStack& in){
			// bottom of stack1 -> top of stack1 -> bottom of stack2 -> top of stack2
			char** head = in.stackHead;
			out.push(*(head));
			while (head++ < in.buffer)
				out.push(*head);
		}
	};
}

#endif
