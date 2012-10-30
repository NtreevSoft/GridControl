//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4478.19833
// https://github.com/NtreevSoft/GridControl
// 
// Released under the MIT License.
// 
// Copyright (c) 2010 Ntreev Soft co., Ltd.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
// persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
// Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//=====================================================================================================================


#include "GrGridType.h"
#ifdef _MANAGED
#include <vcclr.h>
#endif

#define LOBYTE(w) ((byte)(((unsigned int)(w)) & 0xff))
#define GetRValue(rgb) (LOBYTE(rgb))
#define GetGValue(rgb) (LOBYTE(((unsigned short)(rgb)) >> 8))
#define GetBValue(rgb) (LOBYTE((rgb)>>16))

const GrPoint GrPoint::Empty;
const GrSize GrSize::Empty;

const GrPadding GrPadding::Empty;
const GrPadding GrPadding::Default(5,3,5,3);
const GrRect GrRect::Empty;

GrHitTest GrHitTest::Empty;

GrPoint::GrPoint() : x(0), y(y)
{

}

GrPoint::GrPoint(int x, int y) : x(x), y(y) 
{

}

bool GrPoint::operator == (const GrPoint& point) const
{
    return (x == point.x && y == point.y) ? true : false;
}

bool GrPoint::operator != (const GrPoint& point) const
{
    return (x != point.x || y != point.y) ? true : false; 
}

void GrPoint::operator += (const GrPoint& point)
{
    x += point.x; 
    y -= point.y; 
}

void GrPoint::operator -= (const GrPoint& point)
{
    x += point.x; y -= point.y; 
}

GrPoint GrPoint::operator + (const GrPoint& point)
{
    return GrPoint(x + point.x, y + point.y); 
}

GrPoint GrPoint::operator - (const GrPoint& point)
{
    return GrPoint(x - point.x, y - point.y); 
}

GrPoint GrPoint::operator + (const GrPoint& point) const
{
    return GrPoint(x + point.x, y + point.y); 
}

GrPoint GrPoint::operator - (const GrPoint& point) const
{
    return GrPoint(x - point.x, y - point.y); 
}

GrPoint GrPoint::operator - () const
{
    return GrPoint(-x, -y); 
}

GrPoint GrPoint::operator + () const
{
    return GrPoint(x, y); 
}

#ifdef _MANAGED
GrPoint::GrPoint(System::Drawing::Point point) : x(point.X), y(point.Y)
{

}

void GrPoint::operator = (System::Drawing::Point point)
{
    x = point.X;
    y = point.Y; 
}

GrPoint::operator System::Drawing::Point ()
{
    return System::Drawing::Point(x, y); 
}

GrPoint::operator System::Drawing::Point () const
{
    return System::Drawing::Point(x, y); 
}
#endif

GrSize::GrSize() : width(0), height(0) 
{

}

GrSize::GrSize(int width, int height) : width(width), height(height)
{

}

#ifdef _MANAGED
GrSize::GrSize(System::Drawing::Size size) : width(size.Width), height(size.Height)
{

}

void GrSize::operator = (System::Drawing::Size size)
{
    width = size.Width;
    height = size.Height; 
}

GrSize::operator System::Drawing::Size ()
{
    return System::Drawing::Size(width, height); 
}

GrSize::operator System::Drawing::Size () const
{
    return System::Drawing::Size(width, height); 
}
#endif

GrRect::GrRect() : left(0), top(0), right(0), bottom(0)
{

}

GrRect::GrRect(int left, int top, int right, int bottom) : left(left), top(top), right(right), bottom(bottom)
{

}

GrRect::GrRect(const GrPoint& location, const GrSize& size)
{
    left = location.x;
    top = location.y;
    right = left + size.width;
    bottom = top + size.height;
}

GrRect::GrRect(const GrPoint& pt1, const GrPoint& pt2)
{
    left = pt1.x < pt2.x ? pt1.x : pt2.x;
    top = pt1.y < pt2.y ? pt1.y : pt2.y;
    right = left + abs(pt1.x - pt2.x);
    bottom = top + abs(pt1.y - pt2.y);
}

void GrRect::operator += (const GrRect& rect)
{
    left = std::min(left, rect.left);
    top = std::min(top, rect.top);
    right = std::max(right, rect.right);
    bottom = std::max(bottom, rect.bottom);
}

bool GrRect::operator == (const GrRect& rect) const
{
    if(left != rect.left || top != rect.top || right != rect.right || bottom != rect.bottom)
        return false;
    return true;
}

