# FordFulkerson's algorithm

``` sh
/*******************************************************************
                    FordFulkerson's algorithm 

    1.  How to use the Ford-Fulkerson algorithm to calculate the maximum flow.




                                             COMP9024

 *******************************************************************/
``` 

## Introduction

If the reversed edges are not included in the residual graph, we might end up with a scenario where the local maximum flow along a path doesn't contribute to a larger overall flow in the network.

| Local maximum flow 3 on the path Source->Node1->Node3->Node4->Sink, but the flow capacity of the edge Node3->Sink has been wasted. | 
|:-------------:|
| <img src="diagrams/LocalMaximum.png" width="100%" height="100%"> | 

| Local maximum flow 3 on the path Source->Node2->Node1->Node4->Sink, but the flow capacity of the edge Node3->Sink has been wasted. | 
|:-------------:|
| <img src="diagrams/LocalMaximum2.png" width="100%" height="100%"> | 

The sum of local maximums (i.e., 3+3) might be not the global maximum (i.e., 8).

| Global maximum flow 8. | 
|:-------------:|
| <img src="diagrams/GlobalMaximum.png" width="100%" height="100%"> | 

Reversed edges allow for the possibility of "undoing" or "reversing" flow in future iterations.

Augmented paths can contain reversed edges (the red edges in the following diagrams) added during the algorithm.

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
| <img src="images/FordFulkersonMF_0000.png" width="80%" height="80%"> |  

| Path 1: Source -> Node1 -> Node3 -> Node4 -> Sink, Flow: 3 | Update edge weights | Hide reversed edges |
|:-------------:|:-------------:|:-------------:|
| <img src="images/FordFulkersonMF_0001.png" width="80%" height="80%"> |  <img src="images/FordFulkersonMF_0002.png" width="80%" height="80%"> |<img src="images/FordFulkersonMF_0003.png" width="80%" height="80%"> |

| Path 2: Source -> Node2 -> Node1 -> Node4 -> Node3 -> Sink, Flow: 2 |Update edge weights | Hide reversed edges |
|:-------------:|:-------------:|:-------------:|
| <img src="images/FordFulkersonMF_0004.png" width="80%" height="80%"> | <img src="images/FordFulkersonMF_0005.png" width="80%" height="80%"> |<img src="images/FordFulkersonMF_0006.png" width="80%" height="80%"> |

| Path 3: Source -> Node2 -> Node1 -> Node4 -> Sink, Flow: 2 | Update edge weights | Hide reversed edges |
|:-------------:|:-------------:|:-------------:|
| <img src="images/FordFulkersonMF_0007.png" width="80%" height="80%"> | <img src="images/FordFulkersonMF_0008.png" width="80%" height="80%"> | <img src="images/FordFulkersonMF_0009.png" width="80%" height="80%"> |

| Path 4: Source -> Node2 -> Node4 -> Sink, Flow: 1 |Update edge weights | Hide reversed edges |
|:-------------:|:-------------:|:-------------:|
| <img src="images/FordFulkersonMF_0010.png" width="80%" height="80%"> | <img src="images/FordFulkersonMF_0011.png" width="80%" height="80%"> | <img src="images/FordFulkersonMF_0012.png" width="80%" height="80%"> |


### Output

```sh
Path: Source -> Node1 -> Node3 -> Node4 -> Sink, Flow: 3
Path: Source -> Node2 -> Node1 -> Node4 -> Node3 -> Sink, Flow: 2
Path: Source -> Node2 -> Node1 -> Node4 -> Sink, Flow: 2
Path: Source -> Node2 -> Node4 -> Sink, Flow: 1
Max flow is 8 

```

