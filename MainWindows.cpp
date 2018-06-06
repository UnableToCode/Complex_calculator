#include "MainWindows.h"



void MainWindows::start()
{
	while (input != "quit") {
		std::cout << "please input expression:";
		std::cin >> input;
		if (input != "quit") {
			Expression expression(input);
			if (expression.check_mistake())
				expression.calculation();
		}
	}
}

MainWindows::MainWindows()
{
	input.clear();
}


MainWindows::~MainWindows()
{
	input.clear();
}
