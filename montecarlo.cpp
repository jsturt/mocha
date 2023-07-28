#include "montecarlo.hpp"

namespace mc{

	SampleGen::SampleGen(std::vector<std::array<double,2>> bounds) : m_bounds(bounds) , m_mt(std::chrono::system_clock::now().time_since_epoch().count()) 
	{
 		m_dist = std::uniform_real_distribution<double>(0.0f,1.0f);
	}

	double SampleGen::m_scaleToBounds(double uniform_sample, std::array<double,2> bounds)
	{
		// n ∈ [0,1)
		// offset + (n * range)
		return bounds[0] + (uniform_sample * (bounds[1] - bounds[0]) );
	}

	std::vector<std::vector<double>> SampleGen::sample(unsigned int m, samplingMethod method)
	{
		// ::: class for generating samples ::: //
		std::vector<std::vector<double>> samples(m);
		switch(method){
			case RANDOM:
			{
				int n_dims = m_bounds.size();
				std::vector<double> tmp(n_dims);
				for(unsigned int j=0;j<m;j++)
				{
					for(int i=0;i<n_dims;i++)
					{	
						tmp[i] = m_scaleToBounds(m_dist(m_mt), m_bounds[i]);
					}
					samples[j] = tmp;
				}
				return samples;
			}
			case HYPERCUBE:
			{	
				// compute number of dimensions
				int n_dims = m_bounds.size();
				// preallocate vectors to save time over push_back's
				std::vector<std::vector<int>> dimensions(n_dims);
				std::vector<int> index_list(m);
				// initialise rng objects for shuffling
				std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());			
				// create "diagonal" hyperplane
				for(unsigned int j=0;j<m;j++)
				{
					index_list[j] = j;
				}
				// filling & shuffling indices
				dimensions[0] = index_list;
				for(int i=1;i<n_dims;i++)
				{
					std::shuffle(index_list.begin(), index_list.end(), mt);
					dimensions[i] = index_list;
				}

				// sample inside each strata
				std::uniform_real_distribution<> dist(0.0,1.0);
				std::vector<double> strata(n_dims);
				for(unsigned int j=0;j<m;j++)
				{
					for(int i=0;i<n_dims;i++)
					{
						double point = (((double) dimensions[i][j]) + dist(mt)) / ((double) m);
						strata[i] = m_scaleToBounds(point, m_bounds[i]);	
					}
					samples[j] = strata;
				}
				return samples;
			}
			default:
			{
				std::cout<<"default branch\n";
			}
		}
		return samples;
	}

	double stddev(std::array<double,2>& data, unsigned int N, double vol)
	{
		double tmp = sqrt( ( (data[1] / N) - (pow(data[0],2)/N) ) / N);
		return vol*tmp;
	}

	double new_stddev(std::vector<double> values)
	{
		unsigned int N = values.size();
		double V =0;
		double mu=0;
		for(const auto& val : values) {mu += val;}
		mu /= (N);
		for(const auto& val : values) {V += pow(val - mu, 2.0);}
		V /= (N-1);
		return sqrt(V);
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
	
	std::array<double,2> estimate(Expr integrand, std::vector<std::array<double,2>> bounds, unsigned int samples, samplingMethod method)
	{
		// return value array
		std::array<double,2> values = {0.0,0.0};
		// volume of integral
		double vol = volume(bounds);
		// init sampling class
		SampleGen sam(bounds);

		// new std dev calc
		std::vector<double> results;
	
		std::vector<std::vector<double>> xvals = sam.sample(samples, method);
		// take and record samples
		for(const auto& x : xvals)
		{
			double tmp = integrand.evaluate(x);
			results.push_back(tmp);
			values[0] += tmp;
			values[1] += pow(tmp,2);
		}
	
		// manipulate and pack results
		values[0] = vol * values[0] / ((double) samples);
		values[1] = new_stddev(results);
	
		return values;
	}
	
};
