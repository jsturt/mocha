#pragma once
#include <map>
#include <cmath>
#include <array>
#include <chrono>
#include <vector>
#include <random>
#include <memory>
#include <iostream>
#include <algorithm>
#include "loadExpr.hpp"

namespace mc{	
	
	enum samplingMethod{
		RANDOM,
		HYPERCUBE
	};

	inline std::map<std::string, samplingMethod> methodFromString = {{"RANDOM", RANDOM}, {"HYPERCUBE", HYPERCUBE}};

	class Sampling{
	// interface for sampling methods
	public:
		virtual ~Sampling() = 0;
		virtual std::vector<std::vector<double>> sample(unsigned int m) = 0;
	protected:
		std::vector<std::array<double,2>> m_bounds;
		std::mt19937 m_mt;
		std::uniform_real_distribution<> m_dist;
	};

	class RandomSampling : public Sampling{
	public:
		RandomSampling(std::vector<std::array<double,2>> bounds);
		~RandomSampling() {}
		std::vector<std::vector<double>> sample(unsigned int m);
	};

	class LHSSampling : public Sampling{
	public: 
		LHSSampling(std::vector<std::array<double,2>> bounds);
		~LHSSampling() {};
		std::vector<std::vector<double>> sample(unsigned int m);
	};

	class SamplingFactory{
	public:
		std::shared_ptr<Sampling> build(samplingMethod method, std::vector<std::array<double,2>> bounds);
	};

	class Estimator{
	public:
		Estimator(std::shared_ptr<Expr> integrand, std::vector<std::array<double,2>> bounds, samplingMethod method);
		double estimate(unsigned int samples);
	private:
		std::shared_ptr<Expr> m_integrand;
		std::shared_ptr<Sampling> m_sampler;		
		double m_volume;
	};

	double stddev(std::vector<double> values);
	double volume(std::vector<std::array<double,2>> bounds);
	double scaleToBounds(double uniform_sample, std::array<double,2> bounds);
};
