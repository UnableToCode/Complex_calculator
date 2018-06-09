#include "expression.h"

complex<double> Expression::get_complex(string& str)
{
	double a{}, b{};
	smatch result;
	string help{};
	regex rex_complex(complex_number);
	if (regex_match(str, rex_complex)) {
		str.erase(str.begin());
		str.erase(str.end() - 1);
	}
	rex_complex = complex_number_no_paren;
	if (regex_match(str,result,rex_complex)) {
		help = result.str(1);
		stringstream ss(help);
		ss >> a;
		if (help.find('.')) {
			help = result.str(3);
			if (help.front() == '+')
				help.erase(help.begin());
		}
		else {
			help = result.str(4);
		}
		if (help == "-")
			help = "-1";
		if (help.empty())
			help = "1";
		ss.clear();
		ss.str(help);
		ss >> b;
	}
	return complex<double>(a,b);
}

string Expression::complex_to_str(complex<double> complex)
{
	string str{};
	str += "(" + to_string(complex.real());
	if (complex.imag() < 0) {
		str += to_string(complex.imag()) + "i)";
	}
	else {
		str += "+" + to_string(complex.imag()) + "i)";
	}

	return str;
}

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
			cout << "erro: There is ')' whithout '(' first, its place" << endl;
			flag = false;
			break;
		}
	}
	if (num > 0) {
		cout << "erro: The num of '(' is more than the num of ')'" << endl;
		flag = false;
	}
	if (expression.front() != 'a' && expression.front() != 'c' && !(expression.front() >= '0'&&expression.front() <= '9') && expression.front() != '-'&& expression.front() != 'i'&&expression.front() != '(') {
		cout << "erro: Expression begin with: " << expression.front() << endl;
		flag = false;
	}
	if (!(expression.back() >= '0'&&expression.back() <= '9') && expression.back() != 'i'&&expression.back() != ')') {
		cout << "erro: Expression end with: " << expression.back() << endl;
		flag = false;
	}
	pattern = "(\\+|\\-|\\*|\\/)(\\+|\\-|\\*|\\/|\\))";
	reg1 = pattern;
	help = expression;
	while (regex_search(help, result, reg1)) {
		cout << "erro: Unvalidated input: " << result.str(0) << ", its place is: " << expression.find(result.str(0)) << endl;
		flag = false;
		help = result.suffix().str();
	}
	pattern = "(\\d+)(\\(|abs|cjg|arg)";
	reg1 = pattern;
	help = expression;
	while (regex_search(help, result, reg1)) {
		cout << "erro: Unvalidated input: " << result.str(0) << ", its place is: " << expression.find(result.str(0)) << endl;
		flag = false;
		help = result.suffix().str();
	}
	pattern = "i(i|\\(|\\d+|abs|cjg|arg)";
	reg1 = pattern;
	help = expression;
	while (regex_search(help, result, reg1)) {
		cout << "erro: Unvalidated input: " << result.str(0) << ", its place is: " << expression.find(result.str(0)) << endl;
		flag = false;
		help = result.suffix().str();
	}
	pattern = "\\([\\)\\+\\/]";
	reg1 = pattern;
	help = expression;
	while (regex_search(help, result, reg1)) {
		cout << "erro: Unvalidated input: " << result.str(0) << ", its place is: " << expression.find(result.str(0)) << endl;
		flag = false;
		help = result.suffix().str();
	}
	pattern = "\\)(\\(|\\d+|abs|cjg|arg)";
	reg1 = pattern;
	help = expression;
	while (regex_search(help, result, reg1)) {
		cout << "erro: Unvalidated input: " << result.str(0) << ", its place is: "<< expression.find(result.str(0)) << endl;
		flag = false;
		help = result.suffix().str();
	}
	int pos = 0;
	while (expression.find(")i",pos) != string::npos) {
		int index = expression.find(")i");
		int num = 1;
		int j = index;
		while (num != 0) {
			j--;
			if (expression[j] == '(')
				num--;
			if (expression[j] == ')')
				num++;
		}
		string help(expression.begin() + j + 1, expression.begin() + index);
		regex reg_complex(complex_number_no_paren);
		Expression exp_help(help);
		if (regex_match(help, reg_complex)) {
			if (exp_help.check_imag()) {
				cout << "erro: Unvalidated input: )i" << endl;
				flag = false;
				pos = expression.find(")i", pos) + 1;
			}
			else {
				expression.replace(expression.begin() + index, expression.begin() + index + 2, ")*i");
				pos = 0;
			}
		}
		else {
			if (exp_help.check_mistake()) {
				exp_help.preproccess();
				exp_help.calculation();
				if (exp_help.check_imag()) {
					cout << "erro: Unvalidated input: )i" << endl;
					flag = false;
					pos = expression.find(")i", pos) + 1;

				}
				else {
					help = "(" + help + ")i";
					index = expression.find(help);
					string replace_help = exp_help.push_expression() + "*i";
					expression.replace(expression.begin() + index, expression.begin() + index + help.length(), replace_help);
				}
			}
			else {
				flag = false;
				cout << "erro: Unvalidated input: )i" << endl;
				pos = expression.find(")i", pos) + 1;
			}
		}
	}
	return flag;
}


