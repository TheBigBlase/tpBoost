#include <iostream>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graph_utility.hpp>
#include <array>
#include <string>
#include <random>
#include <chrono>
#include <ctime>
#include <map>

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
		std::cout << m_name <<  " :" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.f << std::endl;
	}
};


typedef boost::adjacency_matrix<
				boost::undirectedS,
				boost::property< boost::vertex_color_t, std::string >,
        boost::property< boost::edge_weight_t, int >> MatrixGraph;

int main(int argc, char * argv[]){
	
	// Random
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> unif(0, 1);//uniform distribution between 0 and 1

	int const n = 10000;


	for (double p = 0.01; p <= 1; p += 0.09)
	{
		std::cout << "------------" << std::endl;
		Chrono C("Total");
		std::map<int, int> distrib;
		MatrixGraph G(n);

		for (int v = 0; v < n; v++) {
			for (int w = 0; w < n; w++) {
				if (p > unif(gen) && w != v) {
					boost::add_edge(w, v, G);
				}
			}
		}

		// BAGATATA

		Chrono c("Babidji");
		boost::graph_traits<MatrixGraph>::vertex_iterator v, v_end;

		for (boost::tie(v, v_end) = boost::vertices(G); v != v_end; ++v)
		{
			int degree = boost::out_degree(*v, G);
			//std::cout << *v << " " << boost::out_degree(*v, G) << std::endl;

			distrib[degree] += 1;
		}

		Chrono x("AAAAAAA");

		std::string nameFile = "./FrequencyDegree/" + std::to_string(p) + ".json";

		std::ofstream FrenquencyDegree;
		FrenquencyDegree.open(nameFile);

		if (FrenquencyDegree.is_open())
		{
			std::cout << "ok" << std::endl;
		}

		std::string* json = new std::string;

		//FrenquencyDegree << "[";
		*json = "[";


		for (const auto& it : distrib)
		{
			//std::cout << it.first << "-" << it.second << std::endl;

			//FrenquencyDegree << "{\"degree\":" << std::to_string(it.first) << ",\"freq\":" << std::to_string(it.second) << "},\n";
			*json += ("{\"degree\":" + std::to_string(it.first) + ",\"freq\":" + std::to_string(it.second) + "},\n");
		}

		json->pop_back();
		json->pop_back();

		*json += "]";

		FrenquencyDegree << *json;

		FrenquencyDegree.close();
	}
}
