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
		std::cout << "(" << p[i].x << ", " << p[i].y << ")";
		std::cout << std::endl;
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

bool isO(Point p)
{
	if (p.x == -1 && p.y == -1)
		return true;
	else return false;
}

int main()
{
	int a = 1;
	int b = 3;
	int n = 41;
	std::vector <Point> p;
	getPoints(a,b,n,p);
	printVect(p);
	std::cout << "Order of curve: " << p.size() + 1 << std::endl;
	
	int order = 13;
	std::cout << "Order of point G: " << order << std::endl;
	
	std::vector <Point> vect_G;
	for (int i = 0; i< p.size(); i++)
	{
		if (isO(mult(order, p[i], a, n)))
			vect_G.push_back(p[i]);
	}
	
	srand(time(0));
	Point G = vect_G[rand() % vect_G.size()];
	std::cout << "Generator: " << "(" << G.x << ", " << G.y << ")\n";
	
	int nA = 8;
	int nB = 10;

	Point Pa = mult(nA, G, a, n);
	std::cout << "Pa =  " << "(" << Pa.x << ", " << Pa.y << ")\n";

	Point Pb = mult(nB, G, a, n);
	std::cout << "Pb =  " << "(" << Pb.x << ", " << Pb.y << ")\n";

	Point Ga = mult(nA, Pb, a, n);
	std::cout << "Ga =  " << "(" << Ga.x << ", " << Ga.y << ")\n";

	Point Gb = mult(nB, Pa, a, n);
	std::cout << "Gb =  " << "(" << Gb.x << ", " << Gb.y << ")\n";

	system("pause");
}

