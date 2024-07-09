#ifndef SOLUTION_H
#define SOLUTION_H

#include "ProblemInstance.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>    // std::sort
#include <deque>
#include <random>
#include <chrono>

using namespace std;
//n
//0, n-1 -> gerar um numero aleatorio entre 1, 1e12 pra cada indice
class Solution
{
public: //TROCAR PARA PRIVADO ?

	bool CostCalculated;
	int num_items_in_sol;
	double cost;
	double used_capacity;
	unsigned long hash;
	ProblemInstance* p;
	vector<bool> inside;
	vector<int> freq;
	deque<int> dq;
	vector<unsigned long long> ItemHash;

	friend ostream& operator<<(ostream&, Solution&);

public:

	friend bool operator< (Solution &a, Solution &b);
	friend bool operator> (Solution &a, Solution &b);
	friend bool operator== (Solution &a, Solution &b);

	Solution(ProblemInstance* _p);

	~Solution();

	int get_size();
	double getCost();

	void add_item(int i);
	void add_itemO(int i);
	void remove_item(int i);
	void remove_itemO(int i);
	int remove_oldest_choice();  // remove the first item added to the solution
	int remove_newest_choice();
	bool is_in_sack(int i); // check if an item is in solution
	bool can_add(int i);
	vector<bool> convertToBinaryVector(); 
	bool CheckSol();
	int CountForfeit();


	Solution& operator= (const Solution& other){
		CostCalculated = other.CostCalculated;
		num_items_in_sol = other.num_items_in_sol;
		cost = other.cost;
		used_capacity = other.used_capacity;
		hash = other.hash;
		p = other.p;
		inside = other.inside;
		freq = other.freq;
		dq = other.dq;
		ItemHash = other.ItemHash;
	}	 
};

#endif // SOLUTION_H