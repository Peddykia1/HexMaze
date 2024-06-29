#include "HexCell.h"

#include <cmath>

HexCell::HexCell(int Q, int R) {
    q = Q;
    r = R;
    path_dist_from_start = 0;
    for (int i = 0; i < 6; i++) {
        neighbours[i] = nullptr;
        links[i] = nullptr;
    }
}

void HexCell::link(HexCell* cell) {
    int side = get_neighbour_side(cell);
    if (cell == nullptr || side == -1) 
        return;

    links[side] = cell;
    cell->links[cell->get_neighbour_side(this)] = this;
}

int HexCell::get_neighbour_side(const HexCell* cell) const {
     if (cell == nullptr) {
        return -1;
    }
    int dq = cell->q - q;
    int dr = cell->r - r;

    if (dq == 0 && dr == 1) {
        return S;
    } else if (dq == 1 && dr == 0) {
        return SE;
    } else if (dq == 1 && dr == -1) {
        return NE;
    } else if (dq == 0 && dr == -1) {
        return N;
    } else if (dq == -1 && dr == 0) {
        return NW;
    } else if (dq == -1 && dr == 1) {
        return SW;
    } else {
         return -1;
    }
}

bool HexCell::is_linked(const HexCell* cell) const {
    if (cell == nullptr) return false;
    
    int side = get_neighbour_side(cell);
    return (side >= 0 && links[side] == cell);
    // for(int i = 0; i < 6; i++) {
    //     if(links[i] == cell)
    //         return true;
    // }
    // return false;
}

int HexCell::distance(const HexCell* cell) const {
    if (cell == nullptr) 
        return -1;

    int qdif = abs(q - cell->q);
    int rdif = abs(r - cell->r);
    int x = abs(-q - r + cell->q + cell->r);
    int temp = fmax(qdif, rdif);
    return fmax(x, temp);
}

bool HexCell::is_edge() const {
    for(int i = 0; i<6; i++) {
        if (neighbours[i] == nullptr)
            return true;
    }
    return false;
}