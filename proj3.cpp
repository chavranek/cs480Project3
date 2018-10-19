/*
Authors: Christian Havranek, Christopher Tran, Cole Watson
Project: Project 3 (A* with permutation)
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <time.h>
#include <iomanip>

using namespace std;

int AstarVisited = 0;
int AstarMaxQueueSize = 0;


vector<int> getInput ();
void Astar (vector<int> Permutation, int size);
bool checkGoal (vector<int> Permutation, int size);

struct Node
{
  vector<int> Perm;
  int parent;
  int depth;
  int hvalue;
  
  Node()
  {
    vector<int> Perm;
    parent = -1;
    int depth = 0;
    hvalue = 0;
  }
  friend bool operator < (Node const& node1, Node const& node2)
  {
    return (node1.hvalue < node2.hvalue);
  }

     
};


void printOutput (vector<Node> Pointers, int index);



int gethValue(vector<int> succ)
{
  int hValue = 0;
  int breakPoints = 0;
  for (int i = 0; i < succ.size(); i++)
    {
      if (succ[i] != succ[i]+1 && succ[i] != succ[i]-1)
	{
	  breakPoints++;
	}
    }
  hValue = .5 * breakPoints;
}


void successors(vector<Node>&Pointers, priority_queue<Node>&Queue, vector<int>permutation, int parent, int size)
{
  
  for(int i = 2; i <= size; i++)
    {
      for(int j = 0; j + i <= size; j++)
        {
	  vector<int> cycle;
          vector<int> succ;
	  succ = permutation;
	  int hValue = gethValue(succ);
          reverse(succ.begin()+j, succ.begin()+j+i);
	  cycle = Pointers[parent].Perm;
	  if (!equal(succ.begin(), succ.end(), cycle.begin()))
	    {
	      Node childNode;
	      childNode.hvalue = hValue;
	      childNode.Perm = succ;
	      childNode.parent = parent;
	      Pointers.push_back(childNode);

	      childNode.parent = Pointers.size() - 1;
	      Queue.push(childNode);
	      AstarVisited++;
	      if (Queue.size() > AstarMaxQueueSize)
	        AstarMaxQueueSize = Queue.size();
	    }
	}
    }
}

int main()
{
    vector<int> permutation = getInput();
    int size = permutation.size();
    
    clock_t t;
    t = clock();
    Astar(permutation, size);
    t = clock() - t;

    cout << "A* time in seconds:  " << fixed << setprecision(6) << (float(t))/CLOCKS_PER_SEC << endl;
    cout << "A* total number of visited states: " << AstarVisited << endl;
    cout << "A* max queue size: " << AstarMaxQueueSize << endl;

    cout << "\n" << endl;

    return 0;
}

vector<int> getInput()
{
  int num = 0;
  string P;
  string number;
  vector<int> permutation;
  cout << "Input a permutation with a bracket at the beginning, commas " << endl;
  cout << "between each number, and a closing bracket at the end of" << endl;
  cout << "your permutation. Follow the format of the example below:" << endl;
  cout << "[1,2,3,4,5,6,7,8,9]" << endl;

  // grabs the entire line of user input
  getline(cin, P);
  cout << endl;

  // moves through the entire user input and
  // converts all characters to ints, then
  // pushes them onto a vector.
  for(int i = 0; i < P.length(); i++)
  {
    if (isdigit(P[i]))
      {
	string snum(1, P[i]);
	number += snum;
      }
    else if(P[i] == ' ' || P[i] == ',')
      {
	if (i != P.length()-1)
	  {
	    if (!number.empty())
	      {
		int num = stoi(number);
		permutation.push_back(num);
		number = "";
	      }
	  }
      }
    if(i == P.length()-1)
      {
        int num = stoi(number);
        permutation.push_back(num);
        number = "";
      }

  }
  
  return permutation;
}

void Astar(vector<int> Permutation, int size)
{
  vector<Node> Pointers;
  priority_queue<Node> Queue;
    
    Node initial;
    initial.parent = -1;
    for (int i = 0; i < size; i++)
        initial.Perm.push_back(Permutation[i]);
        
    Pointers.push_back(initial);
    initial.parent = Pointers.size()-1;
    Queue.push(initial);
    AstarVisited++;
    AstarMaxQueueSize = Queue.size();
    
    while(!Queue.empty())
    {
        Node currentNode = Queue.top();
	Queue.pop();
        if (checkGoal(currentNode.Perm, size))
        {
	  cout << "Answer for A*:" << endl;
            printOutput(Pointers, currentNode.parent);
            return;
        }
        
        successors(Pointers, Queue, currentNode.Perm, currentNode.parent, size);
    }
    return;
}

bool checkGoal(vector<int> Permutation, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (Permutation[i] > Permutation[i+1])
            return false;
    }
    return true;
}

void printOutput(vector<Node> Pointers, int index)
{
  vector< vector < int> > moves;

    int moveNum = 1;
    while (index != -1)
    {
      moves.push_back(Pointers[index].Perm);
      index = Pointers[index].parent;
    }
    
    reverse(moves.begin(), moves.end());
    cout << "The number of moves = " << moves.size()-1 << endl;
    for(int i = 1; i < moves.size(); i++)
      {
	cout << "Move " << moveNum << ": " << "[";
	for(int j = 0; j < moves[i].size(); j++)
	  {
	    if(j < (moves[i].size()-1))
	      {
		cout << moves[i][j] << ",";
	      }
	    else
	      {
		cout << moves[i][j] << "]";
	      }
	  }
	moveNum++;
	cout << endl;
      }
}
