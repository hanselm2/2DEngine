#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>

#include "Object.h"
#include "Vector2D.h"
#include "ApplicationState.h"
#include "AppUtils.h"

/*
Quad tree that will be used to store things with spacial properties
so we can do things like get me the 4 objects closest to this point

NOTE: We are assuming that we will keep the Application to have 10 
World View units in the Y Direction and ApplicationState's getInitialRatio
* 10 in the X Direction
*/
template< class T>
class QuadTree : public Object
{
  public:
    /*
    * Methods inherited from Object
    */
    virtual string toString()
    {
      return "QuadTree<T>";
    }

    virtual string dumpObject()
    {
      //TODO
      return "";
    }

    QuadTree<T>()
    {
      root = new Node();
      root->nwChild = NULL;
      root->neChild = NULL;
      root->swChild = NULL;
      root->seChild = NULL;
      root->myData = NULL;
      root->myCenter = Vector2d(0.f, 0.f);

      float yLength = ApplicationState::Instance().getCameraZ();
      float xLength = yLength * ApplicationState::Instance().getAspectRatio();
      root->boundingBox = Vector2d(xLength, yLength);
    }

    QuadTree<T>(float xSize, float ySize)
    {
      root = new Node();
      root->nwChild = NULL;
      root->neChild = NULL;
      root->swChild = NULL;
      root->seChild = NULL;
      root->myData = NULL;
      root->myCenter = Vector2d(0.f, 0.f);
      root->boundingBox = Vector2d(xSize, ySize);

      //Terrible code standard by giving it a float literal, probably going to change it in the future
      yLength = ySize/2.f;
      xLength = yLength * ApplicationState::Instance().getAspectRatio();
    }
    
    QuadTree<T>& operator=(const QuadTree<T>& rhs)
    {
      if (this != &rhs)
      {
        clear();
        copy(*rhs);
      }
      return *this;
    }

    ~QuadTree<T>()
    {
      clear();
    };

    void insert(T& data)
    {
      float lowestX, highestX, lowestY, highestY;
      AppUtils::findLeastAndLargestX(data.getBoundingBox(), lowestX, highestX);
      AppUtils::findLeastAndLargestY(data.getBoundingBox(), lowestY, highestY);

      if (lowestX < root->myCenter.getX())
      {
        if (lowestY < root->myCenter.getY())
        {
          if (root->swChild == NULL)
          {
            root->swChild = new Node();
            root->swChild->myCenter = Vector2d((root->myCenter.getX() - (root->boundingBox.getX() / 2.f)),
              (root->myCenter.getY() - (root->boundingBox.getY() / 2.f)));
            root->swChild->boundingBox = Vector2d((root->boundingBox.getX() / 2.f), (root->boundingBox.getY() / 2.f));
          }
          recursiveInsert(*(root->swChild), data);
        }
        //Not else so that an object can be in multiple nodes
        if (highestY > root->myCenter.getY())
        {
          if (root->nwChild == NULL)
          {
            root->nwChild = new Node();
            root->nwChild->myCenter = Vector2d((root->myCenter.getX() - (root->boundingBox.getX() / 2.f)),
              (root->myCenter.getY() + (root->boundingBox.getY() / 2.f)));
            root->nwChild->boundingBox = Vector2d((root->boundingBox.getX() / 2.f), (root->boundingBox.getY() / 2.f));
          }
          recursiveInsert(*(root->nwChild), data);
        }
      }
      if (highestX > root->myCenter.getX())
      {
        if (lowestY < root->myCenter.getY())
        {
          if (root->seChild == NULL)
          {
            root->seChild = new Node();
            root->seChild->myCenter = Vector2d((root->myCenter.getX() + (root->boundingBox.getX() / 2.f)),
              (root->myCenter.getY() - (root->boundingBox.getY() / 2.f)));
            root->seChild->boundingBox = Vector2d((root->boundingBox.getX() / 2.f), (root->boundingBox.getY() / 2.f));
          }
          recursiveInsert(*(root->seChild), data);
        }
        //Not else so that an object can be in multiple nodes
        if (highestY > root->myCenter.getY())
        {
          if (root->neChild == NULL)
          {
            root->neChild = new Node();
            root->neChild->myCenter = Vector2d((root->myCenter.getX() + (root->boundingBox.getX() / 2.f)),
              (root->myCenter.getY() + (root->boundingBox.getY() / 2.f)));
            root->neChild->boundingBox = Vector2d((root->boundingBox.getX() / 2.f), (root->boundingBox.getY() / 2.f));
          }
          recursiveInsert(*(root->neChild), data);
        }
      }
    }