bool GrRect::operator != (const GrRect& rect) const
{
    if(left != rect.left || top != rect.top || right != rect.right || bottom != rect.bottom)
        return true;
    return false;
}


GrRect GrRect::operator + (const GrPoint& point) const
{
    return GrRect(this->left + point.x, this->top + point.y, this->right + point.x, this->bottom + point.y);
}

GrRect GrRect::operator - (const GrPoint& point) const
{
    return GrRect(this->left - point.x, this->top - point.y, this->right - point.x, this->bottom - point.y);
}

GrRect GrRect::operator + (const GrPadding& padding) const
{
    return GrRect(this->left + padding.left, this->top + padding.top, this->right - padding.right, this->bottom - padding.bottom);
}

GrRect GrRect::operator - (const GrPadding& padding) const
{
    return GrRect(this->left - padding.left, this->top - padding.top, this->right + padding.right, this->bottom + padding.bottom);
}

void GrRect::operator += (const GrPadding& padding)
{
    this->left += padding.left;
    this->top += padding.top;
    this->right -= padding.right;
    this->bottom -= padding.bottom;
}

void GrRect::operator -= (const GrPadding& padding)
{
    this->left -= padding.left;
    this->top -= padding.top;
    this->right += padding.right;
    this->bottom += padding.bottom;
}

bool GrRect::Contains(const GrPoint& location) const
{
    if(location.x < left || location.y < top || location.x >= right || location.y >= bottom)
        return false;
    return true;
}

bool GrRect::Contains(const GrRect& rect) const
{
    if(rect.left < left || rect.top < top || rect.right >= right || rect.bottom >= bottom)
        return false;
    return true;
}

bool GrRect::IsEmpty() const
{
    if(left != 0 || top != 0 || right != 0 || bottom != 0)
        return false;
    return true;
}

int GrRect::GetWidth() const
{
    return right - left;
}

int GrRect::GetHeight() const
{
    return bottom - top;
}

GrSize GrRect::GetSize() const
{
    GrSize size;
    size.width = GetWidth();
    size.height = GetHeight();
    return size;
}

void GrRect::SetSize(int width, int height)
{
    right = left + width;
    bottom = top + height;
}

void GrRect::SetSize(const GrSize& size)
{
    SetSize(size.width, size.height);
}

GrPoint GrRect::GetLocation() const
{
    return GrPoint(left, top);
}

void GrRect::SetLocation(int x, int y)
{
    const GrSize size = GetSize();
    left = x;
    top = y;
    right = left + size.width;
    bottom = top + size.height;
}

void GrRect::SetLocation(const GrPoint& location)
{
    SetLocation(location.x, location.y);
}

void GrRect::Offset(int x, int y)
{
    left += x;
    top += y;
    right += x;
    bottom += y;
}

void GrRect::Offset(const GrPoint& offset)
{
    Offset(offset.x, offset.y);
}

void GrRect::Expand(int left, int top, int right, int bottom)
{
    this->left -= left;
    this->top -= top;
    this->right += right;
    this->bottom += bottom;
}

void GrRect::Expand(const GrPadding& padding)
{
    Expand(padding.left, padding.top, padding.right, padding.bottom);
}

void GrRect::Contract(int left, int top, int right, int bottom)
{
    this->left += left;
    this->top += top;
    this->right -= right;
    this->bottom -= bottom;
}

void GrRect::Contract(const GrPadding& padding)
{
    Contract(padding.left, padding.top, padding.right, padding.bottom);
}

void GrRect::DoEmpty()
{
    *this = Empty;
}

GrPoint GrRect::GetCenter() const
{
    return GrPoint((left & right) + ((left ^ right) >> 1), (top & bottom) + ((top ^ bottom) >> 1));
}

#ifdef _MANAGED
GrRect::GrRect(System::Drawing::Rectangle% rect) : left(rect.Left), top(rect.Top), right(rect.Right), bottom(rect.Bottom)
{

}

GrRect::GrRect(System::Drawing::RectangleF% rect) : left((int)rect.Left), top((int)rect.Top), right((int)rect.Right), bottom((int)rect.Bottom)
{

}

bool GrRect::Contains(System::Drawing::Point% location) const
{
    if(location.X < left || location.Y < top || location.X >= right || location.Y >= bottom)
        return false;
    return true;
}

void GrRect::operator = (System::Drawing::Rectangle% rect)
{
    left = rect.Left;
    top = rect.Top;
    right = rect.Right;
    bottom = rect.Bottom; 
}

