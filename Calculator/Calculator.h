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

	void Transition(string& str);
	void Operation(string& str1, const size_t& index, const char& op);
	int FindCharCount(string str, const string& ch);
}