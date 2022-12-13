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
#include <random>
#include <tuple>
#define NUMBER_NODE 100000

typedef boost::adjacency_matrix<
					boost::undirectedS,
						boost::property< boost::vertex_color_t, int >,
						boost::property< boost::edge_weight_t, int >
				> MatrixGraph;

typedef std::pair<boost::graph_traits<MatrixGraph>::vertex_descriptor, 
									boost::graph_traits<MatrixGraph>::vertex_descriptor>
				PairVertexIterator;

typedef boost::property_map<MatrixGraph, boost::vertex_color_t> color_map_t;

int firstNonWhiteVertex(boost::vector_property_map<boost::default_color_type, 
		boost::typed_identity_property_map<unsigned long>> *colormap,
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

int main(int argc, char * argv[]){

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> unif(0, 1);//uniform distribution between 0 and 1

	int const n = NUMBER_NODE;

	//std::array<std::array<int, n>, n> edgeMatrix {0};

	for(float p {0} ; p < 1 ; p+=0.001){
		MatrixGraph * g = new MatrixGraph(n);
		for(auto v {0} ; v < n ; v++){
			for(auto w {0} ; w < n ; w++){
				if(p > unif(gen) && w != v){
					add_edge(w, v, *g);
				}
			}
		}

		boost::dynamic_properties dp;

		boost::graph_traits<MatrixGraph>::edge_iterator e, e_end;
			for (boost::tie(e,e_end) = edges(*g); e != e_end; ++e)
					boost::put(boost::edge_weight_t(), *g, *e, 1);


		boost::graph_traits<MatrixGraph>::vertex_iterator v, v_end;
			for (boost::tie(v,v_end) = vertices(*g); v != v_end; ++v)
					boost::put(boost::vertex_color_t(), *g, *v, *v);

		dp.property("weight", get(boost::edge_weight_t(), *g));
		dp.property("node_id", get(boost::vertex_color_t(), *g));

		//std::ofstream f("outp=" + std::to_string(p) + ".graphml");
		//boost::write_graphml(f, g, dp, true);


		int res = hasCycle(g);
		std::cout << " number of cluster for p = " << p << " : " << res << std::endl;
		
		if(res == 1){
			std::cout << "1 cluster, graph is connex. We will stop there." <<std::endl;
			break;
		}
		delete g;
	}
}
