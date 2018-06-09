#include "MainWindows.h"
#include <stdlib.h>


void MainWindows::start()
{
	while (input != "quit") {
		std::cout << "please input expression:";
		std::cin >> input;
		if (input == "last") {
			if (list_result.empty()) {
				std::cout << "No records" << std::endl;
			}
			else {
				it--;
				if (it == list_result.begin()) {
					std::cout << "expression:" << it->org << std::endl;
					std::cout << "reslut:" << it->rlt << std::endl;
					std::cout << "It is the first record." << std::endl;
					it++;
				}
				else {
					std::cout << "expression:" << it->org << std::endl;
					std::cout << "reslut:" << it->rlt << std::endl;
				}
			}
			getchar();
			getchar();
			system("cls");
			continue;
		}
		if (input == "next") {
			if (list_result.empty()) {
				std::cout << "No records" << std::endl;
			}
			else {
				it++;
				if (it == list_result.end()) {
					std::cout << "No more result" << std::endl;
					it--;
				}
				else {
				std::cout << "expression:" << it->org << std::endl;
				std::cout << "reslut:" << it->rlt << std::endl;
				}
			}
			getchar();
			getchar();
			system("cls");
			continue;
		}
		if (input != "quit") {
			Expression expression(input);
			if (expression.check_mistake()) {
				Result result;
				result.org = input;
				expression.preproccess();
				expression.calculation();
				expression.print_exp(result.rlt);
				if (list_result.size() < 6) {
					list_result.push_back(result);
				}
				else {
					list_result.erase(list_result.begin());
					list_result.push_back(result);
				}
				it = list_result.end();
				it--;
			}
			getchar();
			getchar();
			system("cls");
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