GrRect::operator System::Drawing::Rectangle ()
{
    return System::Drawing::Rectangle::FromLTRB(left, top, right, bottom); 
}
GrRect::operator System::Drawing::Rectangle () const 
{ 
    return System::Drawing::Rectangle::FromLTRB(left, top, right, bottom); 
}
#endif

GrPadding::GrPadding()
    : left(0), top(0), right(0), bottom(0)
{

}

GrPadding::GrPadding(int all)
    : left(all), top(all), right(all), bottom(all)
{

}

GrPadding::GrPadding(int l, int t, int r, int b)
    : left(l), top(t), right(r), bottom(b)
{

}

int GrPadding::GetHorizontal() const
{
    return left + right;
}

int GrPadding::GetVertical() const
{
    return top + bottom;
}

GrPadding::operator GrRect()
{
    return GrRect(left, top, right, bottom);
}

GrPadding::operator GrRect() const
{
    return GrRect(left, top, right, bottom);
}

bool GrPadding::operator == (const GrPadding& padding) const
{
    if(left != padding.left || top != padding.top || right != padding.right || bottom != padding.bottom)
        return false;
    return true;
}

bool GrPadding::operator != (const GrPadding& padding) const
{
    if(left != padding.left || top != padding.top || right != padding.right || bottom != padding.bottom)
        return true;
    return false;
}

#ifdef _MANAGED
GrPadding::GrPadding(System::Windows::Forms::Padding% padding)
    : left(padding.Left), top(padding.Top), right(padding.Right), bottom(padding.Bottom)
{

}

GrPadding::operator System::Windows::Forms::Padding ()
{
    return System::Windows::Forms::Padding(left, top, right, bottom);
}

GrPadding::operator System::Windows::Forms::Padding () const
{
    return System::Windows::Forms::Padding(left, top, right, bottom);
}

void GrPadding::operator = (System::Windows::Forms::Padding% padding)
{
    left = padding.Left;
    top = padding.Top;
    right = padding.Right;
    bottom = padding.Bottom;
}
#endif

std::map<std::wstring, const GrColor&> GrColor::namedColor;

