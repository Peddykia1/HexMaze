#include "HexBoard.h"

#include <iostream>

#include "queue.h"
#include "stack.h"

/**
     * Constructor. Creates a board with given radius, creating new HexCells and inserting them into this->cells by calling createBoard()
     * and setting cells to have the appropriate neighbours by calling configure_cells(). The default start_coord is (0, 0).
     * @note There is no maze yet! The board is just a beehive for now.
     * @param radius The radius of the board
     */

HexBoard::HexBoard(int radius) {
    this->radius = radius;
    start_coord.first = 0;
    start_coord.second = 0;
    longest_path_dist = 0;
    create_board();
    configure_cells(); 
}

void HexBoard::create_board() {
    for(int q = -radius+1; q < radius; ++q){
      for(int r = max(-radius, -q - radius) + 1; r < min(radius, -q + radius); ++r){
        HexCell* cell = new HexCell(q,r);
        cells[{q, r}] = cell;
      }
    }
}

void HexBoard::configure_cells() {
    for (auto&kv : cells) {
      pair<int, int> coord = kv.first;
      HexCell* current_cell = kv.second; 
      vector<pair<int, int>> neighbourCords = get_neigh_coords(coord);
      // for (auto& nc :neighbourCords) {
        for (int i = 0; i<6; i++ ) {
          auto lookup = cells.find(neighbourCords[i]);
          if (lookup != cells.end()) {
            HexCell* neigbour_cell = lookup->second;
            int side = current_cell->get_neighbour_side(neigbour_cell); 
            current_cell->neighbours[side] = neigbour_cell; 
          }
        }
    }
}
   /**
      * Traverses the board via dfs to generate the maze, updating and keeping track of:
      * A. The length of the longest path to an edge in the maze

      * The algorithm goes as follows:
      * 1. Add the start_coord to the stack, and mark as visited
      * 2. While the stack is not empty do:

      *   a. Peek the top of the stack (call it curr_cell)
      *   c. Use chooseNeighbour to choose the neighbour to visit
      *   d. If the neighbour exists, for that neighbour (call it neigh_cell) do:

      *     i. Link curr_cell with neigh_cell
      *     ii. Set neigh_coord to be visited
      *     iii. Update the neigh_cell's path distance
      *     iv. If necessary, update length of longest path so far (longest_path_dist)
      *     v. Update the end coordinate only if its path distance to start is strictly greater
      *     vi. Push neigh_cell into the stack

      *   e. Otherwise (if chooseNeighbour returns nullptr), pop the stack.

      * @param start_coord The {q, r} coordinate of the start of the maze.
      * @param branch_prob Specifies how often it should "branch out" when choosing neighbours.
      *                    A small branch_prob means neighbours of the same radius (same dist from the centre) are preferred
      *                    A large branch_prob means neighbours of a different radius (diff dist from the centre) are preferred
      * REQUIRE: create_board() is already called
      */
void HexBoard::generate_maze(pair<int, int> start_coord, double branch_prob) {
    int best_path_so_far = 0;
    this->start_coord = start_coord;
    HexCell* start_cell = cells[start_coord];
    map<pair<int, int>, bool> visited;
    Stack<pair<int,int>> stack; 
    stack.push(start_coord);
    visited[start_coord] = true;
    while(!stack.isEmpty()){
      pair<int,int> curr_coord = stack.peek();
      HexCell* curr_cell = cells[curr_coord];
      HexCell* neigh_cell = chooseNeighbour(curr_cell, visited, branch_prob); 
      if (neigh_cell != nullptr) {
        curr_cell->link(neigh_cell);
        pair<int,int> ncc = {neigh_cell->q, neigh_cell->r}; // ncc = neigh_cell_coordinate
        visited[ncc] = true;
        neigh_cell->path_dist_from_start = curr_cell->path_dist_from_start+1;
        if (neigh_cell->path_dist_from_start > longest_path_dist ){
          longest_path_dist = neigh_cell->path_dist_from_start;
        }
        if (neigh_cell->path_dist_from_start > best_path_so_far && neigh_cell->is_edge() ){
            best_path_so_far = neigh_cell->path_dist_from_start;
            end_coord.first =  neigh_cell->q;
            end_coord.second =  neigh_cell->r;
        }
        stack.push(ncc);
      } else {
        stack.pop();
      }
    }
}

void HexBoard::solve_maze() {
    Stack<pair<int,int>> stack;
    map<pair<int, int>, bool> visited;
    stack.push(this->start_coord);
    visited[start_coord] = true;
    while(!stack.isEmpty()){
      pair<int,int> curr_coord = stack.peek();
      HexCell* curr_cell = cells[curr_coord];
      //vector<pair<int, int>> neigh_cords = get_neigh_coords(curr_coord);
      bool pop_checker = true;
      //for (unsigned long i = 0; i < neigh_cords.size(); i++ ){
        for (auto &neigh_cell : curr_cell->neighbours){
            if (!neigh_cell)
                continue;

            pair<int, int> neigh_coord = {neigh_cell->q, neigh_cell->r};
        
        if (curr_cell->is_linked(cells[neigh_coord]) && visited[neigh_coord] == false){
          pop_checker = false;
          stack.push(neigh_coord);
          visited[neigh_coord] = true;
          if (neigh_coord == this->end_coord){
            Stack<pair<int,int>> temp_stack;
            while (!stack.isEmpty()){
              temp_stack.push(stack.pop());
            }
            while (!temp_stack.isEmpty()) {
              solution.push_back(temp_stack.pop());
            }
          }
          break;
        }
      }
      if (pop_checker == true)
        stack.pop();
    }
}