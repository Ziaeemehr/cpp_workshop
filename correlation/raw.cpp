#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

using namespace std;

/**
*	compares two vector lengths
*/
template<class T>
void comp(vector<T> v1, vector<T> v2){
	if(v1.size() != v2.size()){
		cout << "vectors not the same size\n";
		exit(1);
	}
}

/**
*	Euclidean distance between two vectors of type T such that T has binary +,-,* 
*/
template<class T>
double euclidean(vector<T> v1, vector<T> v2){
	comp(v1, v2);
	T diff, sum;

	diff = v1[0] - v2[0];
	sum = diff * diff;

	for (unsigned int i=1; i < v1.size(); i++){
		diff = v1[i] - v2[i];
		sum += diff * diff;
	}
	return sqrt(double(sum));
}

/**
*	Jaccard Coefficient.	Use for asymetric binary values
*/
template<class T>
double jaccard(vector<T> v1, vector<T> v2){
	comp(v1, v2);
	int f11 = 0, f00 = 0;

	for (unsigned int i=0; i < v1.size(); i++){
		if(v1[i] == v2[i]){
			if(v1[i])
				f11++;
			else
				f00++;
		}
	}
	return double(f11) / double(v1.size() - (f11+f00));
}

/**
*	Cosine
*/
template<class T>
double cosine(vector<T> v1, vector<T> v2){
	comp(v1, v2);

	T lv1 = v1[0] * v1[0];
	T lv2 = v2[0] * v2[0];
	T dot12 = v1[0] * v2[0];

	for (unsigned int i=0; i < v1.size(); i++){
		lv1 += v1[i] * v1[i];
		lv2 += v2[i] * v2[i];
		dot12 += v1[i] * v2[i];
	}
	return double(dot12) / ( sqrt(double(lv1)) * sqrt(double(lv2)) );
}

/**
*	The mean of a vector
*/
template<class T>
double mean(vector<T> v1){
	T sum = v1[0];
	for (unsigned int i=1; i < v1.size(); i++)
		sum += v1[i];
	return double(sum) / double(v1.size());
}

/**
*	The Covariance
*/
template<class T>
double covariance(vector<T> v1, vector<T> v2){
	comp(v1, v2);
	double mean1 = mean(v1), mean2 = mean(v2);
	double sum = (double(v1[0]) - mean1) * (double(v2[0]) - mean2);
	for (unsigned int i=1; i < v1.size(); i++){
		sum += (double(v1[i]) - mean1) * (double(v2[i]) - mean2);
	}
	return double(sum) / double(v1.size()-1);
}

/**
*	standard deviation the covariance where both vectors are the same.
*/
template<class T>
double std_dev(vector<T> v1){
	return sqrt(covariance(v1, v1));
}

/**
*	Pearson Correlation
*/
template<class T>
double pearson(vector<T> v1, vector<T> v2){
	if (std_dev(v1) * std_dev(v2) == 0){
		cout << "( a standard deviaton was 0 )";
		return -2; // I dont know what to do here???
	}
	return covariance(v1,v2) / ( std_dev(v1) * std_dev(v2));
}

int main(){
// 	vector<int> aX(4); aX[0] = 1; aX[1] = 1; aX[2] = 1; aX[3] = 1;
// 	vector<int> aY(4); aY[0] = 2; aY[1] = 2; aY[2] = 2; aY[3] = 2; // cosine, correlation, Euclidian
// 	vector<int> bX(4); bX[0] = 0; bX[1] = 1; bX[2] = 0; bX[3] = 1;
// 	vector<int> bY(4); bY[0] = 1; bY[1] = 0; bY[2] = 1; bY[3] = 0; // cosine, correlation, Euclidian, Jacard
// 	vector<int> cX(4); cX[0] = 0; cX[1] = -1; cX[3] = 0; cX[3] = 1;
// 	vector<int> cY(4); cY[0] = 1; cY[1] = 0; cY[2] = -1; cY[3] = 0; // cosine, correlation, Euclidian
// 	vector<int> dX(6); dX[0] = 1; dX[1] = 1; dX[2] = 0; dX[3] = 1; dX[4] = 0; dX[5] = 1;
// 	vector<int> dY(6); dY[0] = 1; dY[1] = 1; dY[2] = 1; dY[3] = 0; dY[4] = 0; dY[5] = 1; // cosine, correlation, Jaccard
// 	vector<int> eX(6); eX[0] = 2; eX[1] = -1; eX[2] = 0; eX[3] = 2; eX[4] = 0; eX[5] = -3;
// 	vector<int> eY(6); eY[0] = -1; eY[1] = 1; eY[2] = -1; eY[3] = 0; eY[4] = 0; eY[5] = -1; // cosine, correlation

	vector<double> p1 {525., 300., 450., 300., -400., 500., 550., 125., 300., 400., 500., 550.};
        vector<double> p2 {250., 225., 275., 350., -325., 375., 450., 400., 500., 550., 600., 525.};
	
        cout << "Correlation Pearson : " << pearson(p1,p2) << endl;
	
// 	cout << "a. cosine: " << cosine(aX, aY) << " correlation: " << pearson(aX, aY) << " euclidean: " << euclidean(aX, aY) << endl;
// 	cout << "b. cosine: " << cosine(bX, bY) << " correlation: " << pearson(bX, bY) << " euclidean: " << euclidean(bX, bY) << endl;
// 	cout << "c. cosine: " << cosine(cX, cY) << " correlation: " << pearson(cX, cY) << " euclidean: " << euclidean(cX, cY) << " jaccard: " << jaccard(cX, cY) << endl;
// 	cout << "d. cosine: " << cosine(dX, dY) << " correlation: " << pearson(dX, dY) << " jaccard: " << jaccard(dX, dY) << endl;
// 	cout << "e. cosine: " << cosine(eX, eY) << " correlation: " << pearson(eX, eY) << endl;
// 	system("pause");
}