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

#include "chrono.hpp"
#include "utils.hpp"

#define NUMBER_NODE 100


int main(int argc, char * argv[]){

	//Chrono C("TOTAL SCRIPT");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> unif(0, 1);//uniform distribution between 0 and 1

	int const n = NUMBER_NODE;

	// On crée le graphe
	double p = 0.8;
	Chrono c("Total " + std::to_string(p));

	MatrixGraph* g = new MatrixGraph(n);

	for (auto v{ 0 }; v < n; v++) {
		for (auto w{ 0 }; w < n; w++) {
			if (p > unif(gen) && w != v) {
				boost::add_edge(w, v, *g);
			}
		}
	}

	// On déroule l'algo

	boost::graph_traits<MatrixGraph>::vertex_iterator v, v_end;
	for (boost::tie(v, v_end) = boost::vertices(*g); v != v_end; ++v)
	{
		vertexIter bestVertex = getBestVertexRandom(g);
		if (bestVertex != v)
		{
			boost::clear_vertex(*v, *g);
			boost::add_edge(*v, *bestVertex, *g);
		}

	}

	// PB : on clear à chaque fois dc pleins de noeuds célib

	boost::print_graph(*g);


}
