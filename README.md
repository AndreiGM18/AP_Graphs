# AP

**Name: Mitran Andrei-Gabriel**
**Group: 323CA**

## Homework #2 (Gigel, almost busy)

### Implementation

* Every functionality required for this homework was implemented.
* Every problem (and all its tasks) was solved.
* The classic graph algorithms are taken directly from the lab pseudocode, and
the skeleton structure utilized for all problems is taken from there as well.

#### Supercomputer: Time: O(n + m), Space: O(n + m)

* Let n be the number of tasks, m be the number of dependencies between any two
tasks.
* The problem boils down to a Topological Sort algorithm, which actually gives
the Time and Space complexities of the problem: O(n + m).
* By utilizing the same logic, however instead of using a single queue as one
would in the classical algorithm, we use two in order to separately store the
tasks that require the first data set as opposed to the ones that require the
second one.
* We then do the normal algorithm, however whenever the queue that is currently
being used is empty, we switch the queue and increase the number of context
switches.
* We need to this twice, as the number of context switches may be lower if we
start with a task that requires the other data set.
* In the end, the lowest of the two is the answer.

#### Railways: Time: O(n + m), Space: O(n + m)

* Let n be the number of stations and m be the number of already built rails.
* Firstly, we need to check how many stations can be reached from the source.
As such, we can use a DFS algorithm to mark all of them as being connected to
the source, or, in other words, as having a rail. (Time: O(n + m))
* Afterwards, we can use Tarjan's algorithm in order to find all strongly
connected components (SCCs). Time: O(n + m)
* We will treat all nodes that are part of an SCC as a "pseudonode" (by having
a single node with all the others' edges given to it, and marking all of them,
besides the singular node that acts as the "pseudonode", as having rails to
them, rendering them non-existant from this moment on, as if we build a rail to
the "pseudonode", all of them will also be connected to the source).
* We can keep doing DFS for all "pseudonodes" and count how many rails are
required to be built. If during a search we find a "pseudonode" with a newly
built rail that has an edge connecting it to another which also has a newly
built rail, we only keep one of them. (Time: O(n + m))
* In the end, after repeating the process for all "pseudonodes", we will get
the minimum amount of rails that ought to be built.
* The Space complexity is also O(n + m), for the graph representation as
adjacency lists, as well as the helper vectors required in the classic
algorithms (DFS and Tarjan).

#### Teleportation: Time: O(n + m + k), Space: O(n + m + k)

* Let n be the number of rooms, m be the number of corridors and k be the number
of portals.
* The problem boils down to changing the Dijkstra algorithm in order to account
for the portals. However, this basically gives the Time and Space complexities
of the problem: Time: O(n + m + k), Space: O(n + m + k).
* First, we need to create a helper array, which acts as a simple visited array,
but it also stores the minimum cost to reach a node at a certain time, for every
combination remainders for the portal periods, since portals can only be used
at a time that is a multiple of the portal's period of activation. The array
should be a NMAX (maximum number of nodes) by PMAX (lcm([1.. 8]) + 1) array,
where lcm is the least common multiple.
* The problem stipulates that the period can not be greater than 8.
* Moreover, it is known that if we have two numbers a and b, for any number n,
(n % lcm(a, b)) % a = n % a and (n % lcm(a, b)) % b = n % b.
* As such, we can take advantage of this by using P. The time complexity that
is generated by calculating all the portal periods' lcm is negligible, as
for lcm(a, b) the complexity is O(log(min(a, b))). As stated before, the
maximum a portal period's can be is 8.
* Let lcm refer to the lcm of all portal periods' from now on.
* Then, we can start the Dijkstra algorithm, using a min priority queue and
inserting the source node into it. Then, we do continuous pops.
* We start be checking if we have reached the destination. If we have, then
that is the minimum cost. If not, we can check if the minimum cost to reach the
node at time node's cost % lcm (stored in P) is less than the current cost (the
node's), then the node was already visited and we can skip it.
* If that is not the case, then we go through all edges and portals of the
current node.
* For edges, we do a similar check: if the minimum cost to reach the neighbor
at time (node's cost + neighbor's cost) % lcm (stored in P) is less than or
equal to the current cost (the node's + the neighbor's), then the neighbour was
already visited and we can skip it. If not, we update P and add the neighbor in
the priority queue.
* For portals, we do the same thing, the key differences being that the cost for
using a portal to reach the neighbor is always 1 and that we add the neighbor
to the priority queue only if the time (the current node's cost) is a multiple
of the portal's period. Even if it is not, P should nonetheless be updated.

#### Shop (Bonus): Time: O(n + q), Space: O(n + q)

* Let n be the number of shops (and dependencies between shops + 1) and q be the
number of queries.
* The problem boils down to a singular DFS path construction. (Time: O(n),
Space: O(n)).
* By starting from the source (1) and going all the way to the end, storing each
node's time (start <=> discovery and finish <=> all children's children
discovered) and position in the DFS path, we ensure having the answer to all
possible queries in 1 go.
* For any query, one can simply find the specified node (d) in the path and jump
e steps to the right, finding the answer to query. If the e is greater than
total number of the node's (d) children's children (also known as the difference
between the finish and start times), then the answer to the query is that
e consecutive steps cannot be done.

### Compilation

* In order to compile, we use:

```bash
make
```

### Resources

* Everything provided by the AP team
* [Linux Manual](https://www.man7.org/linux/man-pages/index.html)
