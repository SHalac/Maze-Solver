// Maze algorithms for fun - 2017
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stack> 
#include <queue>
#include <set>
#include <iostream>
#include <sstream> 
#include <string>
#include <tuple>
#include <unistd.h>
using namespace std;



set<int> visited; 
stack<int> ToVisit;
queue<int> ToVisitBFS;
vector<int> prevNode;
int row_size;
int col_size;
// maze program has different types of cells
// ' ' cells are empty cells
// '#' cells are wall cells
template <typename T>
void PrintVec(vector<T> vec){
	/*
	for(typename vector<T>::iterator it = vec.begin(); it != vec.end(); ++it){
		cout << *it;
	}
	*/
	const string red("\033[0;31m");
	const string green("\033[1;32m");
	const string reef("\033[0m");
	const string magenta("\033[0;35m");
	for(int i=0; i<col_size; i++){
		for(int j=0;j<row_size;j++){
			int pos = (i*row_size)+j;
			if(vec[pos] == '.')
				cout << red << vec[pos] << reef;
			else if(vec[pos] == 'S' || vec[pos] == 'E')
				cout <<  magenta << vec[pos] << reef;
			else if(vec[pos] == '*')
				cout << magenta << vec[pos] << reef;
			else
				cout << vec[pos];
		}
		cout << endl;
	}
	cout << endl;
}
// traces shortest path on solution maze
void shortestPath(vector<char>& solution,int last){
	solution[last] = '*';
	if(prevNode[last] != 0)
		shortestPath(solution,prevNode[last]);
}

template <typename T>
void PrintStack(stack<T> stack1){
	cout << "ToVisit List: " << endl;
	stack<T> stack2 = stack1;
	while(!stack2.empty()){
		T value = stack2.top();
		stack2.pop();
		cout << "| " << value;
	}
	cout << endl;
}
// 1) check down 2) check right 3) check left 4) check up
void BFS(vector<char>& maze, vector<char>& solution,int row, int col){
	visited.clear();
	prevNode.clear();
	int pos = (row*row_size)+col;
	if(maze[pos]=='E')
		return;
	ToVisitBFS.push(pos);
	prevNode[pos] = 0;
	while(!ToVisitBFS.empty()){
		int node = ToVisitBFS.front();
		ToVisitBFS.pop();
		solution[node] = '.';
		if(maze[node]=='E'){
			shortestPath(solution,node);
			return;
		}
		visited.insert(node);
		row = (node)/ row_size;
		col = (node)% row_size;
		//cout << "at position: " << node << "  index: (" << col << ","<< row <<") " << endl;
		int up = node-row_size;
		int left = node-1;
		int right = node+1;
		int down = node+row_size;
		// queue is FIFO
		if ((row+1 < col_size) && (maze[down] != '#'))
			if(visited.find(down) == visited.end()){
				prevNode[down] = node;
				ToVisitBFS.push(down); // visit down
			}
		if ((col+1 < row_size)&& (maze[right] != '#'))
			if(visited.find(right) == visited.end()){
				prevNode[right] = node;
				ToVisitBFS.push(right);// visit right
			}
		if(col-1 >= 0)
			if((visited.find(left) == visited.end())&& (maze[left] != '#')){
				prevNode[left] = node;
				ToVisitBFS.push(left);// visit left			
			}	
		if(row-1 >=0)
			if( (visited.find(up) == visited.end())&& (maze[up] != '#')) {
				prevNode[up] = node;
				ToVisitBFS.push(up);//visit up
			}
		usleep(50000);
		PrintVec(solution);
		//cout << "\033[2J\033[1;1H";
		cout << "\x1B[2J\x1B[H";
	}
}

// 1) check down 2) check right 3) check left 4) check up
//add them to a stack
//check visited 
void DFS(vector<char>& maze, vector<char>& solution,int row, int col){
	visited.clear();
	int pos = (row*row_size)+col;
	if(maze[pos]=='E')
		return;
	ToVisit.push(pos);
	cout << "start pos: (" << col << "," << row << ")"<< endl;
	while(!ToVisit.empty()){
		int node = ToVisit.top();
		ToVisit.pop();
		solution[node] = '.';
		if(maze[node]=='E'){
			shortestPath(solution, node);
			return;
		}
		visited.insert(node); 		
		row = (node)/ row_size;
		col = (node)% row_size;
		//cout << "at position: " << node << "  index: (" << col << ","<< row <<") " << endl;
		int up = node-row_size;
		int left = node-1;
		int right = node+1;
		int down = node+row_size;
		// stack is LIFO, push in reverse order of desired traversal
		// set returns a .end() iterator if it doesnt find the item
		// only explore if it isnt in visited set

		// add to stack if not visited and not a wall
		if(row-1 >=0)
			if( (visited.find(up) == visited.end())&& (maze[up] != '#')) {
				prevNode[up] = node;
				ToVisit.push(up);//visit up
			}
		if(col-1 >= 0)
			if((visited.find(left) == visited.end())&& (maze[left] != '#')){
				prevNode[left] = node;
				ToVisit.push(left);// visit left
			}
		if ((col+1 < row_size)&& (maze[right] != '#'))
			if(visited.find(right) == visited.end()){
				prevNode[right] = node;
				 ToVisit.push(right);// visit right
			}
				
		if ((row+1 < col_size) && (maze[down] != '#'))
			if(visited.find(down) == visited.end()){
				prevNode[down] = node;
				ToVisit.push(down); // visit down
			}
				
		usleep(50000);
		PrintVec(solution);
		//cout << "\033[2J\033[1;1H";
		cout << "\x1B[2J\x1B[H";
	}
}

int main(int argc, char** argv){
	// read a file 
	// tellg() tellp() for get and put position
	vector<char> Maze;
	vector<char> mazeSolution;
	int rows,cols;
	int start;
	string row,col; // #rows, #col
	ifstream myFile ("mazetext.txt");
	getline(myFile,row,'|');
 	getline(myFile,col);
 	col_size = stoi(row);
 	row_size = stoi(col);

 	cout << "size of maze is " << row << " rows " << col << " columns" << endl;
 	if (myFile.is_open())
	{
		string line;
  		 while (getline(myFile,line))
   		{
    		for(string::size_type i = 0; i < line.size(); ++i) {
   			 		Maze.insert(Maze.end(),line[i]);
   			 		if(line[i] == 'S')
   			 			start = i;
   			 			
				}// for loop
   		}//while
    myFile.close();
	}
	PrintVec(Maze);
	mazeSolution = Maze;
	prevNode.assign(Maze.size(),0);
	if(strcmp(argv[1],"DFS") == 0){
		cout << "Starting DFS... " << endl << endl;
		usleep(2000000);
		DFS(Maze,mazeSolution,0,start); // start is on first row 
	}
	else{
		cout << "Starting BFS... " << endl << endl;
		usleep(2000000);
		BFS(Maze,mazeSolution,0,start);
	}

	PrintVec(mazeSolution);

	ofstream outFile("mazeSolution.txt");
	for(int z=0;z<col_size;z++){
		for(int y=0;y<row_size;y++){
			int index = (z*row_size)+y;
			outFile << mazeSolution[index];
		}
		outFile << '\n';
	}
	outFile.close();
	return 0;
}