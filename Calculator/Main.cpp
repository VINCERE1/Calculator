#include "Calculator.h"

using namespace Calculator;

int main()
{
	setlocale(0, "");
	std::cout << "Поддерживаемые символы " << operations + "()" << std::endl;
	string s1;
	while(true)
	{
		std::cout << "Введите выражение>>";
		getline(cin, s1);
		Transition(s1);
	}
	return 0;
}