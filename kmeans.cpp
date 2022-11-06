#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>

#include "referance/matplotplusplus/source/matplot/matplot.h"

using namespace std;

struct Point{
	double x;
	double y;
	Point(double x, double y) : x(x), y(y) {};
};

struct Iris{
	double sepal_length;
	double sepal_width;
	double petal_length;
	double petal_width;
	int label;
	Iris(double sepal_length, double sepal_width, double petal_length, double petal_width, int label = -1) :
		sepal_length(sepal_length), 
		sepal_width(sepal_width), 
		petal_length(petal_length), 
		petal_width(petal_width), 
		label(label)
		{};
};

class Kmeans{
private:
	string fileName;
	int clusterK;
	static const char DELIMITER = ',';
	vector<vector<double> > dataLines;
	vector<Iris> Iris_vect;
public:
	Kmeans(string fileName, int k) : fileName(fileName), clusterK(k){};
	void buildDataset();
	void dataLines2Iris();
	void cluster();
	void paint();
};

void Kmeans::buildDataset(){
	ifstream in(fileName);
	string line;
	getline(in, line);//把第一行标题去掉
	getline(in, line);
	while(!line.empty()){
		size_t i = 0;
		size_t j = 0;
		vector<double> tmp;
		while(i < line.length()){
			j = line.find(DELIMITER, i);
			if( j == string::npos){
				break;
			}else{
				tmp.push_back(stod(line.substr(i, j-i)));
				i = j + 1;
			}
		}
		dataLines.push_back(tmp);
		getline(in, line);
	}
	in.close();
}

void Kmeans::dataLines2Iris(){
	for(auto it = dataLines.begin(); it < dataLines.end(); it++){
		auto dataline = *it;
		Iris item(dataline[0], dataline[1], dataline[2], dataline[3]);
		Iris_vect.push_back(item);
	}
}

double getDistance(Point p1, Point p2){
	return sqrt((pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)));
}

void Kmeans::cluster(){
	vector<Point> centers;//选取中心点
	for(int i = 0; i < clusterK; i++){
		Point p(Iris_vect[i].sepal_length, Iris_vect[i].sepal_width);
		centers.push_back(p);
	}
	int times = 0;
	for(int times = 0; times <100; times++){//迭代100次
		for(auto it = Iris_vect.begin(); it <= Iris_vect.end(); it++){//遍历每一个点
			Point p((*it).sepal_length, (*it).sepal_width);
			double distance = numeric_limits<double>::max();//distance float最大值
			for(int j = 0; j < clusterK; j++){
				double temp_distance = getDistance(p, centers[j]);
				if(temp_distance < distance){
					(*it).label = j;//更新label
					distance = temp_distance;
				}
			}
		}
		//修正中心点
		for(int c = 0; c < clusterK; c++){
			double count = 0.0;
			double sum_x = 0.0;
			double sum_y = 0.0;
			for(auto it = Iris_vect.begin(); it <= Iris_vect.end(); it++){
				if((*it).label == c){
					count ++;
					sum_x += (*it).sepal_length;
					sum_y += (*it).sepal_width;
				}
			}
			centers[c].x = sum_x / count;
			centers[c].y = sum_y / count; 
		}
	}

	for(auto it = centers.begin(); it <= centers.end(); it++){
		cout << (*it).x << "  " << (*it).y << endl; 
	}
}

void Kmeans::paint(){
	using namespace matplot;
	vector<double> x, y;
	for(auto it = Iris_vect.begin(); it < Iris_vect.end(); it++){
		x.push_back((*it).sepal_length);
		std::cout << (*it).sepal_length << std::endl;
		y.push_back((*it).sepal_width);
	}
	scatter(x, y);
	show();
}

int main(){
	int cluster = 2;
	Kmeans k("iris.data", cluster);
	k.buildDataset();
	k.dataLines2Iris();
	k.cluster();
	k.paint();
	return 0;
}
