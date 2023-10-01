#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>

using namespace std;

namespace Calculator
{	
	const string operations = "*/+-";
	const string numbers = "0123456789,";

	void Transition(string& expression);//Функция разбиения введенной строки на последовательность выражений
	void Operation(string& expression, const size_t& index, const char& op);//Функция обработки операции в выражении
	int FindCharCount(string str, const string& ch);//Функция поиска количества включений char-элемента в строку для проверки строки на валидность
}