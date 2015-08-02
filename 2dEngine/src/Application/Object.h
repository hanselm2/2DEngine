#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>

using namespace std;

class Object
{
	public:
		/*
		* Will dump the objects current values of all of its members
		*/
		virtual string dumpObject() = 0;

		/*
		* Will dump the objects class type as a string
		*/
		virtual string toString() = 0;
		
	protected:

	private:
		
};
#endif