void Expression::my_negate()
{
	if (expression.find("-(") == 0) {
		int index{};
		int num = 1;
		int j = index + 1;
		while (num != 0) {
			j++;
			if (expression[j] == '(')
				num++;
			if (expression[j] == ')')
				num--;
		}
		string help(expression.begin() + index + 2, expression.begin() + j);
		regex reg_complex(complex_number_no_paren);
		string replace_help{};
		if (regex_match(help, reg_complex)) {
			complex<double> complex = get_complex(help);
			complex = -complex;
			replace_help = complex_to_str(complex);
		}
		else {
			Expression exp_help(help);
			exp_help.calculation();
			replace_help = exp_help.push_expression();
			complex<double> complex = get_complex(replace_help);
			complex = -complex;
			replace_help = complex_to_str(complex);
		}
		help = "-(" + help + ")";
		expression.replace(expression.begin() + index, expression.begin() + index + help.length(), replace_help);
	}
	while (expression.find("(-(") != string::npos) {
		int index = expression.find("(-(");
		int num = 1;
		int j = index + 2;
		while (num != 0) {
			j++;
			if (expression[j] == '(')
				num++;
			if (expression[j] == ')')
				num--;
		}
		string help(expression.begin() + index + 3, expression.begin() + j);
		regex reg_complex(complex_number_no_paren);
		string replace_help{};
		if (regex_match(help, reg_complex)) {
			complex<double> complex = get_complex(help);
			complex = -complex;
			replace_help = complex_to_str(complex);
		}
		else {
			Expression exp_help(help);
			exp_help.calculation();
			replace_help = exp_help.push_expression();
			complex<double> complex = get_complex(replace_help);
			complex = -complex;
			replace_help = complex_to_str(complex);
		}
		help = "-(" + help + ")";
		expression.replace(expression.begin() + index + 1, expression.begin() + index + 1 + help.length(), replace_help);
	}
}

void Expression::my_abs()
{
	while(expression.find("abs")!=string::npos) {
		int index = expression.find("abs");
		int num = 1;
		int j = index + 3;
		while (num != 0) {
			j++;
			if (expression[j] == '(')
				num++;
			if (expression[j] == ')')
				num--;
		}
		string help(expression.begin() + index + 4, expression.begin() + j);
		regex reg_complex(complex_number_no_paren);	
		string replace_help{};
		if (regex_match(help, reg_complex)) {
			complex<double> complex = get_complex(help);
			complex = abs(complex);
			replace_help = complex_to_str(complex);
		}
		else {
			Expression exp_help(help);
			exp_help.calculation();
			replace_help = exp_help.push_expression();
			complex<double> complex = get_complex(replace_help);
			complex = abs(complex);
			replace_help = complex_to_str(complex);
		}
		help = "abs(" + help + ")";
		while (expression.find(help) != string::npos) {
			index = expression.find(help);
			expression.replace(expression.begin() + index, expression.begin() + index + help.length(), replace_help);
		}
	}
}

