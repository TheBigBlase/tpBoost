#include <iostream>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/undirected_dfs.hpp>
#include <boost/graph/properties.hpp>
#include <array>
#include <string>
#include <chrono>
#include <random>
#include <tuple>

#define NUMBER_NODE 1000

typedef boost::adjacency_matrix<
					boost::undirectedS,
						boost::property< boost::vertex_color_t, int >,
						boost::property< boost::edge_weight_t, int >
				> MatrixGraph;

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
};

void createJsonDegreeFrequency(const std::map<int, int>& distrib, const double p)
{
	std::string nameFile = "./FrequencyDegree/" + std::to_string(p) + ".json";

	std::ofstream FrequencyDegree;
	FrequencyDegree.open(nameFile);

	if (FrequencyDegree.is_open())
	{
		std::string* json = new std::string;
		*json = "[";


		for (const auto& it : distrib)
		{
			*json += ("{\"degree\":" + std::to_string(it.first) + ",\"freq\":" + std::to_string(it.second) + "},\n");
		}

		// On supprime la dernière virgule
		json->pop_back();
		json->pop_back();

		*json += "]";

		FrequencyDegree << *json;

		FrequencyDegree.close();
	}
	else
	{
		std::cout << "Une erreur s'est produite !" << std::endl;
	}

}

int main(int argc, char * argv[]){

	Chrono C("TOTAL SCRIPT");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> unif(0, 1);//uniform distribution between 0 and 1


	//std::array<std::array<int, n>, n> edgeMatrix {0};

	for(double p = 0.001; p < 1 ; p+=0.01){
		// On crée nos objets
		Chrono c("TOTAL " + std::to_string(p)); // Pour calculer temps du script
		std::map<int, int> distribDegree; // Map degree - iteration
		MatrixGraph * g = new MatrixGraph(NUMBER_NODE); // Le graphe

		// On crée les arêtes selon la prob. p 

		for(auto v {0} ; v < NUMBER_NODE; v++){
			for(auto w {0} ; w < NUMBER_NODE; w++){
				if(p > unif(gen) && w != v){
					boost::add_edge(w, v, *g);
				}
			}
		}

		Chrono x("PROCESSING " + std::to_string(p)); // Temps de traitement
		
		// On parcourt chaque noeud pour et on retourne le degree.
		boost::graph_traits<MatrixGraph>::vertex_iterator v, v_end;
		for (boost::tie(v, v_end) = boost::vertices(*g); v != v_end; ++v)
		{
			int degree = boost::out_degree(*v, *g);
			distribDegree[degree] += 1;
		}

		createJsonDegreeFrequency(distribDegree, p); // On crée le JSON
		delete g; // On est dans un vrai langage donc aucun garbage collector
	}
}
