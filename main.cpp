#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include<bits/stdc++.h>
using namespace std;

int vertices;
int k;
int max_coreness;

void signalHandler( int signum ){
  cout << "Get signal " << signum << endl;
  exit(signum);
}

class Graph
{
public:
    Graph(){
      //this->V = V;
      //adj = new vector<int>[V+1];
      adj = new vector<int>[82168];
    };

    //int V; // No. of vertices
    vector<int> *adj;
};

void procedure_core(Graph g1, int deg[]){
  int n, d, md, i, start, num;
  int v, u, w, du, pu, pw;
  int vert[vertices];
  int pos[vertices];
  int bin[vertices];
  //int deg[vertices];

  n = vertices; md = 0;
  for(v=0; v<n; v++){
    d=0;
    for(auto u=g1.adj[v].begin(); u!=g1.adj[v].end(); u++) d++;
    deg[v] = d;
    if(d>md) md = d;
  }

  for(d=0; d<=md; d++) bin[d]=0;
  for(v=0; v<n; v++) bin[deg[v]]++;
  start = 0;

  for(d=0; d<=md; d++){
    num = bin[d];
    bin[d] = start;
    start += num;
  }

  for(v=0; v<n; v++){
    pos[v] = bin[deg[v]];
    vert[pos[v]] = v;
    bin[deg[v]]++;
  }

  for(d=md; d>=1; d--)
    bin[d] = bin[d-1];

  bin[0] = 0;

  for(i=0; i<n; i++){
    v = vert[i];
    for(auto u=g1.adj[v].begin(); u!=g1.adj[v].end(); u++){
      if(deg[*u]>deg[v]){
        du = deg[*u];
        pu = pos[*u];
        pw = bin[du];
        w = vert[pw];
        if(*u != w){
          pos[*u] = pw;
          pos[w] = pu;
          vert[pu] = w;
          vert[pw] = *u;
        }
        bin[du]++;
        deg[*u]--;
      }
    }
  }

  // Create and open a text file
  ofstream MyFile("kcore.txt");

  for(int i=0; i<vertices; i++){
    if(deg[i]>=k)
      MyFile << i << " " << deg[i] << endl;
  }

  // Close the file
  MyFile.close();

  //end of function
}

/*
void clique(Graph g1, int deg[]){
  // Create and open a text file
  ofstream MyFile("clique.txt");

  for(int i=0; i<vertices; i++){
    if(deg[i]==1499)
      MyFile << i << endl;
  }

  // Close the file
  MyFile.close();

  //end of function
}*/

void clique(Graph g1, int deg[]){
  int count = 0;
  bool valid = false;
  //find the largest coreness
  for(int i=0; i<vertices; i++){
    if(deg[i]>max_coreness){
      max_coreness = deg[i];
    }
  }
  // Create and open a text file
  ofstream MyFile("clique.txt");

  for(int i=0; i<vertices; i++){
    if(deg[i]==max_coreness){
      MyFile << i << endl;
    }
  }

  // Close the file
  MyFile.close();
  //end of function

}

int main(int argc, char*argv[]) {
  signal(SIGINT, signalHandler);

  int src, dest;
  src = -1;
  dest = -1;
  bool init_vertices[82168]={0};
  vertices = 0;
  k=0;
  max_coreness = INT_MIN;
  Graph g1;

  string fh = string(argv[1]);
  ifstream input_file;
  input_file.open(fh);

  while(input_file >> src >> dest && !input_file.eof()){
        g1.adj[src].push_back(dest);
        g1.adj[dest].push_back(src);
        if(init_vertices[src]==false){
          init_vertices[src] = true;
          vertices++;
        }
        if(init_vertices[dest]==false){
          init_vertices[dest] = true;
          vertices++;
        }
        src = -1;
        dest = -1;
  }
  k = stoi(argv[2]);
  int deg[vertices];
  procedure_core(g1, deg);
  input_file.close();
  clique(g1, deg);
}