    /*
    Searches the entire tree and uses the getMyId() method to find 
    peice of data we want, returns true if it found the object
    */
    bool remove(T& data)
    {
      return remove(data.getMyId());
    }
    bool remove(int Id)
    {
      if (root == NULL)
        return false;

      return recursiveRemove(*root, Id);
    }

    /*
    function that should be called whenever the position of the objects
    in the quadTreeChanges.

    Simply walks the entire tree and checks all the nodes data to make
    sure its still valid. If not it removes that data from the tree then 
    re-inserts
    */
    void update()
    {
      if (root == NULL)
        return;

      recursiveUpdate(*root);
    }

    /*
    returns the object closest to the point, the real reason why this data
    structure even exists. Should run in O(logn)
    */
    T* findClosest(Vector2d point)
    {
      if (root == NULL)
        return NULL;

      return recursiveSearch(*root, point);
    }

    /*
    A function that will draw ALL of the bounding boxes for the entire quadtree
    Usefull for debugging and visualizing our problems
    */
    void drawTree()
    {
      if (root == NULL)
        return;
      recursiveDraw(*root);
    }

    /*
    A method to return all the objects that have any points inside of a given rectangle
    params: startPoint is the lowest of the x and y of the rectangle
            endPoint is the highest x and y of the rectangle
    */
    vector<T*> getObjectsInRectange(Vector2d startPoint, Vector2d endPoint)
    {

    }
      
  private:

    struct Node
    {
      Node* nwChild;
      Node* neChild;
      Node* swChild;
      Node* seChild;
      Node* parent;
      T* myData;
      Vector2d myCenter;
      Vector2d boundingBox;

      Node()
      {
        nwChild = NULL;
        neChild = NULL;
        swChild = NULL;
        seChild = NULL;
        myData = NULL;
        parent = NULL;
      }
    };

    Node* root;

