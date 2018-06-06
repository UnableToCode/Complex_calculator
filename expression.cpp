#include "expression.h"
//(1+3i)+i+3+(-3+4i)


void Expression::get_expression(string org)
{
	expression = org;
}

bool Expression::check_mistake()
{
	bool flag = true;
	string pattern;
	string help{};
	string help_replace{};
	regex reg1;
	smatch result;
	int num{};
	for (int i = 0; i < expression.length(); i++) {
		if (expression[i] == '(')
			num++;
		if (expression[i] == ')')
			num--;
		if (num < 0) {
			cout << "erro: There is ')' whithout '(' first" << endl;
			flag = false;
			break;
		}
	}
	if (num > 0) {
		cout << "erro: The num of '(' is more than the num of ')'" << endl;
		flag = false;
	}
	if (expression.front() != '|' && !(expression.front() >= '0'&&expression.front() <= '9') && expression.front() != '-'&& expression.front() != 'i'&&expression.front() != '(') {
		cout << "erro: Expression begin with: " << expression.front() << endl;
		flag = false;
	}
	if (expression.back() != '|' && !(expression.back() >= '0'&&expression.back() <= '9') && expression.back() != 'i'&&expression.back() != ')') {
		cout << "erro: Expression end with: " << expression.back() << endl;
		flag = false;
	}
	pattern = "(\\+|\\-|\\*|\\/)(\\+|\\-|\\*|\\/|\\))";
	reg1 = pattern;
	help = expression;
	while (regex_search(help, result, reg1)) {
		cout << "erro: Unvalidated input: " << result.str(0) << endl;
		flag = false;
		help = result.suffix().str();
	}
	pattern = "(\\d+)(\\(|abs|cjg|arg)";
	reg1 = pattern;
	help = expression;
	while (regex_search(help, result, reg1)) {
		cout << "erro: Unvalidated input: " << result.str(0) << endl;
		flag = false;
		help = result.suffix().str();
	}
	pattern = "i(i|\\(|\\d+|abs|cjg|arg)";
	reg1 = pattern;
	help = expression;
	while (regex_search(help, result, reg1)) {
		cout << "erro: Unvalidated input: " << result.str(0) << endl;
		flag = false;
		help = result.suffix().str();
	}
	pattern = "\\([\\)\\+\\/]";
	reg1 = pattern;
	help = expression;
	while (regex_search(help, result, reg1)) {
		cout << "erro: Unvalidated input: " << result.str(0) << endl;
		flag = false;
		help = result.suffix().str();
	}
	pattern = "\\)(\\(|\\d+|abs|cjg|arg)";
	reg1 = pattern;
	help = expression;
	while (regex_search(help, result, reg1)) {
		cout << "erro: Unvalidated input: " << result.str(0) << endl;
		flag = false;
		help = result.suffix().str();
	}
	pattern = "(\\(.*\\))i";
	reg1 = pattern;
	while(regex_search(expression, result, reg1)) {
		help = result.str(1);
		Expression exp_help(help);
		if (exp_help.check_mistake()) {
			exp_help.calculation();
			help = exp_help.push_expression();
			if (help.find('i')) {
				cout << "erro: Unvalidated input: " << result.str(0) << endl;
				flag = false;
				break;
			}
			else {
				help_replace = help + "i";
				help = result.str(0);
				regex reg_replace(help);
				expression = regex_replace(expression, reg_replace, help_replace);
			}
		}
		else {
			flag = false;
			cout << "erro: Unvalidated input: " << result.str(0) << endl;
			break;
		}
	}

	return flag;
}


void Expression::my_negate()
{
}

void Expression::my_abs()
{
}

void Expression::my_cjg()
{
}

void Expression::my_arg()
{
}

void Expression::my_pow()
{
}

void Expression::my_md()
{
}

void Expression::my_as()
{
}

void Expression::preproccess()
{
	regex reg1(complex_number);
	regex reg2("(\\d*i)");
	regex reg3("(\\d+)");
	smatch result;
	string help_str = expression;
	string replace_str{};
	vector<string> help_vec_a;
	vector<string> help_vec_b;
	vector<string> help_vec_c;

	while (regex_search(help_str, result, reg1)) {
		help_vec_a.push_back(result.str(0));
		help_str = result.suffix().str();
	}
	expression = regex_replace(expression, reg1, "a");
	help_str = expression;
	while (regex_search(help_str, result, reg2)) {
		string str = result.str(0);
		replace_str = "(0+" + str + ")";
		help_vec_b.push_back(replace_str);
		help_str = result.suffix().str();
	}
	expression = regex_replace(expression, reg2, "b");
	help_str = expression;
	while (regex_search(help_str, result, reg3)) {
		string str = result.str(0);
		replace_str = "(" + str + "0i)";
		help_vec_c.push_back(replace_str);
		help_str = result.suffix().str();
	}
	expression = regex_replace(expression, reg3, "c");
	for (int i = 0; i < help_vec_a.size(); i++) {
		int index = expression.find("a");
		expression.replace(expression.begin() + index, expression.begin() + index + 1, help_vec_a[i]);
	}
	for (int i = 0; i < help_vec_b.size(); i++) {
		int index = expression.find("b");
		expression.replace(expression.begin() + index, expression.begin() + index + 1, help_vec_b[i]);
	}
	for (int i = 0; i < help_vec_c.size(); i++) {
		int index = expression.find("c");
		expression.replace(expression.begin() + index, expression.begin() + index + 1, help_vec_c[i]);
	}
}

void Expression::calculation()
{
}

Expression::Expression()
{
	complex_number = "\\([\\+|\\-]?[\\d]+(\\.[\\d]+)?(\\+|\\-)[\\d]+(\\.[\\d]+)?i\\)";
}

Expression::Expression(string org)
{
	expression = org;
	complex_number = "\\([\\+|\\-]?[\\d]+(\\.[\\d]+)?(\\+|\\-)[\\d]+(\\.[\\d]+)?i\\)";
}


Expression::~Expression()
{
}
