#include <iostream>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/adjacency_list.hpp>
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
#include "chrono.hpp"
#include "utils.hpp"

#define NUMBER_NODE 10000
int main(int argc, char * argv[]){

	ListGraph* g = new ListGraph(NUMBER_NODE);
	BestVertexList * bestVertex = new BestVertexList;
	auto bestVertexArray = new std::array<BestVertexList *, 10>{0};

	for(auto & k : *bestVertexArray){
		k = new BestVertexList;
		k->maxDegree = 0;
		k->bestVertex = -1;
	}


	bestVertex->maxDegree = 0;
	bestVertex->bestVertex = -1;

	boost::graph_traits<ListGraph>::vertex_iterator v, v_end;

	for (boost::tie(v, v_end) = boost::vertices(*g); v != v_end; ++v){
		auto current_vertex = boost::vertex(*v, *g);
		if (bestVertex->bestVertex != current_vertex){
			int out = boost::degree(current_vertex, *g);
			if(bestVertex->maxDegree <= out){ // select best vertex
				bestVertex->bestVertex = current_vertex;
			}
			//link current vertex w/ best
			boost::add_edge(current_vertex, bestVertex->bestVertex, *g);
			bestVertex->maxDegree++;
			bestVertexArrayAppend(*bestVertexArray, {.bestVertex=current_vertex, .maxDegree=1});
			bestVertexArrayAppend(*bestVertexArray, *bestVertex);
		}
	}
	bestVertexArrayPrint(*bestVertexArray);

	for(auto &k : *bestVertexArray){
		k->bestVertex = -1;
		k->maxDegree = 0;
	}
	bestVertex->maxDegree = 0;
	bestVertex->bestVertex = -1;



}
