#include "montecarlo.hpp"

namespace mc{

	SampleGen::SampleGen(std::vector<std::array<double,2>> bounds) : m_Mt(std::random_device()()) 
	{
		for(const auto& elem : bounds)
		{			
			// push back a list of uniform generators for each range of bounds
			m_Dists.push_back(std::uniform_real_distribution<double>(elem[0],elem[1]));
		}
	}

	std::vector<double> SampleGen::sample(samplingMethod method)
	{
		// ::: class for generating samples ::: //
		std::vector<double> samples;
		switch(method){
			case RANDOM:
			{
				// sample each of the uniform generators 
				for(auto& dist : m_Dists)
				{
					samples.push_back(dist(m_Mt));
				}
				return samples;
			}
			case HYPERCUBE:
			{	
				// do something else
				return samples;
			}
		}
		return samples;
	}

	double stddev(std::array<double,2>& data, unsigned int N, double vol)
	{
		double tmp = sqrt( ( (data[1] / N) - (pow(data[0],2)/N) ) / N);
		return vol*tmp;
	}

	double volume(std::vector<std::array<double,2>> bounds)
	{
		double vol = 1.0;
		for(const auto& elem : bounds)
		{
			vol *= (elem[1] - elem[0]);
		}
		return vol;
	}
	
	std::array<double,2> estimate(Expr integrand, std::vector<std::array<double,2>> bounds, unsigned int samples,samplingMethod method)
	{
		// return value array
		std::array<double,2> values = {0.0,0.0};
		// volume of integral
		double vol = volume(bounds);
		// init sampling class
		SampleGen sam(bounds);
	
		// take and record samples
		for(unsigned int i=0;i<samples;i++)
		{
			std::vector<double> xvals = sam.sample(method);
			double tmp = integrand.evaluate(xvals);
			values[0] += tmp;
			values[1] += pow(tmp,2);
		}
	
		// manipulate and pack results
		values[0] = vol * values[0] / ((double) samples);
		values[1] = stddev(values, samples, vol);
	
		return values;
	}
	
};
