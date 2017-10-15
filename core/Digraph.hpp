// Digraph.hpp
//
// ICS 46 Spring 2017
// Project #4: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph.  The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <limits>
#include <queue> 
#include <algorithm>
 



// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException
{
public:
    DigraphException(const std::string& reason): reason_{reason} { }

    std::string reason() const { return reason_; }

private:
    std::string reason_;
};


// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a template
// struct.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a template struct.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d);

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph();

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d);

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the precedessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.

  //  DigraphVertex* Vertex; //inlude list of edges from this vertex 
  //  DigraphEdge* Edge; 
  //  int vertex_count; 
 //   int edges_count; 
    std::map<int, DigraphVertex<VertexInfo,EdgeInfo>> GraphMap; 

    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.
};



// You'll need to define the member functions of your Digraph class
// template here.

template <typename VertexInfo, typename EdgeInfo>   
Digraph<VertexInfo,EdgeInfo>::Digraph()    
{    
}


// The copy constructor initializes a new Digraph to be a deep copy            // should i keep those comments? 
// of another one (i.e., any change to the copy will not affect the
// original).
template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo,EdgeInfo>::Digraph(const Digraph& d)
{
   GraphMap.clear();                                                           //do i need to clear it ? 
   GraphMap.insert(d.GraphMap.begin(),d.GraphMap.end());
}


//the move constructor initializes a new Digraph from an expiring one. 
template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo,EdgeInfo>::Digraph(Digraph&& d)
{
    GraphMap.clear();
    std::swap(GraphMap,d.GraphMap);
}


//The destructor deallocates any memory associated with the Digraph.
template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo,EdgeInfo>::~Digraph()                                            // do i need it? 
{
    GraphMap.clear(); 
}


// The assignment operator assigns the contents of the given Digraph
// into "this" Digraph, with "this" Digraph becoming a separate, deep
// copy of the contents of the given one (i.e., any change made to
// "this" Digraph afterward will not affect the other).
template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo,EdgeInfo>& Digraph<VertexInfo,EdgeInfo>:: operator=(const Digraph& d)
{
    GraphMap.clear();
    GraphMap.insert(d.GraphMap.begin(),d.GraphMap.end());
    return *this; 
}


//The move assignment operator assigns the contents of an expiring Digraph 
//into "this" Digraph.
template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo,EdgeInfo>& Digraph<VertexInfo,EdgeInfo>:: operator=(Digraph&& d)
{   
    GraphMap.clear();
    std::swap(GraphMap,d.GraphMap);
    return *this; 
}


//vertices () returns a std::vector containing the vertex numbers of 
// every vertex in this digraph. 
template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo,EdgeInfo>::vertices() const                    
{                                   
    std::vector<int> vertex_list;
    for (auto element:GraphMap)
    {
        vertex_list.push_back(element.first);
    }
    return vertex_list;
}


//edges() returns a std:: vector of std:: pair, in which each pair 
//contains the "from" and "to" vertex number of an edge in this Digraph. 
// All edges are included in the std:: vector.
template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int,int>> Digraph<VertexInfo,EdgeInfo>:: edges() const
{
    std::vector<std::pair<int,int>> edges_list; 
    for(auto element:GraphMap)
    {
        for(auto edge_element:element.second.edges) 
        {
            edges_list.push_back(std::pair<int,int>(edge_element.fromVertex,edge_element.toVertex));
        }
    }
    return edges_list;
}


//this overload of edges() return a std:: vector of std::pairs, in which each pair contains the 
//"from" and "to" vertex numbers of an edge in this Digraph. Only edges outgoing fro the given vertex
// number are included in the std:: vector. 
// If the given vertex does not exist, a DigraphException is thrown instead. 
template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int,int>> Digraph<VertexInfo,EdgeInfo>:: edges(int vertex) const
{
    std::vector<std::pair<int,int>> edges_list;
    DigraphException Exception("Given vertex does not exist.");
    if(GraphMap.find(vertex)==GraphMap.end())
    {
        throw Exception;
    }
    else
    { 
        for(auto element:GraphMap.at(vertex).edges)
        {
            edges_list.push_back(std::pair<int,int>(element.fromVertex,element.toVertex));
        }
    } 
    return edges_list;
}


// vertexInfo() returns the VertexInfo object belonging to the vertex
// with the given vertex number.  If that vertex does not exist, a
// DigraphException is thrown instead.
template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo,EdgeInfo>:: vertexInfo(int vertex) const
{
    DigraphException Exception("Vertex does not exist.");
    if(GraphMap.find(vertex)!=GraphMap.end())
    {
        return GraphMap.at(vertex).vinfo;
    }

    else
        throw Exception;
}


