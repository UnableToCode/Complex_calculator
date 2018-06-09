#pragma once
#include<iostream>
#include<string>
#include<complex>
#include<vector>
#include<regex>
#include<sstream>
#include<iomanip>

#define  PRECISION 0.000001

using namespace std;

class Expression
{
private:
	string expression;
	string complex_number;
	string complex_number_no_paren;
	complex<double> get_complex(string& str);
	string complex_to_str(complex<double> complex);

public:
	string push_expression() { return expression; };
	void get_expression(string org);
	bool check_mistake();
	void my_negate();
	void my_abs();
	void my_cjg();
	void my_arg();
	void my_pow();
	void my_md();
	void my_as();
	void preproccess();
	void calculation();
	void print_exp(string & rlt);
	bool check_imag();

	Expression();
	Expression(string org);
	~Expression();
};

