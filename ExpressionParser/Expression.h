#pragma once
#include <map>
#include <string>

class Expression
{
public:
	Expression();
	virtual ~Expression();

	static double lookUp(const std::string& var);
	static void record(const std::string& var, double value);	
	static void record(const std::string function, std::string& param, Expression* body);

	virtual double evaluate() = 0;
	virtual double substitute(std::string var, double value) = 0;
protected:
	static std::map<std::string, double> vars;
	static std::map<std::string, std::pair<std::string, Expression*>> functions;
};

class NumberExpression : public Expression
{
public:
	NumberExpression(double val);

	virtual double evaluate();
	virtual double substitute(std::string var, double value);
private:
	double value;
};

class VariableExpression : public Expression
{
public:
	VariableExpression(const std::string& var);

	virtual double evaluate();
	virtual double substitute(std::string var, double value);

	std::string getVar() const { return variable; }
private:
	std::string variable;
};

class ArithmeticExpression : public Expression
{
public:
	ArithmeticExpression(char op, Expression *left, Expression *right);
	virtual ~ArithmeticExpression();

	virtual double evaluate();
	virtual double substitute(std::string var, double value);
private:
	Expression *left, *right;
	char op;
};

class AssignmentExpression : public Expression
{
public:
	AssignmentExpression(VariableExpression *var, Expression *value);
	virtual ~AssignmentExpression();

	virtual double evaluate();
	virtual double substitute(std::string var, double value);
private:
	VariableExpression *var;
	Expression *value;
};

class FunctionDeclareExpression : public Expression
{
public:
	FunctionDeclareExpression(const std::string& function, const std::string& param, Expression* body);

	virtual double evaluate();
	virtual double substitute(std::string var, double value);
private:
	std::string function, param;	
	Expression* bodyExpression;
};

class FunctionCallExpression : public Expression
{
public:
	FunctionCallExpression(const std::string& function, Expression* value);
	virtual ~FunctionCallExpression();

	virtual double evaluate();
	virtual double substitute(std::string var, double value);
private:
	std::string function;
	Expression* value;
};

