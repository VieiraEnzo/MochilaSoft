#include "ProblemInstance.h"

void ProblemInstance::load_file(const char *const f)
{

	ifstream file(f);
	if (!file.is_open())
	{
		cout << "ERROR: File " << f << " not found!" << endl;
		exit(0);
	}

	NbItens = 0;
	file >> NbItens;

	Value = new int [NbItens];
	Weight = new int[NbItens];

	for (int i = 0; i < NbItens; ++i)
	{
		file >> Value[i];
	}

	knapsackCapacity = 0;
	file >> knapsackCapacity;

	for (int i = 0; i < NbItens; ++i)
	{
		file >> Weight[i];
	}


	file.close();
}

void ProblemInstance::PrintInstance()
{
	cout << "Instance starts here"<<endl;
	cout << NbItens << endl;
	for (int i = 0; i < NbItens; ++i)
	{
		cout << Value[i] << " ";
	}
	cout << endl;
	cout << knapsackCapacity << endl;
	for (int i = 0; i < NbItens; ++i)
	{
		cout << Weight[i] << " ";
	}
	cout << endl;

	cout<<"Instance ends here"<<endl;
}

ProblemInstance::~ProblemInstance()
{
	cout << "Deleting instance...";
	delete[] Value;
	delete[] Weight;
	cout << "Instance deleted. \n";
}