/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15.09.2018 13:08:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>
#include "vector.hpp"
using namespace tmath;
int main()
{
    Vec3 a = Vec3({1,2,3});
    std::cout << length(a) << std::endl;
    Vec3 b = Vec3::unaryOp(a, sin);
    Vec3 c = min(a, b)  ;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    c = normalize(c);
    std::cout << abs(c) << " " << length(c) <<  std::endl;
    std::cout << a / 2.0f << std::endl;
    return 0;
}
