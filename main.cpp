#include <iostream>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graph_utility.hpp>
#include <array>
#include <string>
#include <random>

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, 
				boost::no_property, EdgeWeightProperty > DirectedGraph;
typedef boost::graph_traits<DirectedGraph>::edge_iterator edge_iterator;
typedef boost::adjacency_matrix<boost::undirectedS,
				boost::property< boost::vertex_color_t, std::string >,
        boost::property< boost::edge_weight_t, int >> MatrixGraph;

int main(int argc, char * argv[]){

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> unif(0, 1);//uniform distribution between 0 and 1

	float const p {0.5};
	int const numberOfGraph {0};
	int const n {100};


	MatrixGraph g(n);

	//std::array<std::array<int, n>, n> edgeMatrix {0};

	for(auto k {0} ; k < numberOfGraph ; k++){
		for(auto v {0} ; v < n ; v++){
			for(auto w {0} ; v < n ; v++){
				if(p < unif(gen)){
					//edgeMatrix[w][v] ++;
					//edgeMatrix[v][w] ++;
					add_edge(w, v, g);
				}
			}
		}
	}

	boost::dynamic_properties dp;



	boost::graph_traits<MatrixGraph>::edge_iterator e, e_end;
    for (boost::tie(e,e_end) = edges(g); e != e_end; ++e)
        put(boost::edge_weight_t(), g, *e, 0);

  boost::graph_traits<MatrixGraph>::vertex_iterator v, v_end;
    for (boost::tie(v,v_end) = vertices(g); v != v_end; ++v)
        put(boost::vertex_color_t(), g, *v, std::to_string(*v));

	dp.property("weight", get(boost::edge_weight_t(), g));
	dp.property("node_id", get(boost::vertex_color_t(), g));

	std::ofstream f("/tmp/test");
	boost::write_graphml(std::cout, g, dp, true);

}