// edgeInfo() returns the EdgeInfo object belonging to the edge
// with the given "from" and "to" vertex numbers.  If either of those
// vertices does not exist *or* if the edge does not exist, a
// DigraphException is thrown instead.
template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo,EdgeInfo>:: edgeInfo(int fromVertex, int toVertex) const
{
    DigraphException Exception("Edge does not exist or one of those vertices does not exist");
    for(auto element: GraphMap.at(fromVertex).edges)
    {
        if(toVertex==element.toVertex)
        {
            return element.einfo; 
        }
    }
    throw Exception;
}


// addVertex() adds a vertex to the Digraph with the given vertex
// number and VertexInfo object.  If there is already a vertex in
// the graph with the given vertex number, a DigraphException is
// thrown instead.
template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo,EdgeInfo>:: addVertex(int vertex, const VertexInfo& vinfo)
{
    DigraphException Exception("There is already a vertex in the graph with the given vertex number");
    if(GraphMap.find(vertex)!=GraphMap.end())
    {
        throw Exception; 
    }
    else
    {
        DigraphVertex<VertexInfo,EdgeInfo>* Graph_Vertex= new DigraphVertex<VertexInfo,EdgeInfo>();
        Graph_Vertex->vinfo=vinfo;  
        GraphMap[vertex]=*Graph_Vertex;   
        delete Graph_Vertex;  
    } 
}


//addEdge() adds an edge to the Digraph pointing from the given "from" vertex number 
//to the given "to" vertex number, and associates with the given EdgeInfo objects with it
// if one of the vertices does not exist *or* if the same edge is already present in the graph,
// a DigraphException is thrown instead. 
template <typename VertexInfo, typename EdgeInfo>  //EdgeInfo is a roadsegment 
void Digraph<VertexInfo,EdgeInfo>:: addEdge(int fromVertex,int toVertex,const EdgeInfo& einfo)          //check this function 
{
    //problem: do we need to check if, if not exist what we should do ? 
    //exception 
    DigraphException Exception1("One of the vertices does not exist");
    DigraphException Exception2("the same edge is already present in the graph");
    std::vector<std::pair<int,int>> edges_fromVertex=edges(fromVertex);
    if((GraphMap.find(fromVertex)==GraphMap.end())||(GraphMap.find(toVertex)==GraphMap.end()))
    {
        throw Exception1;
    }
    bool edgefound=find(edges_fromVertex.begin(),edges_fromVertex.end(),std::pair<int,int>(fromVertex,toVertex))!=edges_fromVertex.end();
    if(edgefound)
    {
        throw Exception2;
    }
    else
    { 
        DigraphEdge<EdgeInfo>* Graph_Edge= new DigraphEdge<EdgeInfo>();  
        Graph_Edge->fromVertex=fromVertex;
        Graph_Edge->toVertex=toVertex;
        Graph_Edge->einfo=einfo;
        GraphMap.at(fromVertex).edges.push_back(*Graph_Edge);
        delete Graph_Edge;
    }
}


//removeVertex() removes the vertex ( and all of its incoming and outoing edges)
// with the given vertex number from the Dgiraph. 
// If the vertex does not exist alrady, a DigraphException is thrown instead. 
template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo,EdgeInfo>:: removeVertex(int vertex)
{
    //exception
    DigraphException Exception("The vertex does not exist already.");
    if(GraphMap.find(vertex)==GraphMap.end())
    {
        throw Exception;
    }
    else
    {
        std::vector<DigraphEdge<EdgeInfo>> remove_list; 
        if(GraphMap.find(vertex)!=GraphMap.end())
        {
            GraphMap.erase(vertex);
        }

        for(auto element:GraphMap)
        {
            for(auto sub_element:element.second.edges)
            {
                sub_element=element.second.edges.back();
                element.second.edges.pop_back();
                if(sub_element.toVertex!=vertex)
                {
                    element.second.edges.push_front(sub_element);
                }
            }
        }
    }
}


