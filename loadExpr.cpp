#include "loadExpr.hpp"

Expr::Expr(std::string textExpression, std::vector<std::string> variableNames)
{
	m_expression = textExpression;
	m_names = variableNames;
	// generate variables for each integration variable
	for(unsigned int i=0;i<m_names.size();i++) {m_variables.push_back(std::make_shared<double>(1.0));}
	// declare variables to the parser
	int i=0;
	for(std::shared_ptr<double> variable : m_variables)
	{
		m_p.DefineVar(m_names[i], variable.get());
		i++;
	}
	// parse expression
	m_p.SetExpr(m_expression);
}

double Expr::evaluate(std::vector<double> x)
{
	int i=0;
	for(std::shared_ptr<double> variable : m_variables)
	{
		*variable = x[i];
		i++;
	}
	return m_p.Eval();
}
