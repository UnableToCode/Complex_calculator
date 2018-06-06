#pragma once
#include<iostream>
#include<string>
#include<complex>
#include<vector>
#include<regex>

using namespace std;

class Expression
{
private:
	string expression;
	string complex_number;
	vector<complex<double>> complex;

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

	Expression();
	Expression(string org);
	~Expression();
};

