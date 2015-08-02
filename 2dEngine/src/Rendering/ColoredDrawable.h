#ifndef COLORED_DRAWABLE_H
#define COLORED_DRAWABLE_H

#include "DrawableObject.h"
#include "../Application/ApplicationState.h"

/*
* A class that implements Drawable object but only draws a simple colored object
* defined by a singular RGBA color
*/
class ColoredDrawable : public DrawableObject
{
	public:
		/*
		* Methods inherited from Object
		*/
		virtual string toString()
		{
			return "ColoredDrawable"+to_string(myId);
		}

		virtual string dumpObject()
		{
			string currentState = DrawableObject::dumpObject();
		
			currentState += "myId: " + myId + '\n';
      currentState += "red: " + to_string(r) + '\n';
      currentState += "green: " + to_string(g) + '\n';
      currentState += "blue: " + to_string(b) + '\n';
      currentState += "alpha: " + to_string(a) + '\n';

			return currentState;
		}

		/*
		* Method inherited from DrawableObject
		*/
		virtual void drawObject();

		ColoredDrawable() : DrawableObject()
		{
			id++;
			myId = id;
		}

		ColoredDrawable(vector<Vector2d> points, int z,
                    float red, float green, float blue, float alpha) : 
			DrawableObject( points, z )
		{
      r = red;
      g = green;
      b = blue;
      a = alpha;

			id++;
			myId = id;
		}

    /*
    * getter and setter methods
    */
    float getRed() { return r; }
    void setRed(float red) { r = red; }

    float getGreen() { return g; }
    void setGreen(float green) { g = green; }

    float getBlue() { return b; }
    void setBlue(float blue)  { b = blue; }

    float getAlpha() { return a; }
    void setAlpha(float alpha) { a = alpha; }

    int getMyId() { return myId; }

	protected:
    float r, g, b, a;

		/*
		* A static int that will allow us to identify the object. Possibly a bad idea
		*/
		static int id;
		int myId;
		
};
#endif