//removeEdge() removes the edge pointing from the given "from" vertex number to the 
// given "to" the given "to" vertex number from the Digraph. 
// if either of these vertices does not exist *or* if the edge is not already present in the graph,
// a DigraphException is thrown insead. 
template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo,EdgeInfo>:: removeEdge(int fromVertex,int toVertex)
{
    DigraphException Exception1("One of the vertices does not exist");
    DigraphException Exception2("the edge is not already present in the graph");
    std::vector<std::pair<int,int>> edges_fromVertex=edges(fromVertex);
    if((GraphMap.find(fromVertex)==GraphMap.end())||(GraphMap.find(toVertex)==GraphMap.end()))
    {
        throw Exception1;
    }
    bool edgefound=find(edges_fromVertex.begin(),edges_fromVertex.end(),std::pair<int,int>(fromVertex,toVertex))!=edges_fromVertex.end();
    if(!edgefound)
    {
        throw Exception2;
    }
    else
    {
        for(auto element:GraphMap.at(fromVertex).edges)
        {   
            element=GraphMap.at(fromVertex).edges.back(); 
            GraphMap.at(fromVertex).edges.pop_back(); 
            if(element.toVertex!=toVertex)
            {
                GraphMap.at(fromVertex).edges.push_back(element);
            }
        }
    }
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo,EdgeInfo>:: vertexCount() const
{
    return GraphMap.size();
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo,EdgeInfo>:: edgeCount() const
{
    std::vector<std::pair<int,int>> edgeslist=edges();
    return edgeslist.size(); 
}


// This overload of edgeCount() returns the number of edges in the graph that
// are outgoing from the given vertex number. 
//If the given vertex does not exist, a Digraph Exception is thrown isntead
template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo,EdgeInfo>:: edgeCount(int vertex) const
{
    DigraphException Exception("the given vertex does not exist.");
    if(GraphMap.find(vertex)!=GraphMap.end())
    {
        std::vector<std::pair<int,int>> edgeslist=edges(vertex);
        return edgeslist.size(); 
    }
    else
        throw Exception;
}


// isstronglyConnected() return true if the Digraph is strongly connected
//(i.e., every vertex is reachable from every other), false otherwise. 
template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo,EdgeInfo>:: isStronglyConnected() const
{
    std::map<int,bool> visitedmap;
    for(auto key:GraphMap) 
    {
        visitedmap[key.first]=false;
    }

    for(auto key:GraphMap)
    {
        std::map<int,bool> visitmap_copy=visitedmap; 
        int vertex_count=0; 
        for(auto element:key.second.edges)
        {
            if(visitmap_copy[element.toVertex]==false)
            {
                vertex_count++;
                visitmap_copy[element.toVertex]=true; 
            }
        }
        if(vertex_count!=vertexCount())
        {
            return false;
        }
    }
    return true; 
}


/*findShortestPaths() takes a start vertex number and a function 
that takes an edgeInfo ojbect and determines an edge weight. 
It uses Digkstra's Shortest Path Algorithm to determine the shortest paths from the start vertex to every other vertex in the graph.
The result :
is returned as a std:: map<int, int> where the keys are vertex numbers and teh value associated with each key k is the 
precedessor of that vertex chosen by the algorithm. 
For any vertex without a predecessor (e.g., a vertex that was never rached, or the start vertex itself) 
the value is simply a copy of the key. */
template <typename VertexInfo, typename EdgeInfo>
std::map<int,int> Digraph<VertexInfo,EdgeInfo>:: findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{ 
    std::map<int,bool> kmap; 
    std::map<int,double> dmap; 
    std::map<int,int> pmap; 
    for(auto key:GraphMap)
    {
            kmap[key.first]=false; 
            dmap[key.first]=std::numeric_limits<double>::max(); 
            pmap[key.first]=key.first;
    }
    dmap[startVertex]=0.0; 

    std::priority_queue<double,std::vector<double>,std::greater<double>> distance_queue; 
    distance_queue.push(dmap[startVertex]);

    while(!distance_queue.empty())
    {
        int smallkey;
        for(auto element:dmap)
        {
            if(element.second==distance_queue.top())
            {
                smallkey=element.first; 
            }
        }

        distance_queue.pop(); 

        if(kmap[smallkey]==false)
        {
            kmap[smallkey]=true; 
        
            for(auto edge:GraphMap.at(smallkey).edges)
            {
                if(dmap.at(edge.toVertex)>dmap.at(smallkey)+edgeWeightFunc(edge.einfo))
                {
                    dmap[edge.toVertex]=dmap[smallkey]+edgeWeightFunc(edge.einfo);
                    pmap[edge.toVertex]=smallkey;
                    distance_queue.push(dmap.at(edge.toVertex));
                }
            }

        }
    }
    return pmap; 
}

#endif // DIGRAPH_HPP

