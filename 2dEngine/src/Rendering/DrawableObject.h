#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include "../Application/Utils/AppUtils.h"
#include "../Application/Object.h"
#include "../Application/Utils/Vector2D.h"
#include <vector>

/*
* An abstract class that holds the idea of a "draw box" it does not implement 
* draw Object because we dont know if it will just hold a blank color or some 
* sort of texture
*/
class AppUtils;
class DrawableObject : public Object
{
	public: 

		/*
		* Methods inherited from Object
		*/
		virtual string toString()
		{
			return "DrawableObject" + to_string( getMyId() );
		}

		virtual string dumpObject()
		{
			string currentState = "";
			
			currentState += "bounding boxes are\n";
			for (Vector2d vector : boundingBox)
			{
				currentState += vector.dumpObject();
			}
      currentState += "points are \n";
      for (Vector2d vector : points)
      {
        currentState += vector.dumpObject();
      }
			currentState += "Position\n";
			currentState += center.dumpObject();
			currentState += "Layer " + layer + '\n';
      currentState += "Clickable is " + to_string(clickable)+'\n';

			return currentState;
		}

		/* 
		* Default constructor
		*/
		DrawableObject()
		{
			// boundingBox will be empty and so will the points
			layer = 0;
			actualZ = 0;
      clickable = false;
		}

		DrawableObject(vector<Vector2d> points, int z)
		{
			layer = z;
			actualZ = layer * zDiff;
      setPoints(points);
		}

		virtual void drawObject() = 0;

    //Each object needs to have a unique ID for each instantiation
    //Because Drawable is abstract, it will not define this method though
    virtual int getMyId() = 0;

 		/*
		* getter and setter methods
		*/
		void setPoints(vector<Vector2d> newPoints)
		{
			points = newPoints;
       
      //Now find the center X
      float lowestX, highestX, lowestY, highestY;
      AppUtils::findLeastAndLargestX(points, lowestX, highestX);
      AppUtils::findLeastAndLargestY(points, lowestY, highestY);
 
      center.setX( ( (highestX - lowestX) / 2.f) + lowestX);
      center.setY( ( (highestY - lowestY) / 2.f) + lowestY);

      boundingBox.clear();
      boundingBox.push_back(Vector2d(lowestX, lowestY));
      boundingBox.push_back(Vector2d(highestX, lowestY));
      boundingBox.push_back(Vector2d(highestX, highestY));
      boundingBox.push_back(Vector2d(lowestX, highestY));
		}

    const vector<Vector2d> getBoundingBox()
    {
      return boundingBox;
    }

    const vector<Vector2d> getPoints()
    {
      return points;
    }

    bool removePoint(const Vector2d& point)
    {
      for (auto it = points.begin(); it != points.end(); it++)
      {
        //Because of floating point, they will not be exactly equal
        if (it->floatEquals(point))
        { 
          points.erase(it);
          setPoints(points);
          return true;
        }
      }
      return false;
    }

    bool movePoint( Vector2d& point, Vector2d& moveAmount)
    {
      for (auto it = points.begin(); it != points.end(); it++)
      {
        //Because of floating point, they will not be exactly equal
        if (it->floatEquals(point) )
        {
          *it = *it + moveAmount;
          point = point + moveAmount;
          setPoints(points);
          return true;
        }
      }
      return false;
    }

    void move(Vector2d& moveAmount)
    {
      for (auto it = points.begin(); it != points.end(); it++ )
      {
        *it = *it + moveAmount;
      }
      setPoints(points);
    }

    /*
    The Center does not have a setter because it is auto set off the 
    draw box
    */
		Vector2d getCenter()
		{
      return center;
		}

		int getLayer()
		{
			return layer;
		}

		void setLayer(int newLayer)
		{
			layer = newLayer;
			actualZ = layer * zDiff;
		}

    bool isClickable()
    {
      return clickable;
    }
    void setClickable(bool b){ clickable = b; }
    
    /*
    * Method that looks at a Vector2d of the mouse and returns true if we have been clicked on
    * This method will use a ray trace approach to determine if the mouse vector is inside
    * or outside of the drawable objects boundingBox
    */
    bool isClicked(Vector2d mousePosition);

	protected:
	
    /*
    * The bounding box on the object, is deliniated by a rectangle
    */
		vector<Vector2d> boundingBox;

    /*
    * The member that holds the information of what makes up the drawable
    * objects area. This allows for drawable objects to be triangles, or any
    * polygon
    */
    vector<Vector2d> points;

		/*
		* The objects center will be the average middle of the X positions,
    * and the middle of the Y positions
		*/
		Vector2d center;

		/*
		* Z buffering number that tells us what layer we are on
		*/
		int layer;

    /*
    Member that tells us if this object can be clicked
    */
    bool clickable;

		/*
		* To save a little bit of processing we will store the actual z position
		* so we dont have to keep doing float multiplication
		*/
		float actualZ;

		/*
		* The small amount of actual difference in the Z direction used to get layering
		*/
		static float zDiff;
};
#endif