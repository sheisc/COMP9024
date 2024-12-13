# Prim's algorithm

## Introduction

Prim's algorithm finds the Minimum Spanning Tree (MST) of a connected, undirected graph.

It can reuse a lot of the structure from [Dijkstra's algorithm](../Dijkstra/README.md). 

Both algorithms share some similarities in their approach, and this makes it easier to implement Prim's algorithm by leveraging Dijkstra's code. 

### Key differences.

- Dijkstra's Algorithm: 

    Accumulates the shortest path distances to all nodes from a source node. 

- Prim's MST Algorithm: 
  
    It expands the MST by selecting the shortest edge connecting the already selected tree to new nodes.

| Dijkstra: the 6 in (Node 2: 6) is the shortest distance accumulated | Prim: the -5 in (Node 5: -5) is the weight of the shortest edge |
|:-------------:|:-------------:|
| <img src="diagrams/DijkstraDirected_0005.png" width="50%" height="50%"> |   <img src="diagrams/PrimMST_0004.png" width="50%" height="50%"> |

###  make view

**Ensure that you have executed 'make' and './main' before 'make view'.**


```sh
PrimMST$ make view
find ./images -name "*.png" | sort | xargs feh -g 1024x768 &
```


| Initial | 
|:-------------:|
| <img src="images/PrimMST_0000.png" width="50%" height="50%"> |  


| Step 1 |  |
|:-------------:|:-------------:|
| <img src="images/PrimMST_0001.png" width="50%" height="50%"> |  <img src="images/PrimMST_0002.png" width="50%" height="50%"> | 


| Step 2 |  |
|:-------------:|:-------------:|
| <img src="images/PrimMST_0003.png" width="50%" height="50%"> |  <img src="images/PrimMST_0004.png" width="50%" height="50%"> | 


|Step 3 |  |
|:-------------:|:-------------:|
|<img src="images/PrimMST_0005.png" width="50%" height="50%"> | <img src="images/PrimMST_0006.png" width="50%" height="50%">  | 


|Step 4 | |
|:-------------:|:-------------:|
| <img src="images/PrimMST_0007.png" width="50%" height="50%"> | | 


| Step 5 | |
|:-------------:|:-------------:|
| <img src="images/PrimMST_0008.png" width="50%" height="50%">  |  <img src="images/PrimMST_0009.png" width="50%" height="50%">| 

| Step 6 | |
|:-------------:|:-------------:|
| <img src="images/PrimMST_0010.png" width="50%" height="50%">  |   | 

| Step 7 ||
|:-------------:|:-------------:|
| <img src="images/PrimMST_0011.png" width="50%" height="50%">  | <img src="images/PrimMST_0012.png" width="50%" height="50%">| 


| Step 8 |  |
|:-------------:|:-------------:|
| <img src="images/PrimMST_0013.png" width="50%" height="50%"> |  | 