    // Helper functions for recursive calls
    void recursiveInsert(Node& node, T& data)
    {
#if DEBUG
      cout << "in recursive Insert for data " + to_string(data.getMyId()) << endl;
#endif
      //Check to see if this is a new node, if it is then were done recursing
      if (node.myData == NULL && node.nwChild == NULL && node.neChild == NULL &&
        node.swChild == NULL && node.seChild == NULL)
      {
#if DEBUG
        cout << "setting data on this node" << endl;
#endif
        node.myData = &data;
        return;
      }

      //If its a leaf node we need to split it into 4
      if (isLeafNode(node))
      {
        T* oldData = node.myData;
        node.myData = NULL;
#if DEBUG
        cout << "Found Leaf Node myData is ";
        for (Vector2d vector : oldData->getBoundingBox())
          cout << vector.dumpObject();
#endif

        float lowestX, highestX, lowestY, highestY;
        AppUtils::findLeastAndLargestX(oldData->getBoundingBox(), lowestX, highestX);
        AppUtils::findLeastAndLargestY(oldData->getBoundingBox(), lowestY, highestY);
        if (lowestX < node.myCenter.getX())
        {
          if (lowestY < node.myCenter.getY())
          {
            if (node.swChild == NULL)
            {
              node.swChild = new Node();
              node.swChild->myCenter = Vector2d((node.myCenter.getX() - (node.boundingBox.getX() / 2.f)),
                (node.myCenter.getY() - (node.boundingBox.getY() / 2.f)));
              node.swChild->boundingBox = Vector2d((node.boundingBox.getX() / 2.f), (node.boundingBox.getY() / 2.f));
            }
            recursiveInsert(*(node.swChild), *oldData);
          }
          //Not else so that an object can be in multiple nodes
          if (highestY > node.myCenter.getY())
          {
            if (node.nwChild == NULL)
            {
              node.nwChild = new Node();
              node.nwChild->myCenter = Vector2d((node.myCenter.getX() - (node.boundingBox.getX() / 2.f)),
                (node.myCenter.getY() + (node.boundingBox.getY() / 2.f)));
              node.nwChild->boundingBox = Vector2d((node.boundingBox.getX() / 2.f), (node.boundingBox.getY() / 2.f));
            }
            recursiveInsert(*(node.nwChild), *oldData);
          }
        }
        if (highestX > node.myCenter.getX())
        {
          if (lowestY < node.myCenter.getY())
          {
            if (node.seChild == NULL)
            {
              node.seChild = new Node();
              node.seChild->myCenter = Vector2d((node.myCenter.getX() + (node.boundingBox.getX() / 2.f)),
                (node.myCenter.getY() - (node.boundingBox.getY() / 2.f)));
              node.seChild->boundingBox = Vector2d((node.boundingBox.getX() / 2.f), (node.boundingBox.getY() / 2.f));
            }
            recursiveInsert(*(node.seChild), *oldData);
          }
          //Not else so that an object can be in multiple nodes
          if (highestY > node.myCenter.getY())
          {
            if (node.neChild == NULL)
            {
              node.neChild = new Node();
              node.neChild->myCenter = Vector2d((node.myCenter.getX() + (node.boundingBox.getX() / 2.f)),
                (node.myCenter.getY() + (node.boundingBox.getY() / 2.f)));
              node.neChild->boundingBox = Vector2d((node.boundingBox.getX() / 2.f), (node.boundingBox.getY() / 2.f));
            }
            recursiveInsert(*(node.neChild), *oldData);
          }
        }
      }

      //Now handle inserting the actual data
      float lowestX, highestX, lowestY, highestY;
      AppUtils::findLeastAndLargestX(data.getBoundingBox(), lowestX, highestX);
      AppUtils::findLeastAndLargestY(data.getBoundingBox(), lowestY, highestY);
      if (lowestX < node.myCenter.getX())
      {
        if (lowestY < node.myCenter.getY())
        {
          if (node.swChild == NULL)
          {
            node.swChild = new Node();
            node.swChild->myCenter = Vector2d((node.myCenter.getX() - (node.boundingBox.getX() / 2.f)),
              (node.myCenter.getY() - (node.boundingBox.getY() / 2.f)));
            node.swChild->boundingBox = Vector2d((node.boundingBox.getX() / 2.f), (node.boundingBox.getY() / 2.f));
          }
          recursiveInsert(*(node.swChild), data);
        }
        //Not else so that an object can be in multiple nodes
        if (highestY > node.myCenter.getY())
        {
          if (node.nwChild == NULL)
          {
            node.nwChild = new Node();
            node.nwChild->myCenter = Vector2d((node.myCenter.getX() - (node.boundingBox.getX() / 2.f)),
              (node.myCenter.getY() + (node.boundingBox.getY() / 2.f)));
            node.nwChild->boundingBox = Vector2d((node.boundingBox.getX() / 2.f), (node.boundingBox.getY() / 2.f));
          }
          recursiveInsert(*(node.nwChild), data);
        }
      }
      if (highestX > node.myCenter.getX())
      {
        if (lowestY < node.myCenter.getY())
        {
          if (node.seChild == NULL)
          {
            node.seChild = new Node();
            node.seChild->myCenter = Vector2d((node.myCenter.getX() + (node.boundingBox.getX() / 2.f)),
              (node.myCenter.getY() - (node.boundingBox.getY() / 2.f)));
            node.seChild->boundingBox = Vector2d((node.boundingBox.getX() / 2.f), (node.boundingBox.getY() / 2.f));
          }
          recursiveInsert(*(node.seChild), data);
        }
        //Not else so that an object can be in multiple nodes
        if (highestY > node.myCenter.getY())
        {
          if (node.neChild == NULL)
          {
            node.neChild = new Node();
            node.neChild->myCenter = Vector2d((node.myCenter.getX() + (node.boundingBox.getX() / 2.f)),
              (node.myCenter.getY() + (node.boundingBox.getY() / 2.f)));
            node.neChild->boundingBox = Vector2d((node.boundingBox.getX() / 2.f), (node.boundingBox.getY() / 2.f));
          }
          recursiveInsert(*(node.neChild), data);
        }
      }
    }

