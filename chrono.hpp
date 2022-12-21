#include <iostream>

struct Chrono
{
	std::chrono::steady_clock::time_point begin;
	std::string m_name;

	Chrono(const std::string& name)
	{
		m_name = name;
		begin = std::chrono::steady_clock::now();
	}

	~Chrono()
	{
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << m_name << " :" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.f << std::endl;
	}
} ;