void Expression::my_cjg()
{
	while (expression.find("cjg") != string::npos) {
		int index = expression.find("cjg");
		int num = 1;
		int j = index + 3;
		while (num != 0) {
			j++;
			if (expression[j] == '(')
				num++;
			if (expression[j] == ')')
				num--;
		}
		string help(expression.begin() + index + 4, expression.begin() + j);
		regex reg_complex(complex_number_no_paren);
		string replace_help{};
		if (regex_match(help, reg_complex)) {
			complex<double> complex = get_complex(help);
			complex = conj(complex);
			replace_help = complex_to_str(complex);
		}
		else {
			Expression exp_help(help);
			exp_help.calculation();
			replace_help = exp_help.push_expression();
			complex<double> complex = get_complex(replace_help);
			complex = conj(complex);
			replace_help = complex_to_str(complex);
		}
		help = "cjg(" + help + ")";
		while (expression.find(help) != string::npos) {
			index = expression.find(help);
			expression.replace(expression.begin() + index, expression.begin() + index + help.length(), replace_help);
		}
	}
}

void Expression::my_arg()
{
	while (expression.find("arg") != string::npos) {
		int index = expression.find("arg");
		int num = 1;
		int j = index + 3;
		while (num != 0) {
			j++;
			if (expression[j] == '(')
				num++;
			if (expression[j] == ')')
				num--;
		}
		string help(expression.begin() + index + 4, expression.begin() + j);
		regex reg_complex(complex_number_no_paren);
		string replace_help{};
		if (regex_match(help, reg_complex)) {
			complex<double> complex = get_complex(help);
			complex = arg(complex);
			replace_help = complex_to_str(complex);
		}
		else {
			Expression exp_help(help);
			exp_help.calculation();
			replace_help = exp_help.push_expression();
			complex<double> complex = get_complex(replace_help);
			complex = arg(complex);
			replace_help = complex_to_str(complex);
		}
		help = "arg(" + help + ")";
		while (expression.find(help) != string::npos) {
			index = expression.find(help);
			expression.replace(expression.begin() + index, expression.begin() + index + help.length(), replace_help);
		}
	}
}

void Expression::my_pow()
{
	while (expression.find("^")!=string::npos) {
		int index = expression.find("^");
		regex reg_complex(complex_number_no_paren);
		string replace_help{};
		complex<double> complex1, complex2;
		int num = 1;
		int j = index - 1;
		while (num != 0) {
			j--;
			if (expression[j] == '(')
				num--;
			if (expression[j] == ')')
				num++;
		}
		string help1(expression.begin() + j + 1, expression.begin() + index - 1);
		num = 1;
		j = index + 1;
		while (num != 0) {
			j++;
			if (expression[j] == '(')
				num++;
			if (expression[j] == ')')
				num--;
		}
		string help2(expression.begin() + index + 2, expression.begin() + j);
		string help = "(" + help1 + ")^(" + help2 + ")";
		if (regex_match(help1, reg_complex)) {
			complex1 = get_complex(help1);
		}
		else {
			Expression exp_help(help1);
			exp_help.calculation();
			help1 = exp_help.push_expression();
			complex1 = get_complex(help1);
		}
		if (regex_match(help2, reg_complex)) {
			complex2 = get_complex(help2);
		}
		else {
			Expression exp_help(help2);
			exp_help.calculation();
			help2 = exp_help.push_expression();
			complex2 = get_complex(help2);
		}
		complex<double> result_complex = pow(complex1, complex2);
		replace_help = complex_to_str(result_complex);
		index = expression.find(help);
		expression.replace(expression.begin() + index, expression.begin() + index + help.length(), replace_help);
	}
}