    bool recursiveRemove(Node& node, int Id)
    {
      if (node.myData != NULL && *(node.myData).getMyId() == Id)
      {
        node.MyData == NULL;
        return true;
      }

      if (node.nwChild != NULL && recursiveRemove(*(node.nwChild), Id))
        return true;
      if (node.swChild != NULL && recursiveRemove(*(node.swChild), Id))
        return true;
      if (node.seChild != NULL && recursiveRemove(*(node.seChild), Id))
        return true;
      if (node.neChild != NULL && recursiveRemove(*(node.neChild), Id))
        return true;

      return false;
    }

    void recursiveUpdate(Node& node)
    {
      if (node.myData != NULL && !dataBelongsInNode(node))
      {
        insert(*(node.myData));
        node.myData = NULL;
      }

      if (node.nwChild != NULL)
        recursiveUpdate(*(node.nwChild));
      if (node.swChild != NULL)
        recursiveUpdate(*(node.swChild));
      if (node.neChild != NULL)
        recursiveUpdate(*(node.neChild));
      if (node.seChild != NULL)
        recursiveUpdate(*(node.seChild));
    }

    T* recursiveSearch(Node& node, Vector2d point) const
    {
      
      if (isLeafNode(node))
      {
        return node.myData;
      }

      if (point.getX() <= node.myCenter.getX())
      {
        if (point.getY() < node.myCenter.getY() && node.swChild != NULL)
          return recursiveSearch(*(node.swChild), point);
        else if (node.nwChild != NULL)
          return recursiveSearch(*(node.nwChild), point);
      }
      else
      {
        if (point.getY() < node.myCenter.getY() && node.seChild != NULL)
          return recursiveSearch(*(node.seChild), point);
        else if (node.neChild != NULL)
          return recursiveSearch(*(node.neChild), point);
      }
      return NULL;
    }

    void recursiveDraw(Node& node) const
    {     
      float lowX, highX, lowY, highY;
      lowX = node.myCenter.getX() - node.boundingBox.getX();
      highX = node.myCenter.getX() + node.boundingBox.getX();
      lowY = node.myCenter.getY() - node.boundingBox.getY();
      highY = node.myCenter.getY() + node.boundingBox.getY();

      glLineWidth(1.f);
      glColor3f(1.f, 0.f, 0.f);
      glBegin(GL_LINE_STRIP);
        glVertex3f(lowX, lowY, 0.f);
        glVertex3f(highX, lowY, 0.f);
        glVertex3f(highX, highY, 0.f);
        glVertex3f(lowX, highY, 0.f);
        glVertex3f(lowX, lowY, 0.f);
      glEnd();
      glFlush();

      if (node.neChild != NULL)
        recursiveDraw(*(node.neChild));
      if (node.nwChild != NULL)
        recursiveDraw(*(node.nwChild));
      if (node.swChild != NULL)
        recursiveDraw(*(node.swChild));
      if (node.seChild != NULL)
        recursiveDraw(*(node.seChild));

    }
    
