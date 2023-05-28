// SPDX-License-Identifier: EUPL-1.2
/* Copyright Mitran Andrei-Gabriel 2023 */

#include <bits/stdc++.h>

using namespace std;

// Non-existent node
#define NIL -1

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	// Maximum number of nodes
	static constexpr int NMAX = (int)1e5 + 5;  // 10^5 + 5 = 100.005

	// A node is a deposit and an edge is a link that indicates that the
	// deposit at the source of the link can be used to fill the deposit at
	// the destination of the link
	// n = number of nodes, q = number of queries
	int n, q;

	// adj[aux] = adjacency list of node aux
	// example: if adj[aux] = {..., neigh, ...} => arc (aux, neigh) exists
	vector<int> adj[NMAX];

	// queries[i] = (d, e) => the i-th query is (d, e)
	vector<pair<int, int>> queries;

	// answers[i] = the answer for the i-th query
	// started[i] = the start time of the i-th node in the DFS
	// finished[i] = the finish time of the i-th node in the DFS
	// path[i] = the i-th node in the DFS path
	// position[i] = the position of the i-th node in the DFS path
	// parent[i] = the parent of the i-th node in the DFS tree
	vector<int> answers, started, finished, path, position, parent;

	// time = current timestamp in the DFS
	int time = 0;

	/**
	 * @brief
	 * Time: O(n + q)
	 * Space: O(n + q), for the adjacency lists and the queries
	 *
	 * Reads the input from the file.
	 */
	void read_input() {
		// Input file
		ifstream fin("magazin.in");

		// Reads n and q
		fin >> n >> q;

		// Initializes the queries
		queries.reserve(q + 1);

		// Reads the edges
		for (int i = 1, x; i < n; ++i) {
			fin >> x;
			adj[x].push_back(i + 1);
		}

		// Adds a dummy query
		queries.push_back({NIL, 0});

		// Reads the queries
		for (int i = 1, d, e; i <= q; ++i) {
			fin >> d >> e;
			queries.push_back({d, e});
		}

		// Closes the input file
		fin.close();
	}

	/**
	 * @brief
	 * Time: O(n)
	 * Auxiliary Space: O(n), for the helper vectors
	 *
	 * DFS traversal of the tree.
	 *
	 * @param node the current node
	 */
	void dfs(int node) {
		// Increase the timestamp
		++time;

		// Mark the start time of the current node
		started[node] = time;

		// Add the current node to the path
		path.push_back(node);

		// Mark the position of the current node in the path
		position[node] = path.size() - 1;

		// Traverse the neighbours of the current node
		for (auto neigh : adj[node]) {
			// If the neighbour is not visited, visit it
			if (parent[neigh] == NIL) {
				parent[neigh] = node;
				dfs(neigh);
			}
		}

		// Mark the finish time of the current node
		finished[node] = time;
	}

	/**
	 * @brief
	 * Time: O(n + q)
	 * Space: O(n + q), overall
	 *
	 * Computes the answer for each query.
	 *
	 * @return the answers for each query
	*/
	vector<int> get_result() {
		// Initializations
		answers.resize(q + 1, NIL);
		parent.resize(n + 1, NIL);
		path.reserve(n + 1);
		position.resize(n + 1, 0);
		started.resize(n + 1, 0);
		finished.resize(n + 1, 0);

		// DFS traversal of the tree
		dfs(1);

		// Computes the answer for each query
		for (int i = 1; i <= q; ++i) {
			// d = the node
			int d = queries[i].first;

			// e = the number of steps
			int e = queries[i].second;

			// If the node is not visited or the number of steps is greater
			// than the number of nodes in the path from the node to all the
			// children of the node, then the answer is -1
			if (e > finished[d] - started[d]) {
				continue;
			}

			// The node's position in the path
			auto start = position[d];

			// If the required node is in the path, then the answer is the
			// node at e steos after the given node d
			if (start + e < (int)path.size())
				answers[i] = path[start + e];
		}

		// Returns the answers
		return answers;
	}

	/**
	 * @brief Prints the output to the file.
	 *
	 * @param answers the answers for each query
	*/
	void print_output(const vector<int>& answers) {
		ofstream fout("magazin.out");

		for (auto i = 1; i <= q; ++i)
			fout << answers[i] << '\n';

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