void Expression::my_md()
{
	while (expression.find('*') != string::npos || expression.find('/') != string::npos) {
		int index = expression.find('*');
		if (expression.find('/') != string::npos)
			if (index > expression.find('/'))
				index = expression.find('/');
		if (expression.find('*') == string::npos)
			index = expression.find('/');
		regex reg_complex(complex_number_no_paren);
		string replace_help{};
		complex<double> complex1, complex2, result_complex;
		string help{};
		int num = 1;
		int j = index - 1;
		while (num != 0) {
			j--;
			if (expression[j] == '(')
				num--;
			if (expression[j] == ')')
				num++;
		}
		string help1(expression.begin() + j + 1, expression.begin() + index - 1);
		num = 1;
		j = index + 1;
		while (num != 0) {
			j++;
			if (expression[j] == '(')
				num++;
			if (expression[j] == ')')
				num--;
		}
		string help2(expression.begin() + index + 2, expression.begin() + j);
		if (regex_match(help1, reg_complex)) {
			complex1 = get_complex(help1);
		}
		else {
			Expression exp_help(help1);
			exp_help.calculation();
			help1 = exp_help.push_expression();
			complex1 = get_complex(help1);
		}
		if (regex_match(help2, reg_complex)) {
			complex2 = get_complex(help2);
		}
		else {
			Expression exp_help(help2);
			exp_help.calculation();
			help2 = exp_help.push_expression();
			complex2 = get_complex(help2);
		}
		if (expression[index] == '*') {
			help = "(" + help1 + ")*(" + help2 + ")";
			result_complex = complex1 * complex2;
		}
		if (expression[index] == '/') {
			help = "(" + help1 + ")/(" + help2 + ")";
			result_complex = complex1 / complex2;
		}
		replace_help = complex_to_str(result_complex);
		index = expression.find(help);
		expression.replace(expression.begin() + index, expression.begin() + index + help.length(), replace_help);
	}
}

void Expression::my_as()
{
	while (expression.find(")+(") != string::npos || expression.find(")-(") != string::npos) {
		int index = expression.find(")+(") + 1;
		if (expression.find(")-(") != string::npos)
			if (index > expression.find(expression.find(")-("))) {
				index = expression.find(")-(") + 1;
			}
		if (expression.find(")+(") == string::npos)
			index = expression.find(")-(") + 1;
		regex reg_complex(complex_number_no_paren);
		string replace_help{};
		complex<double> complex1, complex2, result_complex;
		string help{};
		int num = 1;
		int j = index - 1;
		while (num != 0) {
			j--;
			if (expression[j] == '(')
				num--;
			if (expression[j] == ')')
				num++;
		}
		string help1(expression.begin() + j + 1, expression.begin() + index - 1);
		num = 1;
		j = index + 1;
		while (num != 0) {
			j++;
			if (expression[j] == '(')
				num++;
			if (expression[j] == ')')
				num--;
		}
		string help2(expression.begin() + index + 2, expression.begin() + j);
		if (regex_match(help1, reg_complex)) {
			complex1 = get_complex(help1);
		}
		else {
			Expression exp_help(help1);
			exp_help.calculation();
			help1 = exp_help.push_expression();
			complex1 = get_complex(help1);
		}
		if (regex_match(help2, reg_complex)) {
			complex2 = get_complex(help2);
		}
		else {
			Expression exp_help(help2);
			exp_help.calculation();
			help2 = exp_help.push_expression();
			complex2 = get_complex(help2);
		}
		if (expression[index] == '+') {
			help = "(" + help1 + ")+(" + help2 + ")";
			result_complex = complex1 + complex2;
		}
		if (expression[index] == '-') {
			help = "(" + help1 + ")-(" + help2 + ")";
			result_complex = complex1 - complex2;
		}
		replace_help = complex_to_str(result_complex);
		index = expression.find(help);
		expression.replace(expression.begin() + index, expression.begin() + index + help.length(), replace_help);
	}
}

void Expression::preproccess()
{
	regex reg1(complex_number);
	regex reg2("(\\d*i)");
	regex reg3("(\\d+)");
	smatch result;
	string help_str = expression;
	string replace_str{};
	vector<string> help_vec_x;
	vector<string> help_vec_y;
	vector<string> help_vec_z;

	while (regex_search(help_str, result, reg1)) {
		help_vec_x.push_back(result.str(0));
		help_str = result.suffix().str();
	}
	expression = regex_replace(expression, reg1, "x");
	help_str = expression;
	while (regex_search(help_str, result, reg2)) {
		string str = result.str(0);
		replace_str = "(0+" + str + ")";
		help_vec_y.push_back(replace_str);
		help_str = result.suffix().str();
	}
	expression = regex_replace(expression, reg2, "y");
	help_str = expression;
	while (regex_search(help_str, result, reg3)) {
		string str = result.str(0);
		replace_str = "(" + str + "+0i)";
		help_vec_z.push_back(replace_str);
		help_str = result.suffix().str();
	}
	expression = regex_replace(expression, reg3, "z");
	for (int i = 0; i < help_vec_x.size(); i++) {
		int index = expression.find("x");
		expression.replace(expression.begin() + index, expression.begin() + index + 1, help_vec_x[i]);
	}
	for (int i = 0; i < help_vec_y.size(); i++) {
		int index = expression.find("y");
		expression.replace(expression.begin() + index, expression.begin() + index + 1, help_vec_y[i]);
	}
	for (int i = 0; i < help_vec_z.size(); i++) {
		int index = expression.find("z");
		expression.replace(expression.begin() + index, expression.begin() + index + 1, help_vec_z[i]);
	}
}

