#include "mpir.h"
#include "mpirxx.h"
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>

mpz_class ext_ev(mpz_class a, mpz_class b)
{
	mpz_class mod = b;
	mpz_class q, r, x1, x2, result;
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

mpz_class pow_mod(mpz_class a, mpz_class n, mpz_class mod)
{
	mpz_class res = 1;
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

int main()
{
	//Alice
	mpz_class p_A("499771428689208687507002855988208789391");
	mpz_class q_A("554677246676568531371124230621777774897");
	mpz_class n_A = p_A*q_A;
	mpz_class fi_A = (p_A-1)*(q_A-1);
	mpz_class e_A("65537");
	mpz_class d_A = ext_ev(e_A, fi_A);
	
	//Bob
	mpz_class p_B("413186364950093010470440626406088437181");
	mpz_class q_B("601278659213303294487127752251799507917");
	mpz_class n_B = p_B*q_B;
	mpz_class fi_B = (p_B - 1)*(q_B - 1);
	mpz_class e_B("182953808031264514175650543291738357305730922373157828904016782750918577004833");
	mpz_class d_B = ext_ev(e_B, fi_B);

	std::string plain;
	std::cout << "Enter text: \n";
	std::getline(std::cin, plain);
	std::string sign;
	std::cout << "Enter signature: \n";
	std::getline(std::cin, sign);
	
	//encryption
	std::vector <mpz_class> cipher_code;
	for (int i = 0; i < plain.length(); i++)
	{
		cipher_code.push_back(pow_mod((int)plain[i], e_A, n_A));
	}
	std::cout << "\nCiphertext: " << std::endl;
	for (int i = 0; i < cipher_code.size(); i++)
	{
		std::cout << cipher_code[i] << std::endl;
	}

	//signature
	std::vector <mpz_class> sign_code;
	for (int i = 0; i < sign.length(); i++)
	{
		sign_code.push_back(pow_mod((int)sign[i], d_B, n_B));
		sign_code[i] = pow_mod(sign_code[i], e_A, n_A);
	}
	std::cout << "\nSignature: " << std::endl;
	for (int i = 0; i < sign_code.size(); i++)
	{
		std::cout << sign_code[i] << std::endl;
	}
	
	//decryption
	std::vector <mpz_class> dec_code;
	for (int i = 0; i < cipher_code.size(); i++)
	{
		dec_code.push_back(pow_mod(cipher_code[i], d_A, n_A));
	}
	std::cout << "\nMessage: " << std::endl;
	int val;
	std::string s;
	for (int i = 0; i < dec_code.size(); i++)
	{
		s = dec_code[i].get_str();
		std::istringstream iss(s, std::istringstream::in);
		iss >> val;
		std::cout << (char)val;
	}
	//verification of signature
	std::vector <mpz_class> dec_sign_code;
	for (int i = 0; i < sign_code.size(); i++)
	{
		dec_sign_code.push_back(pow_mod(sign_code[i], d_A, n_A));
		dec_sign_code[i] = pow_mod(dec_sign_code[i], e_B, n_B);
	}
	std::cout << "\nSignature: " << std::endl;
	for (int i = 0; i < dec_sign_code.size(); i++)
	{
		s = dec_sign_code[i].get_str();
		std::istringstream iss(s, std::istringstream::in);
		iss >> val;
		std::cout << (char)val;
	}
	std::cout << std::endl;
	system("pause");
}

