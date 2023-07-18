#include <iostream>
#include <fstream>
#include <array>
#include "montecarlo.hpp"
#include "loadExpr.hpp"
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv)
{
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
	/* standard deviation
{{{
		for our estimation we can calculate the standard deviation via
										
																[f(x)^2] - [f(x)]^2
										σ_N = V * √---------------------
																				 N
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
	Expr expression(data["integrand"], data["variables"]);

	// perform estimation of integral of function derived from string over domain `boundsList` with samples specified by CLI args
	std::array<double,2> result = mc::estimate(expression , data["bounds"], data["samples"], mc::methodFromString[data["method"]]);
	// report estimated value and standard deviation
	std::cout<<"value: "<<result[0]<<" with std: "<<result[1]<<std::endl;

	return 0;
}
