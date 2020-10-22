#include <vector>
#include <iostream>
#include <boost/python.hpp>
#include "util.hpp"

using namespace std;

double treegkr(Tree T, vector<double> creation, vector<double> destruction, vector<double> x, vector<double> y);

double treegkr_python_wrapper(boost::python::list vs,
                          boost::python::list us,
                          boost::python::list cost,
                          boost::python::list creation,
                          boost::python::list destruction,
                          boost::python::list x,
                          boost::python::list y){
  int n = boost::python::len(x);
  assert(boost::python::len(vs) == n - 1);
  assert(boost::python::len(us) == n - 1);
  assert(boost::python::len(cost) == n - 1);
  assert(boost::python::len(y) == n);
  assert(boost::python::len(creation) == n);
  assert(boost::python::len(destruction) == n);
  Tree T(n);
  Unionfind uf(n);
  for(int i = 0; i < n - 1; i++){
    int v = boost::python::extract<int>(vs[i]);
    int u = boost::python::extract<int>(us[i]);
    double c = boost::python::extract<double>(cost[i]);
    T.add_edge(v, u, c);
    if(uf.same(u, v)){
      cerr << "Input is not a tree" << endl;
      exit(1);
    }
    uf.unite(u, v);
  }
  if(uf.size[uf.find(0)] != n){
    cerr << "Input is not a tree" << endl;
    exit(1);
  }
  vector<double> xv(n), yv(n), creationv(n), destructionv(n);
  for(int i = 0; i < n; i++){
    xv[i] = boost::python::extract<double>(x[i]);
    yv[i] = boost::python::extract<double>(y[i]);
    creationv[i] = boost::python::extract<double>(creation[i]);
    destructionv[i] = boost::python::extract<double>(destruction[i]);
  }
  return treegkr(T, creationv, destructionv, xv, yv);
}

BOOST_PYTHON_MODULE(treegkr){
  boost::python::def("treegkr",
                     treegkr_python_wrapper,
                     boost::python::args("vs",
                                         "us",
                                         "cost",
                                         "creation",
                                         "destruction",
                                         "x",
                                         "y"),
                     "Fast computation of the generalized Kantorovich Rubinstein (GKR) distance on tree metrics\n"
                     "\n"
                     "Parameters\n"
                     "----------\n"
                     "vs : list of ints\n"
                     "    A list of endpoints of edges\n"
                     "    The node indices should start from zero\n"
                     "    The length of the list should be n-1\n"
                     "us : list of ints\n"
                     "    A list of endpoints of edges\n"
                     "    The node indices should start from zero\n"
                     "    The length of the list should be n-1\n"
                     "cost : list of floats\n"
                     "    A list of costs of edges\n"
                     "    The i-th edge connects node vs[i] and us[i] with cost[i].\n"
                     "    The length of the list should be n-1\n"
                     "creation : list of floats\n"
                     "    A list of creation costs\n"
                     "    A mass on node i can be created by cost creation[i].\n"
                     "    The length of the list should be n\n"
                     "destruction : list of floats\n"
                     "    A list of destruction costs\n"
                     "    A mass on node i can be destructed by cost destruction[i].\n"
                     "    The length of the list should be n\n"
                     "x : list of floats\n"
                     "    A list of source mass\n"
                     "    There exist x[i] target mass on node i.\n"
                     "    The length of the list should be n\n"
                     "y : list of floats\n"
                     "    A list of target mass\n"
                     "    There exist y[i] target mass on node i.\n"
                     "    The length of the list should be n\n"
                     "\n"
                     "Returns\n"
                     "-------\n"
                     "gkr: float\n"
                     "    The GKR distance\n");
}

