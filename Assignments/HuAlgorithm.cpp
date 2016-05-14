#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include "mpi.h"

using namespace std;

vector<int> nodes;

class Tree {
private:
	vector<int> nodes;
	vector<int>* adj_list;
	map<int, int> priority_table;
	bool* visited;
	int counter;

	bool is_valid_nodes(vector<int> nodes) {
		int* freq = new int[nodes.size()];

		for (int i = 0; i < nodes.size(); i++) {
			freq[nodes[i]]++;

			if (freq[nodes[i]] >= 2) {
				throw "Nodes contains duplication!";

				return false;
			}
		}

		return true;
	}
	bool is_node_exist(int node) {
		for (int i = 0; i < this->nodes.size(); i++) {
			if (this->nodes[i] == node) {
				return true;
			}
		}

		throw "Node isn't exist in the tree!";
		return false;
	}
	void initialize_visited_array() {
		this->visited = new bool[nodes.size()];

		for (int i = 0; i < this->nodes.size(); i++)
			this->visited[i] = false;
	}

public:
	Tree() {}
	Tree(vector<int> nodes) {
		this->nodes = nodes;
		this->adj_list = new vector<int>[nodes.size()];
	}

	int get_number_of_nodes() {
		return this->nodes.size();
	}
	void set_nodes(vector<int> nodes) {
		this->nodes = nodes;
	}
	vector<int> get_nodes() {
		return this->nodes;
	}
	void make_directed_edge(int u, int v) {
		this->is_node_exist(u);
		this->is_node_exist(v);

		this->adj_list[u].push_back(v);
	}
	void make_undirected_edge(int u, int v) {
		this->is_node_exist(u);
		this->is_node_exist(v);

		this->adj_list[u].push_back(v);
		this->adj_list[v].push_back(u);
	}
	int number_of_successor_nodes(int u) {
		visited[u] = true;

		for (int i = 0; i < this->adj_list[u].size(); i++) {
			int next = this->adj_list[u][i];

			if (!visited[next]) {
				//printf("%d, \n", counter);
				counter++;
				number_of_successor_nodes(next);
			}
		}

		return counter;
	}
	void display_connections() {
		for (int i = 0; i < this->nodes.size(); i++) {
			printf("%d\n", this->nodes[i]);
			for (int j = 0; j < this->adj_list[i].size(); j++) {
				printf("%d ", this->adj_list[i][j]);
			}
			puts("");
		}
	}
	void display_node_direct_childs(int u) {
		this->is_node_exist(u);
		printf("%d\n", this->nodes[u]);

		for (int j = 0; j < this->adj_list[u].size(); j++) {
			printf("%d ", this->adj_list[u][j]);
		}
		puts("");
	}
	map<int, int> get_priority_table_map() {
		for (int i = 1; i < nodes.size(); i++) {
			this->initialize_visited_array();
			this->counter = 0;
			int priority = this->number_of_successor_nodes(nodes[i]);
			this->priority_table[nodes[i]] = priority;
		}

		return priority_table;
	}
	vector<pair<int, int>> get_priority_table_vector() {
		struct cmp
		{
			bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) {
				return left.second > right.second;
			}
		};
		vector<pair<int, int>> priority_table_vector;

		for (map<int, int>::iterator it = priority_table.begin(); it != priority_table.end(); it++) {
			priority_table_vector.push_back(make_pair(it->first, it->second));
		}

		sort(priority_table_vector.begin(), priority_table_vector.end(), cmp());

