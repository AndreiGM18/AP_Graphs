// SPDX-License-Identifier: EUPL-1.2
/* Copyright Mitran Andrei-Gabriel 2023 */

#include <bits/stdc++.h>

using namespace std;

// Very large value
#define INF (1LL << 30)

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	// Maximum number of nodes
	static constexpr int NMAX = (int)1e5 + 5;  // 10^5 + 5 = 100.005

	// A node is a station, and an edge is a rail that connects two stations.
	// n = number of nodes, m = number of edges, s = source node
	int n, m, s;

	// adj[aux] = adjacency list of node aux
	// example: if adj[aux] = {..., neigh, ...} => arc (aux, neigh) exists
	vector<int> adj[NMAX];

	// found[i] = discovery time of node i
	vector<int> found;
	// low_link[i] = lowest discovery time of a node that can be reached from i
	vector<int> low_link;
	// in_stack[i] = true <=> node i is in the stack
	vector<bool> in_stack;
	// st = stack used in Tarjan's algorithm
	stack<int> st;
	// has_rail[i] = true <=> node i has a rail
	vector<bool> has_rail;
	// redirect[i] = the node that node i redirects to
	vector<int> redirect;
	// has_edge[i] = true <=> node i has an edge to a pseudonode that has
	// a rail
	vector<bool> has_edge;
	// scc = vector that stores the nodes of a SCC
	vector<int> scc;
	// time = current time, cnt = number of rails, source_dfs = source node
	int time, cnt = 0, source_dfs;

	/**
	 * @brief
	 * Time: O(n + m)
	 * Space: O(n + m), for the adjacency lists
	 *
	 * Reads the input from the file.
	 */
	void read_input() {
		// Input file
		ifstream fin("ferate.in");

		// Reads n, m and s
		fin >> n >> m >> s;

		// Reads the edges
		for (int i = 1, x, y; i <= m; i++) {
			fin >> x >> y;
			adj[x].push_back(y);
		}

		// Initializes the redirection vector
		redirect.resize(n + 1);
		for (int i = 1; i <= n; ++i) {
			redirect[i] = i;
		}

		// Close the input file
		fin.close();
	}

	/**
	 * @brief
	 * Time: O(n + m)
	 * Auxiliary Space: O(n), for the helper vectors
	 *
	 * DFS traversal of the tree.
	 *
	 * @param node the current node
	 */
	void dfs(int node) {
		// Sets the node as having a rail
		has_rail[node] = true;

		// Goes through the neighbours of the node
		for (auto aux_neigh : adj[node]) {
			// Gets the pseudonode that the neighbour redirects to
			int neigh = redirect[aux_neigh];

			// If the neighbour doesn't have a rail, then it is visited
			if (!has_rail[neigh]) {
				dfs(neigh);
			// If the neighbour is connected to a pseudonode that already has a
			// rail that is newly constructed, then we can remove the original
			// rail
			} else if (has_edge[neigh] && source_dfs != neigh) {
				has_edge[neigh] = false;
				--cnt;
			}
		}
	}

	/**
	 * @brief
	 * Time: O(n + m)
	 * Auxiliary Space: O(n), for the helper vectors
	 *
	 * Tarjan's algorithm for finding the SCCs.
	 *
	 * @param all_sccs vector that stores all the SCCs
	 * @param u the current node
	 */
	void tarjan(vector< vector<int> > &all_sccs, int u) {
		int aux;

		// Sets the discovery time and the low link of the node
		found[u] = low_link[u] = ++time;

		// Pushes the node in the stack and sets it as being in the stack
		st.push(u);
		in_stack[u] = true;

		// Goes through the neighbours of the node
		for (auto v : adj[u]) {
			// If the neighbour hasn't been visited and it doesn't have a rail,
			// then it is visited
			if (found[v] == INF && !has_rail[v]) {
				tarjan(all_sccs, v);

				// Updates the low link of the node
				low_link[u] = min(low_link[u], low_link[v]);
			// If the neighbour has been visited and it is in the stack, then
			// the low link of the node is updated
			} else if (in_stack[v]) {
				low_link[u] = min(low_link[u], low_link[v]);
			}
		}

		// If the low link of the node is equal to its discovery time, then a
		// SCC has been found
		if (found[u] == low_link[u]) {
			// Clears the SCC vector
			scc.clear();

			// Gets the nodes of the SCC
			do {
				aux = st.top();
				st.pop();

				// Sets the node as not being in the stack
				in_stack[aux] = false;

				scc.push_back(aux);
			} while (aux != u);

			// Adds the SCC to the vector of SCCs
			all_sccs.push_back(scc);
		}
	}

	/**
	 * @brief
	 * Time: O(n + m)
	 * Space: O(n + m), overall
	 *
	 * Finds the minimum number of rails that need to be built.
	 *
	 * @return the minimum number of rails that need to be built
	 */
	int get_result() {
		vector<vector<int>> all_sccs;

		// Initializes the vectors
		found.resize(n + 1, INF);
		low_link.resize(n + 1);
		in_stack.resize(n + 1, false);
		has_rail.resize(n + 1, false);
		has_edge.resize(n + 1, false);

		// Sets the inital nodes that have rails
		dfs(s);

		// Gets the SCCs
		for (int i = 1; i <= n; ++i) {
			if (found[i] == INF && !has_rail[i]) {
				tarjan(all_sccs, i);
			}
		}

		// Redirects the nodes to the pseudonodes
		// Goes through all the SCCs
		for (auto &scc : all_sccs) {
			// Goes through all the nodes of the SCC
			for (auto &node : scc) {
				// The pseudonode is the first node of the SCC
				redirect[node] = scc[0];
				// If the node is the first node of the SCC, then it is skipped
				if (node == scc[0]) {
					continue;
				}
				// Adds the edges of all neighbours of the node to the
				// pseudonode
				for (auto &neigh : adj[node]) {
					adj[scc[0]].push_back(neigh);
				}
				// Sets the node as having a rail
				has_rail[node] = true;
			}
		}

		// Counts the number of rails that need to be built
		for (int i = 1; i <= n; ++i) {
			// If the node has a rail, then it is skipped
			if (has_rail[i]) {
				continue;
			}
			// Increments the number of rails that need to be built
			++cnt;
			// Sets the node as having an edge
			has_edge[i] = true;
			// Sets the source node for the DFS
			source_dfs = i;
			// Starts the DFS
			dfs(i);
		}

		// Returns the number of rails that need to be built
		return cnt;
	}

	/**
	 * @brief Prints the result.
	 *
	 * @param cnt the number of rails that need to be built
	 */
	void print_output(int cnt) {
		ofstream fout("ferate.out");
		fout << cnt << '\n';
		fout.close();
	}
};

int main() {
	auto* task = new (nothrow) Task();

	if (!task) {
		cerr << "new failed: WTF are you doing? Throw your PC!\n";
		return -1;
	}

	task->solve();

	delete task;

	return 0;
}
