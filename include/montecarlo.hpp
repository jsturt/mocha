#pragma once
#include <map>
#include <cmath>
#include <array>
#include <chrono>
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
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
		std::vector<std::vector<double>> sample(unsigned int m, samplingMethod method);
	private:
		double m_scaleToBounds(double uniform_sample, std::array<double,2> bounds);
		std::vector<std::array<double,2>> m_bounds;
		std::mt19937 m_mt;
		std::uniform_real_distribution<> m_dist;
	};

	double stddev(std::array<double,2>& data, unsigned int N, double vol);
	double volume(std::vector<std::array<double,2>> bounds);
	std::array<double,2> estimate(Expr integrand, std::vector<std::array<double,2>> bounds , unsigned int samples, samplingMethod method);
};
