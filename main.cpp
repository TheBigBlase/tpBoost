#include <iostream>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/undirected_dfs.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/copy.hpp>
#include <array>
#include <string>
#include <chrono>
#include <random>
#include <tuple>

#define NUMBER_NODE 500

typedef boost::adjacency_matrix<
					boost::undirectedS,
						boost::property< boost::vertex_color_t, int >,
						boost::property< boost::edge_weight_t, int >
				> MatrixGraph;


typedef std::pair<boost::graph_traits<MatrixGraph>::vertex_descriptor, 
									boost::graph_traits<MatrixGraph>::vertex_descriptor>
				PairVertexIterator;

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

int firstNonWhiteVertex(boost::vector_property_map<boost::default_color_type, boost::typed_identity_property_map<size_t>> *colormap,MatrixGraph * g){

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

		//FrenquencyDegree << "[";
		*json = "[";


		for (const auto& it : distrib)
		{
			//std::cout << it.first << "-" << it.second << std::endl;

			//FrenquencyDegree << "{\"degree\":" << std::to_string(it.first) << ",\"freq\":" << std::to_string(it.second) << "},\n";
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

typedef boost::graph_traits<MatrixGraph>::vertex_iterator vertexIter;

vertexIter getBestVertex(const MatrixGraph* G)
{
	
	vertexIter v, v_end;

	int maxDegree = -1;
	vertexIter bestVertex;

	for (boost::tie(v, v_end) = boost::vertices(*G); v != v_end; ++v)
	{
		//if (v != pickedVertex)
		//{
			int degree = boost::out_degree(*v, *G);

			if (degree > maxDegree)
			{
				maxDegree = degree;
				bestVertex = v;
				//std::cout << "Degree : " << degree << " Max D : " << maxDegree << " New V " << *v << std::endl;
			}
		//}

	}
	return bestVertex;

}

int main(int argc, char * argv[]){

	//Chrono C("TOTAL SCRIPT");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> unif(0, 1);//uniform distribution between 0 and 1

	int const n = NUMBER_NODE;

	for(double p = 0.4; p < 0.55 ; p+=0.5){
		Chrono c("Total " + std::to_string(p));

		MatrixGraph* g = new MatrixGraph(n);
		
		MatrixGraph* g2 = new MatrixGraph(n);

		for(auto v {0} ; v < n ; v++){
			for(auto w {0} ; w < n ; w++){
				if(p > unif(gen) && w != v){
					boost::add_edge(w, v, *g);
					boost::add_edge(w, v, *g2);
				}
			}
		}

		//boost::print_graph(*g);
		std::cout << "---------------------" << std::endl;

		//boost::print_graph(*g2);
		std::cout << "---------------------" << std::endl;
		std::cout << "---------------------" << std::endl;

		

		Chrono x("PAS OPTI " + std::to_string(p));


		boost::graph_traits<MatrixGraph>::vertex_iterator v, v_end;
		for (boost::tie(v, v_end) = boost::vertices(*g); v != v_end; ++v)
		{
			//boost::print_graph(*g);
			//int degree = boost::out_degree(*v, *g);
			//std::cout << *v << " - degree : " << degree << std::endl;
			vertexIter bestVertex = getBestVertex(g);
			//std::cout << "Best vertex : " <<  *bestVertex << std::endl;
			if (bestVertex != v)
			{
				boost::clear_vertex(*v, *g);
				boost::add_edge(*v, *bestVertex, *g);
			}

		}

		//boost::print_graph(*g);

		x.~Chrono();

		std::cout << "---------------------" << std::endl;

		// OPTI

		Chrono x2("OPTI " + std::to_string(p));
		
		vertexIter bV = getBestVertex(g2);

		boost::graph_traits<MatrixGraph>::vertex_iterator v2, v2_end;
		for (boost::tie(v2, v2_end) = boost::vertices(*g2); v2 != v2_end; ++v2)
		{
			//boost::print_graph(*g);
			//int degree = boost::out_degree(*v, *g);
			//std::cout << *v << " - degree : " << degree << std::endl;
			//std::cout << "Best vertex : " <<  *bestVertex << std::endl;
			if (bV != v2)
			{
				boost::clear_vertex(*v2, *g2);
				boost::add_edge(*v2, *bV, *g2);
			}
			//std::cout << "---------------------" << std::endl;
		}
		

		//boost::print_graph(*g2);

		delete g;
	}
}
