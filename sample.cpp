#include <vector>
#include <iostream>
#include "util.hpp"

using namespace std;

double treegkr(Tree T, vector<double> creation, vector<double> destruction, vector<double> x, vector<double> y);

int main(int argc, char **argv){

  int n;
  cin >> n;

  Tree T(n);
  for(int i = 0; i < n-1; i++){
    int a, b;
    double l;
    cin >> a >> b >> l;
    a--;
    b--;
    T.add_edge(a, b, l);
  }

  vector<double> creation(n);
  vector<double> destruction(n);
  for(int i = 0; i < n; i++){
    cin >> creation[i] >> destruction[i];
  }

  vector<double> x(n);
  vector<double> y(n);
  for(int i = 0; i < n; i++){
    cin >> x[i] >> y[i];
  }
    
  double result = treegkr(T, creation, destruction, x, y);

  cout << result << endl;
  
  return 0;
}
