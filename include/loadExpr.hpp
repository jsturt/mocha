#pragma once
#include <iostream>
#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "muParser.h"


class Expr{
public:
	Expr(std::string textExpression, std::vector<std::string> variableNames);
	double evaluate(std::vector<double> x);
private:
	mu::Parser m_p;
	std::string m_expression;
	std::vector<std::string> m_names;
	std::vector<std::shared_ptr<double>> m_variables;
};
