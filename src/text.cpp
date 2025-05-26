#include <text.hpp>

void Text::draw() const {
    DrawText(text_.c_str(), pos_.x, pos_.y, font_size_, BLACK);
}
