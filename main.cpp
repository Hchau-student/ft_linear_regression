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
public:
	vector<float> dataset[2];
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

			if (!file.eof()) {
				dataset[0].push_back(tmp.first);
				dataset[1].push_back(tmp.second);
			}
		}
		file.close();
	}

public:
	void print_dataset() {
		for (auto & elem : dataset[0]) {
			cout << elem << "," << "\n";
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
	float cur_mse = 0;
	float prev_mse = 0;
	float learning_rate = 0.001;//TODO simple realization
	vector<float> xs;
	vector<float> ys;

public:
	float estimatePrice(float mileage) {
		return (theta.first + theta.second * mileage);
	}

private:
	void mse() {
		float mse_sum = 0;
		float err;
		for (int i = 0; i < xs.size() && ys.size(); i++) {
			err = ys[i] - estimatePrice(xs[i]);
			mse_sum += err * err;
		}
		prev_mse = cur_mse;
		cur_mse = mse_sum / (float)(xs.size());
	}

private: void calc_theta_1() {
		float sum = 0;
		for (int i = 0; i < xs.size(); i++)
			sum += (estimatePrice(xs[i]) - ys[i]) * xs[i];
		tmp_theta.second = theta.second;
		theta.second = sum / (float)xs.size();
	}

private: void calc_theta_0() {
		float sum = 0;
		for (int i = 0; i < xs.size(); i++)
			sum += estimatePrice(xs[i]) - ys[i];
		tmp_theta.first = theta.first;
		theta.first = learning_rate * sum / (float)xs.size();
	}

private: void calc_thetas() {
		calc_theta_0();
		calc_theta_1();
	}

private: float delta() {
		return (abs(cur_mse - prev_mse));
	}

public:
	Calc(Data data) {
		xs = data.dataset[0];
		ys = data.dataset[1];
		theta.first = theta.second = 0;
		tmp_theta.first = tmp_theta.second = 0;
		do {
			mse();
			calc_thetas();
//			cout << delta();
		} while (delta() >= 0.000001);
		ofstream result("res.txt", ifstream::out);
		result << "theta0: " << theta.first << endl;
		result << "theta1: " << theta.second << endl;
	}
};

int main(int argc, char *argv[]) {
//	Data data = Data(argc, argv);
//	data.print_dataset();
	Calc(Data(argc, argv));
	return 0;
}
