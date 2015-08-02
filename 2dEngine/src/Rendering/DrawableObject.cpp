#include "DrawableObject.h"

float DrawableObject::zDiff = 0.000000000001f;

bool DrawableObject::isClicked(Vector2d mousePosition)
{
  bool isInside = false;
  // For no particular reason i will shoot the vector straight down on this ray trace
  for (unsigned int i = 0; i <points.size(); i++)
  {
    Vector2d vector = points[i];

    // Handle the case that our ray goes through an actual pointsVector (a corner)
    if (vector.getX() == mousePosition.getX())
    {
      isInside = !isInside;
      continue;
    }

    //check if it hits a "wall" which means we need the next vector in the box
    int secondPoint = i + 1;
    if (secondPoint >= points.size())
    {
      secondPoint = 0;
    }

    Vector2d secondVector = points[secondPoint];
       
    // Check that the mouse's X is lower than 1 vectors x and larger than one And that its Y position
    // is above the the Y on the line between the two vectors by using simple slope formula
    float yOnLine = (secondVector.getY() - vector.getY()) * 
                    ( (mousePosition.getX() - vector.getX()) / (secondVector.getX() - vector.getX()) ) 
                    + vector.getY();
    if ( (vector.getX() > mousePosition.getX()) != (secondVector.getX() > mousePosition.getX()) &&
          mousePosition.getY() > yOnLine )
    {
      isInside = !isInside;
    }
  }
  return isInside;
}
