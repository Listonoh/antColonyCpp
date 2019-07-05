#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class ant{ 
  vector<int> path;
public: 
  virtual void moveNext() {};
};

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
  vector<int> vertex;

  vector<int> getAdjVal(int vertex){
    auto k = vector<int>();
    k.emplace_back(vertex);
    return k;
  }

public:
  my_plane(){
    edges = map<int, vector<tuple<int, int>>>();
  };

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
      std::cout << key << ':';
      for( auto const& [a, b] : val ) {
        cout << "to: " << a << ", val :" << b << "|";
      }      // string's value
        cout << std::endl ;
    }
  };


  int getNextVertex(int vertex) override {
    return 7;
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
  
  mp->WA();
  

  
  // cout << mp->getAdjVal(1)[0];
}