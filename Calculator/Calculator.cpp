#include "Calculator.h"

using namespace Calculator;

void Calculator::Transition(string& str)
{
	try
	{
		std::erase(str, ' ');
		if (str.find_first_not_of(operations + numbers + "()") != string::npos)
			throw std::string("Обнаружены неизвестные калькулятору символы");
		if (str.find_first_of(numbers) == string::npos)
			throw std::string("Неверный синтаксис выражения");
		if ((str.find("-*") != string::npos) || (str.find("-/") != string::npos))
			throw std::string("Неверный синтаксис операций");
		if (FindCharCount(str, "(") != FindCharCount(str, ")"))
			throw std::string("Количество открывающих скобок не соответствует количеству закрывающих");
		if ((str.find_first_of("*/") == 0) || (str.find_last_of(operations) == (str.size() - 1)))
			throw std::string("Неверный синтаксис операций");
		while ((str.find_first_of(operations) != string::npos) && (str.find_last_of("+-") != 0))
		{
			std::string str1 = str;
			while (str1.find("--") != string::npos)
			{
				size_t index{ str1.find("--") };
				str1.erase(index, 2);
				str1.insert(index, "+");
			}
			bool Brackets{ false };
			size_t iter{};
			while (str1.find_first_of("(") != string::npos)
			{
				Brackets = true;
				size_t begin = str1.find_first_of("(");
				iter += begin + 1;
				size_t end = str1.substr(str1.find_first_of("("),str1.size()).find_first_of(")") + str1.find_first_of("(");
				str1 = str1.substr(begin + 1, end - begin - 1);
			}
			while (str1.find_first_of("*/") != string::npos)
			{
				char ch = str1[str1.find_first_of("*/")];
				size_t index = str1.find_first_of("*/");
				Operation(str1, index, ch);
			}
			while ((str1.substr(1, str1.size()).find_first_of("+-") != string::npos) && (str1.find_last_of("+-") != 0))
			{
				char ch = str1.substr(1,str1.size())[str1.substr(1, str1.size()).find_first_of("+-")];
				size_t index = str1.substr(1, str1.size()).find_first_of("+-") + 1;
				Operation(str1, index, ch);
			}
			if (Brackets)
			{
				str.erase(iter - 1, str.substr(iter,str.size() - iter).find_first_of(')') + 2);
				str.insert(iter - 1, str1);
			}
			else str = str1;
		}
		std::cout << std::setprecision(10) << str << std::endl;
	}
	catch (const string& err)
	{
		std::cout << err << std::endl;
	}
}

void Calculator::Operation(string& str1, const size_t& index, const char& op)
{
	double num1{};
	double num2{};
	string str_num1{};
	string str_num2{};
	size_t begin{};
	size_t end{};
	string result{};
	begin = str1.substr(0, str1.find_first_of(op)).find_last_of(operations);
	end = index;
	if (begin == string::npos)
		begin = 0;
	str_num1 = str1.substr(begin, end - begin);
	num1 = std::stod(str_num1);
	begin = index + 1;
	end = str1.substr(index + 2, str1.size()).find_first_of(operations);
	if (end == string::npos)
		end = str1.size();
	str_num2 = str1.substr(begin, end - begin + index + 2);
	num2 = std::stod(str_num2);

	begin = str1.substr(0, str1.find_first_of(op)).find_last_of(operations);
	switch (op)
	{
	case '*':
		result = to_string(num1 * num2);
		break;
	case '/':
		result = to_string(num1 / num2);
		break;
	case '+':
		result = to_string(num1 + num2);
		break;
	case '-':
		result = to_string(num1 - num2);
		break;
	default:
		break;
	}
	if (begin == string::npos)
		begin = 0;
	str1.erase(begin, end - begin + index + 2);
	str1.insert(begin, result);
}

int Calculator::FindCharCount(string str, const string& ch)
{
	int count{};
	while (str.find_first_of(ch) != string::npos)
	{
		count++;
		str = str.substr(str.find_first_of(ch) + 1, str.size() - str.find_first_of(ch));
	}
	return count;
}