void Expression::calculation()
{
	regex reg(complex_number_no_paren);
	if (regex_match(expression, reg))
		return;
	reg = complex_number;
	if (regex_match(expression, reg)) {
		expression.erase(expression.begin());
		expression.erase(expression.end() - 1);
		return;
	}
	smatch result;
	string help{};
	for (int i = 0; i < expression.length(); i++) {
		if (expression[i] == '(') {
			int num = 1;
			int j = i;
			while (num != 0) {
				j++;
				if (expression[j] == '(')
					num++;
				if (expression[j] == ')')
					num--;
			}
			help.assign(expression, i + 1, j - i - 1);
			Expression help_exp(help);
			help_exp.calculation();
			string help_replace = help_exp.push_expression();
			if (help != help_replace) {
				expression.replace(expression.begin() + i + 1, expression.begin() + j, help_replace);
			}

		}
	}
	my_abs();
	my_cjg();
	my_arg();
	my_pow();
	my_negate();
	my_md();
	my_as();
	if (!regex_match(expression,reg)) {
		reg = complex_number_no_paren;
		if (regex_search(expression, result, reg)) {
			expression = "(" + result.str(0) + ")";
		}
	}
}

void Expression::print_exp(string & rlt)
{
	double a{}, b{};
	smatch result;
	string help{};
	string real{};
	string imag{};
	regex rex_complex(complex_number);
	if (regex_match(expression, result, rex_complex)) {
		expression.erase(expression.begin());
		expression.erase(expression.end() - 1);
	}
	rex_complex = complex_number_no_paren;
	if (regex_match(expression, result, rex_complex)) {
		help = result.str(1);
		real = help;
		stringstream ss(help);
		ss >> a;
		if (help.find('.')) {
			help = result.str(3);
			if (help.front() == '+')
				help.erase(help.begin());
		}
		else {
			help = result.str(4);
		}
		if (help == "-")
			help = "-1";
		if (help.empty())
			help = "1";
		imag = help;
		ss.clear();
		ss.str(help);
		ss >> b;
	}
	if (abs(a) < PRECISION)
		if (abs(b) < PRECISION) {
			cout << "0";
			rlt = "0";
		}
		else {
			cout << b << "i";
			rlt = imag + "i";
		}
	else {
		cout << a;
		rlt += real;
		if (abs(b) > PRECISION) {
			if (b > 0) {
				cout << "+" << b << "i";
				rlt += "+" + imag + "i";
			}
			if (b < 0) {
				cout << b << "i" ;
				rlt += imag + "i";
			}
		}
	}
	cout << endl;
}

bool Expression::check_imag()
{
	bool flag{};
	complex<double> complex;
	complex = get_complex(expression);
	expression = "(" + expression + ")";
	if (abs(complex.imag()) < PRECISION)
		flag = false;
	else
		flag = true;
	return flag;
}

Expression::Expression()
{
	complex_number = "\\(([\\+|\\-]?[\\d]+(\\.[\\d]+)?)([\\+|\\-][\\d]*(\\.[\\d]+)?)i\\)";
	complex_number_no_paren = "([\\+|\\-]?[\\d]+(\\.[\\d]+)?)([\\+|\\-][\\d]*(\\.[\\d]+)?)i";
}

Expression::Expression(string org)
{
	expression = org;
	complex_number = "\\(([\\+|\\-]?[\\d]+(\\.[\\d]+)?)([\\+|\\-][\\d]*(\\.[\\d]+)?)i\\)";
	complex_number_no_paren = "([\\+|\\-]?[\\d]+(\\.[\\d]+)?)([\\+|\\-][\\d]*(\\.[\\d]+)?)i";
}


Expression::~Expression()
{
}