const GrColor GrColor::Empty(0, 0, 0, 0, L"Empty");
const GrColor GrColor::AliceBlue(255, 240, 248, 255, L"AliceBlue");
const GrColor GrColor::AntiqueWhite(255, 250, 235, 215, L"AntiqueWhite");
const GrColor GrColor::Aqua(255, 0, 255, 255, L"Aqua");
const GrColor GrColor::Aquamarine(255, 127, 255, 212, L"Aquamarine");
const GrColor GrColor::Azure(255, 240, 255, 255, L"Azure");
const GrColor GrColor::Beige(255, 245, 245, 220, L"Beige");
const GrColor GrColor::Bisque(255, 255, 228, 196, L"Bisque");
const GrColor GrColor::Black(255, 0, 0, 0, L"Black");
const GrColor GrColor::BlanchedAlmond(255, 255, 235, 205, L"BlanchedAlmond");
const GrColor GrColor::Blue(255, 0, 0, 255, L"Blue");
const GrColor GrColor::BlueViolet(255, 138, 43, 226, L"BlueViolet");
const GrColor GrColor::Brown(255, 165, 42, 42, L"Brown");
const GrColor GrColor::BurlyWood(255, 222, 184, 135, L"BurlyWood");
const GrColor GrColor::CadetBlue(255, 95, 158, 160, L"CadetBlue");
const GrColor GrColor::Chartreuse(255, 127, 255, 0, L"Chartreuse");
const GrColor GrColor::Chocolate(255, 210, 105, 30, L"Chocolate");
const GrColor GrColor::Coral(255, 255, 127, 80, L"Coral");
const GrColor GrColor::CornflowerBlue(255, 100, 149, 237, L"CornflowerBlue");
const GrColor GrColor::Cornsilk(255, 255, 248, 220, L"Cornsilk");
const GrColor GrColor::Crimson(255, 220, 20, 60, L"Crimson");
const GrColor GrColor::Cyan(255, 0, 255, 255, L"Cyan");
const GrColor GrColor::DarkBlue(255, 0, 0, 139, L"DarkBlue");
const GrColor GrColor::DarkCyan(255, 0, 139, 139, L"DarkCyan");
const GrColor GrColor::DarkGoldenrod(255, 184, 134, 11, L"DarkGoldenrod");
const GrColor GrColor::DarkGray(255, 169, 169, 169, L"DarkGray");
const GrColor GrColor::DarkGreen(255, 0, 100, 0, L"DarkGreen");
const GrColor GrColor::DarkKhaki(255, 189, 183, 107, L"DarkKhaki");
const GrColor GrColor::DarkMagenta(255, 139, 0, 139, L"DarkMagenta");
const GrColor GrColor::DarkOliveGreen(255, 85, 107, 47, L"DarkOliveGreen");
const GrColor GrColor::DarkOrange(255, 255, 140, 0, L"DarkOrange");
const GrColor GrColor::DarkOrchid(255, 153, 50, 204, L"DarkOrchid");
const GrColor GrColor::DarkRed(255, 139, 0, 0, L"DarkRed");
const GrColor GrColor::DarkSalmon(255, 233, 150, 122, L"DarkSalmon");
const GrColor GrColor::DarkSeaGreen(255, 143, 188, 139, L"DarkSeaGreen");
const GrColor GrColor::DarkSlateBlue(255, 72, 61, 139, L"DarkSlateBlue");
const GrColor GrColor::DarkSlateGray(255, 47, 79, 79, L"DarkSlateGray");
const GrColor GrColor::DarkTurquoise(255, 0, 206, 209, L"DarkTurquoise");
const GrColor GrColor::DarkViolet(255, 148, 0, 211, L"DarkViolet");
const GrColor GrColor::DeepPink(255, 255, 20, 147, L"DeepPink");
const GrColor GrColor::DeepSkyBlue(255, 0, 191, 255, L"DeepSkyBlue");
const GrColor GrColor::DimGray(255, 105, 105, 105, L"DimGray");
const GrColor GrColor::DodgerBlue(255, 30, 144, 255, L"DodgerBlue");
const GrColor GrColor::Firebrick(255, 178, 34, 34, L"Firebrick");
const GrColor GrColor::FloralWhite(255, 255, 250, 240, L"FloralWhite");
const GrColor GrColor::ForestGreen(255, 34, 139, 34, L"ForestGreen");
const GrColor GrColor::Fuchsia(255, 255, 0, 255, L"Fuchsia");
const GrColor GrColor::Gainsboro(255, 220, 220, 220, L"Gainsboro");
const GrColor GrColor::GhostWhite(255, 248, 248, 255, L"GhostWhite");
const GrColor GrColor::Gold(255, 255, 215, 0, L"Gold");
const GrColor GrColor::Goldenrod(255, 218, 165, 32, L"Goldenrod");
const GrColor GrColor::Gray(255, 128, 128, 128, L"Gray");
const GrColor GrColor::Green(255, 0, 128, 0, L"Green");
const GrColor GrColor::GreenYellow(255, 173, 255, 47, L"GreenYellow");
const GrColor GrColor::Honeydew(255, 240, 255, 240, L"Honeydew");
const GrColor GrColor::HotPink(255, 255, 105, 180, L"HotPink");
const GrColor GrColor::IndianRed(255, 205, 92, 92, L"IndianRed");
const GrColor GrColor::Indigo(255, 75, 0, 130, L"Indigo");
const GrColor GrColor::Ivory(255, 255, 255, 240, L"Ivory");
const GrColor GrColor::Khaki(255, 240, 230, 140, L"Khaki");
const GrColor GrColor::Lavender(255, 230, 230, 250, L"Lavender");
const GrColor GrColor::LavenderBlush(255, 255, 240, 245, L"LavenderBlush");
const GrColor GrColor::LawnGreen(255, 124, 252, 0, L"LawnGreen");
const GrColor GrColor::LemonChiffon(255, 255, 250, 205, L"LemonChiffon");
const GrColor GrColor::LightBlue(255, 173, 216, 230, L"LightBlue");
const GrColor GrColor::LightCoral(255, 240, 128, 128, L"LightCoral");
const GrColor GrColor::LightCyan(255, 224, 255, 255, L"LightCyan");
const GrColor GrColor::LightGoldenrodYellow(255, 250, 250, 210, L"LightGoldenrodYellow");
const GrColor GrColor::LightGray(255, 211, 211, 211, L"LightGray");
const GrColor GrColor::LightGreen(255, 144, 238, 144, L"LightGreen");
const GrColor GrColor::LightPink(255, 255, 182, 193, L"LightPink");
const GrColor GrColor::LightSalmon(255, 255, 160, 122, L"LightSalmon");
const GrColor GrColor::LightSeaGreen(255, 32, 178, 170, L"LightSeaGreen");
const GrColor GrColor::LightSkyBlue(255, 135, 206, 250, L"LightSkyBlue");
const GrColor GrColor::LightSlateGray(255, 119, 136, 153, L"LightSlateGray");
const GrColor GrColor::LightSteelBlue(255, 176, 196, 222, L"LightSteelBlue");
const GrColor GrColor::LightYellow(255, 255, 255, 224, L"LightYellow");
const GrColor GrColor::Lime(255, 0, 255, 0, L"Lime");
const GrColor GrColor::LimeGreen(255, 50, 205, 50, L"LimeGreen");
const GrColor GrColor::Linen(255, 250, 240, 230, L"Linen");
const GrColor GrColor::Magenta(255, 255, 0, 255, L"Magenta");
const GrColor GrColor::Maroon(255, 128, 0, 0, L"Maroon");
const GrColor GrColor::MediumAquamarine(255, 102, 205, 170, L"MediumAquamarine");
const GrColor GrColor::MediumBlue(255, 0, 0, 205, L"MediumBlue");
const GrColor GrColor::MediumOrchid(255, 186, 85, 211, L"MediumOrchid");
const GrColor GrColor::MediumPurple(255, 147, 112, 219, L"MediumPurple");
const GrColor GrColor::MediumSeaGreen(255, 60, 179, 113, L"MediumSeaGreen");
const GrColor GrColor::MediumSlateBlue(255, 123, 104, 238, L"MediumSlateBlue");
const GrColor GrColor::MediumSpringGreen(255, 0, 250, 154, L"MediumSpringGreen");
const GrColor GrColor::MediumTurquoise(255, 72, 209, 204, L"MediumTurquoise");
const GrColor GrColor::MediumVioletRed(255, 199, 21, 133, L"MediumVioletRed");
const GrColor GrColor::MidnightBlue(255, 25, 25, 112, L"MidnightBlue");
const GrColor GrColor::MintCream(255, 245, 255, 250, L"MintCream");
const GrColor GrColor::MistyRose(255, 255, 228, 225, L"MistyRose");
const GrColor GrColor::Moccasin(255, 255, 228, 181, L"Moccasin");
const GrColor GrColor::NavajoWhite(255, 255, 222, 173, L"NavajoWhite");
const GrColor GrColor::Navy(255, 0, 0, 128, L"Navy");
const GrColor GrColor::OldLace(255, 253, 245, 230, L"OldLace");
const GrColor GrColor::Olive(255, 128, 128, 0, L"Olive");
const GrColor GrColor::OliveDrab(255, 107, 142, 35, L"OliveDrab");
const GrColor GrColor::Orange(255, 255, 165, 0, L"Orange");
const GrColor GrColor::OrangeRed(255, 255, 69, 0, L"OrangeRed");
const GrColor GrColor::Orchid(255, 218, 112, 214, L"Orchid");
const GrColor GrColor::PaleGoldenrod(255, 238, 232, 170, L"PaleGoldenrod");
const GrColor GrColor::PaleGreen(255, 152, 251, 152, L"PaleGreen");
const GrColor GrColor::PaleTurquoise(255, 175, 238, 238, L"PaleTurquoise");
const GrColor GrColor::PaleVioletRed(255, 219, 112, 147, L"PaleVioletRed");
const GrColor GrColor::PapayaWhip(255, 255, 239, 213, L"PapayaWhip");
const GrColor GrColor::PeachPuff(255, 255, 218, 185, L"PeachPuff");
const GrColor GrColor::Peru(255, 205, 133, 63, L"Peru");
const GrColor GrColor::Pink(255, 255, 192, 203, L"Pink");
const GrColor GrColor::Plum(255, 221, 160, 221, L"Plum");
const GrColor GrColor::PowderBlue(255, 176, 224, 230, L"PowderBlue");
const GrColor GrColor::Purple(255, 128, 0, 128, L"Purple");
const GrColor GrColor::Red(255, 255, 0, 0, L"Red");
const GrColor GrColor::RosyBrown(255, 188, 143, 143, L"RosyBrown");
const GrColor GrColor::RoyalBlue(255, 65, 105, 225, L"RoyalBlue");
const GrColor GrColor::SaddleBrown(255, 139, 69, 19, L"SaddleBrown");
const GrColor GrColor::Salmon(255, 250, 128, 114, L"Salmon");
const GrColor GrColor::SandyBrown(255, 244, 164, 96, L"SandyBrown");
const GrColor GrColor::SeaGreen(255, 46, 139, 87, L"SeaGreen");
const GrColor GrColor::SeaShell(255, 255, 245, 238, L"SeaShell");
const GrColor GrColor::Sienna(255, 160, 82, 45, L"Sienna");
const GrColor GrColor::Silver(255, 192, 192, 192, L"Silver");
const GrColor GrColor::SkyBlue(255, 135, 206, 235, L"SkyBlue");
const GrColor GrColor::SlateBlue(255, 106, 90, 205, L"SlateBlue");
const GrColor GrColor::SlateGray(255, 112, 128, 144, L"SlateGray");
const GrColor GrColor::Snow(255, 255, 250, 250, L"Snow");
const GrColor GrColor::SpringGreen(255, 0, 255, 127, L"SpringGreen");
const GrColor GrColor::SteelBlue(255, 70, 130, 180, L"SteelBlue");
const GrColor GrColor::Tan(255, 210, 180, 140, L"Tan");
const GrColor GrColor::Teal(255, 0, 128, 128, L"Teal");
const GrColor GrColor::Thistle(255, 216, 191, 216, L"Thistle");
const GrColor GrColor::Tomato(255, 255, 99, 71, L"Tomato");
const GrColor GrColor::Turquoise(255, 64, 224, 208, L"Turquoise");
const GrColor GrColor::Violet(255, 238, 130, 238, L"Violet");
const GrColor GrColor::Wheat(255, 245, 222, 179, L"Wheat");
const GrColor GrColor::White(255, 255, 255, 255, L"White");
const GrColor GrColor::WhiteSmoke(255, 245, 245, 245, L"WhiteSmoke");
const GrColor GrColor::Yellow(255, 255, 255, 0, L"Yellow");
const GrColor GrColor::YellowGreen(255, 154, 205, 50, L"YellowGreen");

