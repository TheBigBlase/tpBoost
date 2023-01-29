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

#define NUMBER_NODE 10000

typedef boost::adjacency_matrix<boost::undirectedS, boost::property< boost::vertex_color_t, int >, boost::property< boost::edge_weight_t, int >> MatrixGraph;

typedef std::pair<boost::graph_traits<MatrixGraph>::vertex_descriptor, boost::graph_traits<MatrixGraph>::vertex_descriptor> PairVertexIterator;

typedef boost::property_map<MatrixGraph, boost::vertex_color_t> color_map_t;

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

int firstNonWhiteVertex(boost::vector_property_map<boost::default_color_type, 
		boost::typed_identity_property_map<size_t>> *colormap,
		MatrixGraph * g){

	MatrixGraph::vertex_iterator it, itEnd;

	for(boost::tie(it, itEnd) = boost::vertices(*g) ; it != itEnd ; it++){
		if((*colormap)[*it] != 4){
			return *it;
		}
	}
	return -1;
}

int hasCycle(MatrixGraph * g){

	auto indexmap = boost::get(boost::vertex_index, *g);
	auto colormap = boost::make_vector_property_map<boost::default_color_type>(indexmap);

	const char* color_names[] = {"white", "gray", "green", "red", "black"};

	MatrixGraph::vertex_iterator it, itEnd;
	int n = firstNonWhiteVertex(&colormap, g);
	int count {0};

	do {
		boost::depth_first_visit(*g, boost::vertex(n, *g), 
				boost::default_dfs_visitor(), colormap);
		count++;
	}
	while((n = firstNonWhiteVertex(&colormap, g)) != -1);

	return count;
}

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

	//Chrono C("TOTAL SCRIPT"); // benchmark

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> unif(0, 1); //uniform distribution between 0 and 1

	int const n = NUMBER_NODE;

	// On génère le graphe
	for(double p = 0.01; p < 1 ; p+=0.02){

		//Chrono c("Total " + std::to_string(p)); // benchmark

		std::map<int, int> distribDegree; // tableau pour la distribution des degrées

		MatrixGraph * g = new MatrixGraph(n); // Graphe

		// On génère le graphe

		for(auto v {0} ; v < n ; v++){
			for(auto w {0} ; w < n ; w++){
				if(p > unif(gen) && w != v){
					boost::add_edge(w, v, *g);
				}
			}
		}

		//Chrono x("TRAITEMENT " + std::to_string(p)); // Benchmarking

		boost::dynamic_properties dp;

		// Pour la génération du fichier .ml
		boost::graph_traits<MatrixGraph>::edge_iterator e, e_end;
		for (boost::tie(e, e_end) = boost::edges(*g); e != e_end; ++e)
		{
			boost::put(boost::edge_weight_t(), *g, *e, 1);
		}

		dp.property("weight", get(boost::edge_weight_t(), *g));
		dp.property("node_id", get(boost::vertex_color_t(), *g));

		// Enregistrement des degrés
		boost::graph_traits<MatrixGraph>::vertex_iterator v, v_end;
		for (boost::tie(v, v_end) = boost::vertices(*g); v != v_end; ++v)
		{
			int degree = boost::out_degree(*v, *g);
			distribDegree[degree] += 1;
			boost::put(boost::vertex_color_t(), *g, *v, *v);
		}

		// On crée le fichier .json de la distribution des degrés pour une probabilité p donnée
		createJsonDegreeFrequency(distribDegree, p);



		// On crée le .ml
		// 
		//std::ofstream f("outp=" + std::to_string(p) + ".graphml");
		//boost::write_graphml(f, g, dp, true);

		// Première partie de l'exercice 1

		int res = hasCycle(g);

		if(res == 1){
			std::cout << "1 cluster, graph is connex. We will stop there." <<std::endl;
			delete g;
		}
		else
		{
			std::cout << "Number of cluster for p = " << p << " : " << res << std::endl;
			delete g;
		}

	}
}
