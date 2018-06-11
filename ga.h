#ifndef _GA_
#define _GA_
#include <vector>
using namespace std;

class VRP {
	private:
		vector<vector<double> > customers_;
		int customers_num_; // number of customers;
		double depot_[2] = {0, 50}; // location of original depot;
		double depot_new_[2]; // new depot chosen from one of the customers;
		int depot_new_index; // index of new depot, ranging from 1 to 50;
		int capacity_ = 60; // maximum capacity of vehicles;
		double pc_;	// probability of crossover;
		int C_;			// Number of genes in crossover;
		double pm_;	// probability of mutation;
		int M_;			// number of genes swapped in mutation;
		// genetic populations:
		vector<vector<int> > population_;
		int pop_size_; // size of population;

		double lower_bound_; // lower bound of the total distance;

		// Functions:
		bool GetRoutePath(vector<int> & route, double* path);
		// Calculate the total path of vehicles of specific chromosome;
		// g: chromosome;
		// return the total path length;
		double GetPath(vector<int> &g);

		// Calculate cost function of specific chromosome;
		// f(g) = 1 / (total_path_length - lower_bound_)
		// return the value of cost function;
		double CostFunc(vector<int> &g);

		// Fitness proportionate selection, also known as roulette wheel selection; calculate the accumulative selcting probability of each individual chromosome;
		// population: population of chromosome;
		// p_cum: accumulative selecting probability distribution;
		// return the index of object with largest fitness in this population;
		int Select(vector<vector<int> > &population, vector<double> &p_cum);

		// Order-based two-point crossover operator (OX2);
		// The starting crossover point is randomly picked from parent chromosomes with ending crossover point follows C_ points behind;
		// Crossover operation happens with pc_ probability;
		// gA: target parent chromosome A;
		// gB: target parent chromosome B;
		// Both gA and gB are randomly chosen from parent population based on selecting probability;
		void Crossover(vector<int> &gA, vector<int> &gB);

		// Mutation operator;
		// Swap M_ gene pairs randomly;
		// Mutate operation happens with pm_ probability;
		// g: target chromosome;
		void Mutate(vector<int> &g);

		void PrintRoute(vector<int> &route, int car_num, bool depot_flag, bool output_flag);
	public:
		// VRP constructor;
		// Construct with list of customers;
		VRP(vector<vector<double> > & customers) {
			customers_ = customers;
			customers_num_ = customers.size();
			pop_size_ = 10000;
			pc_ = 0.9;
			C_  = 10;
			pm_ = 0.1;
			M_  = 10;
			lower_bound_ = 0;
		}
		// Set parameters:
		void SetCapacity(double val) {capacity_ = val;}
		void SetPopSize(int val) {pop_size_ = val;}
		void SetPc(double val) {pc_ = val;}
		void SetC(int val) {C_ = val;}
		void SetPm(double val) {pm_ = val;}
		void SetM(int val) {M_ = val;}

		void SetLowerBound(double val) {lower_bound_ = val;}

		// Select second depot:
		// choose one existing customer as new depot, index ranging from 1 to 50;
		void ChooseDepot(int index);

		// Initialization;
		void Initialization(int seed);

		// Generate offspring population based on parent population, replace population with new offspring population;
		// return the total distance in the parent population, which is used for analysis.
		double Evolve();

		// Search for the best object in current population;
		int FindBest(double *distance);

		// Decode the chromosome of specific object, print out the vehicle routing strategy;
		// g: target chromosome;
		// output_flag: true for output result to external file, false for not;
		void Decode(vector<int> &g, bool output_flag);

		// OutRoute:
		// Output the target Route;
		void OutRoute(int id, vector<int> & route);
};
#endif