const GrColor GrColor::DefaultLineColor(255, 208, 215, 229);

GrColor::GrColor(int a, int r, int g, int b, const std::wstring& name)
    : name(name)
{
    const int maxValue = (int)0xff;
    this->a = (byte)std::min(a, maxValue);
    this->r = (byte)std::min(r, maxValue);
    this->g = (byte)std::min(g, maxValue);
    this->b = (byte)std::min(b, maxValue);

    GrColor::namedColor.insert(std::map<std::wstring, const GrColor&>::value_type(name, *this));
}

GrColor::GrColor()
    : value(0), name(L"Empty")
{
    //RGB(255,255,255)
}

GrColor::GrColor(int argb)
{
    this->a = 0xff;
    this->r = GetRValue(argb);
    this->g = GetGValue(argb);
    this->b = GetBValue(argb);
}

GrColor::GrColor(int a, int r, int g, int b)
{
    const int maxValue = (int)0xff;
    this->a = (byte)std::min(a, maxValue);
    this->r = (byte)std::min(r, maxValue);
    this->g = (byte)std::min(g, maxValue);
    this->b = (byte)std::min(b, maxValue);
}

GrColor::GrColor(int r, int g, int b)
{
    const int maxValue = (int)0xff;
    this->a = maxValue;
    this->r = (byte)std::min(r, maxValue);
    this->g = (byte)std::min(g, maxValue);
    this->b = (byte)std::min(b, maxValue);
}

