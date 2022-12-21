#include "utils.hpp"
#include <boost/graph/depth_first_search.hpp>
#include <array>
#include <iostream>
#include <fstream>
#include <string>

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

vertexDescrMatr getBestVertex(const MatrixGraph* G, BestVertexList * bestVertex, vertexDescrMatr v)
{
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
