/*
 * =====================================================================================
 *
 *       Filename:  model.hpp
 *
 *    Description:  Description of various models.
 *
 *        Version:  1.0
 *        Created:  19.09.2018 15:12:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fedor Novikov (rstar000)
 *   Organization
 *
 * =====================================================================================
 */

#include "rasterizer.hpp"
#include "shader.hpp"

using VertexVec = std::vector<Vertex>;

using TriIndeces = std::tuple<unsigned, unsigned, unsigned>;

struct Model
{
    std::vector<AttribVec> v_attrs;
    std::vector<TriIndeces> indeces; 
};