void GrColor::operator *= (float f)
{
    f = std::max(0.0f, f);

    A(A() * f);
    R(R() * f);
    G(G() * f);
    B(B() * f);

    this->name.clear();
}

GrColor GrColor::operator * (float f) const
{
    GrColor clr;
    clr.A(A() * f);
    clr.R(R() * f);
    clr.G(G() * f);
    clr.B(B() * f);

    return clr;
}

GrColor GrColor::operator * (const GrColor& color) const
{
    GrColor clr;
    clr.A(A() * color.A());
    clr.R(R() * color.R());
    clr.G(G() * color.G());
    clr.B(B() * color.B());
    return clr;
}

bool GrColor::operator != (const GrColor& color) const
{
    return !(*this == color);
}

bool GrColor::operator == (const GrColor& color) const
{
    if(this->name.length() == 0 && color.name.length() == 0)
        return value == color.value;
    return _wcsicmp(this->name.c_str(), color.name.c_str()) == 0;
}

float GrColor::A() const
{
    return (float)a / 255.0f;
}

float GrColor::R() const
{
    return (float)r / 255.0f;
}

float GrColor::G() const
{
    return (float)g / 255.0f;
}

float GrColor::B() const
{
    return (float)b / 255.0f;
}

void GrColor::A(float value)
{
    value *= 255.0f;
    a = value > 255.0f ? 0xff : (byte)value;
}

void GrColor::R(float value)
{
    value *= 255.0f;
    r = value > 255.0f ? 0xff : (byte)value;
}

