
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include <iostream>
#include <vector>

using namespace std;

class Axis
{
private:

	int centerX, centerY;
	int lengthX, lengthY;

	struct point 
	{
   		double x;
   		double y;
	};

	vector<point> points;
	int pSize;

public:
	double scaleX, scaleY;
	double dX, dY;

	Axis(int centerX, int centerY, int lengthX, int lengthY, double scaleX, double scaleY)
	{
		this->centerX = centerX;
		this->centerY = centerY;
		this->lengthX = lengthX;
		this->lengthY = lengthY;
		this->points = {};
		this->scaleX = scaleX;
		this->scaleY = scaleY;
		this->dX = 1;
		this->dY = 1;
		this->pSize = 2;
	}

	void setScaleX(double newScale)
	{
		this->scaleX = newScale;
	}

	void setScaleY(double newScale)
	{
		this->scaleY = newScale;
	}

	void setPointSize(int size)
	{
		this->pSize = size;
	}

	void setCoords(double x, double y)
	{
		point coord;
		coord.x = x;
		coord.y = y;
		this->points.push_back(coord);
	}

	void draw()
	{
		glPointSize(10);
		glLineWidth(2.5); 
		glColor3f(0.0f, 0.0f, 0.0f);

		glBegin(GL_LINES);

		//draw axis

		glVertex3f(this->centerX - this->lengthX, this->centerY, 0.0);
		glVertex3f(this->centerX + this->lengthX, this->centerY, 0.0);
		glVertex3f(this->centerX, this->centerY - this->lengthY, 0.0);
		glVertex3f(this->centerX, this->centerY + this->lengthY, 0.0);

		for(int i = 0; i*this->scaleX * dX < this->lengthX; i++)
		{
			glVertex3f(this->centerX + i * scaleX * dX, this->centerY + 1.2, 0);
			glVertex3f(this->centerX + i * scaleX * dX, this->centerY - 1.2, 0);

			glVertex3f(this->centerX - i * scaleX * dX, this->centerY + 1.2, 0);
			glVertex3f(this->centerX - i * scaleX * dX, this->centerY - 1.2, 0);
		}

		for(int i = 0; i*this->scaleY * dY < this->lengthY; i++)
		{
			glVertex3f(this->centerX + 1.2, this->centerY - i * scaleY * dY, 0);
			glVertex3f(this->centerX - 1.2, this->centerY - i * scaleY * dY, 0);

			glVertex3f(this->centerX + 1.2, this->centerY + i * scaleY * dY, 0);
			glVertex3f(this->centerX - 1.2, this->centerY + i * scaleY * dY, 0);
		}
		
		glEnd();

		//draw points

		glColor3f(0.0f, 0.749f, 1.0f);
		glPointSize(this->pSize);	

		glBegin(GL_POINTS);
		for (auto iter = points.begin(); iter != points.end(); iter++)
    	{
        	point coord = *iter;
        	if ((coord.x * this->scaleX < this->lengthX) & (coord.y * this->scaleY < this->lengthY) & 
        		(coord.x * this->scaleX > -(this->lengthX)) & (coord.y * this->scaleY > -(this->lengthY)))
        		//(coord.x*this->scaleX > -lengthX)&(coord.y*this->scaleY > -lengthY))
        		glVertex3f(coord.x * this->scaleX + this->centerX, coord.y * this->scaleY + this->centerY, 0.0);
    	}

		glEnd();
	}
};