#pragma once

#include <chrono>
#include <string>
#include <iostream>

class ScopeTimer{
public:
	ScopeTimer(std::string report_message) : m_message(report_message), m_start(std::chrono::high_resolution_clock::now()){}
	~ScopeTimer() 
	{
		m_end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::ratio<1>> span = (m_end - m_start);
		std::cout<<m_message<<" "<<span.count()<<" seconds.\n";
	}
private:
	std::string m_message;
	std::chrono::high_resolution_clock::time_point m_start;
	std::chrono::high_resolution_clock::time_point m_end;
};

class ReportTimer{
public:
	ReportTimer() : m_start(std::chrono::high_resolution_clock::now()) {}
	double Report()
	{
		m_end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::ratio<1>> span = (m_end - m_start);
		return span.count();
	}
private:
	std::chrono::high_resolution_clock::time_point m_start;
	std::chrono::high_resolution_clock::time_point m_end;
};
