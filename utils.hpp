#ifndef UTILS_HPP
#define UTILS_HPP
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <random>
#include <array>

#define NUMBER_NODE 100

typedef boost::adjacency_matrix<
					boost::undirectedS,
						boost::property< boost::vertex_color_t, int >,
						boost::property< boost::edge_weight_t, int >
				> MatrixGraph;

typedef boost::adjacency_list<
					boost::vecS,
					boost::vecS,
					boost::undirectedS,
						boost::property< boost::vertex_color_t, int >
				> ListGraph;

typedef std::pair<boost::graph_traits<MatrixGraph>::vertex_descriptor, 
									boost::graph_traits<MatrixGraph>::vertex_descriptor>
				PairVertexIterator;

typedef boost::graph_traits<MatrixGraph>::vertex_iterator vertexIter;
typedef boost::graph_traits<MatrixGraph>::vertex_descriptor vertexDescrMatr;
typedef boost::graph_traits<ListGraph>::vertex_descriptor vertexDescrList;

typedef struct {
    vertexDescrList bestVertex;
    int maxDegree;
} BestVertexList;

void createJsonDegreeFrequency(const std::map<int, int>& distrib, const double p);
int hasCycle(MatrixGraph * g);
int firstNonWhiteVertex(boost::vector_property_map<boost::default_color_type, boost::typed_identity_property_map<size_t>> *colormap,MatrixGraph * g);

vertexIter getBestVertex(const MatrixGraph* G);
vertexIter getBestVertexRandom(const MatrixGraph* G);
vertexDescrMatr getBestVertex(const MatrixGraph* G, BestVertexList * bestVertex, vertexDescrMatr v);

void bestVertexArrayAppend(std::array<BestVertexList *, 10>, BestVertexList v);
void bestVertexArrayPrint(std::array<BestVertexList *, 10>);

#endif
