#include "Calculator.h"

using namespace Calculator;

void Calculator::Transition(string& expr_orig)
{
	try
	{
		std::erase(expr_orig, ' ');
		if (expr_orig.find_first_not_of(operations + numbers + "()") != string::npos)
			throw std::string("���������� ����������� ������������ �������");
		if (expr_orig.find_first_of(numbers) == string::npos)
			throw std::string("�������� ��������� ���������");
		if ((expr_orig.find("-*") != string::npos) || (expr_orig.find("-/") != string::npos))
			throw std::string("�������� ��������� ��������");
		if (FindCharCount(expr_orig, "(") != FindCharCount(expr_orig, ")"))
			throw std::string("���������� ����������� ������ �� ������������� ���������� �����������");
		if ((expr_orig.find_first_of("*/") == 0) || (expr_orig.find_last_of(operations) == (expr_orig.size() - 1)))
			throw std::string("�������� ��������� ��������");
		while ((expr_orig.find_first_of(operations) != string::npos) && (expr_orig.find_last_of("+-") != 0))
		{//���� � ������ ������������ ������� �������������� �������� � ��� �������� +- ����� ��������
			std::string expr = expr_orig;//������������� ���������� ��� �������� ������ ���������
			while (expr.find("--") != string::npos)//�������� �� ������� �������� ������ �� ��������� ����
			{
				size_t index{ expr.find("--") };
				expr.erase(index, 2);//��������� ������� �����
				expr.insert(index, "+");//��������� ����
			}
			bool Brackets{ false };//��� ����������� ����������� ��������� ���������
			size_t iter{};//������ ������ ����������� �������� � ������� �������� �����
			while (expr.find_first_of("(") != string::npos)
			{//���� ���� ��������� ��������� ��������� ���������
				Brackets = true;//����������� ���������� ���������
				size_t begin = expr.find_first_of("(");//������ ���������� ���������
				iter += begin + 1;//���������� ������� ������ ��������� ������ + 1 (������ ���������� ���������)
				size_t end = expr.substr(expr.find_first_of("("),expr.size()).find_first_of(")") + expr.find_first_of("(");//����� ���������� ��������� - ������ ��������� )
				expr = expr.substr(begin + 1, end - begin - 1);//����� ��������� ���������� ���������
			}
			while (expr.find_first_of("*/") != string::npos)
			{//���� � ������ ��������� ���� �������� ��������� � ��������� - ��������� ��
				char ch = expr[expr.find_first_of("*/")];//���������� ������ ��������
				size_t index = expr.find_first_of("*/");//���������� ������ �������
				try
				{
					Operation(expr, index, ch);//������� ������ � ���������� ��������� �������� � ������ ���������
				}
				catch (const int&)
				{
					throw(std::string("���������� ������� �� ����"));
				}
			}
			while ((expr.substr(1, expr.size()).find_first_of("+-") != string::npos) && (expr.find_last_of("+-") != 0))
			{//���� � ������ ��������� ���� �������� �������� ������� - ��������� �� (���� ����� �� �����������)
				char ch = expr.substr(1,expr.size())[expr.substr(1, expr.size()).find_first_of("+-")];
				size_t index = expr.substr(1, expr.size()).find_first_of("+-") + 1;
				Operation(expr, index, ch);
			}
			if (Brackets)//���� ���� ������� ��������� ���������
			{
				expr_orig.erase(iter - 1, expr_orig.substr(iter,expr_orig.size() - iter).find_first_of(')') + 2);//�������� ��������� ���������
				expr_orig.insert(iter - 1, expr);//��������� ������������� ���������� � �����������
			}
			else expr_orig = expr;//����� ����������� �������� ���������� ��������� - ������������� �����������������
		}
		std::cout << "�����>>";
		std::cout << std::setprecision(10) << expr_orig << std::endl;//������� ����� � ��������� 10 ��������
	}
	catch (const string& err)//����� ��������������� ������
	{
		std::cout << err << std::endl;
	}
	catch (const exception& err)//�������������� ����������
	{
		std::cout << err.what() << std::endl;
	}
}

	void Calculator::Operation(string& expression, const size_t& index, const char& op)
	{

		double num1{};//���������� ������� ��������
		double num2{};//���������� ������� ��������
		string str_num1{};//���������� ������ ������� ��������
		string str_num2{};//���������� ������ ������� ��������
		size_t begin{};//���������� ������� ������ ������ �������� � ������
		size_t end{};//���������� ������� ����� ������ �������� � ������
		string result{};//���������� ������ ���������� ��������
		begin = expression.substr(0, expression.find_first_of(op)).find_last_of(operations);//������� ������ ������� ���������� �������� ��� ������ ������ ������� ��������
		end = index;//����� ������ ������� �������� - ������ ������� ������� ��������
		if (begin == string::npos)//���� ���������� �������� �� ����������, �� �� ��������� ���� ����������� ����
			begin = 0;
		else
			begin++;//����� ������+1 (�������� ������ ������ ������� ��������)
		str_num1 = expression.substr(begin, end - begin);//�������� ������ �������
		num1 = std::stod(str_num1);//������������ �� ������ � ������������ ���
		begin = index + 1;//������ ������ ������� �������� - ������ ������� ������� ��������+1
		end = expression.substr(index + 2, expression.size()).find_first_of(operations);//����� ������ ������� �������� - ������ ������� ����������� ��������
		if (end == string::npos)//���� ��������� �������� ��� - ����������� ����� ������
			end = expression.size();
		str_num2 = expression.substr(begin, end - begin + index + 2);
		num2 = std::stod(str_num2);

		switch (op)//���������� �������� ����� ���������� ���������
		{
		case '*':
			result = to_string(num1 * num2);
			break;
		case '/':
			if (num2 == 0)
			{
				throw(0);
			}
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

		begin = expression.substr(0, expression.find_first_of(op)).find_last_of(operations);//����� ���� ������ ������� ��������
		if (begin == string::npos)
			begin = 0;
		else
			begin++;
		expression.erase(begin, end - begin + index + 2);//� �������� ����������� ��������� � ������ ���������� �����������
		expression.insert(begin, result);
	}

int Calculator::FindCharCount(string str, const string& ch)
{
	int count{};//���������� �������� ��������� ������� � ������
	while (str.find_first_of(ch) != string::npos)//����� ������� ���� ������� ������ ������� ��������� ������� �� ������ npos (�� �������)
	{
		count++;
		str = str.substr(str.find_first_of(ch) + 1, str.size() - str.find_first_of(ch));//������� ��������� ����� ������� ���������� ��������� �������
	}
	return count;
}