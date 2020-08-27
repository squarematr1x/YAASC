#include "Scanner.h"

namespace scanner {

void HandleInput(std::string& input)
{
	CleanDuplicateOperators(input);
	AddMultiplySign(input);
	AddUnaryToken(input);
	InfixToPostfix(input);
	UnaryTokenOff(input);
}

void InfixToPostfix(std::string& input)
{
	std::string postfix_string = "";
	std::stack<char> operator_stack;

	for (unsigned int i = 0; i < input.length(); i++)
	{
		while (input[i] == ' ')
			++i;

		if (Variable(input[i]))
		{
			postfix_string += input[i];
			postfix_string += ' ';
		}

		else if (isdigit(input[i]))
		{
			if (i + 1 < input.length() && isdigit(input[i + 1]))
				postfix_string += input[i];
			else if (i + 1 < input.length() && input[i + 1] == '.')
			{
				postfix_string += input[i];
				postfix_string += input[i + 1];
				++i;
			}
			else
			{
				postfix_string += input[i];
				postfix_string += ' ';
			}
		}

		else if (input[i] == '(')
			operator_stack.push('(');

		else if (input[i] == ')')
		{
			while (!operator_stack.empty() && operator_stack.top() != '(')
				AddStackTopToString(postfix_string, operator_stack);

			if (operator_stack.size() > 0 && operator_stack.top() == '(')
				operator_stack.pop();
		}

		else if (input[i] == '_')
			postfix_string += input[i];

		// Operator is scanned
		else
		{
			while (!operator_stack.empty() && Precedence(input[i]) <= Precedence(operator_stack.top()))
				AddStackTopToString(postfix_string, operator_stack);

			operator_stack.push(input[i]);
		}
	}
	// Popping remaining elements from the stack
	while (!operator_stack.empty())
		AddStackTopToString(postfix_string, operator_stack);

	input = postfix_string;
}

void AddUnaryToken(std::string& input)
{
	std::string new_string = "";

	for (int i = 0; i < (int)input.length(); i++)
	{
		if (i - 1 > 0 && input[i] == '-')
		{
			if (!Operand(input[i - 1]) && !RightParenthesis(input[i - 1]))
				new_string += '_';
			else
				new_string += input[i];
		}
		else
			new_string += input[i];
	}

	input = new_string;
}

void UnaryTokenOff(std::string& input)
{
	std::string new_string = "";

	for (unsigned int i = 0; i < input.length(); i++)
	{
		if (input[i] == '_')
			new_string += '-';
		else
			new_string += input[i];
	}

	input = new_string;
}

void AddMultiplySign(std::string& input)
{
	std::string new_input = "";
	for (unsigned int i = 0; i < input.length(); i++)
	{
		new_input += input[i];

		if (CanAddMultiplySign(input[i], input[i + 1]) && i + 1 < input.length())
			new_input += '*';
	}
	input = new_input;
}

void RemoveMultiplySign(std::string& input)
{
	std::string new_input = "";
	for (unsigned int i = 0; i < input.length(); i++)
	{
		if (input[i] != '*')
			new_input += input[i];
	}
	input = new_input;
}

void CleanDuplicateOperators(std::string& input)
{
	std::string new_input = "";

	for (unsigned int i = 0; i < input.length(); i++)
	{
		new_input += input[i];

		if (!Operand(input[i]))
		{
			while (i + 1 < input.length() && input[i] == input[i + 1])
				++i;
		}
	}

	input = new_input;
}

void AddStackTopToString(std::string& input, std::stack<char>& char_stack)
{
	char stack_top = char_stack.top();
	char_stack.pop();
	input += stack_top;
	input += ' ';
}

bool Variable(char c)
{
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
		return true;

	return false;
}

bool Operand(char c)
{
	if (Variable(c) || isdigit(c))
		return true;

	return false;
}

bool LeftParenthesis(char c)
{
	if (c == '(')
		return true;

	return false;
}

bool RightParenthesis(char c)
{
	if (c == ')')
		return true;

	return false;
}

bool CanAddMultiplySign(char c, char next_c)
{
	if (Variable(c) && Operand(next_c))
		return true;
	else if (Variable(c) && LeftParenthesis(next_c))
		return true;
	else if (isdigit(c) && Variable(next_c))
		return true;
	else if (isdigit(c) && LeftParenthesis(next_c))
		return true;
	else if (RightParenthesis(c) && Operand(next_c))
		return true;
	else if (RightParenthesis(c) && LeftParenthesis(next_c))
		return true;

	return false;
}

bool MissingParenthesis(std::string input)
{
	int left_count = 0;
	int right_count = 0;

	for (unsigned int i = 0; i < input.length(); i++)
	{
		if (input[i] == '(')
			left_count++;
		else if (input[i] == ')')
			right_count++;
	}

	if (left_count == right_count)
		return false;

	return true;
}

int Precedence(char c)
{
	if (c == '+' || c == '-')
		return 1;
	else if (c == '*' || c == '/')
		return 2;
	else if (c == '^')
		return 3;
	else if (c == '!')
		return 4;
	else if (c == '_')
		return 5;

	return -1;
}

} // namespace scanner
