#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Headers/Axis.h"
#include <complex>

#include <iostream>

int init();
void processInput(GLFWwindow* window);
void display();
template <typename T> int sgn(T val);
template <typename T> int Heaviside(T x);
template <typename T> double Rect(T x, double d, double l);
double max(double x, double y);
vector<complex<double>> dft(vector<double> x, int numX);
vector<complex<double>> FFT(vector<double> X);
vector<complex<double>> rfft(vector<double> X, vector<complex<double>> &W);
int toDegree (double N);
 	
GLFWwindow* window;
Axis func(150, 380, 100, 100 , 20, 50);
Axis freq(150, 150, 100, 100, 0.1, 10);
Axis magn(380, 150, 100, 100, 1, 10);

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

double max(double x, double y)
{
	double res;
	if (x > y)
		return x;
	else 
		return y;
}

vector<complex<double>> FFT(vector<double> X)
{
	int N = X.size();
	vector<complex<double>> W(N);
	for (int i = 0; i < N; i++)
	{
		W[i] = polar(1.0, -2*M_PI*i/N);
	}
	vector<complex<double>> res = rfft(X, W);
	cout << res.capacity() << endl;
	return res;
}

vector<complex<double>> rfft(vector<double> X, vector<complex<double>> &W)
{
	int N = X.size();	
	if (N == 1)
	{	
		vector<complex<double>> cX(1);
		cX[0] = complex<double> (X[0], 0);
		return cX;
	}
	else 
	{
		int M = N/2;
		vector<double> Xeven(M, 0);
		vector<double> Xodd(M, 0);
		for (int i = 0; i<M; i++)
		{
			Xeven[i] = X[2 * i];
			Xodd[i] = X[2 * i + 1];
		}

		vector<complex<double>> Feven(M, 0);
		Feven = rfft(Xeven, W);
		vector<complex<double>> Fodd(M, 0);
		Fodd = rfft(Xodd, W);

		vector<complex<double>> fft(N, 0);

		for (int i = 0; i<N/2; i++)
		{
			fft[i] = Feven[i] + Fodd[i] * W[i * W.size() / N];
			fft[i + N/2] = Feven[i] - Fodd[i] * W[i * W.size() / N];
		}

		return fft;
	}
}

vector<complex<double>> dft(vector<double> x, int numX)
{
	vector<complex<double>> dft(numX, (0,0));
	complex<double> eiler;
	double tmp = 0;
	for (int i = 0; i<numX; i++)
	{
		for (int j = 0; j<numX; j++)
		{
			tmp = (-2*i*j*M_PI) / (double)numX;
			eiler = complex<double> (cos(tmp), sin(tmp));
			cout << x[j] * eiler << endl;
			dft[i] += x[j] * eiler;
		}
	}

	return dft;
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
        func.setScaleX(max(0.00001, func.scaleX * 0.999));
        freq.setScaleX(max(0.00001, freq.scaleX * 0.999));
        magn.setScaleX(max(0.00001, magn.scaleX * 0.999));
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        func.setScaleY(func.scaleY * 1.001);
        freq.setScaleY(freq.scaleY * 1.001);
        magn.setScaleY(magn.scaleY * 1.001);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        func.setScaleY(max(0.00001, func.scaleY * 0.999));
        freq.setScaleY(max(0.00001, freq.scaleY * 0.999));
        magn.setScaleY(max(0.00001, magn.scaleY * 0.999));
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

	glOrtho(0,500,0,500,0.0,1.0); 

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
