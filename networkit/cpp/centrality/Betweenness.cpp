/*
 * Betweenness.cpp
 *
 *  Created on: 29.07.2014
 *      Author: cls, ebergamini
 */

#include <stack>
#include <queue>
#include <memory>

#include "Betweenness.h"
#include "../auxiliary/PrioQueue.h"
#include "../auxiliary/Log.h"
#include "../graph/SSSP.h"
#include "../graph/Dijkstra.h"
#include "../graph/BFS.h"

namespace NetworKit {

Betweenness::Betweenness(const Graph& G, bool normalized, bool computeEdges) : Centrality(G, normalized, computeEdges) {

}

void Betweenness::run() {
	// TODO: we might want to add a parallel version
	count z = G.upperNodeIdBound();
	scoreData.clear();
	scoreData.resize(z);
	if (computeEdges) {
		count z2 = G.upperEdgeIdBound();
		edgeData.clear();
		edgeData.resize(z2);
	}
	double c;

	auto computeDependencies = [&](node s) {

		std::vector<double> dependency(z, 0.0);

		// run SSSP algorithm and keep track of everything
		std::unique_ptr<SSSP> sssp;
		if (G.isWeighted()) {
			sssp.reset(new Dijkstra(G, s, true, true));
		} else {
			sssp.reset(new BFS(G, s, true, true));
		}

		sssp->run();

		// compute dependencies for nodes in order of decreasing distance from s
		std::stack<node> stack = sssp->getStack();
		while (!stack.empty()) {
			node t = stack.top();
			stack.pop();
			for (node p : sssp->getPredecessors(t)) {
				c = (double(sssp->numberOfPaths(p)) / sssp->numberOfPaths(t)) * (1 + dependency[t]);
				dependency[p] += c;
				if (computeEdges) {
					//if (G.hasEdge(p,t)) { //TODO needed?
						edgeData[G.edgeId(p,t)] += c;
					//}
				}

			}
			if (t != s) {
				scoreData[t] += dependency[t];
			}
		}
	};

	G.forNodes(computeDependencies);
	if (normalized) {
		// divide by the number of possible pairs
		count n = G.numberOfNodes();
		count pairs = (n-2) * (n-1);
		count edges =  n    * (n-1);
		G.forNodes([&](node u){
			scoreData[u] = scoreData[u] / pairs;
			 edgeData[u] =  edgeData[u] / edges;
		});
	}
}


} /* namespace NetworKit */
