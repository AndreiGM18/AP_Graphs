// SPDX-License-Identifier: EUPL-1.2
/* Copyright Mitran Andrei-Gabriel 2023 */

#include <bits/stdc++.h>

using namespace std;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	// Maximum number of nodes
	static constexpr int NMAX = (int)1e5 + 5;  // 10^5 + 5 = 100.005

	// A node is a task, and an edge is a dependency between two tasks.
	// n = number of nodes, m = number of edges
	int n, m;

	// adj[aux] = adjacency list of node aux
	// example: if adj[aux] = {..., neigh, ...} => arc (aux, neigh) exists
	vector<int> adj[NMAX];

	// vertices_cnt[i] = number of nodes that point to node i
	vector<unsigned long> vertices_cnt;

	// data_set[i] = 1 if vertex i requires data set 1; or 2 otherwise
	vector<int> data_set;

	/**
	 * @brief
	 * Time: O(n + m)
	 * Space: O(n + m), for the adjacency lists
	 *
	 * Reads the input from the file.
	 */
	void read_input() {
		// Input file
		ifstream fin("supercomputer.in");

		// Reads n and m
		fin >> n >> m;

		// Adds a dummy node
		data_set.push_back(0);

		// Initializes the number of nodes that point to each node
		vertices_cnt.resize(n + 1, 0);

		// Reads the data set for each node
		for (int i = 1, set; i <= n; ++i) {
			fin >> set;
			data_set.push_back(set);
		}

		// Reads the edges
		for (int i = 1, x, y; i <= m; ++i) {
			fin >> x >> y;
			++vertices_cnt[y];
			adj[x].push_back(y);
		}

		// Closes the input file
		fin.close();
	}

	/**
	 * @brief
	 * Time: O(n + m)
	 * Auxiliary Space: O(n + m), for the queues and the copy of vertices_cnt
	 *
	 * Utilizes the topological sort algorithm to find the minimum
	 * number of context switches.
	 *
	 * @param switched If true, the starting queue is q2; or q1 otherwise.
	 * It is used to start from both data sets at some point in order to
	 * make sure that the minimum number of context switches is found.
	 *
	 * @return The minimum number of context switches.
	 */
	int topo_sort_cnt(bool switched) {
		// q1 = queue for data set 1, q2 = queue for data set 2
		queue<int> q1, q2;

		// vertices_cnt_copy = copy of vertices_cnt
		vector<unsigned long> *vertices_cnt_copy =
			new vector<unsigned long> (vertices_cnt);

		// context_switches = number of times the context was switched
		int context_switches = 0, q1_data_set, q2_data_set;

		// Sets the starting queue
		if (switched) {
			q1_data_set = 2;
			q2_data_set = 1;
		} else {
			q1_data_set = 1;
			q2_data_set = 2;
		}

		// Adds the nodes that have no dependencies to the queues if they
		// require their respective data set
		for (int i = 1; i <= n; ++i) {
			if ((*vertices_cnt_copy)[i] == 0 && data_set[i] == q1_data_set) {
				q1.push(i);
			}
			if ((*vertices_cnt_copy)[i] == 0 && data_set[i] == q2_data_set) {
				q2.push(i);
			}
		}

		// Stops when there are no more nodes to be processed
		while (!q1.empty() || !q2.empty()) {
			// If the queue is not empty, continously removes the nodes
			// from the queue and decreases the number of nodes that point
			// to the current node. If the number of nodes that point to
			// the current node is 0, and the node requires the respective
			// data set, the node is added to the queue.
			while (!q1.empty()) {
				int node = q1.front();
				q1.pop();

				for (auto neigh : adj[node]) {
					--(*vertices_cnt_copy)[neigh];
					if ((*vertices_cnt_copy)[neigh] == 0 && data_set[neigh] ==
						q1_data_set) {
						q1.push(neigh);
					}

					if ((*vertices_cnt_copy)[neigh] == 0 && data_set[neigh] ==
						q2_data_set) {
						q2.push(neigh);
					}
				}
			}

			// If the queue is not empty, the context is switched
			if (!q2.empty()) {
				++context_switches;
			}

			// Repeats the same process for the second queue
			while (!q2.empty()) {
				int node = q2.front();
				q2.pop();

				for (auto neigh : adj[node]) {
					--(*vertices_cnt_copy)[neigh];
					if ((*vertices_cnt_copy)[neigh] == 0 && data_set[neigh] ==
						q1_data_set) {
						q1.push(neigh);
					}

					if ((*vertices_cnt_copy)[neigh] == 0 && data_set[neigh] ==
						q2_data_set) {
						q2.push(neigh);
					}
				}
			}

			// If the queue is not empty, the context is switched
			if (!q1.empty()) {
				++context_switches;
			}
		}

		// Deletes the copy of vertices_cnt
		delete vertices_cnt_copy;

		// Returns the minimum number of context switches
		return context_switches;
	}

	/**
	 * @brief
	 * Time: O(n + m)
	 *
	 * Space: O(n + m), overall
	 * 
	 * Finds the minimum number of context switches.
	 *
	 * @return The minimum number of context switches.
	 */
	int get_result() {
		// Initializes the number of nodes that point to each node
		vertices_cnt.resize(n + 1);

		// Finds the minimum number of context switches by starting from
		// the queues for both data sets
		int context_switches = min(topo_sort_cnt(false), topo_sort_cnt(true));

		return context_switches;
	}

	/**
	 * @brief Prints the result.
	 *
	 * @param context_switches The minimum number of context switches.
	 */
	void print_output(const int context_switches ) {
		ofstream fout("supercomputer.out");
		fout << context_switches << '\n';
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
