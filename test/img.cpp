#include "image.hpp"
#include "color.hpp"
int main()
{
    RGBAImage img(300, 200);

    img(50, 50) = RGBAColor({1.0, 0.0, 0.0, 1.0});
    auto rgb = image_a2s(img);
    save_image(rgb, "img.png");
    return 0;
}
