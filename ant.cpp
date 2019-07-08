#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <cmath>
#include <random>
#include <ctime>
#include <climits>

using namespace std;

class plane{
public:
  plane() {};
  virtual int getNextVertex(int vertex, set<int>& missingVert) = 0;
  virtual void updatePheromons() = 0;
};
 

class my_plane : plane{
  map<int, vector<tuple<int, int>>> edges; //from , <to , value>
  map<tuple<int, int>, int> edgesValues;
  map<tuple<int, int>, int> pheromones;
  std::default_random_engine re;
  double BETA = 1;
  double ALPHA = 1;

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
      int temp = to;
      to = from;
      from = temp;
    }
    return edgesValues[make_tuple(from, to)];
  }

  int getPheromons(int from, int to){
    if(from > to){
      int temp = to;
      to = from;
      from = temp;
    }
    return pheromones[make_tuple(from, to)];
  }

  void updatePheromons() override {
    //cout << "updating pheromones";
  };

  void updatePheromons(vector<int>& path){
    // cout << "updating pheromones \n";
  }

  void insEdge2(int from , int to, int value){
    if (from != to){
      edges[from].emplace_back(make_tuple(to, value));
      edges[to].emplace_back(make_tuple(from, value));

      if(from > to) {
        int temp = from;
        from = to;
        to = temp;
      };

      edgesValues[make_tuple(from, to)] = value; // from < to 
      pheromones[make_tuple(from, to)] = 1;
    }
  };

  //write all
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
    double prob [nPosib]; 
    double sigma = 0; //all probabilties 
    int i = 0; //counter

    //initial search for posible edges 
    for (auto const& [to, value] : edges[vertex]){
      //if is not in missing then skip him
      if (missingVert.find(to) == missingVert.end()){
        nPosib--;
        continue;
      }


      double ETAij = (double) pow ((double)1 / value, BETA);
	    double TAUij = (double) pow (getPheromons(vertex,to),   ALPHA);

      prob[i] = ETAij * TAUij;
      trgVert[i] = to;
      sigma += prob[i];
      i++;
    }

    //if no edge is selected we choose randomly from the rest
    if(sigma == 0){
      // cout << "n";
      i = 0;
      nPosib = edges[vertex].size();
      for (auto const& [to, value] : edges[vertex]){

        double ETAij = (double) pow ((double)1 / value, BETA);
        double TAUij = (double) pow (getPheromons(vertex,to),   ALPHA);

        // cout << ETAij << "><" << TAUij << "!" << value << " |" ;
        prob[i] = ETAij * TAUij;
        trgVert[i] = to;
        sigma += prob[i];
        i++;
      }
    }

    double upper_bound = sigma;
    // double lower_bound = 0;
    // std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    // double a_random_double = unif(re);
    double a_random_double = (upper_bound) * ( (double)rand() / (double)RAND_MAX );

    //cout << sigma << " " << a_random_double << " " << prob[0] << " " << nPosib << "\t";
    for (size_t i = 0; i < nPosib; i++)
    {
      a_random_double -= prob[i];
      if (a_random_double <= 0) {
        //cout << " selected: " << i << " from: " << nPosib <<  "\n";
        // return get<0>(edges[vertex][i]);
        return trgVert[i];
        };  
    }
    
    //souldnt return here 
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



};



class ant{ 
  vector<int> path;
public: 
  virtual tuple<int, vector<int>> findPath() = 0;
};

class AntTSP : ant{
  my_plane pl;
  public:

  AntTSP(my_plane& plan) {
    pl = plan;
  };

  tuple<int, vector<int>> findPath() override {
    auto missingVertexes = pl.getVertexes();
    int max = missingVertexes.size();
    max *= 4; //it shouldnt be more then 2* but its "tree" and tree has n-1 edges

    // cout << max << "\n";

    int r = rand() % max;
    auto from = *missingVertexes.begin();
    auto path = vector<int>();
    int location = from;
    path.emplace_back(location);
    missingVertexes.erase(from);

    // cout << "count of vertexes: "<< missingVertexes.size() << "\n";

    int value = 0;
    int timer = 0;
    
    // completing route/path
    while(missingVertexes.size() != 0 && timer < max){
      timer++;
      int nextVertex = pl.getNextVertex(location, missingVertexes);
      value += pl.getValue(location, nextVertex);
      missingVertexes.erase(nextVertex);
      path.emplace_back(nextVertex);
      location = nextVertex;
    }

    // returning back home
    missingVertexes.insert(from);
    while(location != from && timer < max){
      timer++;
      int nextVertex = pl.getNextVertex(location, missingVertexes);
      value += pl.getValue(location, nextVertex);
      path.emplace_back(nextVertex);
      location = nextVertex;
    }


    // cout << timer << "\n";
    // cout << "size of path: " << path.size() << " value of path: " << value <<" \n" ;
    if (timer < max)
    {
      return make_tuple(value, path);
    }
    else{
      // return make_tuple(value, path);
      return make_tuple(INT_MAX, vector<int>{});
    }
  };
};


///-------------------main
int mValue = -1;
int mCounter = 0;
bool isOptimal(int value, int iteration){
  if (mValue == -1) {
    mValue = value;
    return false;
  }

  if (mValue < value){
    mCounter = 0;
    mValue = value;
    return false;
  }

  mCounter ++;
  if (mCounter >= 20)
  {
    return true;
  }
  
  return false;
}

tuple<int, vector<int>> AntColonyTSP(my_plane& plan, int n = 20){
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
  return bestResult;
}



int main() {
  string file = "g1.in";
  std::srand(std::time(nullptr));
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
  auto tK = AntColonyTSP(mp, 10);
  auto k = get<1>(tK);

  cout << "final size: " << k.size() << "\n";
  cout << "final value: " << get<0>(tK) << "\n";
  for (size_t i = 0; i < k.size(); i++){
    cout << k[i] << " ";
  }
  

  return 0;
  // cout << mp->getAdjVal(1)[0];
}