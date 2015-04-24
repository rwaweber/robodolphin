#include <stdio.h>
#include "bool.h"
#include "graph.h"
#include "stack.h"

extern bool processed[];	/* which vertices have been processed */
extern bool discovered[];	/* which vertices have been found */
extern int parent[];		/* discovery relation */

extern int entry_time[];	/* time of vertex entry */
extern int exit_time[];		/* time of vertex exit */

int low[MAXV+1];		/* oldest vertex surely in component of v */
int scc[MAXV+1];		/* strong component number for each vertex */

stack active;			/* active vertices of unassigned component */
int components_found;		/* number of strong components identified */
graph strongForward; 			/* Strong components graph */
graph strongBackward;

void process_vertex_early(int v)
{
  /*printf("entered vertex %d at time %d\n",v, entry_time[v]);*/

  push(&active,v);
  return;

}

void process_vertex_late(int v)
{
// printf("exit vertex %d at time %d\n",v, exit_time[v]);
//  fflush(stdout);
  if (low[v] == v) {
    /* edge (parent[v],v) cuts off scc */
    int outgoing, incoming;
    //THIS MEANS THAT THIS IS AN OUTGOING EDGE FROM AN SCC SO
    // SO parent[v] component points to v's component
    //find SCC OF p[v] and v then put in a SCCpv -> SCCv edge if none exists
    /*printf("strong commonent started backtracking from %d\n",v);*/
    pop_component(v);
//    printf("1");
//    fflush(stdout);
    outgoing =  find_strong_id(parent[v]);//this probably won't work
//    printf("2");
//    fflush(stdout);
    
    incoming = find_strong_id(v);
//    printf("3");
//    fflush(stdout);
   
    //create the node v within the SCC graph with an edge from o to i
    insert_edge( &strongForward, outgoing, incoming, 1 ); /*one with outgoing(parent)->incoming(v)*/
//    printf("4");
//    fflush(stdout);

    insert_edge( &strongBackward, incoming, outgoing, 1 ); /*one with incoming(v)->outgoing(parent)*/
  }
  if (entry_time[low[v]] < entry_time[low[parent[v]]])
    low[parent[v]] = low[v];
  
  return;
}

int find_strong_id(int n){
   if(scc[n] != NULL){
      return(scc[n]);
   }
   return(-1);
}

pop_component(int v)
{
  
  //this method doesn't happen till after a component is found. 
  int t;
  /* vertex placeholder */

  components_found = components_found + 1;
  printf("%d is in component %d \n",v,components_found);
    
  scc[ v ] = components_found;
  while ((t = pop(&active)) != v) {
    scc[ t ] = components_found;
    printf("%d is in component %d with %d \n",t,components_found,v);
  }//this be where we place all the new graphmaking stuff (probs)
}

int process_edge(int x, int y)
{
  int class;		/* edge class */

  class = edge_classification(x,y);
  printf("(%d,%d) class %d\n", x,y,class);
 
  if (class == BACK) {
    if (entry_time[y] < entry_time[ low[x] ] )
      low[x] = y;//place something in the new graph?
    return;
    printf("process BACK (%d,%d) low[%d]=%d  low[%d]=%d\n",x,y,x,low[x],y,low[y]);
  }

  if (class == CROSS) {
    if (scc[y] == -1)	/* component not yet assigned */
      if (entry_time[y] < entry_time[ low[x] ] )
	low[x] = y;//place stuff in the new graph?
    return;
    printf("process CROSS (%d,%d) low[%d]=%d  low[%d]=%d\n",x,y,x,low[x],y,low[y]);
  }
}



void strong_components(graph *g)
{
  int i;				/* counter */
  //graph simple; //newly created graph of strongly connected components

  
  
  for (i=1; i<=(g->nvertices); i++)
    {
      low[i] = i;
      scc[i] = -1;
    }
  

  components_found = 0;

  init_stack(&active);
  initialize_search(&g);

  for (i=1; i<=(g->nvertices); i++)
    if (discovered[i] == FALSE) {
      dfs(g,i);
      
      /*pop_component(i);*/
    }
}


/* ########################## User created methods ########################## */

//Identify single, cyclic components_found, and then print those same components
/*int id_one_component(graph *g)
{
  int i;
  if (components_found == 1)
    {
      printf("Sources:");
      for(i = 1; i<= (g->nvertices); i++)
	{
	  printf("%d", i );
	}
      printf("\n");
      
      printf("Sponges:");
      for(i = 1; i<= (g->nvertices); i++)
	{
	  printf("%d", i );
	}
      printf("\n");
      return 1;
    }
  else
    return 0;
}
*/

// Identify multiple component graphs, and then determine their sinks and sponges
int id_mul_component(graph *g)
{
  int i, flag = 0; //flag is to indicate if it
  for(i = 1; i <= (g->nvertices); i++)
    {
      
    }
}


      // End user created methods
int main(int argc, char* argv[])
{
  graph g;
  int i;

  read_graph(&g,TRUE);
//  print_graph(&g);

  strong_components(&g);
  print_graph(&strongForward);
  print_graph(&strongBackward);
  //if(id_one_component(&g))
  //  printf("All of the above vertices are Sources and Sponges because there is only one component.\n");
  
      
  return 0;
}


/*	strong.c
	Identify strongly connected components in a graph

	by: Steven Skiena
	begun: March 27, 2002
*/

/*
  Copyright 2003 by Steven S. Skiena; all rights reserved. 

  Permission is granted for use in non-commerical applications
  provided this copyright notice remains intact and unchanged.

  This program appears in my book:

  "Programming Challenges: The Programming Contest Training Manual"
  by Steven Skiena and Miguel Revilla, Springer-Verlag, New York 2003.

  See our website www.programming-challenges.com for additional information.

  This book can be ordered from Amazon.com at

  http://www.amazon.com/exec/obidos/ASIN/0387001638/thealgorithmrepo/

*/


