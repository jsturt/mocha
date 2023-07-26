#include "util.hpp"

void SEM(std::vector<double> samples, unsigned int N)
{
	// calculate the standard error of the mean of population samples
	unsigned int k = samples.size();
	double mu = 0.0;
	double sqd_residuals = 0.0;
	double stddev = 0.0;
	// calculate population mean
	for(const auto& elem : samples)	{	mu += elem;}
	mu /= k;

	// calculate squared residuals
	for(const auto& elem : samples)	{	sqd_residuals += pow(elem - mu,2);}
	
	// calculate standard deviation
	stddev = sqrt(sqd_residuals/(N-1));

	// calculate & return standard error of the mean
	std::cout<<mu<<" "<<stddev<<" "<<stddev/sqrt(N)<<std::endl;
}
