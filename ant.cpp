#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <cmath>
#include <random>

using namespace std;

class plane{
public:
  plane() {};
  // virtual vector<int> getAdjVal(int vertex) = 0;
  virtual int getNextVertex(int vertex, set<int>& missingVert) = 0;
  virtual vector<int> getAdjPheromons(int vertex) = 0;
  virtual bool addAdjPheromon(int vertex) = 0;
  virtual void updatePheromons() = 0;
};
 

class my_plane : plane{
  // vector<tuple<int, int, int>> edge;
  map<int, vector<tuple<int, int>>> edges; //from , <to , value>
  map<tuple<int, int>, int> edgesValues;
  std::default_random_engine re;
  //double pheromones[];
  // vector<int> vertex;

  vector<int> getAdjVal(int vertex){
    auto k = vector<int>();
    k.emplace_back(vertex);
    return k;
  }

public:
  my_plane(){
    edges = map<int, vector<tuple<int, int>>>();
  };

  int getValue(int from, int to){
    if(from > to){
      to, from = from, to;
    }
    return edgesValues[make_tuple(from, to)];
  }

  void updatePheromons(vector<int>& path){
    cout << "updating pheromones \n";
  }

  void insEdge2(int from , int to, int value){
    if (from != to){
      edges[from].emplace_back(make_tuple(to, value));
      edges[to].emplace_back(make_tuple(from, value));
      if(from > to) from, to = to, from;
      edgesValues[make_tuple(from, to)] = value;
      // pheromones[from][to] = 1;
    }
  };

  void WA(){
    for( auto const& [key, val] : edges ){
      std::cout << key << ":  ";
      for( auto const& [a, b] : val ) {
        cout << "to: " << a << ", val :" << b << " | ";
      }     
        cout << std::endl ;
    }
  };


  int getNextVertex(int vertex, set<int>& missingVert) override {
    int nPosib = edges[vertex].size();
    int trgVert[nPosib];
    double BETA = 1;
    double ALPHA = 1;
    double prob [nPosib]; 
    double sigma = 0;
    int i = 0;
    for (auto const& [to, value] : edges[vertex]){
      if (missingVert.find(to) == missingVert.end()){
        nPosib--;
        continue;
      }
      int a, b;
      a, b = vertex, to;
      if (vertex > to) a, b = to, vertex;
      double ETAij = (double) pow ((double)1 / value, BETA);
	    // double TAUij = (double) pow (pheromones[make_tuple(a,b)],   ALPHA);
	    // double TAUij = (double) pow (pheromones[a][b],   ALPHA);
	    double TAUij = 1;
      // cout << ETAij << "><" << TAUij << "!" << value << " |" ;
      prob[i] = ETAij * TAUij;
      trgVert[i] = to;
      sigma += prob[i];
      i++;
    }

    if(sigma == 0){
      cout << "n";
      i = 0;
      nPosib = edges[vertex].size();
      for (auto const& [to, value] : edges[vertex]){
        int a, b;
        a, b = vertex, to;
        if (vertex > to) a, b = to, vertex;
        double ETAij = (double) pow ((double)1 / value, BETA);
        // double TAUij = (double) pow (pheromones[make_tuple(a,b)],   ALPHA);
        // double TAUij = (double) pow (pheromones[a][b],   ALPHA);
        double TAUij = 1;
        // cout << ETAij << "><" << TAUij << "!" << value << " |" ;
        prob[i] = ETAij * TAUij;
        trgVert[i] = to;
        sigma += prob[i];
        i++;
      }
    }

    double lower_bound = 0;
    double upper_bound = sigma;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);

    double a_random_double = unif(re);
    cout << sigma << " " << a_random_double << " " << prob[0] << " " << nPosib<< "\n";
    for (size_t i = 0; i < nPosib; i++)
    {
      a_random_double -= prob[i];
      if (a_random_double <= 0) {
        cout << "selected: " << i << " from: " << nPosib <<  "|";
        // return get<0>(edges[vertex][i]);
        return trgVert[i];
        };  
    }
    
    return get<0>(edges[vertex][nPosib]); 
  };


  set<int> getVertexes(){
    set<int> ret;
    for (auto const& [key, val] :edges)
    {
      ret.insert(key);
    }
    return ret;
  }

  vector<int> getAdjPheromons(int vertex) override {
    return vector<int>();
  };
  
  bool addAdjPheromon(int vertex) override{
    return false;

  };


  void updatePheromons() override {

  };

};



class ant{ 
  vector<int> path;
public: 
  virtual void moveNext() = 0;
  virtual tuple<int, vector<int>> findPath(int from) = 0;
};

class AntTSP : ant{
  my_plane pl;
  public:

  AntTSP(my_plane& plan) {
    pl = plan;
  };

  void moveNext() override {

  };

  tuple<int, vector<int>> findPath(int from) override {
    auto missingVertexes = pl.getVertexes();
    int max = missingVertexes.size();
    max *= 4;
    cout << max << "\n";
    // unique_ptr<vector<int>> path (new vector<int>);
    auto path = vector<int>();
    int value = 0;
    int location = from;
    path.emplace_back(location);
    cout << "count of vertexes: "<< missingVertexes.size() << "\n";
    int timer = 0;

    while(missingVertexes.size() != 0 && timer < max){
      timer++;
      int nextVertex = pl.getNextVertex(location, missingVertexes);
      value += pl.getValue(location, nextVertex);
      missingVertexes.erase(nextVertex);
      path.emplace_back(nextVertex);
      location = nextVertex;
    }

    missingVertexes.insert(from);
    while(location != from && timer < max){
      timer++;
      int nextVertex = pl.getNextVertex(location, missingVertexes);
      value += pl.getValue(location, nextVertex);
      path.emplace_back(nextVertex);
      location = nextVertex;
    }
    cout << timer << "\n";
    cout << path.size() << ": size of path";
    if (timer < max)
    {
      return make_tuple(value, path);
    }
    else{
      return make_tuple(value, path);
      return make_tuple(-1, vector<int>{});
    }
  };
};


///-------------------main

bool isOptimal(int value, int iteration){
  return true;
}

vector<int> AntColonyTSP(my_plane& plan, int n = 20, int start = 0){
  auto ant1 = new AntTSP(plan);
  // tuple<int, vector<int>> bestResult = ant1->findPath();
  auto bestResult = ant1->findPath(start);
  int iteration = 0;

  while (!isOptimal(get<0>(bestResult), iteration)){
    iteration ++;
    for (size_t i = 0; i < n; i++){
      delete(ant1);
      ant1 = new AntTSP(plan);
      auto result = ant1->findPath(start);
      if (get<0>(result) <= get<0>(bestResult)) bestResult = result;
    }
    plan.updatePheromons(get<1>(bestResult));
  }
  return get<1>(bestResult);
}



int main() {
  string file = "g1.in";

  auto mp = my_plane();
  ifstream inpu(file);

  if (!inpu)
  { 
    cout << "unable to open file";
    return -1;
  }
  
  int from, to, value;

  while (inpu >> from >> to >> value)
  {
    mp.insEdge2(from, to, value);
  }
  //impu.close();
  // mp->WA();
  auto k = AntColonyTSP(mp, 10, 1);
  cout << "final: " << k.size() << "\n";
  for (size_t i = 0; i < k.size(); i++){
    cout << k[i] << " ";
  }
  

  return 0;
  // cout << mp->getAdjVal(1)[0];
}