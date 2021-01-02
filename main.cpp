#include <iostream>
#include<iostream>
#include <list>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <iomanip>
#include <chrono> 

using namespace std;
using namespace std::chrono; 

class Graph {

public:
    int numberOfVertices; // No. of vertices

    // Pointer to an array containing adjacency lists
    vector<vector<int>> adjacencyList;
    unordered_map<int,int> incoming_counter;
    unordered_map<int,int> outgoing_counter;
    void traverse(int v, bool visited[]);

    Graph(int numberOfVertices);

    // ~Graph(); 

    void addEdge(int v, int w) ;

};


Graph::Graph(int numberOfVertices) {

    this->numberOfVertices = numberOfVertices;
    vector<vector<int>> adjacencyList ;
}

// Graph::~Graph() {
//     delete[] adjacencyList;
// }

void Graph::addEdge(int v, int w) {
    unordered_map<int,int>::iterator itr;

    try {
        adjacencyList.at(v).push_back(w); 

    }
    catch(...){
        vector<int> temp;
        temp.push_back(w);
        adjacencyList.push_back(temp);

    }

	try {

        itr = incoming_counter.find(w);
        if (itr != incoming_counter.end())
        {
            itr->second = itr->second + 1;
        }
        else
        {
            incoming_counter.insert({w, 1});
        }
            
        itr = outgoing_counter.find(v);
        if (itr != outgoing_counter.end()){
            itr->second = itr->second + 1;
        }
        else{
            outgoing_counter.insert ({v, 1});
        }


	}
	catch(...) {

       outgoing_counter.insert ( pair<int,int>(v, 1));
       try{
            itr = incoming_counter.find(w);
            if (itr != incoming_counter.end())
            {               
                itr->second = itr->second + 1;
            }
            else
            {
                incoming_counter.insert ( {w, 1});
            }
           }

       catch(...) {
            
       }
	}
    // Add w to v’s list.
}

// void Graph::traverse(int v, bool visited[]) {

//     // Mark the current node as visited and
//     // print it

//     visited[v] = true;
//     cout << v << " ";

//     // Recur for all the vertices adjacent
//     // to this vertex
//     list<int>::iterator i;
//     for (i = adjacencyList[v].begin(); i != adjacencyList[v].end(); ++i)
//         if (!visited[*i])
//             traverse(*i, visited);
// }


bool IsEulerian(Graph graph){


    for(int i = 0; i< graph.adjacencyList.size(); i++){

        // cout<< graph.incoming_counter.find(i)->second << " " << graph.outgoing_counter.find(i)->second<<endl;
        if(graph.incoming_counter.find(i)->second !=graph.outgoing_counter.find(i)->second){
            cout<<"not"<<endl;
            return 0;
        }
        else{
            return 1;
        }

    }
}


vector<string> word_parser(string array){

    string word;
    vector<string> input_list;
        for (auto x : array) 
        {
            if (x == ' ')
            {
                input_list.push_back(word);
                word = "";
            }
            else {
                word = word + x;                    
            }                              
        }
    input_list.push_back(word);

    return input_list;
}

int find_vertex_with_nonused_edge(vector<int> eulerian_circuit, vector<vector<int>> edge_vector){
    for(int i =0; i< eulerian_circuit.size(); i++){

        auto it1 = next(eulerian_circuit.begin(), i);
        if(edge_vector.at(*it1).size() > 0)
        {
            return *it1;
        }
    }

    return -1;
}

int main(int argc, char const *argv[]) {

    auto start = high_resolution_clock::now(); 

    ifstream infile;
    infile.open(argv[1]);
    string line;
    vector<string> input;
    vector<string> input_array;
    int number_of_vertices;
    int starting_vertex;
    int previous_start;
    int edge_count =0;



    
    // read the input file
    while (!infile.eof())
    {
        getline (infile,line);   
        input_array.push_back(line);                
    }
    infile.close();


    number_of_vertices = stoi(input_array.at(0));
    starting_vertex = stoi(input_array.at(input_array.size()-1));


    // Initiliaze the graph

    auto start1 = high_resolution_clock::now(); 
    Graph graph(number_of_vertices);

    for (int i=1; i< input_array.size()-1 ; i++){
        
        input = word_parser(input_array.at(i));
        int vertex = stoi(input.at(0));

        for(int j=2; j<input.size(); j++){
            graph.addEdge(vertex, stoi(input.at(j)));
            edge_count++;
        }

    }

    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start1);

    cout<<endl;
    cout << "Time taken by function: "
         << duration.count() / 1000000 << " seconds" << endl; 

    // cout<<endl;

