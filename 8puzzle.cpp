#include <bits/stdc++.h> 
using namespace std; 
#define N 3 
int final[N][N] = 
	{ 
		{1, 2, 3}, 
		{4, 5, 6}, 
		{7, 8, 0} 
	}; 
int heur;
int max_fringe = 0;
int num_nodes = 0;
int path_length=0;
// state space tree nodes 
struct Node 
{ 
	Node* parent;  	
	int mat[N][N];		// stores matrix 
	int x, y; 		// stores blank tile coordinates 
	int cost; 		// stores the number of misplaced tiles 
	int level;		// stores the number of moves so far  	
}; 

vector<Node*> visited;
// Function to print N x N matrix 
void printMatrix(int mat[N][N]) 
{ 
	for (int i = 0; i < N; i++) 
	{ 
		for (int j = 0; j < N; j++) 
			printf("%d ", mat[i][j]); 
		printf("\n"); 
	} 
} 
int matrixCheck(int a[3][3], int b[3][3])
{
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(a[i][j]!=b[i][j])
				return 0;
		}
	}
	return 1;
}

int checkVisited(Node* node)
{
	for(int i=0;i<visited.size();i++)
	{
		if(matrixCheck(visited[i]->mat,node->mat))
			return 1;
	}
	return 0;
}

// Function to allocate a new node 
Node* newNode(int mat[N][N], int x, int y, int newX, 
			int newY, int level, Node* parent) 
{ 
	Node* node = new Node; 
	node->parent = parent; 
	memcpy(node->mat, mat, sizeof node->mat);	// copy data from parent node to current node  
	swap(node->mat[x][y], node->mat[newX][newY]); 	// move tile by 1 position 
	node->cost = INT_MAX; 	// set number of misplaced tiles 
	node->level = level; 	// set number of moves so far 

	// update new blank tile cordinates 
	node->x = newX; 
	node->y = newY; 

	return node; 
} 

// bottom, left, top, right 
int row[] = { 1, 0, -1, 0 }; 
int col[] = { 0, -1, 0, 1 }; 

// Function to calculate the number of misplaced tiles 
int calculateCost(int initial[N][N], int final[N][N]) 
{ 
	// cout<<"using misplaced tiles"<<endl;
	int count = 0; 
	for (int i = 0; i < N; i++) 
		for (int j = 0; j < N; j++) 
			if (initial[i][j] && initial[i][j] != final[i][j]) 
			count++; 
	return count; 
} 

int manhattan(int a[3][3], int goal[3][3])
{
	int dist=0;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(a[i][j]!=0)
			{
				for(int k=0;k<3;k++)
				{
					for(int l=0;l<3;l++)
					{
						if(a[i][j]==goal[k][l])
							dist+=abs(i-k)+abs(j-l);
					}
				}
			}
		}
	}
	
	return dist;
}

int heuristic(int initial[N][N], int final[N][N])
{
	if(heur==1)
		return calculateCost(initial,final);
	else
		return manhattan(initial,final);
}

// Function to check if (x, y) is a valid matrix cordinate 
int isSafe(int x, int y) 
{ 
	return (x >= 0 && x < N && y >= 0 && y < N); 
} 

// print path from root node to destination node 
void printPath(Node* root) 
{ 
	if (root == NULL) 
		return; 
	printPath(root->parent); 
	path_length++;
	printMatrix(root->mat); 

	printf("\n"); 
} 

// Comparison object to be used to order the heap 
struct comp 
{ 
	bool operator()(const Node* lhs, const Node* rhs) const
	{ 
		return (lhs->cost + lhs->level) > (rhs->cost + rhs->level); 
	} 
}; 

deque<Node*> expand(Node* node)
{
	deque<Node*> deq;
	// do for each child of node 
	// max 4 children for a node 
	for (int i = 0; i < 4; i++) 
	{ 
		if (isSafe(node->x + row[i], node->y + col[i])) 
		{ 
			// create a child node and calculate 
			// its cost 
			Node* child = newNode(node->mat, node->x, 
						node->y, node->x + row[i], 
						node->y + col[i], 
						node->level + 1, node); 
			child->cost = heuristic(child->mat, final); 

			// Add child to list of live nodes 
			deq.push_back(child); 
		} 
	}
	return deq;
}

deque<Node*> modifiedExpand(Node* node, int depth)
{
	deque<Node*> temp1;
	deque<Node *> temp2;
	deque<Node *> temp3;
	temp3.push_back(node);
	temp1.push_back(node);
	// cout<<"-----------"<<"\n";
	while(depth>0 && !temp1.empty()){
		int l=temp1.size();
		// cout<<"l: "<<l<<"\n";
		while(l--){
			Node * c=temp1.front();
			temp1.pop_front();
			temp2=expand(c);
			// printMatrix(c->mat);
			// cout<<"qsize: "<<temp1.size()<<"\n";
			// cout<<"q2size"<<temp2.size()<<"\n";
			for(auto it=temp2.begin(); it!=temp2.end(); it++){
				
				if(!checkVisited(*it)){
					
					temp3.push_back(*it);
					
				}
				temp1.push_back(*it);
			}
			// cout<<"q1111size: "<<temp1.size()<<"\n";
		}
		// cout<<"depth: "<<depth<<"\n";
		depth--;
	}
	return temp3;
}

