#include "montecarlo.hpp"

namespace mc{

	SampleGen::SampleGen(std::vector<std::array<double,2>> bounds) : m_bounds(bounds) , m_mt(std::random_device()()) 
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
				std::cout<<"RANDOM BRANCH\n";
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
				std::cout<<"HYPERCUBE BRANCH\n";
				/* theory notes
				{{{
				requires a set of bins (m) to divide each axis (N) into.
				LHS optimally returns all samples at once, better results are found for
				1 set of m=1000 rather than 10 sets of m=100.

				::: algorithm ::: //
				generate indices 
				shuffle along N-1 axes
				sample inside each strata
				record result and return

				--- generate hyperplane & shuffle
				{{{
				for 2D:
				■ 1 2 3 4 5      ■ 1 2 3 4 5
				1 x              1   x       
				2   x        =>  2         x   
				3     x          3     x     
				4       x    =>  4 x         
				5         x      5       x      
				aka (1,1,...,1), (2,2,...,2), ..., (m-1,m-1,...,m-1) where each vector has N entries.
				}}}
				--- compute binning 
				{{{
				for each dimension sample selected strata 
				- take sample in [index/m,index/m + 1/m)
					aka take real dist on [0,1), div output by m, add index/m
				}}}
				}}}
				*/

				// compute number of dimensions
				int n_dims = m_bounds.size();
				// preallocate vectors to save time over push_back's
				std::vector<std::vector<int>> dimensions(n_dims);
				std::vector<int> index_list(m);
				// initialise rng objects for shuffling
				std::random_device rd;
				std::mt19937 mt(rd());			
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
	
		std::vector<std::vector<double>> xvals = sam.sample(samples, method);
		// take and record samples
		for(const auto& x : xvals)
		{
			//for(const auto& val : x)
			//{
			//	std::cout<<val<<" ";
			//}
			//std::cout<<std::endl;
			double tmp = integrand.evaluate(x);
			values[0] += tmp;
			values[1] += pow(tmp,2);
		}
	
		// manipulate and pack results
		values[0] = vol * values[0] / ((double) samples);
		values[1] = stddev(values, samples, vol);
	
		return values;
	}
	
};
