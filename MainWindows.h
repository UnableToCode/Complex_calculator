#pragma once
#include<iostream>
#include<string>
#include<list>
#include"expression.h"

class MainWindows
{
	typedef struct _result
	{
		string org;
		string rlt;
	}Result;

	std::string	input;
	std::list<Result> list_result;
	std::list<Result>::iterator it;
public:

	void start();

	MainWindows();
	~MainWindows();
};