// Show neighbors
// for(int i = 0; i < graph.adjacencyList.size(); i ++){
//     cout<<i<<" ";
//     for(int j = 0; j < graph.adjacencyList.at(i).size(); j ++)
//     {
//         cout<<graph.adjacencyList.at(i).at(j)<< " ";

//     }
//     cout<<endl;

// }


// cout<<"176"<<endl;
// cout<<endl;



if(IsEulerian(graph) == 0){

    cout<<"no path"<< endl;
    ofstream myfile (argv[2]);

    myfile<<"no path";
    return 0;
}


else{
	cout<<"Eulerian" << endl;
}


// cout<<graph.outgoing_counter.size()<< endl;
// Hierholzer’s Algorithm

int candidate;


vector<int> eulerian_circuit;
cout<<starting_vertex<<endl;
eulerian_circuit.push_back(starting_vertex);

vector<vector<int>> edge_vector = graph.adjacencyList;
while (eulerian_circuit.size() < edge_count){

    vector<int>tour;
    previous_start = starting_vertex;
    while (edge_vector.at(starting_vertex).size() > 0){
    
        candidate = edge_vector.at(starting_vertex).at(0);
        edge_vector.at(starting_vertex).erase(edge_vector.at(starting_vertex).begin());
        starting_vertex = candidate;
        tour.push_back(starting_vertex);
    }

    // for(int i=0; i < tour.size(); i++)
    //     {
    //     auto it1 = next(tour.begin(), i);
    //         cout<<*it1<<" ";
    // }

    // cout<<endl;
    vector<int>::iterator location = find( eulerian_circuit.begin(), eulerian_circuit.end(), previous_start);;
    // cout<<previous_start<<endl;

    vector<int>::iterator first = eulerian_circuit.begin();
    int index = distance(first, location );
    cout<<"index: "<<index<<endl;
    auto ilist = tour;
    eulerian_circuit.insert(location + 1 , tour.begin(), tour.end());
    // for (int i = 0; i < tour.size(); i++){
    //     cout<<"circuit size: "<< eulerian_circuit.size()<<" "<< "distance: "<<distance(first, location + i )<<endl;
    //     if(eulerian_circuit.size()-1>distance(first, location + i )){
    //         cout<<previous_start<<endl;
    //         cout<<tour.at(i)<<endl;
    //         index = distance(first, location + i + 1);
    //         cout<<index<<endl;
    //         cout<<eulerian_circuit.at(index)<<endl;;
    //         eulerian_circuit.insert(location + i + 1 , tour.at(i));
    //         cout<<"girdi"<<endl;
    //     }
    //     else{
    //         cout<<tour.at(i)<<endl;
    //         eulerian_circuit.push_back(tour.at(i));
    //     }
    // }

    // eulerian_circuit.insert( eulerian_circuit.end(), tour.begin(), tour.end() );
    starting_vertex = find_vertex_with_nonused_edge( eulerian_circuit, edge_vector);
    cout<<"starting_vertex: " << starting_vertex<<endl;
    if( starting_vertex == -1){
        break;
    }
}

ofstream myfile (argv[2]);


for(int i=0; i < eulerian_circuit.size(); i++)
{
auto it1 = next(eulerian_circuit.begin(), i);
if (myfile.is_open()){
    myfile<<*it1<<" ";
}


}







 stop = high_resolution_clock::now(); 
 duration = duration_cast<microseconds>(stop - start);

cout<<endl;
cout << "Time taken by function: "
     << duration.count() / 1000000 << " seconds" << endl; 











	return 0;
}