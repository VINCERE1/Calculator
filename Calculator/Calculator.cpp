#include "Calculator.h"

using namespace Calculator;

void Calculator::Transition(string& expr_orig)
{
	try
	{
		std::erase(expr_orig, ' ');
		if (expr_orig.find_first_not_of(operations + numbers + "()") != string::npos)
			throw std::string("Обнаружены неизвестные калькулятору символы");
		if (expr_orig.find_first_of(numbers) == string::npos)
			throw std::string("Неверный синтаксис выражения");
		if ((expr_orig.find("-*") != string::npos) || (expr_orig.find("-/") != string::npos))
			throw std::string("Неверный синтаксис операций");
		if (FindCharCount(expr_orig, "(") != FindCharCount(expr_orig, ")"))
			throw std::string("Количество открывающих скобок не соответствует количеству закрывающих");
		if ((expr_orig.find_first_of("*/") == 0) || (expr_orig.find_last_of(operations) == (expr_orig.size() - 1)))
			throw std::string("Неверный синтаксис операций");
		while ((expr_orig.find_first_of(operations) != string::npos) && (expr_orig.find_last_of("+-") != 0))
		{//Пока в строке присутствуют символы поддерживаемых операций и нет символов +- кроме значащих
			std::string expr = expr_orig;//Промежуточная переменная для хранения строки выражения
			while (expr.find("--") != string::npos)//Проверка на наличие двойного минуса во избежании сбоя
			{
				size_t index{ expr.find("--") };
				expr.erase(index, 2);//Извлекаем двойной минус
				expr.insert(index, "+");//Вставляем плюс
			}
			bool Brackets{ false };//Для обозначения обнаружения скобочных выражений
			size_t iter{};//Индекс начала выполняемой операции в текущей итерации цикла
			while (expr.find_first_of("(") != string::npos)
			{//Цикл ищет последнее вложенное скобочное выражение
				Brackets = true;//Обнаружение скобочного выражения
				size_t begin = expr.find_first_of("(");//Начало скобочного выражения
				iter += begin + 1;//Присвоение индекса первой найденной скобки + 1 (начало скобочного выражения)
				size_t end = expr.substr(expr.find_first_of("("),expr.size()).find_first_of(")") + expr.find_first_of("(");//Конец скобочного выражения - первая найденная )
				expr = expr.substr(begin + 1, end - begin - 1);//Берем подстроку скобочного выражения
			}
			while (expr.find_first_of("*/") != string::npos)
			{//Пока в строке выражения есть операции умножения и вычитания - выполняем их
				char ch = expr[expr.find_first_of("*/")];//Запоминаем символ операции
				size_t index = expr.find_first_of("*/");//Запоминаем индекс символа
				try
				{
					Operation(expr, index, ch);//Функция поиска и выполнения требуемой операции в строке выражения
				}
				catch (const int&)
				{
					throw(std::string("Обнаружено деление на ноль"));
				}
			}
			while ((expr.substr(1, expr.size()).find_first_of("+-") != string::npos) && (expr.find_last_of("+-") != 0))
			{//Пока в строке выражения есть операции сложения деления - выполняем их (знак числа не учитывается)
				char ch = expr.substr(1,expr.size())[expr.substr(1, expr.size()).find_first_of("+-")];
				size_t index = expr.substr(1, expr.size()).find_first_of("+-") + 1;
				Operation(expr, index, ch);
			}
			if (Brackets)//Если было найдено скобочное выражение
			{
				expr_orig.erase(iter - 1, expr_orig.substr(iter,expr_orig.size() - iter).find_first_of(')') + 2);//Вырезаем скобочное выражения
				expr_orig.insert(iter - 1, expr);//Вставляем промежуточную переменную с результатом
			}
			else expr_orig = expr;//Иначе присваиваем основной переменной выражения - промежуточную отредактированную
		}
		std::cout << "Ответ>>";
		std::cout << std::setprecision(10) << expr_orig << std::endl;//Выводим ответ с точностью 10 символов
	}
	catch (const string& err)//Вывод предусмотренных ошибок
	{
		std::cout << err << std::endl;
	}
	catch (const exception& err)//Непредвиденные исключения
	{
		std::cout << err.what() << std::endl;
	}
}

	void Calculator::Operation(string& expression, const size_t& index, const char& op)
	{

		double num1{};//Переменная первого операнда
		double num2{};//Переменная второго операнда
		string str_num1{};//Переменная строки первого операнда
		string str_num2{};//Переменная строки второго операнда
		size_t begin{};//Переменная индекса начала поиска операнда в строке
		size_t end{};//Переменная индекса конца поиска операнда в строке
		string result{};//Переменная строки результата операции
		begin = expression.substr(0, expression.find_first_of(op)).find_last_of(operations);//Находим индекс символа предыдущей операции для начала поиска первого операнда
		end = index;//Конец поиска первого операнда - индекс символа текущей операции
		if (begin == string::npos)//Если предыдущая операция не обнаружена, то во избежание сбоя присваиваем ноль
			begin = 0;
		else
			begin++;//Иначе индекс+1 (получаем индекс начала первого операнда)
		str_num1 = expression.substr(begin, end - begin);//Вырезаем первый операнд
		num1 = std::stod(str_num1);//Конвертируем из строки в вещественный тип
		begin = index + 1;//Начало поиска второго операнда - индекс символа текущей операции+1
		end = expression.substr(index + 2, expression.size()).find_first_of(operations);//Конец поиска второго операнда - индекс символа последующей операции
		if (end == string::npos)//Если следующей операции нет - присваиваем длину строки
			end = expression.size();
		str_num2 = expression.substr(begin, end - begin + index + 2);
		num2 = std::stod(str_num2);

		switch (op)//Производим операцию после нахождения операндов
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

		begin = expression.substr(0, expression.find_first_of(op)).find_last_of(operations);//Опять ищем начало первого операнда
		if (begin == string::npos)
			begin = 0;
		else
			begin++;
		expression.erase(begin, end - begin + index + 2);//И заменяем выполненное выражение в строке полученным результатом
		expression.insert(begin, result);
	}

int Calculator::FindCharCount(string str, const string& ch)
{
	int count{};//Переменная подсчета вхождений символа в строку
	while (str.find_first_of(ch) != string::npos)//Поиск ведется пока функция поиска первого вхождения символа не выдаст npos (не найдено)
	{
		count++;
		str = str.substr(str.find_first_of(ch) + 1, str.size() - str.find_first_of(ch));//Берется подстрока после первого найденного вхождения символа
	}
	return count;
}