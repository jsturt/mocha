#pragma once
#include <memory>
#include <string>
#include <vector>
#include "mpParser.h"

class Expr{
public:
	virtual ~Expr() = 0;
	virtual double evaluate(std::vector<double> x) = 0;
};

class ExprMOCK : public Expr{
public:
	ExprMOCK() {}
	double evaluate(std::vector<double> x) {return 1.0;}
};

class ExprJSON : public Expr{
public:
	ExprJSON(std::string textExpression, std::vector<std::string> variableNames);
	double evaluate(std::vector<double> x);
private:
	mup::ParserX m_p;
	std::string m_expression;
	std::vector<std::string> m_names;
	std::vector<std::shared_ptr<mup::Value>> m_variables;
};
