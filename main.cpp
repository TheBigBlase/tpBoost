#include <iostream>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graph_utility.hpp>
#include <array>
#include <string>
#include <random>

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_matrix<boost::undirectedS,
	boost::property< boost::vertex_color_t, std::string >,
	boost::property< boost::edge_weight_t, int >> MatrixGraph;

typedef boost::graph_traits<MatrixGraph>::vertex_iterator vertexIter;

vertexIter getBestVertex(const MatrixGraph* G)
{

	vertexIter v, v_end;

	int maxDegree = -1;
	vertexIter bestVertex;

	for (boost::tie(v, v_end) = boost::vertices(*G); v != v_end; ++v)
	{

		int degree = boost::out_degree(*v, *G);

		if (degree > maxDegree)
		{
			maxDegree = degree;
			bestVertex = v;
		}

	}
	return bestVertex;
}

vertexIter getBestVertexRandom(const MatrixGraph* G) {
	//IDEA TO HAVE BETTER REPARTITION
	//make it select a random node in the top 10%
	//could "simulate" bandwith (as in top cant be connected
	//to everyone)
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> unifReal(0.5, 1.5);//uniform distribution between 0.5 and 1.5

	vertexIter v, v_end;

	double maxDegree = 0;
	vertexIter bestVertex;

	std::cout << "[NEW NODE]" << std::endl;
	for (boost::tie(v, v_end) = boost::vertices(*G); v != v_end; ++v) {
		if (v != bestVertex) {
			int degree = boost::out_degree(*v, *G);//undirected so out = in
			double randomDegree = (double)(degree == 0 ? 1 : degree) * unifReal(gen);

			std::cout << "Real degree : " << degree << " | percieved degree : " << randomDegree;

			if (randomDegree > maxDegree) {
				std::cout << " NEW MAX";
				maxDegree = randomDegree;
				bestVertex = v;
				//std::cout << "Degree : " << degree << " Max D : " << maxDegree << " New V " << *v << std::endl;
			}
			std::cout << std::endl;
		}

	}
	return bestVertex;

}


int main(int argc, char* argv[]) {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> unif(0, 1);//uniform distribution between 0 and 1

	float const p = 0.7f;
	int const n = 30;

	// On crée le graphe
	MatrixGraph* g = new MatrixGraph(n);

	for (auto v{ 0 }; v < n; v++) {
		// vertexIter bestVertex = getBestVertex(g); // première partie
		vertexIter bestVertex = getBestVertexRandom(g); // seconde partie
		if (v != *bestVertex)
		{
			boost::add_edge(*bestVertex, v, *g);
		}

	}

	boost::print_graph(*g); // On l'affiche

	// On crée le .graphml

	boost::dynamic_properties dp;

	boost::graph_traits<MatrixGraph>::edge_iterator e, e_end;
	for (boost::tie(e, e_end) = edges(*g); e != e_end; ++e)
		put(boost::edge_weight_t(), *g, *e, 1);


	boost::graph_traits<MatrixGraph>::vertex_iterator v, v_end;
	for (boost::tie(v, v_end) = vertices(*g); v != v_end; ++v)
		put(boost::vertex_color_t(), *g, *v, std::to_string(*v));

	dp.property("weight", get(boost::edge_weight_t(), *g));
	dp.property("node_id", get(boost::vertex_color_t(), *g));

	std::ofstream f("out.graphml");
	boost::write_graphml(f, *g, dp, true);
}
