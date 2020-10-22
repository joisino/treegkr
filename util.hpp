#include <vector>
#include <tuple>

using namespace std;

struct Tree{
  vector<vector<pair<int, double>>> G;
  int n;
  Tree(int size){
    n = size;
    G.assign(n, vector<pair<int, double>>(0));
  }
  void add_edge(int a, int b, double c){
    G[a].emplace_back(b, c);
    G[b].emplace_back(a, c);
  }
  vector<tuple<int, int, double>> edges(){
    vector<tuple<int, int, double>> es;
    for(int i = 0; i < n; i++){
      for(pair<int, int> j: G[i]){
        if(i < j.first){
          es.emplace_back(i, j.first, j.second);
        }
      }
    }
    return es;
  }
};


struct Unionfind{
  vector<int> size, par;
  Unionfind(){}
  Unionfind(int n) :  size(n, 1), par(n){
    for(int i = 0; i < n; i++){
      par[i] = i;
    }
  }
  int find(int x){
    if(par[x] == x){
      return x;
    }
    return par[x] = find(par[x]);
  }
  bool unite(int x, int y){
    x = find(x);
    y = find(y);
    if(x == y){
      return false;
    }
    if(size[y] < size[x]){
      swap( x , y );
    }
    par[x] = y;
    size[y] += size[x];
    return true;
  }
  bool same(int x, int y){
    return find(x) == find(y);
  }
};
