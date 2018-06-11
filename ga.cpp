#include "ga.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>

struct cost {
	int id;
	double val;
};

bool comp(const cost &x, const cost &y) {
	return x.val > y.val;
}

void Swap(int *x1, int *x2) {
	int tmp = *x1;
	*x1 = *x2;
	*x2 = tmp;
}

inline double distance(double x1, double x2, double y1, double y2) {
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

//bool VRP::GetRoutePath(vector<int> & route, double* path) {
//	double tmp_path = 0;
//	double path_to_depot;
//	if (route.size() > 1) {
//		for (int i = 1; i < route.size(); i ++) {
//			tmp_path += distance(customers_[route[i - 1]][0], customers_[route[i]][0], customers_[route[i - 1]][1], customers_[route[i]][1]);
//		}
//	}
//	path_to_depot = distance(depot_[0], customers_[route.front()][0], depot_[1], customers_[route.front()][1]) + distance(depot_[0], customers_[route.back()][0], depot_[1], customers_[route.back()][1]);
//	*path += path_to_depot + tmp_path;
//	return false;
//}

bool VRP::GetRoutePath(vector<int> & route, double* path) {
	double tmp_path = 0;
	double path_to_depot, path_to_new_depot;
	if (route.size() == 1) {
		path_to_depot = 2 * distance(depot_[0], customers_[route[0]][0], depot_[1], customers_[route[0]][1]);
		path_to_new_depot = 2 * distance(depot_new_[0], customers_[route[0]][0], depot_new_[1], customers_[route[0]][1]);
	} else if (route.size() > 1) {
		for (int i = 1; i < route.size(); i ++) {
			tmp_path += distance(customers_[route[i - 1]][0], customers_[route[i]][0], customers_[route[i - 1]][1], customers_[route[i]][1]);
		}
		path_to_depot = distance(depot_[0], customers_[route.front()][0], depot_[1], customers_[route.front()][1]) + distance(depot_[0], customers_[route.back()][0], depot_[1], customers_[route.back()][1]);
		path_to_new_depot = distance(depot_new_[0], customers_[route.front()][0], depot_new_[1], customers_[route.front()][1]) + distance(depot_new_[0], customers_[route.back()][0], depot_new_[1], customers_[route.back()][1]);
	}
	if (path_to_depot < path_to_new_depot) {
		*path += path_to_depot + tmp_path;
		return false;
	} else {
		*path += path_to_new_depot + tmp_path;
		return true;
	}
}

// Path function;
double VRP::GetPath(vector<int> &g) {
	double path_tot = 0;
	int load = 0;
	vector<int> route;
	for (int i = 0; i < customers_num_; i ++) {
		if (load == 0) {
			load += customers_[g[i]][2];
			route.push_back(g[i]);
		} else {
			load += customers_[g[i]][2];
			if (load <= capacity_) {
				route.push_back(g[i]);
			} else if (load > capacity_) {
				GetRoutePath(route, &path_tot);
				route.clear();
				load = customers_[g[i]][2];
				route.push_back(g[i]);
			}
		}
		if (i == customers_num_ - 1) {
			GetRoutePath(route, &path_tot);
		}
	}
	return path_tot;
}

double VRP::CostFunc(vector<int> &g) {
	return 1.0 / (GetPath(g) - lower_bound_);
}

int VRP::Select(vector<vector<int> > &population, vector<double> &p_cum) {
	vector<cost> costs(pop_size_);
	for (int i = 0; i < pop_size_; i ++) {
		costs[i].id = i;
		costs[i].val = CostFunc(population[i]);
	}
	p_cum.clear();
	p_cum.resize(population.size(), 0.0);
	p_cum[0] = costs[0].val;
	for (int i = 1; i < pop_size_; i ++) {
		p_cum[i] = p_cum[i - 1] + costs[i].val;
	}
	for (int i = 0; i < pop_size_; i ++) {
		p_cum[i] /= p_cum[pop_size_ - 1];
	}
	sort(costs.begin(), costs.end(), comp);
	//cout << "selection done" << endl;
	return costs.front().id;
}

void VRP::Crossover(vector<int> &gA, vector<int> &gB) {
	double x = rand() / (RAND_MAX * 1.0);
	if (x < pc_) { // operate OX crossover with cross length C_;
		int start = rand() % (customers_num_ - C_ + 1);
		vector<int> subA, subB;
		subA.insert(subA.begin(), gA.begin() + start, gA.begin() + start + C_);
		subB.insert(subB.begin(), gB.begin() + start, gB.begin() + start + C_);
		// scan genes in gA, remove genes appears in subB;
		for (int i = 0; i < C_; i ++) {
			for (int j = 0; j < gA.size(); j ++) {
				if (gA[j] == subB[i]) {
					gA.erase(gA.begin() + j);
					break;
				}
			}
		}
		gA.insert(gA.begin() + start, subB.begin(), subB.end());
		// scan genes in gB, remove genes appears in subA;
		for (int i = 0; i < C_; i ++) {
			for (int j = 0; j < gB.size(); j ++) {
				if (gB[j] == subA[i]) {
					gB.erase(gB.begin() + j);
					break;
				}
			}
		}
		gB.insert(gB.begin() + start, subA.begin(), subA.end());
		//cout << "crossover done" << endl;
	}
}

void VRP::Mutate(vector<int> &g) {
	double x = rand() / (RAND_MAX * 1.0);
	if (x < pm_) { // operate mutation M_ times;
		int g_len = g.size();
		int m_idx1, m_idx2;
		for (int i = 0; i < M_; i ++) {
			m_idx1 = rand() % g_len;
			m_idx2 = rand() % g_len;
			Swap(&g[m_idx1], &g[m_idx2]);
		}	
		//cout << "mutate done" << endl;
	}
}

int myrandom(int i) {return rand()%i;}
void VRP::Initialization(int seed) {
	vector<int> x;
	for (int i = 0; i < customers_num_ + 1; i ++) {
		if (i != depot_new_index - 1) {
			x.push_back(i);
		}
	}
	population_.clear();
	srand(seed);
	for (int i = 0; i < pop_size_; i ++) {
		random_shuffle(x.begin(), x.end(), myrandom);
		population_.push_back(x);
	}
	srand((unsigned)time(NULL));
}

void VRP::ChooseDepot(int index) {
	depot_new_index = index;
	index --;
	depot_new_[0] = customers_[index][0];
	depot_new_[1] = customers_[index][1];
	customers_num_ = customers_.size() - 1;
}

double VRP::Evolve() {
	vector<vector<int> > offspring(pop_size_);
	vector<double> p_select;
	int id = Select(population_, p_select);
	offspring[0] = population_[id];
	offspring[1] = population_[id];
	double x;
	vector<int> a_copy, b_copy;
	for (int i = 2; i < pop_size_ ; i += 2) {
		// Choose parent object A;
		x = rand() / (RAND_MAX * 1.0);
		id = 0;
		while (x > p_select[id]) id ++;
		a_copy = population_[id];
		// Choose parent object B;
		x = rand() / (RAND_MAX * 1.0);
		id = 0;
		while (x > p_select[id]) id ++;
		b_copy = population_[id];
		// Crossover:
		Crossover(a_copy, b_copy);
		// Mutate:
		Mutate(a_copy);
		Mutate(b_copy);
		// Add to offspring population;
		offspring[i] = a_copy;
		offspring[i+1] = b_copy;
	}
	// replace population with offspring;
	//cout << offspring.size() << endl;
	population_ = offspring;
	return GetPath(offspring[0]);
}

int VRP::FindBest(double *distance) {
	vector<cost> costs(pop_size_);
	for (int i = 0; i < pop_size_; i ++) {
		costs[i].id = i;
		costs[i].val = GetPath(population_[i]);
	}
	sort(costs.begin(), costs.end(), comp);
	*distance = costs.back().val;
	return costs.back().id;
}

void VRP::PrintRoute(vector<int> &route, int car_num, bool depot_flag, bool output_flag) {
	cout << ">> Route " << car_num <<	": ";
	if (output_flag) {
		ofstream ofile("optimal_strategy.csv", ios::app);
		if (depot_flag) {
			cout << "0";
			ofile << "0" << '\t';
		} else {
			cout << depot_new_index;
			ofile << depot_new_index << '\t';
		}
		cout << " -> ";
		for (int i = 0; i < route.size(); i++ ) {
			cout << route[i] + 1 << " -> ";
			ofile << route[i] + 1 << '\t';
		}
		if (depot_flag) {
			cout << "0" << '\n';
			ofile << "0" << '\n';
		} else {
			cout << depot_new_index << '\n';
			ofile << depot_new_index << '\n';
		}
		ofile.close();
	} else {
		if (depot_flag) cout << "0";
		else cout << depot_new_index;
		cout << " -> ";
		for (int i = 0; i < route.size(); i++ ) {
			cout << route[i] + 1 << " -> ";
		}
		if (depot_flag) cout << "0" << endl;
		else cout << depot_new_index << endl;
	}
}

void VRP::Decode(vector<int> &g, bool output_flag) {
	double path_tot = 0;
	int car_num = 1;
	int load = 0;
	bool depot_flag;
	vector<int> route;
	if (output_flag) {
		ofstream ofile("optimal_strategy.csv");
		ofile.close();
	}
	cout << "Optimized strategy goes as follows: " << endl;
	for (int i = 0; i < customers_num_; i ++ ) {
		if (load == 0) {
			load += customers_[g[i]][2];
			route.push_back(g[i]);
		} else {
			load += customers_[g[i]][2];
			if (load <= capacity_) {
				route.push_back(g[i]);
			} else if (load > capacity_) {
				depot_flag = GetRoutePath(route, &path_tot);
				PrintRoute(route, car_num, depot_flag, output_flag);
				car_num ++;
				route.clear();
				load = customers_[g[i]][2];
				route.push_back(g[i]);
			}
		}
		if (i == customers_num_ - 1) {
			depot_flag = GetRoutePath(route, &path_tot);
			PrintRoute(route, car_num, depot_flag, output_flag);
		}
	}
	cout << "In total, this strategy requires " << car_num << " cars, and the total traval distance is " << path_tot << "." << endl;
}

void VRP::OutRoute(int id, vector<int> & route) {
	route = population_[id];
}
