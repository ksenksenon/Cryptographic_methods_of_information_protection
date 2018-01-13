#include <iostream>
#include <vector>
#include <ctime>

struct Point
{
	int x, y;
};

int ext_ev(int a, int b)
{
	while (a < 0)
	{
		a += b;
	}
	int mod = b;
	int q, r, x1, x2, result;
	x2 = 1, x1 = 0;
	while (b > 0)
	{
		q = a / b, r = a % b;
		result = x2 - q * x1;
		a = b, b = r;
		x2 = x1, x1 = result;
	}
	if (x2 < 0)
	{
		result = x2 + mod;
	}
	else
	{
		result = x2;
	}
	return result;
}

void getPoints(int a, int b, int n, std::vector<Point> &p)
{
	Point point;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if ((i*i) % n == (j*j*j + a*j + b) % n)
			{
				point.x = j;
				point.y = i;
				p.push_back(point);
			}
		}
	}
}

void printVect(std::vector<Point> &p)
{
	std::cout << "Points: \n";
	for (int i = 0; i < p.size(); i++)
	{
		std::cout << "(" << p[i].x << ", " << p[i].y << ") ";
	}
}

int modul(int res, int N)
{
	if (res >= 0)
		return res % N;
	else
		while (res < 0)
		{
			res += N;
		}
	return res % N;
}

int getLambda(Point &p, Point &q, int a, int n)
{
	if (p.x == q.x && p.y == q.y)
	{
		if (ext_ev(2 * p.y, n) == 0)
			return -1;
		else
			return modul((3 * p.x * p.x + a) * ext_ev(2 * p.y, n), n);
	}
	else
	{
		if (ext_ev(q.x - p.x, n) == 0)
			return -1;
		else
			return modul((q.y - p.y) * ext_ev(q.x - p.x, n), n);
	}
}

int getx3(Point &p, Point &q, int a, int n)
{
	int l = getLambda(p, q, a, n);
	return modul((l*l - p.x - q.x), n);
}

int gety3(Point &p, Point &q, int a, int n)
{
	int l = getLambda(p, q, a, n);
	int x3 = getx3(p, q, a, n);
	return modul((l*(p.x - x3) - p.y), n);
}

Point mult(int k, Point p, int a, int n)
{
	int l, new_x, new_y;
	Point tmp = p;
	for (int i = 0; i < k - 1; i++)
	{
		l = getLambda(p, tmp, a, n);
		if (l != -1)
		{

			new_x = getx3(p, tmp, a, n);
			new_y = gety3(p, tmp, a, n);
			p.x = new_x;
			p.y = new_y;
		}
		else
		{
			p.x = -1;
			p.y = -1;
		}
	}
	return p;
}

bool isO(Point &p)
{
	if (p.x == -1 && p.y == -1)
		return true;
	else return false;
}

Point sum(Point p, Point q, int a, int n)
{
	Point res;
	res.x = getx3(p, q, a, n);
	res.y = gety3(p, q, a, n);
	return res;
}

int main()
{
	int a = 1;
	int b = 3;
	int mod = 41;
	std::vector <Point> vect;
	getPoints(a, b, mod, vect);
	std::cout << "y^2 = x^3 + x + 3 (mod 41) \n";
	printVect(vect);
	std::cout << std::endl;
	std::cout << "\nOrder of curve: " << vect.size() + 1 << std::endl;
	int N = 13;
	std::cout << "Order of point G: " << N << std::endl;
	
	srand(time(0));

	std::vector <Point> vect_G;
	for (int i = 0; i< vect.size(); i++)
	{
		if (isO(mult(N, vect[i], a, mod)))
			vect_G.push_back(vect[i]);
	}
	Point P = vect_G[rand() % vect_G.size()];
	int d = rand() % N-1 + 1;
	Point Q = mult(d, P, a, mod);

	std::cout << "\nPublic key: " << "\nEp (a,b) = " << mod << "\nP =  " << "(" << P.x << ", " << P.y << ")" << "\nN = " << N << "\nQ = " 
		<< "(" << Q.x << ", " << Q.y << ")" << "\n";
	
	std::cout << "\nPrivate key: " << "\nd = " << d <<"\n";
	
	std::string text = "hello";
	int hash = 0;
	for (int i = 0; i < text.length(); i++)
	{
		hash += (int)text[i];
	}
	hash %= N;

	int k, x1, r, s, k_inv;
	Point tmp;
	do
	{
		k = rand() % N - 1 + 1;
		tmp = mult(k, P, a, mod);
		x1 = tmp.x;
		r = x1 % N;
		k_inv = ext_ev(k, N);
		s = (k_inv*(hash + d * r)) % N;
	} 
	while (r == 0 || s == 0);
	
	std::cout << "\nk =  " << k << "\n";
	std::cout << "(x1, y1) =  " << "(" << tmp.x << ", " << tmp.y << ")\n";
	std::cout << "\nSignature: \nr =  " << r << "\n";
	std::cout << "s =  " << s << "\n";
	
	int s_inv = ext_ev(s, N);

	std::cout << "\nVerification: \n";
	if (r > 0 && r < N && s > 0 && s < N)
	{
		int u1 = (s_inv*hash) % N;
		int u2 = (s_inv*r) % N;
		std::cout << "u1 =  " << u1 << "\n";
		std::cout << "u2 =  " << u2 << "\n";

		Point u1_P = mult(u1, P, a, mod);
		Point u2_Q = mult(u2, Q, a, mod);

		Point res = sum(u1_P, u2_Q, a, mod);
		std::cout << "Result =  " << "(" << res.x << ", " << res.y << ")\n";

		int v = res.x % N;
		
		std::cout << "\nr =  " << r << "\n";
		std::cout << "v =  " << v << "\n";

		if (r == v) std::cout << "Success! :)\n";
	}
	else std::cout << "The signature is incorrect!\n";
	system("pause");
}

