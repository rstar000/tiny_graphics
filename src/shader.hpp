/*
 * =====================================================================================
 *
 *       Filename:  shader.hpp
 *
 *    Description:  Types for defining a shader function
 *
 *        Version:  1.0
 *        Created:  16.09.2018 16:02:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fedor Novikov (rstar000)
 *   Organization:
 *
 * =====================================================================================
 */
#ifndef SHADER_HPP
#define SHADER_HPP
#include <functional>
#include <variant>
#include <vector>

#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "framebuffer.hpp"

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

struct Vertex;
struct Shader;
using tmath::Vec3;
using tmath::Vec4;
using tmath::Mat3;
using tmath::Mat4;

using VshInput = std::variant<int, float, bool, Vec3, Vec4>;
using Attribute = std::variant<float, Vec3, Vec4>;
using Uniform = std::variant<float, int, Vec3, Vec4, Mat3, Mat4, Framebuffer>;

using AttribVec = std::vector<Attribute>;
using UniformVec = std::vector<Uniform>;

using VertexShader = std::function<Vertex(const AttribVec &attribs,
                                         const UniformVec &uniforms
                                         )>;

using FragmentShader = std::function<Vec4(const AttribVec &input,
                                          const UniformVec &uniforms
                                         )>;

using PartialVSH = std::function<Vertex(const AttribVec &attribs)>;
using PartialFSH = std::function<Vec4(const AttribVec &input)>;


struct Vertex
{
    Vec3 position;
    AttribVec attr;
};

inline PartialVSH apply_vsh_uniform(VertexShader vsh, const UniformVec &uniforms)
{
    using namespace std::placeholders;
    return std::bind(vsh, _1, uniforms);
}

inline PartialFSH apply_fsh_uniform(FragmentShader fsh, const UniformVec &uniforms)
{
    using namespace std::placeholders;
    return std::bind(fsh, _1, uniforms);
}

inline Attribute interp_attr(const Attribute &a1, const Attribute &a2, float t)
{
    Attribute res = 0.0f;
    std::visit(
        [t, &res](auto &&v1, auto &&v2) {
            if constexpr(std::is_same<decltype(v1), decltype(v2)>::value) {
                res = (1.0f - t) * v1 + t * v2;
            }
        }
        , a1, a2);

    return res;
}

inline AttribVec interpolate(const AttribVec &v1, const AttribVec &v2, float t)
{
    using namespace std::placeholders;

    int n = v1.size();
    AttribVec res(n);

    auto interp_t = std::bind(interp_attr, _1, _2, t);
    std::transform(v1.begin(), v1.end(), v2.begin(), res.begin(), interp_t);

    return res;
}

inline Vertex interpolate(const Vertex &v1, const Vertex &v2, float t)
{
    Vec3 new_pos = tmath::interpolate(v1.position, v2.position, t);
    AttribVec new_attr = interpolate(v1.attr, v2.attr, t);
    return Vertex({new_pos, new_attr});
}

struct Shader
{
    VertexShader vert;
    FragmentShader frag;
};

const static VertexShader dummyVSH = [](const AttribVec &attribs, const UniformVec &uniforms)
{
    return Vertex({
        std::get<Vec3>(attribs[0]),
        { }
    });
};

const static FragmentShader dummyFSH = [](const AttribVec &input, const UniformVec &uniforms)
{
    return Vec4({ 1.0f, 1.0f, 1.0f, 1.0f });
};

const Shader DummyShader = { dummyVSH, dummyFSH };

#endif