		return priority_table_vector;
	}
	void display_priority_table() {
		for (map<int, int>::iterator it = priority_table.begin(); it != priority_table.end(); it++) {
			printf("Node %d --> %d\n", it->first, it->second);
		}
	}
	int get_node_priority(int u) {
		return this->priority_table[u];
	}
	vector<int>* run_scheduling(int nproc) {
		vector<pair<int, int>> priority_table_vector = get_priority_table_vector();
		vector<int>* schedule_table;
		schedule_table = new vector<int>[100];
		vector<bool> scheduled_nodes(nodes.size());

		for (int i = 0; i < nodes.size(); i++)
			scheduled_nodes[i] = false;

		scheduled_nodes[nodes[0]] = true;
		int row = 0;

		puts("-----------BEGIN----------");

		for (int it = 0; it < priority_table_vector.size(); ) {
			vector<int> v;
			bool all_visited = false;

			for (int i = 0; i < nproc; i++) {
				if (!scheduled_nodes[priority_table_vector[it].first]) {
					all_visited = true;
					v.push_back(priority_table_vector[it].first);
					if (it < priority_table_vector.size() - 1)
						it++;
				}
			}

			if (!all_visited) break;

			vector<bool> local_visited(nodes.size());
			for (int i = 0; i < v.size(); i++)
				local_visited[i] = false;

			for (int vis_index = 0; vis_index <  v.size(); vis_index++) {
				int current_highest_priority_node = v[vis_index];

				if (!scheduled_nodes[current_highest_priority_node]) {
					map<int, bool> all_pred_visited_for_each_node;

					for (int t = 1; t < nodes.size(); t++)
						all_pred_visited_for_each_node[nodes[t]] = true;

					for (int i = 0; i < nodes.size(); i++) {
						for (int j = 0; j < this->adj_list[i].size(); j++) {
							if (current_highest_priority_node == this->adj_list[i][j]) {
								if (scheduled_nodes[nodes[i]]) {
									all_pred_visited_for_each_node[current_highest_priority_node] = false;
								}
							}
						}
					}

					for (auto node : all_pred_visited_for_each_node) {
						if (current_highest_priority_node == node.first) {
							if (node.second == false) {
								local_visited[current_highest_priority_node] = true;
							}
						}
					}
				}
			}

			for (int i = 0; i < v.size(); i++) {
				if (local_visited[v[i]]) {
					scheduled_nodes[v[i]] = true;
					if (schedule_table[row].size() < nproc) {
						schedule_table[row].push_back(v[i]);
					} else {
						schedule_table[++row].push_back(v[i]);
					}
				} else {
					it--;
					if (schedule_table[row].size() < nproc) {
						schedule_table[row].push_back(0);
					} else {
						schedule_table[++row].push_back(0);
					}
				}
			}
		}

		return schedule_table;
	}
};

void fill_nodes(vector<int> &nodes, int size) {
	int dummy = 0;
	nodes.push_back(dummy);

	for (int i = 1; i <= size; i++) {
		nodes.push_back(i);
	}
}
void create_tree(vector<int> nodes, Tree &t) {
	t.make_directed_edge(0, 1);
	t.make_directed_edge(0, 2);

	t.make_directed_edge(1, 3);
	t.make_directed_edge(1, 4);
	t.make_directed_edge(1, 5);

	t.make_directed_edge(2, 5);
	t.make_directed_edge(2, 6);

	t.make_directed_edge(3, 6);
	t.make_directed_edge(3, 7);

	t.make_directed_edge(4, 6);
	t.make_directed_edge(4, 7);

	t.make_directed_edge(5, 7);

	t.make_directed_edge(6, 8);

	t.make_directed_edge(7, 8);
	t.make_directed_edge(7, 9);
	t.make_directed_edge(7, 10);
}

void master(int nproc) {
	fill_nodes(nodes, 10);
	Tree t(nodes);
	create_tree(nodes, t);
	map<int, int> priority_table_map = t.get_priority_table_map();
	t.display_priority_table();
	vector<int>* gantt_chart = t.run_scheduling(nproc);

	for (int i = 0; i < nproc + 1; i++) {
		for (int j = 0; j < gantt_chart[i].size(); j++) {
			printf("%d ", gantt_chart[i][j]);
		}
		puts("");
	}
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int rank;
	int nproc;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	if (rank == 0) {
		master(nproc);
	} else {

	}

	MPI_Finalize();

	return 0;
}