void GrColor::G(float value)
{
    value *= 255.0f;
    g = value > 255.0f ? 0xff : (byte)value;
}

void GrColor::B(float value)
{
    value *= 255.0f;
    b = value > 255.0f ? 0xff : (byte)value;
}

const std::wstring& GrColor::GetName() const
{
    return this->name;
}

float GrColor::GetBrightness() const
{
    float num = this->R();
    float num2 = this->G();
    float num3 = this->B();
    float num4 = num;
    float num5 = num;
    if (num2 > num4)
    {
        num4 = num2;
    }
    if (num3 > num4)
    {
        num4 = num3;
    }
    if (num2 < num5)
    {
        num5 = num2;
    }
    if (num3 < num5)
    {
        num5 = num3;
    }
    return ((num4 + num5) / 2.0f);

}

float GrColor::GetHue() const
{
    if ((this->r == this->g) && (this->g == this->b))
    {
        return 0.0f;
    }
    float num = this->R();
    float num2 = this->G();
    float num3 = this->B();
    float num7 = 0.0f;
    float num4 = num;
    float num5 = num;
    if (num2 > num4)
    {
        num4 = num2;
    }
    if (num3 > num4)
    {
        num4 = num3;
    }
    if (num2 < num5)
    {
        num5 = num2;
    }
    if (num3 < num5)
    {
        num5 = num3;
    }
    float num6 = num4 - num5;
    if (num == num4)
    {
        num7 = (num2 - num3) / num6;
    }
    else if (num2 == num4)
    {
        num7 = 2.0f + ((num3 - num) / num6);
    }
    else if (num3 == num4)
    {
        num7 = 4.0f + ((num - num2) / num6);
    }
    num7 *= 60.0f;
    if (num7 < 0.0f)
    {
        num7 += 360.0f;
    }
    return num7;

}

float GrColor::GetSaturation() const
{
    float num = this->R();
    float num2 = this->G();
    float num3 = this->B();
    float num7 = 0.0f;
    float num4 = num;
    float num5 = num;
    if (num2 > num4)
    {
        num4 = num2;
    }
    if (num3 > num4)
    {
        num4 = num3;
    }
    if (num2 < num5)
    {
        num5 = num2;
    }
    if (num3 < num5)
    {
        num5 = num3;
    }
    if (num4 == num5)
    {
        return num7;
    }
    float num6 = (num4 + num5) / 2.0f;
    if (num6 <= 0.5f)
    {
        return ((num4 - num5) / (num4 + num5));
    }
    return ((num4 - num5) / ((2.0f - num4) - num5));
}


#ifdef _MANAGED
GrColor::GrColor(System::Drawing::Color color)
{
    a = color.A;
    r = color.R;
    g = color.G;
    b = color.B;
    if(color.IsNamedColor == true)
    {
        pin_ptr<const wchar_t> unmngStr = PtrToStringChars(color.Name);
        this->name = (const wchar_t*)unmngStr;
    }
}

GrColor::operator System::Drawing::Color ()
{
    System::Drawing::Color color;
    //const wchar_t* d = this->name.c_str();
    if(this->name.length() != 0)
        color = System::Drawing::Color::FromName(gcnew System::String(this->name.c_str()));
    else
        color = System::Drawing::Color::FromArgb(a, r, g, b);
    return color;
}

GrColor::operator System::Drawing::Color () const
{
    if(this->name.length() != 0)
        return System::Drawing::Color::FromName(gcnew System::String(this->name.c_str()));
    return System::Drawing::Color::FromArgb(a, r, g, b);
}
#endif

GrFlag::GrFlag()
    : m_flag(0)
{

}

GrFlag::GrFlag(ulong flag)
    : m_flag(flag)
{

}
GrFlag::operator ulong() const
{
    return m_flag;
}

ulong GrFlag::Add(ulong flag, ulong mask)
{
    flag &= mask;
    ulong returnFlag = flag & ~m_flag;
    m_flag |= flag; 
    return returnFlag;
}

ulong GrFlag::Remove(ulong flag, ulong mask) 
{
    flag &= mask;
    ulong returnFlag = flag & m_flag;
    m_flag &= ~flag; 
    return returnFlag;
}

void GrFlag::Clear()
{
    m_flag = 0; 
}

bool GrFlag::Has(ulong flag, ulong mask) const
{
    flag &= mask;
    return m_flag & flag ? true : false; 
}

ulong GrFlag::Get(ulong mask) const
{
    return m_flag & mask; 
}

ulong GrFlag::operator += (ulong flag)
{
    return Add(flag);
}

