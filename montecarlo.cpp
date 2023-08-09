#include "montecarlo.hpp"

namespace mc{
	
	Sampling::~Sampling() {}

	std::shared_ptr<Sampling> SamplingFactory::build(samplingMethod method, std::vector<std::array<double,2>> bounds)
	{
		switch (method){
		case RANDOM:
			return std::make_shared<RandomSampling>(bounds);
		case HYPERCUBE:
			return std::make_shared<LHSSampling>(bounds);	
		default:
			return std::make_shared<RandomSampling>(bounds);
		}
	}

	RandomSampling::RandomSampling(std::vector<std::array<double,2>> bounds) 
	{
		m_bounds = bounds;
		m_mt = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
 		m_dist = std::uniform_real_distribution<double>(0.0f,1.0f);
	}
	std::vector<std::vector<double>> RandomSampling::sample(unsigned int m)
	{
		std::vector<std::vector<double>>samples(m);		
		int n_dims = m_bounds.size();
		std::vector<double> tmp(n_dims);
		for(unsigned int j=0;j<m;j++)
		{
			for(int i=0;i<n_dims;i++)
			{	
				tmp[i] = scaleToBounds(m_dist(m_mt), m_bounds[i]);
			}
			samples[j] = tmp;
		}
		return samples;	
	}

	LHSSampling::LHSSampling(std::vector<std::array<double,2>> bounds) 
	{
		m_bounds = bounds;
		m_mt = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
 		m_dist = std::uniform_real_distribution<double>(0.0f,1.0f);
	}

	std::vector<std::vector<double>> LHSSampling::sample(unsigned int m)
	{	
		std::vector<std::vector<double>> samples(m);
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
				strata[i] = scaleToBounds(point, m_bounds[i]);	
			}
			samples[j] = strata;
		}
		return samples;
	}

	Estimator::Estimator(std::shared_ptr<Expr> integrand, std::vector<std::array<double,2>> bounds, samplingMethod method)
	{
		SamplingFactory factory;
		m_sampler = factory.build(method, bounds);
		m_integrand = integrand;
		m_volume = volume(bounds);
	}

	double Estimator::estimate(unsigned int samples)
	{
		// return value
		double result = 0;
		// sample parameter space
		std::vector<std::vector<double>> xvals = m_sampler->sample(samples);
		// calculate samples
		for(const auto& x : xvals)
		{
			double tmp = m_integrand->evaluate(x);
			result += tmp;
		}
		// manipulate and pack results
		result = m_volume * result / ((double) samples);	
		return result;
	}

	double stddev(std::vector<double> values)
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

	double scaleToBounds(double uniform_sample, std::array<double,2> bounds)
	{
		// n ∈ [0,1)
		// offset + (n * range)
		return bounds[0] + (uniform_sample * (bounds[1] - bounds[0]) );
	}

};
