#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <time.h>

#define byte unsigned char

int ext_ev(int a, int b)
{
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


long long pow_mod(long long a, int n, int mod)
{
	long long res = 1;
	if (n == 0) return 1;
	while (n)
	{
		if (n % 2 == 0)
		{
			n /= 2;
			a *= a;
			a %= mod;
		}
		else
		{
			n--;
			res *= a;
			res %= mod;
		}
	}
	return res % mod;
}

int del(int a, int b) 
{
	while (a != b) {
		if (a > b) {
			int tmp = a;
			a = b;
			b = tmp;
		}
		b = b - a;
	}
	return a;
}

int main()
{
	int p = 269; //случайное простое число
	int g = 2; //первообразный корень p
	int x = 137; //закрытый ключ
	int y = pow_mod(g,x,p);
	//p,g,y - открытый ключ

	std::string plain;	
	std::cout << "Enter text: \n";
	std::getline(std::cin, plain);

	int a; 
	int b;
	int k;

	std::vector <unsigned int> cipher;

	srand(time(0));

	byte temp;
	int M = 0;
	int tmp = 0;
	int res = 0;

	for (int i = 0; i < plain.length(); i++)
	{
		
		k = rand() % (p - 2) + 1; //сессионный ключ
		a = pow_mod(g, k, p);
		temp = plain[i];
		M += (int)temp;
		b = (pow_mod(y, k, p) * (int)temp) % p;
		cipher.push_back(a);		
		cipher.push_back(b);	
	}

	//вычисление подписи
	M %= p; //hash
	std::cout <<"Hash: " << M << std::endl;

	int r;
	int s;

	while (del(k, p - 1) != 1) 
	{
		k = rand() % (p - 2) + 1;
	}	

	r= pow_mod(g, k, p);
	std::cout << "r = " << r << std::endl;

	tmp = ext_ev(k, p - 1);
	s = ((M - x*r) * tmp) % (p - 1);
	if (s < 0) s = s + p - 1;
	std::cout << "s = " << s << std::endl;
	cipher.push_back(r);
	cipher.push_back(s);

	std::cout << std::endl;

	std::cout << "Ciphertext: \n";
	for (int i = 0; i < cipher.size(); i ++)
	{
		std::cout << cipher[i] << " ";
	}
	std::cout << std::endl;

	std::vector <unsigned int> result;
	for (int i = 0; i < cipher.size()-2; i+=2)
	{
		tmp = pow_mod(cipher[i], x, p);
		tmp = ext_ev(tmp, p);
		res = (cipher[i+1] * tmp) % p;
		result.push_back(res);
	}

	std::cout << "\nResult: "<< std::endl;
	int hash = 0;
	for (int i = 0; i < result.size(); i++)
	{
		hash += result[i];
		std::cout << (byte)result[i];
	}
	hash %= p;
	std::cout << std::endl;

	//проверка подписи
	int new_r = cipher[cipher.size() - 2];
	int new_s = cipher[cipher.size() - 1];
	bool flag = false;
	int sign, sign1;
	if ((new_r > 0) && (new_r < (p - 1)) && (new_s > 0) && (new_s < (p - 1))) flag = true;
	sign = (pow_mod(y, new_r, p)*pow_mod(r, new_s, p)) % p;
	sign1 = pow_mod(g, hash, p);
	if (flag && (sign == sign1))
		std::cout << "The signature is true\n";
	else std::cout << "The signature is false\n";

	system("pause");

}

