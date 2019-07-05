#include <iostream>
#include <string>
#include <vector>
using namespace std;

class ant{ 
public: 
  virtual void moveNext() {};
};

class plane{
public:
  plane() {};
  virtual vector<int> getAdjVal(int vertex) = 0;
  virtual vector<int> getAdjPheromons(int vertex) = 0;
  virtual bool addAdjPheromon(int vertex) = 0;
  virtual void updatePheromons() = 0;
};


class my_plane : plane{
  vector<tuple<int, int, int>> edge;
  vector<int> vertex;
public:
  my_plane(){

  };

  

  vector<int> getAdjVal(int vertex) override {
    auto k = vector<int>();
    k.emplace_back(vertex);
    return k;
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


int main() {
  std::string name;
  std::cout << "What is your name? ";
  getline (std::cin, name);
  std::cout << "Hello, " << name << "!\n";


  auto mp = new my_plane();
  cout << mp->getAdjVal(1)[0];
}