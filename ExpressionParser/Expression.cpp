#include "Expression.h"
#include <cmath>
#include <iostream>

std::map<std::string, double> Expression::vars;
std::map<std::string, std::pair<std::string, Expression*>> Expression::functions;

Expression::Expression() {}
Expression::~Expression() {}

double Expression::lookUp(const std::string& var) {
	if (vars.find(var) != vars.end())
		return vars[var];
	return 0;
}

void Expression::record(const std::string& var, double value)
{
	vars[var] = value;
}

void Expression::record(const std::string function, std::string& param, Expression* body) {
	functions[function] = std::make_pair(param, body);	
}

NumberExpression::NumberExpression(double val) : value(val) 
{
}

double NumberExpression::evaluate() {
	return value;
}

double NumberExpression::substitute(std::string var, double value) {
	return this->value;
}

VariableExpression::VariableExpression(const std::string& var) : variable(var)
{
}

double VariableExpression::evaluate() {
	return Expression::lookUp(variable);
}

double VariableExpression::substitute(std::string var, double value) {
	if ((this->variable.compare(var)) == 0)
		return value;	
	return evaluate();
}

ArithmeticExpression::ArithmeticExpression(char ch, Expression *l, Expression *r)
	: left(l), right(r), op(ch)
{
}

ArithmeticExpression::~ArithmeticExpression() {
	if (left)
		delete left;
	if (right)
		delete right;
}

double ArithmeticExpression::evaluate() {
	if (left == nullptr || right == nullptr)
		return 0;

	double result = 0;
	double a = left->evaluate();
	double b = right->evaluate();
	switch (op) {
	case '+':
		result = a + b;
		break;
	case '-':
		result = a - b;
		break;
	case '*':
		result = a * b;
		break;
	case '/':
		result = a / b;
		break;
	case '^':
		result = std::pow(a, b);
		break;
	}
	return result;
}

double ArithmeticExpression::substitute(std::string var, double value) {
	if (left == nullptr || right == nullptr)
		return 0;

	double result = 0;
	double a = left->substitute(var, value);
	double b = right->substitute(var, value);
	switch (op) {
	case '+':
		result = a + b;
		break;
	case '-':
		result = a - b;
		break;
	case '*':
		result = a * b;
		break;
	case '/':
		result = a / b;
		break;
	case '^':
		result = std::pow(a, b);
		break;
	}
	return result;
}

AssignmentExpression::AssignmentExpression(VariableExpression *v, Expression *val)
	: var(v), value(val)
{
}

AssignmentExpression::~AssignmentExpression() {
	if (var)
		delete var;
	if (value)
		delete value;
}

double AssignmentExpression::evaluate() {
	double x = 0;
	if (value)
		x = value->evaluate();
	if (var) {
		Expression::record(var->getVar(), x);
	}
	return x;
}

double AssignmentExpression::substitute(std::string var, double value) {
	double x = 0;
	if (this->value)
		x = this->value->substitute(var, value);
	if (this->var) {
		Expression::record(this->var->getVar(), x);
	}
	return x;
}

FunctionDeclareExpression::FunctionDeclareExpression(const std::string& function, const std::string& param, Expression* body): 
	function(function), param(param), bodyExpression(body)
{
}

double FunctionDeclareExpression::evaluate()
{
	if (bodyExpression)
		Expression::record(function, param, bodyExpression);
	return 0;
}

double FunctionDeclareExpression::substitute(std::string var, double value) {
	return evaluate();
}

FunctionCallExpression::FunctionCallExpression(const std::string& function, Expression* value) :
	function(function), value(value)
{
}

FunctionCallExpression::~FunctionCallExpression()
{
	if (value)
		delete value;
}

double FunctionCallExpression::evaluate()
{
	if (functions.find(function) != functions.end()) {
		std::pair<std::string, Expression*> f = functions[function];
		std::string param = std::get<0>(f);
		Expression* body = std::get<1>(f);
		double x = value->evaluate();

		return body->substitute(param, x);
	}
	return 0;
	 
}

double FunctionCallExpression::substitute(std::string var, double value) {
	if (functions.find(function) != functions.end()) {
		std::pair<std::string, Expression*> f = functions[function];
		std::string param = std::get<0>(f);
		Expression* body = std::get<1>(f);
		double x = this->value->substitute(var, value);

		return body->substitute(param, x);
	}
	return 0;
}