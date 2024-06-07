# Dot2Png

``` sh
/*******************************************************************
                    Dot2Png

    1. How to use *.dot files to represent graphs

    2. How to generate images (*.png) from *.dot files

                                             COMP9024 24T2

 *******************************************************************/
``` 

## Undirected Graph

An undirected graph is a data structure consisting of a set of vertices (also called nodes) and a set of edges. 

Each edge connects two nodes and does not have a direction associated with it. 

## Directed Graph

A directed graph is a data structure comprising a set of vertices (nodes) and a set of directed edges (arcs).

Unlike in undirected graphs, the edges in directed graphs have a specific direction associated with them. 

This direction indicates a one-way relationship between vertices. 


## 1 How to download this project in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/Graphs/Dot2Png

Dot2Png$ 

```



## 2 How to generate *.png files from *.dot files


``` sh

Dot2Png$ make

dot -T png images/UndirectedGraph.dot -o images/UndirectedGraph.png 

Dot2Png$ make view

find . -name "*.png" | sort | xargs feh

```

## 3 Undirected Graphs

| Undirected Graph | Undirected Graph (Visited) |  Undirected Graph with Labels |
|:-------------:|:-------------:|:-------------:|
| <img src="images/UndirectedGraph.png" width="100%" height="100%"> |  <img src="images/UndirectedGraphVisited.png" width="100%" height="100%"> | <img src="images/UndirectedGraphWithLabel.png" width="100%" height="100%"> |


## [images/UndirectedGraphVisited.dot](./images/UndirectedGraphVisited.dot)
```sh
graph OurUndirectedGraph {    
"3" -- {"0"}
"0" -- {"2"}
"0" -- {"4"}
"4" -- {"2"}
"2" -- {"5"}
"2" -- {"1"}
"2" -- {"6"}
"1" -- {"5"}
"6" -- {"7"}
"3" [color=red]
"0" [color=red]
}

```

## 4 Directed Graphs

| Directed Graph | Directed Graph (Visited) |  Directed Graph with Labels |
|:-------------:|:-------------:|:-------------:|
| <img src="images/DirectedGraph.png" width="100%" height="100%"> |  <img src="images/DirectedGraphVisited.png" width="100%" height="100%"> | <img src="images/DirectedGraphWithLabel.png" width="100%" height="100%"> |


## [images/DirectedGraphVisited.dot](./images/DirectedGraphVisited.dot)
```sh
graph OurDirectedGraph {    
"3" -> {"0"}
"0" -> {"2"}
"0" -> {"4"}
"4" -> {"2"}
"2" -> {"5"}
"2" -> {"1"}
"2" -> {"6"}
"1" -> {"5"}
"6" -> {"7"}
"3" [color=red]
"0" [color=red]
}

```

