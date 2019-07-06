#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class plane{
public:
  plane() {};
  // virtual vector<int> getAdjVal(int vertex) = 0;
  virtual int getNextVertex(int vertex) = 0;
  virtual vector<int> getAdjPheromons(int vertex) = 0;
  virtual bool addAdjPheromon(int vertex) = 0;
  virtual void updatePheromons() = 0;
};
 

class my_plane : plane{
  // vector<tuple<int, int, int>> edge;
  map<int, vector<tuple<int, int>>> edges; //from , <to , value>
  map<tuple<int, int> , int> pheromones;
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

  void updatePheromons(vector<int>& path){
    cout << "updating pheromones \n"
  }

  void insEdge2(int from , int to, int value){
    if (from != to)
    {
      edges[from].emplace_back(make_tuple(to, value));
      edges[to].emplace_back(make_tuple(from, value));

    }
  };

  void WA(){
    for( auto const& [key, val] : edges )
    {
      std::cout << key << ":  ";
      for( auto const& [a, b] : val ) {
        cout << "to: " << a << ", val :" << b << " | ";
      }     
        cout << std::endl ;
    }
  };


  int getNextVertex(int vertex) override {
   int nPosib = edges[vertex].size();

  };


 

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
  virtual tuple<int, vector<int>> findPath() = 0;
};

class AntTSP : ant{
  my_plane pl;
  public:
  AntTSP(my_plane& plan) {
    pl = plan;
  };

  void moveNext() override {

  };

  tuple<int, vector<int>> findPath() override {
    

    // auto a = {1,2,3};
    auto a = vector<int>{1,2,3};
    cout << a[2];
    return make_tuple(1,a);
    // return make_tuple(1, make_vector(1,2,3));
  };
};

bool isOptimal(int value, int iteration){
  return true;
}

vector<int> AntColonyTSP(my_plane& plan, int n = 20){
  auto ant1 = new AntTSP(plan);
  // tuple<int, vector<int>> bestResult = ant1->findPath();
  auto bestResult = ant1->findPath();
  int iteration = 0;

  while (!isOptimal(get<0>(bestResult), iteration)){
    iteration ++;
    for (size_t i = 0; i < n; i++){
      delete(ant1);
      ant1 = new AntTSP(plan);
      auto result = ant1->findPath();
      if (get<0>(result) <= get<0>(bestResult)) bestResult = result;
    }
    plan.updatePheromons(get<1>(bestResult));
  }
}



int main() {
  string file = "g1.in";

  auto mp = new my_plane();
  ifstream inpu(file);

  if (!inpu)
  { 
    cout << "unable to open file";
    return -1;
  }
  
  int from, to, value;

  while (inpu >> from >> to >> value)
  {
    mp->insEdge2(from, to, value);
  }
  //impu.close();
  mp->WA();
  

  
  // cout << mp->getAdjVal(1)[0];
}