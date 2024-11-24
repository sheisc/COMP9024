# Edmonds-Karp's algorithm

``` sh
/*******************************************************************
                    Edmonds-Karp's algorithm 

    1.  How to use the Edmonds-Karp algorithm to calculate the maximum flow.




                                             COMP9024

 *******************************************************************/
``` 

### Edmonds-Karp vs. Ford-Fulkerson

Think about augmenting paths as highways:

- BFS finds the shortest highway, one with fewer exits, fewer turns, and fewer traffic lights—allowing for the most direct travel from start to end.

- DFS, on the other hand, might find a highway that is longer or more complicated. Even though it might eventually reach the destination, it might take much longer and use unnecessary roads.


By using BFS to find shorter paths, the Edmonds-Karp algorithm ensures that each augmenting path has a meaningful impact on the flow, reducing the total number of iterations needed to reach the maximum flow.

Summary:

- BFS always finds the shortest augmenting path in terms of edges: It explores the graph in layers and finds the path with the fewest edges.

- Significant flow contribution: Shorter paths contribute more efficiently to the total flow, meaning each augmenting path makes better progress toward the maximum flow.

- Fewer iterations: Since BFS ensures that each path makes effective progress, the algorithm converges faster, leading to a polynomial time complexity.

This is why BFS is the key to ensuring Edmonds-Karp runs efficiently and avoids the inefficiencies that can arise when using DFS in the Ford-Fulkerson method.

###  make view

**Ensure that you have executed 'make' and './main' before 'make view'.**


```sh
Kruskal$ make view
find ./images -name "*.png" | sort | xargs feh -g 1024x768 &
```

**Click on the window of 'feh' or use your mouse scroll wheel to view images**.

Here, **feh** is an image viewer available in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/).


| Initial | 
|:-------------:|
| <img src="images/EdmondsKarpMF_0000.png" width="80%" height="80%"> |  

| Path 1: Source -> Node1 -> Node3 -> Sink, Flow: 2 | Update edge weights | Hide reversed edges |
|:-------------:|:-------------:|:-------------:|
| <img src="images/EdmondsKarpMF_0001.png" width="80%" height="80%"> |  <img src="images/EdmondsKarpMF_0002.png" width="80%" height="80%"> |<img src="images/EdmondsKarpMF_0003.png" width="80%" height="80%"> |

| Path 2: Source -> Node1 -> Node4 -> Sink, Flow: 1 |Update edge weights | Hide reversed edges |
|:-------------:|:-------------:|:-------------:|
| <img src="images/EdmondsKarpMF_0004.png" width="80%" height="80%"> | <img src="images/EdmondsKarpMF_0005.png" width="80%" height="80%"> |<img src="images/EdmondsKarpMF_0006.png" width="80%" height="80%"> |

| Path 3: Source -> Node2 -> Node4 -> Sink, Flow: 3 | Update edge weights | Hide reversed edges |
|:-------------:|:-------------:|:-------------:|
| <img src="images/EdmondsKarpMF_0007.png" width="80%" height="80%"> | <img src="images/EdmondsKarpMF_0008.png" width="80%" height="80%"> | <img src="images/EdmondsKarpMF_0009.png" width="80%" height="80%"> |

| Path 4: Source -> Node2 -> Node1 -> Node4 -> Sink, Flow: 2 |Update edge weights | Hide reversed edges |
|:-------------:|:-------------:|:-------------:|
| <img src="images/EdmondsKarpMF_0010.png" width="80%" height="80%"> | <img src="images/EdmondsKarpMF_0011.png" width="80%" height="80%"> | <img src="images/EdmondsKarpMF_0012.png" width="80%" height="80%"> |


### Output

```sh
Path: Source -> Node1 -> Node3 -> Sink, Flow: 2
Path: Source -> Node1 -> Node4 -> Sink, Flow: 1
Path: Source -> Node2 -> Node4 -> Sink, Flow: 3
Path: Source -> Node2 -> Node1 -> Node4 -> Sink, Flow: 2

Max flow is 8 
```
 