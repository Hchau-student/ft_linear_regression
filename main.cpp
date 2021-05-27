#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

void	err(string msg) {
	cout << msg << "\n";
	exit(0);
}

class Data {
	vector<pair<int, int>> dataset;
private:
	ifstream file_check(int argc, char *argv[]) {//TODO mb no need for variable ifstream dataset?
		if (argc == 1)//TODO: Exceptions?
			err("Too few arguments. Expected 1");
		if (argc > 2)
			err("Too many arguments. Expected 1");
		ifstream dataset(argv[1], ifstream::in);
		return (dataset);
	}

private:
	void read_file(ifstream file) {
		string line;
		getline(file, line);

		while (!file.eof()) {
			pair<int, int> tmp;
			char skip;

			file >> tmp.first >> skip >> tmp.second;
			getline(file, line);
			if (line.length() > 0)
				err("Parser error");//TODO exception

			if (!file.eof())
				dataset.push_back(tmp);
		}
		file.close();
	}

public:
	void print_dataset() {
		for (auto & elem : dataset) {
			cout << elem.first << "," << elem.second << "\n";
		}
	}
public:
	Data(int argc, char *argv[]) {
		try {
			read_file(file_check(argc, argv));
		} catch (exception e) {
			cout << e.what() << endl;
			exit(-1);
		}
	}
};

class Calc {
	pair<float, float> theta;
	pair<float, float> tmp_theta;
	float learning_rate = 1.0;
	
public:
	float estimatePrice(float mileage) {
		return (theta.first + theta.second * mileage);
	}

public:
	Calc() {
		theta.first = theta.second = 0;
		tmp_theta.first = tmp_theta.second = 0;
		ofstream result("res.txt", ifstream::out);
		result << "theta0: " << theta.first << endl;
		result << "theta1: " << theta.second << endl;
	}
};

int main(int argc, char *argv[]) {
	Data data = Data(argc, argv);
	data.print_dataset();
	Calc();
	return 0;
}