// x and y are blank tile coordinates in initial state 
void solve(int initial[N][N], int x, int y, 
		int final[N][N], int depth) 
{ 
	// Create a priority queue to store live nodes of 
	// search tree; 
	priority_queue<Node*, std::vector<Node*>, comp> pq; 

	// create a root node and calculate its cost 
	Node* root = newNode(initial, x, y, x, y, 0, NULL); 
	root->cost = heuristic(initial, final); 
	cout<<"running...."<<endl;
	while(root->cost)
	{
		// printPath(root);
		// cout<<"---"<<endl;
		deque<Node*> temp = modifiedExpand(root,depth);
		num_nodes+= temp.size();
		for(auto it=temp.begin();it!=temp.end();it++)
		{
			if(!checkVisited(*it)){
				pq.push(*it);
				visited.push_back(*it);
			}
				
		}

		if(pq.size()>max_fringe)
			max_fringe=pq.size();

		
		// visited.push_back(root);
		temp.clear();
		pq.pop();
		root = pq.top();
	}
	printPath(root);
	cout<<"max fringe : "<<max_fringe<<endl;
	cout<<"number of nodes visited : "<<num_nodes<<endl;
	
} 


void generate(int initial[][N])
{
	// srand(time(0));
	int num = rand()%300;
	while(num--)
	{
		int m = rand()%4;
		int x = rand()%3;
		int y = rand()%3;
		switch(m)
		{
			case 0: 	//up
			{
				if(y!=0 && initial[x][y-1]==0)
				{
					swap(initial[x][y],initial[x][y-1]);
				}
			}
			case 1:		//down
			{
				if(y!=2 && initial[x][y+1]==0)
				{
					swap(initial[x][y],initial[x][y+1]);
				}
			}
			case 2:		//left
			{
				if(x!=0 && initial[x-1][y]==0)
				{
					swap(initial[x][y],initial[x-1][y]);
				}
			}
			case 3:		//right
			{
				if(x!=2 && initial[x+1][y]==0)
				{
					swap(initial[x][y],initial[x+1][y]);
				}
			}
		}
	}
}

void clear()
{
	visited.clear();
	max_fringe=0;
	num_nodes=0;
	path_length=0;
	// while(!fringe.empty()){
	// 	fringe.pop();
	// }
}
// Driver code 
int main() 
{ 
	// Initial configuration 
	// Value 0 is used for empty space 
	srand(time(0));
	int a=10;
	int d;
	// int initial[N][N] = 
	// 	{ 
	// 		{1, 6, 4}, 
	// 		{7, 0, 8}, 
	// 		{2, 3, 5} 
	// 	};
	cout<<"enter depth"<<endl;
	cin>>d;
	while(a--)
	{
		int initial[N][N] = 
		{ 
			{1, 2, 3}, 
			{4, 5, 6}, 
			{7, 8, 0} 
		}; 
		generate(initial);
		// for(int i=0;i<N;i++)
		// {
		// 	for(int j=0;j<N;j++)
		// 		cout<<initial[i][j]<<" ";
		// 	cout<<endl;
		// }
		// Blank tile coordinates in initial 
		// configuration 
		int x,y;
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
				if(initial[i][j]==0)
				{
					x=i;
					y=j;
					break;
				}
		}
		cout<<"initial matrix : "<<endl;
		printMatrix(initial);

		heur=1;
		clear();
		cout<<"solution with misplaced tiles and depth = 1"<<endl;
		solve(initial, x, y, final,1);
		cout<<"path_length : "<<path_length<<endl;


		clear();	
		cout<<"solution with misplaced tiles and depth = "<<d<<endl;
		solve(initial, x, y, final,d);
		cout<<"path_length : "<<path_length<<endl;
		

		heur=0;
		clear();
		cout<<"solution with manhattan and depth = 1"<<endl;
		solve(initial, x, y, final,1);
		cout<<"path_length : "<<path_length<<endl;
	
		clear();
		cout<<"solution with manhattan and depth = "<<d<<endl;
		solve(initial, x, y, final,d);

		// cout<<"enter 1 for misplaced tiles, 2 for manhattan."<<endl;
		// cin>>heur;
		cout<<"path_length : "<<path_length<<endl;
		cout<<"--------------------------------"<<endl<<endl;

	}
	return 0; 
} 
