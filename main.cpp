#include "ga.h"
#include "get-config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
//#include <windows.h>
using namespace std;

struct comp_t {
	int id;
	double val;
};

bool comp(const comp_t &x, const comp_t &y) {return x.val < y.val;}

//	Read 2-D data from *.csv files; data type can be int or double;
//	path: path of target file;
//	data: container of data;
//	return: none;
void Read2D(string path, vector<vector<double> >& data) {
  data.clear();
  ifstream ifile(path.c_str());
  string s, buffer;
  vector<double> add_T;
  istringstream s_input;
  while (getline(ifile, s)) {
    add_T.clear();
    s_input.clear();
    s_input.str("");
    s_input.str(s);
    while (getline(s_input, buffer, ',')) {
      if (buffer.find('.') == string::npos) {
        add_T.push_back(atoi(buffer.c_str()));
      } else add_T.push_back(atof(buffer.c_str()));
    }
    data.push_back(add_T);
  }
  ifile.close();
}

int main() {
	clock_t start, finish;
	start = clock();
	// Load config.ini:
	string net_config_path = "config.ini";
	map<string, string> m_map_config;
	ReadConfig(net_config_path,m_map_config);
	cout << ">> [Config.ini]:\n#####\n";
	PrintConfig(m_map_config);
	cout << "#####\n";
	// Load customer list:
	vector<vector<double> > customers;
	Read2D("vrp_dat.csv", customers);
	VRP vrp_prob(customers);
	// Load paramters;
	vrp_prob.SetCapacity(atof(m_map_config["MaxCapacity"].c_str()));
	vrp_prob.SetPopSize(atoi(m_map_config["PopSize"].c_str()));
	vrp_prob.SetPc(atof(m_map_config["CrossoverProbability"].c_str()));
	vrp_prob.SetC(atoi(m_map_config["CrossoverLength"].c_str()));
	vrp_prob.SetPm(atof(m_map_config["MutateProbability"].c_str()));
	vrp_prob.SetM(atoi(m_map_config["MutateNumber"].c_str()));
	// setup filter;
	double distance_bound = atof(m_map_config["PreScanInitialBound"].c_str());
	double step_size = atof(m_map_config["PreScanBoundStep"].c_str());
	int step_num = atoi(m_map_config["PreScanNumStep"].c_str());
	// create temp variables;
	double dis_val;
	int sol_id;
	// Start pre-scan;
	int pre_scan_gen_num = atoi(m_map_config["PreScanGenerationNumber"].c_str());
	int gen_num_in_step = pre_scan_gen_num / step_num;
	vector<comp_t> pre_scan_result(50);
	vector<int> tmp_route;
	ofstream ofile("pre_scan_least_path.csv");
	for (int i = 1; i < 51; i ++) {
		//vrp_prob.ChooseDepot(atoi(m_map_config["NewDepot"].c_str()));
		vrp_prob.ChooseDepot(i);
		vrp_prob.Initialization(atoi(m_map_config["Seed"].c_str()));
		for (int j = 0; j < step_num; j ++) {
			vrp_prob.SetLowerBound(distance_bound);
			for (int k = 0; k < gen_num_in_step; k ++) {
				dis_val = vrp_prob.Evolve();
				ofile << dis_val << ',';
			}
			distance_bound -= step_size;
		}
		ofile << endl;
		distance_bound = atof(m_map_config["PreScanInitialBound"].c_str());
		sol_id = vrp_prob.FindBest(&dis_val);
		pre_scan_result[i-1].id = i;
		pre_scan_result[i-1].val = dis_val;
		// Print out the results;
		vrp_prob.OutRoute(sol_id, tmp_route);
		vrp_prob.Decode(tmp_route, false);
	}
	ofile.close();
	// sort pre-scan result; select several candidates for deep search;
	sort(pre_scan_result.begin(), pre_scan_result.end(), comp);
	int ds_num = atoi(m_map_config["DeepSearchNumber"].c_str());
	vector<int> ds_list;
	for (int i = 0; i < ds_num; i ++) {
		ds_list.push_back(pre_scan_result[i].id);
	}
	// Load filter parameter for deep search;
	distance_bound = atof(m_map_config["DeepSearchInitialBound"].c_str());
	step_size = atof(m_map_config["DeepSearchBoundStep"].c_str());
	step_num = atoi(m_map_config["DeepSearchNumStep"].c_str());
	// Start deep search;
	int deep_search_gen_num = atoi(m_map_config["DeepSearchGenerationNumber"].c_str());
	gen_num_in_step = deep_search_gen_num / step_num;
	int optimal_depot;
	int optimal_id;
	double optimal_distance = 100000;
	vector<int> optimal_route;
	ofile.open("deep_search_least_path.csv");
	for (int i = 0; i < ds_num; i ++) {
		vrp_prob.ChooseDepot(ds_list[i]);
		vrp_prob.Initialization(atoi(m_map_config["Seed"].c_str()));
		for (int j = 0; j < step_num; j ++) {
			vrp_prob.SetLowerBound(distance_bound);
			for (int k = 0; k < gen_num_in_step; k ++) {
				dis_val = vrp_prob.Evolve();
				ofile << dis_val << ',';
			}
			distance_bound -= step_size;
		}
		ofile << endl;
		distance_bound = atof(m_map_config["DeepSearchInitialBound"].c_str());
		sol_id = vrp_prob.FindBest(&dis_val);
		if (dis_val < optimal_distance) {
			optimal_distance = dis_val;
			optimal_depot = ds_list[i];
			optimal_id = i;
			vrp_prob.OutRoute(sol_id, optimal_route);
		}
		// Print out the results;
		//vrp_prob.Decode(sol_id, false);
	}
	ofile.close();

	vector<vector<double> > tmp;
	Read2D("deep_search_least_path.csv", tmp);
	ofile.open("optimal_least_path.csv");
	for (vector<double>::iterator it = tmp[optimal_id].begin(); it != tmp[optimal_id].end(); it ++) {
		ofile << *it << '\n';
	}
	ofile.close();
	// Print out the optimal strategy;
	vrp_prob.ChooseDepot(optimal_depot);
	bool output_flag;
	istringstream(m_map_config["OutputFlag"]) >> boolalpha >> output_flag;
	vrp_prob.Decode(optimal_route, output_flag);

	finish = clock();
	cout << ">> It takes " << (finish - start)* 1.0/ CLOCKS_PER_SEC << " s" << endl;
	system("pause");
	return 0;
}
