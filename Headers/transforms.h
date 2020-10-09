#include <complex>
#include <cmath>

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