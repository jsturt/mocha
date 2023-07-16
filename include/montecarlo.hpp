#pragma once
#include <map>
#include <cmath>
#include <array>
#include <vector>
#include <random>
#include "loadExpr.hpp"

namespace mc{	
	
	enum samplingMethod{
		RANDOM,
		HYPERCUBE,
		STRATIFIED
	};

	inline std::map<std::string, samplingMethod> methodFromString = {{"RANDOM", RANDOM}, {"HYPERCUBE", HYPERCUBE}, {"STRATIFIED", STRATIFIED}};

	class SampleGen{
	public:
		SampleGen(std::vector<std::array<double,2>> bounds); 
		std::vector<double> sample(samplingMethod method);
	private:
		std::mt19937 m_Mt;
		std::vector<std::uniform_real_distribution<>> m_Dists;
	};

	double stddev(std::array<double,2>& data, unsigned int N, double vol);
	double volume(std::vector<std::array<double,2>> bounds);
	std::array<double,2> estimate(Expr integrand, std::vector<std::array<double,2>> bounds , unsigned int samples, samplingMethod method);
};
