 <div align="center">
  <h1 align="center">Graphs </h1>

  <p align="center">
    <h3>Graphs and stuff like that</h4> 
    <h4>This is a project I wrote as a university project in subject A&DS (Algorithms and Data Structures).</h5>
    <a href="./examples">All Examples</a>
    <br />
  </p>
</div>
 <h3>The task of this program is to load graphs and determine the following parameters:</h4>

1. the degree sequence;
2. the number of components;
3. bipartiteness;
4. the eccentricity of vertices (within the components)
5. vertices colours (consecutive natural numbers from 1) obtained using algorithms:

   a. greedy (the vertex order according to its number)
   
   b. LF method (ties are solved by the vertex number)
   
   c. SLF method (ties are solved by highest vertex degree, and if there is still a tie choose the vertex with the lowest number)
   
6. the number of different C4 subgraphs
7. the number of the graph complement's edges
  
# Input
1. In the first line there's a number k of graphs.

2. The following lines contain k graph data sets.

3. A single data set contains a number n - the graph order - and n lists of neighbours for each vertex. A list contains the number of neighbours (s) and s neighbours IDs.

4. Neighbours IDs are numbers from 1 to n.

# Output
You should print k sets of responses. 
Each set should contain the designated parameters in the order in which the parameters are listed. For point
- 1, 4, 5a, 5b, 5c you should print sequence of n numbers;
- 2, 6, 7 you should print a number;
- 3 you should print a character T or F (true/false).