    bool isLeafNode(Node& node) const
    {
#if DEBUG
      if (node.myData == NULL)
        cout << "myData is NULL" << endl;
      if (node.nwChild == NULL)
        cout << "nwChild isNULL" << endl;
      if (node.seChild == NULL)
        cout << "seChild isNULL" << endl;
      if (node.neChild == NULL)
        cout << "neChild isNULL" << endl;
      if (node.swChild == NULL)
        cout << "swChild isNULL" << endl;
#endif
      if (node.myData == NULL || node.nwChild != NULL || node.seChild != NULL ||
          node.neChild != NULL || node.swChild != NULL)
        return false;
      return true;
    }
    bool dataBelongsInNode(Node& node) const
    {
      if (node.myData == NULL)
        return false;

      T data = *(node.myData);
      float lowestX, highestX, lowestY, highestY;
      AppUtils::findLeastAndLargestX(data.getBoundingBox(), lowestX, highestX);
      AppUtils::findLeastAndLargestY(data.getBoundingBox(), lowestY, highestY);

      float lowestBoundX, highestBoundX, lowestBoundY, highestBoundY;
      lowestBoundX = node.myCenter.getX() - node.boundingBox.getX();
      highestBoundX = node.myCenter.getX() + node.boundingBox.getX();
      lowestBoundY = node.myCenter.getY() - node.boundingBox.getY();
      highestBoundY = node.myCenter.getY() + node.boundingBox.getY();
      if (highestX < lowestBoundX || lowestX > highestBoundX)
        return false;
      if (highestY < lowestBoundY || lowestY > highestBoundY)
        return false;
    }

    /*
    Copy and clear are helper functions used by the copy constructor
    and the destructor
    */
    void copy( QuadTree<T>& other)
    {
      if (root != NULL)
      {
        this->clear()
      }

      root = new Node();
      //They will be pointing to the same reference
      root->myData = other.root->myData;

      root->nwChild = new Node();
      root->neChild = new Node();
      root->swChild = new Node();
      root->seChild = new Node();

      *(root->nwChild->parent) = root;
      *(root->neChild->parent) = root;
      *(root->swChild->parent) = root;
      *(root->seChild->parent) = root;

      copySubNode(*(root->nwChild), *(other.root->nwChild));
      copySubNode(*(root->neChild), *(other.root->neChild));
      copySubNode(*(root->swChild), *(other.root->swChild));
      copySubNode(*(root->seChild), *(other.root->seChild));
    }
    void clear()
    {
      if (root == NULL)
      {
        return;
      }

      clearSubNode(*root);
      delete root;
      root = NULL;
    }

    void copySubNode(Node& ourNode, Node& node)
    {
      if (node == NULL)
      {
        return;
      }

      //They will be pointing to the same reference
      ourNode.myData = node.myData;

      ourNode.nwChild = new Node();
      ourNode.neChild = new Node();
      ourNode.swChild = new Node();
      ourNode.seChild = new Node();

      *(ourNode.nwChild->parent) = ourNode;
      *(ourNode.neChild->parent) = ourNode;
      *(ourNode.swChild->parent) = ourNode;
      *(ourNode.seChild->parent) = ourNode;

      copySubNode(*ourNode.nwChild, *(node.nwChild));
      copySubNode(*ourNode.neChild, *(node.neChild));
      copySubNode(*ourNode.swChild, *(node.swChild));
      copySubNode(*ourNode.seChild, *(node.seChild));
    }

    void clearSubNode(Node& node)
    {
      if (node.nwChild != NULL)
      {
        clearSubNode(*(node.nwChild));
        delete node.nwChild;
        node.nwChild = NULL;
      }
      if (node.neChild != NULL)
      {
        clearSubNode(*(node.neChild));
        delete node.neChild;
        node.neChild = NULL;
      }
      if (node.swChild != NULL)
      {
        clearSubNode(*(node.swChild));
        delete node.swChild;
        node.swChild = NULL;
      }
      if (node.seChild != NULL)
      {
        clearSubNode(*(node.seChild));
        delete node.seChild;
        node.seChild = NULL;
      }

      // Now delete the actual data
      if (node.myData != NULL)
      {
        delete node.myData;
        node.myData = NULL;
      }

      if (node.parent != NULL)
      {
        node.parent = NULL;
      }

    }
};

#endif