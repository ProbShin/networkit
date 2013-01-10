/*
 * GraphGTest.cpp
 *
 *  Created on: 12.12.2012
 *      Author: cls
 */

#include "GraphGTest.h"

namespace EnsembleClustering {

TEST_F(GraphGTest, testEdgeIteration) {

	int64_t n = 100;
	Graph G = this->gen.makeCompleteGraph(n);

	int64_t edgeCount = 0;
	READ_ONLY_FORALL_EDGES_BEGIN(G) {
		node u = EDGE_SOURCE;
		node v = EDGE_DEST;
		if (u < v) {
			edgeCount += 1;
		}
	} READ_ONLY_FORALL_EDGES_END();

	EXPECT_EQ((n * (n-1)) / 2, edgeCount) << "There are (n * (n-1)) / 2 undirected edges in a compete graph";
}


TEST_F(GraphGTest, testLambdaEdgeIteration) {

	int64_t n = 100;
	Graph G = this->gen.makeCompleteGraph(n);

	int64_t edgeCount = 0;
	G.forallEdges([&](node u, node v) {
		if (u < v) {
			edgeCount += 1;
		}
	},"readonly");

	EXPECT_EQ((n * (n-1)) / 2, edgeCount) << "There are (n * (n-1)) / 2 undirected edges in a compete graph";
}



TEST_F(GraphGTest, testParallelLambdaEdgeIteration) {

	int64_t n = 100;
	Graph G = this->gen.makeCompleteGraph(n);

	int64_t edgeCount = 0;
	G.forallEdges([&](node u, node v) {
		if (u < v) {
			#pragma omp atomic update
			edgeCount += 1;
		}
	}, "parallel", "readonly");

	EXPECT_EQ((n * (n-1)) / 2, edgeCount) << "There are (n * (n-1)) / 2 undirected edges in a compete graph";
}




TEST_F(GraphGTest, testLambdaEdgeModification) {

	int64_t n = 100;
	Graph G = this->gen.makeCompleteGraph(n);

	G.forallEdges([&](node u, node v){
		G.removeEdge(u, v);
	});

	EXPECT_EQ(0, G.numberOfEdges()) << "all edges should have been deleted";
}



TEST_F(GraphGTest, testLambdaNodeIteration) {

	int64_t n = 100;
	Graph G = this->gen.makeCompleteGraph(n);

	int64_t nodeCount = 0;
	G.forallNodes([&](node v) {
		nodeCount++;
	});

	EXPECT_EQ(n, nodeCount);
}


TEST_F(GraphGTest, testParallelLambdaNodeIteration) {

	int64_t n = 100;
	Graph G = this->gen.makeCompleteGraph(n);

	int64_t nodeCount = 0;
	G.forallNodes([&](node v) {
		#pragma omp atomic update
		nodeCount++;
	}, "parallel");

	EXPECT_EQ(n, nodeCount);
}


TEST_F(GraphGTest, testLambdaNeighborIteration) {

	Graph G(4);
	node v = 1;
	G.insertEdge(v, 2);
	G.insertEdge(v, 3);
	G.insertEdge(v, 4);

	int neighborCount = 0;
	G.forallNeighborsOf(v, [&](node w){
		neighborCount += 1;
	});

	EXPECT_EQ(3, neighborCount) << "node v has 3 neighbors";
}


TEST_F(GraphGTest, testLambdaIncidentEdgeIteration) {

	Graph G(4);
	node v = 1;
	G.insertEdge(v, 2);
	G.insertEdge(v, 3);
	G.insertEdge(v, 4);

	int edgeCount = 0;
	G.forallEdgesOf(v, [&](node v, node w){
		edgeCount += 1;
	});

	EXPECT_EQ(3, edgeCount) << "node v has 3 incident edges";
}


TEST_F(GraphGTest, testNodeBFS) {

	Graph G(4);
	node v = 1;
	G.insertEdge(v, 2);
	G.insertEdge(v, 3);
	G.insertEdge(v, 4);

	int nodeCount = 0;
	G.breadthFirstNodesFrom(v, [&](node w) {
		nodeCount += 1;
	});

	EXPECT_EQ(4, nodeCount) << "4 nodes should have been visited by BFS";
}

TEST_F(GraphGTest, testEdgeBFS) {

	Graph G(4);
	node v = 1;
	G.insertEdge(v, 2);
	G.insertEdge(v, 3);
	G.insertEdge(v, 4);
	G.insertEdge(4, 3);

	int edgeCount = 0;
	G.breadthFirstEdgesFrom(v, [&](node x, node y) {
		edgeCount += 1;
	});

	EXPECT_EQ(4, edgeCount) << "4 edges should have been visited by BFS";
}


TEST_F(GraphGTest, testNodeIteration) {
	int64_t n = 42;
	Graph G(n);

	int64_t counter = 0;

	G.forallNodes([&](node v) {
		counter += 1;
	});

	EXPECT_EQ(n, counter) << "all nodes should have been counted";
}


TEST_F(GraphGTest, testExtendNodeRange) {
	int64_t n = 17;
	int64_t n2 = 42;
	Graph G(n);
	G.extendNodeRange(n2);

	int64_t counter = 0;

	G.forallNodes([&](node v) {
		counter += 1;
	});

	EXPECT_EQ(n2, counter) << "all nodes should have been counted";
}



} /* namespace EnsembleClustering */

void EnsembleClustering::GraphGTest::SetUp() {
}

void EnsembleClustering::GraphGTest::TearDown() {
}
