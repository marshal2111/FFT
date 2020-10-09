#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "Headers/Signal.h"
#include "Headers/transforms.h"
#include <complex>

#include <iostream>

int init();
void processInput(GLFWwindow* window);
void display();
template <typename T> int sgn(T val);
template <typename T> int Heaviside(T x);
template <typename T> double Rect(T x, double d, double l);
int toDegree (double N);

GLFWwindow* window;
Signal func(150, 380, 100, 100 , 20, 50);
Signal freq(150, 150, 100, 100, 0.1, 10);
Signal magn(380, 150, 100, 100, 1, 10);

int main()
{
	init();

	double fs = 200;
	double f = 1000;
	int N = 512;
	int bN = 16384;
	double axisCoeff = f / (fs / bN);

	double left =  -10;
	double right = 16;
	double length = right - left;
	double ts = 1 / fs;
	int nulls = bN - N;

	magn.dX = M_PI/4;
	freq.dX = axisCoeff;

	for (double t = left; t <= right; t += ts)
	{		
		func.setCoords(t, Rect(t, 2, 1));
	}

	vector<double> values(N + nulls, 0);

	for (int i = 0; i<N; i++)
	{
		values[i] = Rect(i*ts, 2, 1);
	}

	vector<complex<double>> dftCoeff = FFT(values);

	freq.setPointSize(3);
	for (double x = 1; x <= N + nulls; x++)
	{
		freq.setCoords(x, abs(dftCoeff[x]));
		freq.setCoords(-x, abs(dftCoeff[x]));
	} 

	magn.setPointSize(3);
	for (double x = 1; x <= N + nulls; x++)
	{
		magn.setCoords(x, arg(dftCoeff[x]));
		magn.setCoords(-x, -arg(dftCoeff[x]));
	} 

	while(!glfwWindowShouldClose(window))
	{	
		glfwPollEvents();
		processInput(window);

		display();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

void display()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	func.draw();
	freq.draw();
	magn.draw();
}

int toDegree (double N)
{
	int x = 1;
	while (x < N)
	{
		x = x*2;
	}
	return x;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        std::cout << "Closed by esc" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        func.setScaleX(func.scaleX * 1.001);
        freq.setScaleX(freq.scaleX * 1.001);
        magn.setScaleX(magn.scaleX * 1.001);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        func.setScaleX(func.scaleX * 0.999);
        freq.setScaleX(freq.scaleX * 0.999);
        magn.setScaleX(magn.scaleX * 0.999);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        func.setScaleY(func.scaleY * 1.001);
        freq.setScaleY(freq.scaleY * 1.001);
        magn.setScaleY(magn.scaleY * 1.001);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        func.setScaleY(func.scaleY * 0.999);
        freq.setScaleY(freq.scaleY * 0.999);
        magn.setScaleY(magn.scaleY * 0.999);
    }
}

int init()
{
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit( EXIT_FAILURE );
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);	

	window = glfwCreateWindow( 1280, 720, "FFT", NULL, NULL );

	glfwSetWindowPos(window, 100, 200);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit())
	{
		std::cout << "Failed to init GLEW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	if (!window)
	{
		fprintf( stderr, "Failed to open GLFW window\n" );
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport( 0, 0, width, height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glOrtho(0, 500, 0, 500, 0.0, 1.0); 

    return 0;
}

template <typename T> int sgn(T val) 
{
    return (T(0) < val) - (val < T(0));
}

template <typename T> double Rect(T x, double d, double l)
{
	double res;
	if (abs(x-d) > l/2)
		res = 0;
	if (abs(x-d) < l/2)
		res = 1;
	if (abs(abs(x-d) - l/2) < 0.001)
		res = 0.5;
	return res;
}

template <typename T> int Heaviside(T x)
{
	return (0.5*(sgn<T>(x))+1);
}
