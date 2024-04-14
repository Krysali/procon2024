#include "types.h"

std::vector<Stencil> GenerateStencils() {
    std::vector<Stencil> stencils;

    for (int size = 1; size <= MAX_DIMENSION; size *= 2) {
        // Type I Stencil: All cells are 1
        Stencil type1_stencil;
        type1_stencil.width = size;
        type1_stencil.height = size;
        type1_stencil.cells.resize(size, std::vector<bool>(size, true));
        stencils.push_back(type1_stencil);

        // Type II Stencil: Even rows are 1, odd rows are 0
        Stencil type2_stencil;
        type2_stencil.width = size;
        type2_stencil.height = size;
        type2_stencil.cells.resize(size);
        if (size != 1) {
            for (int i = 0; i < size; ++i) {
                type2_stencil.cells[i].resize(size);
                for (int j = 0; j < size; ++j) {
                    type2_stencil.cells[i][j] = (i % 2 == 0);
                }
            }
            stencils.push_back(type2_stencil);
        }
        // Type III Stencil: Even columns are 1, odd columns are 0
        Stencil type3_stencil;
        type3_stencil.width = size;
        type3_stencil.height = size;
        type3_stencil.cells.resize(size);
        if (size != 1) {
            for (int i = 0; i < size; ++i) {
                type3_stencil.cells[i].resize(size);
                for (int j = 0; j < size; ++j) {
                    type3_stencil.cells[i][j] = (j % 2 == 0);
                }
            }
        stencils.push_back(type3_stencil);
        }
    }

    return stencils;
}