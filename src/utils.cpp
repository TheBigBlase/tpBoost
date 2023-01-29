#include "utils.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <array>
#include <iostream>
#include <fstream>
#include <string>


//gdb sucks with c++ data structure, for instance map.
//this is for debugging purposes : 
//

void testPrint(std::map<vertexIter, std::pair<int, int>>& m) {
	for(auto k : m){
		std::cout << "key: " << *k.first
			<< ", value : " << k.second.first 
			<< ", " << k.second.second 
			<< std::endl;
	}
}

void createJsonDegreeFrequency(const std::map<int, int>& distrib, const double p)
{
	std::string nameFile = "./FrequencyDegree/" + std::to_string(p) + ".json";

	std::ofstream FrequencyDegree(nameFile);

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

int firstNonWhiteVertex(boost::vector_property_map<boost::default_color_type, boost::typed_identity_property_map<size_t>> *colormap,MatrixGraph * g){

	MatrixGraph::vertex_iterator it, itEnd;

	for(boost::tie(it, itEnd) = boost::vertices(*g) ; it != itEnd ; it++){
		if((*colormap)[*it] != 4){
			return *it;
		}
	}
	return -1;
}

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

vertexIter getBestVertexRandom(const MatrixGraph* G){
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
			double randomDegree = (double)(degree == 0 ? 1 : degree)*unifReal(gen);

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

vertexIter selectBestVertexDefect(const MatrixGraph* G, std::map<vertexIter, 
		std::pair<int, int>> * karmaMap){

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> unifReal(0.5, 1.5);//uniform distribution between 0.5 and 1.5
																											//
	double maxDegree = 0;
	vertexIter bestVertex; // check if not change at the end
																											//
	vertexIter v, v_end;
	//select best vertex for our
	for(boost::tie(v, v_end) = boost::vertices(*G); v != v_end; ++v) {
		std::cout << "looking at " << *v;

		double karma = (double)(*karmaMap)[v].first / (*karmaMap)[v].second;
		std::cout << " | karma : " << karma;
		if(karma < 0.5){//connect only to min karma
			std::cout << " | Karma too low" << std::endl;
			continue;
		}
		else {
			int degree = boost::out_degree(*v, *G);
			double randomDegree = (double)(degree)*unifReal(gen);
			std::cout << " | real degree : " << degree << " | percieved degree : " << randomDegree;

			if(randomDegree > maxDegree) {
				maxDegree = randomDegree;
				std::cout << " | new best";
				bestVertex = v;
			}
			std::cout << std::endl;
		}
 	}
	return bestVertex;
}

void defect(MatrixGraph * G, std::map<vertexIter, 
		std::pair<int, int>> * karmaMap) {
	//complexity worst case: O(n^2) : not good
	//idea : if comparing the other way around 
	//(ie first check the karma (conencted node)
	//to the one connecting) we get better perf


	vertexIter v, v_end;
	//select best vertex for our
	for(auto v {0} ; v < NUMBER_NODE; ++v) {
		vertexIter selectedV = selectBestVertexDefect(G, karmaMap);
		if(v != *selectedV) {//if cheating
			int degree = boost::out_degree(boost::vertex(v, *G), *G);
			boost::clear_vertex(boost::vertex(v, *G), *G);
			boost::add_edge(*selectedV, v, *G);

			(*karmaMap)[selectedV] = {(*karmaMap)[selectedV].first,
				(*karmaMap)[selectedV].second + 1};//best vertex has cheated for 1 con

			vertexIter currentV = (vertexIter)boost::vertex(v, *G);
			(*karmaMap)[currentV] = {(*karmaMap)[currentV].first + 1,
				(*karmaMap)[currentV].second + degree};//worst has cheated for its degree

			//clueless
			std::cout << "[CLEAR VERTEX]"
					<< std::endl;
		}
		else {//not cheating
			(*karmaMap)[selectedV] = {(*karmaMap)[selectedV].first + 1,
				(*karmaMap)[selectedV].second + 1};//reinforce its karma
			std::cout << "[NOTHING DONE]" << std::endl;
		}
	}
}

vertexDescrMatr getBestVertex(const MatrixGraph* G, 
		BestVertexList * bestVertex, vertexDescrMatr v) {
	int out = boost::out_degree(v, *G);
	if(bestVertex->maxDegree > out){
		return bestVertex->bestVertex;
	}
	else{
		bestVertex->maxDegree = out;
		bestVertex->bestVertex = v;
		return v;
	}
}

void bestVertexArrayAppend(std::array<BestVertexList *, 10> a, BestVertexList v){
	for(auto &k : a){
		if(k->bestVertex == v.bestVertex){
			k->maxDegree = v.bestVertex;
			return;
		}
		if(k->maxDegree < v.maxDegree){
			k->maxDegree = v.maxDegree;
			k->bestVertex = v.bestVertex;
			return;
		}
	}
}

void bestVertexArrayPrint(std::array<BestVertexList *, 10> a){
	for(auto &k : a){
		std::cout << "vertex: "<< k->bestVertex << " degree: " << k->maxDegree << std::endl;
	}
}

void initKarmaMap(std::map<vertexIter, std::pair<int, int>> * map,
		const MatrixGraph * G){
	vertexIter v, v_end;
	for (boost::tie(v, v_end) = boost::vertices(*G); v != v_end; ++v) {
		(*map)[v] = {std::pair<int, int> {1,1}};//??????
	}
}
