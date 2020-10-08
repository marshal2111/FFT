
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <list>
#include <vector>

using namespace std;

class Axis
{
private:
	int centerX, centerY;
	int lengthX, lengthY;
	char* nameOX, nameOY;
	//list<vector<double>> points;
	struct point 
	{
   		double x;
   		double y;
	};

	list<point> points;
	int pSize;
public:
	double scaleX, scaleY;
	Axis(int centerX, int centerY, int lengthX, int lengthY, double scaleX, double scaleY)
	{
		this->centerX = centerX;
		this->centerY = centerY;
		this->lengthX = lengthX;
		this->lengthY = lengthY;
		this->points = {};
		this->scaleX = scaleX;
		this->scaleY = scaleY;
		this->pSize = 2;
	}

	void setScaleX(double newScale)
	{
		this->scaleX = newScale;
	}

	void setPointSize(int size)
	{
		this->pSize = size;
	}

	void setScaleY(double newScale)
	{
		this->scaleY = newScale;
	}

	void setCoords(double x, double y)
	{
		//vector<double> coords(2);
		//coords[0] = x + this->centerX;
		//coords[1] = y + this->centerY;
		point coord;
		coord.x = x;
		coord.y = y;
		this->points.push_back(coord);
	}

	void draw(double cfcX, double cfcY)
	{
		glPointSize(10);
		glLineWidth(2.5); 
		glColor3f(0.0f, 0.0f, 0.0f);

		glBegin(GL_LINES);
		//draw axis
		glVertex3f(this->centerX-this->lengthX,this->centerY,0.0);
		glVertex3f(this->centerX+this->lengthX,this->centerY,0.0);
		glVertex3f(this->centerX, this->centerY-this->lengthY, 0.0);
		glVertex3f(this->centerX, this->centerY+this->lengthY, 0.0);

		for(int i = 0; i*this->scaleX*cfcX < this->lengthX; i++)
		{
			glVertex3f(this->centerX + i*scaleX*cfcX, this->centerY + 1.2, 0);
			glVertex3f(this->centerX + i*scaleX*cfcX, this->centerY - 1.2, 0);

			glVertex3f(this->centerX - i*scaleX*cfcX, this->centerY + 1.2, 0);
			glVertex3f(this->centerX - i*scaleX*cfcX, this->centerY - 1.2, 0);
		}
		for(int i = 0; i*this->scaleY*cfcY < this->lengthY; i++)
		{
			glVertex3f(this->centerX + 1.2, this->centerY - i*scaleY*cfcY, 0);
			glVertex3f(this->centerX - 1.2, this->centerY - i*scaleY*cfcY, 0);

			glVertex3f(this->centerX + 1.2, this->centerY + i*scaleY*cfcY, 0);
			glVertex3f(this->centerX - 1.2, this->centerY + i*scaleY*cfcY, 0);
		}
		
		glEnd();

		//draw points
		glColor3f(0.0f, 0.749f, 1.0f);
		glPointSize(this->pSize);	
		glBegin(GL_POINTS);
		for (auto iter = points.begin(); iter != points.end(); iter++)
    	{
        	point coord = *iter;
        	if ((coord.x*this->scaleX < this->lengthX)&(coord.y*this->scaleY < this->lengthY)& 
        		(coord.x*this->scaleX > -(this->lengthX))&(coord.y*this->scaleY > -(this->lengthY)))
        		//(coord.x*this->scaleX > -lengthX)&(coord.y*this->scaleY > -lengthY))
        		glVertex3f(coord.x*this->scaleX+this->centerX, coord.y*this->scaleY+this->centerY, 0.0);
    	}

		glEnd();
	}
};