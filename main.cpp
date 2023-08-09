#include <iostream>
#include <fstream>
#include "montecarlo.hpp"
#include "loadExpr.hpp"
#include "util.hpp"
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv)
{
	// timing
	ScopeTimer timer("Calculation finished in");
	/* estimation
{{{
		for the integral
												b
										I = ∫ f(x) dx 
												a

		we can make the estimate I~F by

														 1		 N		   _  
		                F = V * --- *  Σ	{  f(x_i)  }
														 N	  i=1
						 _
			where, {x_i} are a set of uniformly sampled vectors 
						 V is the volume of the integration region
						 N is large
}}}
	*/ 

	// sample count check
	if(argc!=2) 
	{
		std::cout<<"please provide JSON input only.\n";
		return -1;
	}

	// ::: read JSON input ::: //
	std::ifstream f(argv[1]);
	json data = json::parse(f);
	f.close();

	// ::: recover mathematical expression from string input ::: //
	std::shared_ptr<ExprJSON> expression = std::make_shared<ExprJSON>(data["integrand"], data["variables"]);
	//std::shared_ptr<ExprMOCK> expression = std::make_shared<ExprMOCK>();
	
	mc::Estimator estimator(expression, data["bounds"], mc::methodFromString[data["method"]]);
	std::vector<double> samples(15);
	for(int i=0;i<15;i++)
	{
		samples[i] = estimator.estimate(data["samples"]);
	}
	SEM(samples,data["samples"]);
	
	return 0;
}

