#include "Parser.h"
#include "Expression.h"
#include "Tokenizer.h"

Parser::Parser(char* line)
	: tokens(new Tokenizer(line))
{
}

Parser::~Parser()
{
	delete tokens;
}

Expression* Parser::statement() {
	Expression *exp = nullptr;
	if ((exp=assignment()) || (exp=calculation()) || (exp=functionDeclare()))
		;
	return exp;
}

Expression* Parser::assignment()
{
	int mark = tokens->mark();
	VariableExpression *var = nullptr;
	Expression *rhs = nullptr;
	if ((var=tokens->variable()) && tokens->character('=') && (rhs=sum()) && tokens->atEnd()) 
		return new AssignmentExpression(var, rhs);
	if (var)
		delete var;
    if (rhs)
		delete rhs;
	tokens->reset(mark);
	return nullptr;
}

Expression* Parser::functionDeclare()
{
	int mark = tokens->mark();
	VariableExpression* function = nullptr, *param = nullptr;
	Expression* rhs = nullptr;

	if ((function=tokens->variable()) && tokens->character('(') && (param = tokens->variable()) && tokens->character(')') &&
		tokens->character('=') && (rhs = sum()) && tokens->atEnd())

		return new FunctionDeclareExpression(function->getVar(), param->getVar(), rhs);

	if (function) delete function;
	if (param) delete param;
	if (rhs) delete rhs;
	tokens->reset(mark);
	return nullptr;
		
}

Expression* Parser::calculation()
{
	Expression *result;
	int mark = tokens->mark();
	if ((result=sum()) && tokens->atEnd())
		return result;
    if(result) {
		delete result;
		result == nullptr;
	}
	tokens->reset(mark);
	return nullptr;
}

Expression* Parser::functionCall()
{
	int mark = tokens->mark();
	VariableExpression* function = nullptr;
	Expression* value = nullptr;
	if ((function = tokens->variable()) && tokens->character('(') && (value = sum()) && tokens->character(')'))
		return new FunctionCallExpression(function->getVar(), value);

	if (function) delete function;
	if (value) delete value;
	tokens->reset(mark);
	return nullptr;
}

Expression* Parser::sum()
{
	int mark = tokens->mark();
	Expression *lhs = product();
	Expression *rhs;

	while (lhs) {
		if (tokens->character('+')) {
			if (rhs=product())
				lhs = new ArithmeticExpression('+', lhs, rhs);
			else {
				delete lhs;
				lhs = nullptr;
			}
		}
		else if (tokens->character('-')) {
			if (rhs=product())
				lhs = new ArithmeticExpression('-', lhs, rhs);
			else {
				delete lhs;
				lhs = nullptr;
			}
		}
		else
			break;
	}

	if (lhs == nullptr)
		tokens->reset(mark);
	return lhs;
}

Expression* Parser::product()
{
	int mark = tokens->mark();
	Expression *lhs = factor();
	Expression *rhs;
	while (lhs) {
		if (tokens->character('*')) {
			if (rhs=factor())
				lhs = new ArithmeticExpression('*', lhs, rhs);
			else {
				delete lhs;
				lhs = nullptr;
			}
		}
		else if (tokens->character('/')) {
			if (rhs=factor())
				lhs = new ArithmeticExpression('/', lhs, rhs);
			else {
				delete lhs;
				lhs = nullptr;
			}
		}
		else
			break;
	}
	if (lhs == nullptr)
		tokens->reset(mark); 
	return lhs;
}

Expression* Parser::factor() {
	Expression *exp = nullptr;
	if ((exp=power()) || (exp=term()))
		;
	return exp;
}

Expression* Parser::power()
{
	int mark = tokens->mark();
	Expression *lhs = nullptr;
	Expression *rhs = nullptr;
	if ((lhs=term()) && tokens->character('^') && (rhs=factor()))
		return new ArithmeticExpression('^', lhs, rhs);
	if (lhs)
		delete lhs;
	tokens->reset(mark);
	return nullptr;
}

Expression* Parser::term()
{
	Expression *exp = nullptr;
	/*if ((exp = group()) || (exp = functionCall()) || (exp = tokens->variable()) || (exp = tokens->number()))*/
	if ((exp = group()) || (exp = tokens->number()) || (exp = functionCall()) || (exp = tokens->variable()))
		;
	return exp;
}

Expression* Parser::group() {
	int mark = tokens->mark();
	Expression *exp = nullptr;
	if (tokens->character('(') && (exp=sum()) && (tokens->character(')')))
		;
	else if (exp) {
		delete exp;
		exp = nullptr;
		tokens->reset(mark);
	}
	return exp;
}
