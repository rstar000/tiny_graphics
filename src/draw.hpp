#ifndef DRAW_HPP
#define DRAW_HPP

#include "model.hpp"
#include "shader.hpp"
#include "rasterizer.hpp"



void draw_model(const Model &model, Shader shader, Framebuffer &fbo, const UniformVec &uni)
{
    PartialFSH fsh = apply_fsh_uniform(shader.frag, uni);
    PartialVSH vsh = apply_vsh_uniform(shader.vert, uni);
    
    std::vector<Vertex> vertices(model.v_attrs.size());

    std::transform(model.v_attrs.begin(), model.v_attrs.end(), vertices.begin(), vsh);

    for(auto &idx : model.indeces) {
        unsigned v0, v1, v2;
        std::tie(v0, v1, v2) = idx;
        Triangle tri = std::forward_as_tuple(vertices[v0], vertices[v1], vertices[v2]);
        rasterize_triangle(tri, fsh, fbo);
    }
}

#endif
