#include "mpir.h"
#include "mpirxx.h"
#include <iostream>
#include <vector>

mpz_class pow_mod(mpz_class a, mpz_class n, mpz_class mod)
{
	mpz_class res=1;
	if (n == 0) return 1;
	while (n>0)
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

mpz_class NOD (mpz_class a, mpz_class b) 
{
	if (a == 0L)
		return b;
	if (b == 0L)
		return a;
	if (a == b)
		return a;
	if (a == 1L || b == 1L)
		return 1L;
	if (a % 2L == 0L && b % 2L == 0L)
		return 2L * NOD(a / 2L, b / 2L);
	if (a % 2L == 0L && b % 2L != 0L)
		return NOD(a / 2L, b);
	if (a % 2L != 0L && b % 2L == 0L)
		return NOD(a, b / 2L);
	if (a < b)
		return NOD((b - a) / 2L, a);
	else
		return NOD((a - b) / 2L, b);
}

mpz_class NOK(std::vector <int> &fb)
{
	mpz_class res=1;
	for (int i = 0; i < fb.size(); i++)
	{
		res = res*fb[i] / NOD(res,fb[i]);
	}
	return res;
}

int main()
{
	std::vector <int> FB = { 2, 3, 4, 5, 7, 8, 9, 11 };
	std::cout << "Factor base: \n";
	for (int i = 0; i < FB.size(); i++)
	{
		std::cout << FB.at(i) << " ";
	}
	std::cout << std::endl;
	std::cout << "NOK = " << NOK(FB) << std::endl;

	mpz_class N ("269603579723");
	std::cout << "N = " << N << std::endl;

	mpz_class result;
	for (mpz_class a = 2; a < N; a++)
	{
		result = NOD(pow_mod(a, NOK(FB), N) - 1, N);
		if (result != 1)
			break;		
	}
	std::cout << "p = " << result << std::endl;
	std::cout << "q = " << N / result << std::endl;
	
	system("pause");
}