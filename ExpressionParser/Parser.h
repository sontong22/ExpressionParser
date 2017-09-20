#pragma once
class Expression;
class Tokenizer;

class Parser
{
public:
	Parser(char* line);
	~Parser();

	Expression* statement();
private:
	Tokenizer* tokens;

	Expression* assignment();
	Expression* functionDeclare();
	Expression* calculation();	
	Expression* functionCall(); 
	Expression* sum();
	Expression* product();
	Expression* factor();
	Expression* power();
	Expression* term();
	Expression* group();
};
