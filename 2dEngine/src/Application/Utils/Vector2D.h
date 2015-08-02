#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "../Object.h"

/*
* A class that will hold the mathematical idea of a 2 d vector 
* this class will hold the operators and methods that are standard
* to vectors 
*/
class Vector2d : public Object
{
	public:
		/*
		* Methods inherited from Object
		*/
		virtual string toString()
		{
			return "Vector2d";
		}

		virtual string dumpObject()
		{
			string currentState = "";
			currentState += "x: " + to_string(x) + '\n';
			currentState += "y: " + to_string(y) + '\n';
			return currentState;
		}

		Vector2d()
		{
			x = 0;
			y = 0;
		}

		Vector2d(float myX, float myY)
		{
			x = myX;
			y = myY;
		}

		/*
		* returns the distance from the instantiantion of this object and the vector given
		*/
		float distance(const Vector2d& other);

		/*
		* returns the magnitude of this current vector (x^2) + (y^2) square rooted
		*/
		float magnitude();

		/*
		* operators between this vector and another
		*/
		Vector2d operator * (float number) const;
		Vector2d operator / (float number) const;
		Vector2d operator + (float number) const;
		Vector2d operator + (Vector2d other) const;
		Vector2d operator - (float number) const;
		Vector2d operator - (Vector2d other) const;
		bool operator == (const Vector2d& other) const;
    /*
    Because floating point isnt perfect, we check to see if their less than 10^-5 close to eachother
    */
    bool floatEquals(const Vector2d& other) const;

		/*
		* Normalize makes it so the object has a magnitude of 1 and rescales the 
		* x and y so that this is true
		*/
		void normalize();

		/*
		* Dot product projects one vector onto the second
		*/
		float dotProduct(const Vector2d& other) const;

    /*
    A utility function that will tell us if a vector2d is within a square of this vector
    give the size of the square
    */
    bool isWithinSquareRange(const Vector2d& other, float distance);

		/*
		* getter and setter for x and y
		*/
		float getX() { return x; }
		void setX(float newX) { x = newX; }

		float getY() { return y; }
		void setY(float newY) { y = newY; }
				
	private:
		float x, y;

};
#endif