ulong GrFlag::operator -= (ulong flag)
{
    return Remove(flag);
}

void GrFlag::operator = (ulong flag)
{
    m_flag = flag;
}

ulong GrFlag::operator + (ulong flag) const
{
    return m_flag | flag;
}

ulong GrFlag::operator - (ulong flag) const
{
    return m_flag & ~flag;
}

GrHitTest::GrHitTest()
{
    pHitted = nullptr;
    localHit = GrPoint::Empty;
}

RGBHSL::HSL::HSL()
{
    _h = 0;
    _s = 0;
    _l = 0;
}

float RGBHSL::HSL::H() const
{
    return _h; 
}

void RGBHSL::HSL::H(float value)
{
    _h = value;
    _h = _h > 1 ? 1 : _h < 0 ? 0 : _h;
}

float RGBHSL::HSL::S() const
{
    return _s; 
}

void RGBHSL::HSL::S(float value)
{
    _s = value;
    _s = _s > 1 ? 1 : _s < 0 ? 0 : _s;
}

float RGBHSL::HSL::L() const
{
    return _l; 
}

void RGBHSL::HSL::L(float value)
{
    _l = value;
    _l = _l > 1 ? 1 : _l < 0 ? 0 : _l;
}

GrColor RGBHSL::SetBrightness(const GrColor& c, float brightness)
{
    HSL hsl = RGB_to_HSL(c);
    hsl.L(brightness);
    return HSL_to_RGB(hsl);
}

GrColor RGBHSL::ModifyBrightness(const GrColor& c, float brightness)
{
    HSL hsl = RGB_to_HSL(c);
    hsl.L(hsl.L() * brightness);
    return HSL_to_RGB(hsl);
}

GrColor RGBHSL::SetSaturation(const GrColor& c, float Saturation)
{
    HSL hsl = RGB_to_HSL(c);
    hsl.S(Saturation);
    return HSL_to_RGB(hsl);
}

GrColor RGBHSL::ModifySaturation(const GrColor& c, float Saturation)
{
    HSL hsl = RGB_to_HSL(c);
    hsl.S(hsl.S() * Saturation);
    return HSL_to_RGB(hsl);
}

GrColor RGBHSL::SetHue(const GrColor& c, float Hue)
{
    HSL hsl = RGB_to_HSL(c);
    hsl.H(Hue);
    return HSL_to_RGB(hsl);
}

GrColor RGBHSL::ModifyHue(const GrColor& c, float Hue)
{
    HSL hsl = RGB_to_HSL(c);
    hsl.H(hsl.H() * Hue);
    return HSL_to_RGB(hsl);
}

GrColor RGBHSL::HSL_to_RGB(HSL hsl)
{
    float r = 0, g = 0, b = 0;
    float temp1, temp2;

    if (hsl.L() == 0)
    {
        r = g = b = 0;
    }
    else
    {
        if (hsl.S() == 0)
        {
            r = g = b = hsl.L();
        }
        else
        {
            temp2 = ((hsl.L() <= 0.5f) ? hsl.L() * (1.0f + hsl.S()) : hsl.L() + hsl.S() - (hsl.L() * hsl.S()));
            temp1 = 2.0f * hsl.L() - temp2;

            float t3[] = { hsl.H() + 1.0f / 3.0f, hsl.H(), hsl.H() - 1.0f / 3.0f };
            float clr[] = { 0.0f, 0.0f, 0.0f };
            for (int i = 0; i < 3; i++)
            {
                if (t3[i] < 0)
                    t3[i] += 1.0f;
                if (t3[i] > 1)
                    t3[i] -= 1.0f;

                if (6.0f * t3[i] < 1.0f)
                    clr[i] = temp1 + (temp2 - temp1) * t3[i] * 6.0f;
                else if (2.0f * t3[i] < 1.0f)
                    clr[i] = temp2;
                else if (3.0f * t3[i] < 2.0f)
                    clr[i] = (temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - t3[i]) * 6.0f);
                else
                    clr[i] = temp1;
            }
            r = clr[0];
            g = clr[1];
            b = clr[2];

        }
    }

    return GrColor((int)(255 * r), (int)(255 * g), (int)(255 * b));
}

RGBHSL::HSL RGBHSL::RGB_to_HSL(const GrColor& c)
{
    HSL hsl;

    hsl.H(c.GetHue() / 360.0f); // we store hue as 0-1 as opposed to 0-360
    hsl.L(c.GetBrightness());
    hsl.S(c.GetSaturation());

    return hsl;
}
