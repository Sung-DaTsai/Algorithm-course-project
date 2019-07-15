# Algorithm-course-project
Programming projects for algorithm course, 2019 Spring

PA_HW1: paintBucket
-------------------
In this HW, I use BFS to do the searching.
### Task 1
Given a coordinate, search its neighbors that is the same color with itself. Then fill all these coordinates with
the given color.
### Task 2
Count the number of connected components. That is, the number of areas that coordinates could be separated into by different colors.

PA_HW2: Optimal Wiring Topology for Electromigration Avoidance 
-------------------
In this HW, I use Bellman-Ford algorithm to find out negative weight cycle to optimize total wire area.
Given many sources and sinks, construct a wiring topology to all of them such that the total wire area is minimized and EM is avoided. 

Final_Project: ICCAD 2019 CAD Contest Problem E: Rectilinear Polygon Operations for Physical Design 
-------------------
Given many rectilinear polygons, do merge, clip and split operation.
### Version 1: myversion
All the data structures are designed by myself. Algorithm is based on the paper cited in readme.md in "myversion" directory. 
### Version 2: boost library
Use Boost Library (Polygon) to do all these operations.
