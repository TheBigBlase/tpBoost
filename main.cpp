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

#include <tuple>
#include <vector>

#include "chrono.hpp"
#include "utils.hpp"



int main(int argc, char * argv[]){

	//Chrono C("TOTAL SCRIPT");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> unif(0, 1);//uniform distribution between 0 and 1

	int const n = NUMBER_NODE;

	// On crée le graphe
	double p = 1;
	Chrono c("Total " + std::to_string(p));

	MatrixGraph* g = new MatrixGraph(n);

	for (auto v{ 0 }; v < n; v++) {
		vertexIter bestVertex = getBestVertexRandom(g);
		if (v != *bestVertex) 
		{
			boost::add_edge(*bestVertex, v, *g);
		}
		
	}

	boost::print_graph(*g);

	std::cout << "Cycles : " << hasCycle(g) << std::endl;
}
