// SPDX-License-Identifier: EUPL-1.2
/* Copyright Mitran Andrei-Gabriel 2023 */

#include <bits/stdc++.h>

using namespace std;

// Non-existent node
#define NIL -1

// Very large value
#define INF (1LL << 60)

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	// Maximum number of nodes
	static constexpr int NMAX = (int)1e5 + 5;  // 10^5 + 5 = 100.005
	static constexpr int PMAX = (int)845;  // 845 = 3 * 5 * 7 * 8 + 5

	// A node is a room and an edge is a corridor
	// n = number of nodes, m = number of edges, k = number of portals
	int n, m, k;

	// adj[aux] = adjacency list of node aux
	// example: if adj[aux] = {..., neigh, ...} => arc (aux, neigh) exists
	// portal_adj[aux] = adjacency list of node aux, but only for portals
	// example: if portal_adj[aux] = {..., neigh, ...} => there is a portal
	// from aux to neigh
	vector<pair<int, int>> adj[NMAX], portal_adj[NMAX];

	// P[i][j] = the minimum cost to reach node i at time j
	// It acts as a simple visited array, but it also stores the minimum cost
	// to reach a node at a certain time, for every combination remainders for
	// the portal periods
	long long P[NMAX][PMAX];

	// lcm_aux = the least common multiple of all portal periods
	int lcm_aux = 1;

	/**
	 * @brief
	 * Time: O(log(min(a, b)))
	 * Auxiliary Space: O(1)
	 * 
	 * Computes the greatest common divisor of a and b.
	 */
	int gcd(int a, int b) {
		if (b == 0)
			return a;
		return gcd(b, a % b);
	}

	/**
	 * @brief
	 * Time: O(log(min(a, b))), as it calls gcd(a, b)
	 * Auxiliary Space: O(1)
	 * 
	 * Computes the least common multiple of a and b.
	 */
	int lcm(int a, int b) {
		return a * b / gcd(a, b);
	}

	/**
	 * @brief
	 * Time: O(n + m + k)
	 * Space: O(n + m + k), for the adjacency lists (including portal_adj)
	 * 
	 * Reads the input from the file.
	 */
	void read_input() {
		// Input file
		ifstream fin("teleportare.in");

		// Reads n, m and k
		fin >> n >> m >> k;

		// Reads the edges
        for (int i = 1, x, y, w; i <= m; ++i) {
			fin >> x >> y >> w;
			adj[x].push_back({y, w});
			adj[y].push_back({x, w});
		}

		// Reads the portals and computes the least common multiple
		for (int i = 1, x, y, period; i <= k; ++i) {
			fin >> x >> y >> period;
			lcm_aux = lcm(lcm_aux, period);

			portal_adj[x].push_back({y, period});
			portal_adj[y].push_back({x, period});
		}

		// Closes the input file
		fin.close();
	}

	/**
	 * @brief
	 * Time: O(n + m + k)
	 * Space: O(n + m + k), overall
	 * 
	 * Computes the minimum cost to reach node n.
	 *
	 * @return The minimum cost to reach node n.
	 */
	long long get_result() {
		// Initializes the minimum cost to reach node i at time j with INF
		for (int i = 1; i <= n; ++i) {
			for (int j = 0; j <= lcm_aux; ++j) {
				P[i][j] = INF;
			}
		}

		// min_queue (by default -> max_queue)
		// Compares using the first element of the pair
		priority_queue<pair<long long, int>, vector<pair<long long, int>>,
						greater<pair<long long, int>>> pq;

		// Adds the first node to the queue
		pq.push(make_pair(0LL, 1));

		int node, neigh, cost, period;
		long long cost_node;

		// Dijkstra
		while (!pq.empty()) {
			// Gets the node with the minimum cost
			node = pq.top().second;
			cost_node = pq.top().first;

			// Removes the node from the queue
			pq.pop();

			// If the node is the destination, returns the minimum cost
			if (node == n)
				return cost_node;

			// If the minimum cost to reach the node at time
			// cost_node % lcm_aux is less than the current cost,
			// then the node was already visited
			if (P[node][cost_node % lcm_aux] < cost_node)
				continue;

			// For each neighbour of the current node
			for (auto neigh_w : adj[node]) {
				// neigh = neighbour, cost = cost of the arc (node, neigh)
				neigh = neigh_w.first;
				cost = neigh_w.second;

				// If the minimum cost to reach node neigh at time
				// (cost_node + cost) % lcm_aux is less than or equal to the
				// current cost, then the node was already visited
				if (P[neigh][(cost_node + cost) % lcm_aux] <= cost_node + cost)
					continue;

				// Updates the minimum cost to reach node neigh at time
				P[neigh][(cost_node + cost) % lcm_aux] = cost_node + cost;

				// Adds the node to the queue
				pq.push(make_pair(cost_node + cost, neigh));
			}

			// For each portal of the current node
			for (auto neigh_w : portal_adj[node]) {
				// neigh = neighbour, period = period of the portal
				neigh = neigh_w.first;
				period = neigh_w.second;

				// 1 is the cost of the portal
				// If the minimum cost to reach node neigh at time
				// (cost_node + 1) % lcm_aux is less than or equal to the
				// current cost, then the node was already visited
				if (P[neigh][(cost_node + 1) % lcm_aux] <= cost_node + 1)
					continue;

				// Updates the minimum cost to reach node neigh at time
				// (cost_node + 1) % lcm_aux and adds the node to the queue
				// if the time is a multiple of the period
				if (cost_node % period == 0) {
					P[neigh][(cost_node + 1) % lcm_aux] = cost_node + 1;
					pq.push(make_pair(cost_node + 1, neigh));
				}
			}
		}

		return -1;
	}

	/**
	 * @brief Prints the result.
	 *
	 * @param result The minimum cost to reach node n.
	 */
	void print_output(long long result) {
		ofstream fout("teleportare.out");

		fout << result << "\n";
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
