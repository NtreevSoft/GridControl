#include "StdAfx.h"
#include "GrGridType.h"

#define LOBYTE(w)           ((byte)(((uintptr_t)(w)) & 0xff))
#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((unsigned short)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

const GrPoint	GrPoint::Empty(0, 0);
const GrSize	GrSize::Empty(0, 0);

const GrPadding GrPadding::Empty(0,0,0,0);
const GrPadding GrPadding::Default(5,2,5,2);
const GrRect GrRect::Empty(0, 0, 0, 0);

GrRect::GrRect()
	: left(0), top(0), right(0), bottom(0)
{
	
}

GrRect::GrRect(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b)
{

}

GrRect::GrRect(GrPoint pt, GrSize sz)
{
	left	= pt.x;
	top		= pt.y;
	right	= left + sz.width;
	bottom	= top  + sz.height;
}

GrRect::GrRect(GrPoint pt1, GrPoint pt2)
{
	left   = pt1.x < pt2.x ? pt1.x : pt2.x;
	top    = pt1.y < pt2.y ? pt1.y : pt2.y;
	right  = left + abs(pt1.x - pt2.x);
	bottom = top  + abs(pt1.y - pt2.y);
}

void	GrRect::operator += (const GrRect& rt)
{
	left	= std::min(left, rt.left);
	top		= std::min(top, rt.top);
	right	= std::max(right, rt.right);
	bottom	= std::max(bottom, rt.bottom);
}

bool	GrRect::operator == (const GrRect& rt) const
{
	if(left != rt.left || top != rt.top || right != rt.right || bottom != rt.bottom)
		return false;
	return true;
}

bool	GrRect::operator != (const GrRect& rt) const
{
	if(left != rt.left || top != rt.top || right != rt.right || bottom != rt.bottom)
		return true;
	return false;
}


GrRect GrRect::operator + (const GrPoint& p) const
{
	return GrRect(this->left + p.x, this->top + p.y, this->right + p.x, this->bottom + p.y);
}

GrRect GrRect::operator - (const GrPoint& p) const
{
	return GrRect(this->left - p.x, this->top - p.y, this->right - p.x, this->bottom - p.y);
}

bool	GrRect::IsIn(GrPoint pt) const
{
	if(pt.x < left || pt.y < top || pt.x >= right || pt.y >= bottom)
		return false;
	return true;
}

bool	GrRect::IsIn(const GrRect* pRect) const
{
	if(pRect->left < left || pRect->top < top || pRect->right >= right || pRect->bottom >= bottom)
		return false;
	return true;
}

bool	GrRect::IsEmpty() const
{
	if(left != 0 || top != 0 || right != 0 || bottom != 0)
		return false;
	return true;
}

int		GrRect::GetWidth() const
{
	return right - left;
}

int		GrRect::GetHeight() const
{
	return bottom - top;
}

GrSize	GrRect::GetSize() const
{
	GrSize sz;
	sz.width  = GetWidth();
	sz.height = GetHeight();
	return sz;
}

void	GrRect::SetSize(GrSize size)
{
	right	= left + size.width;
	bottom	= top  + size.height;
}

GrPoint GrRect::GetLocation() const
{
	return GrPoint(left, top);
}

void GrRect::SetLocation(GrPoint pt)
{
	left = pt.x;
	top  = pt.y;
}

void	GrRect::Offset(int x, int y)
{
	left	+= x;
	top		+= y;
	right	+= x;
	bottom	+= y;
}

void	GrRect::Offset(GrPoint pt)
{
	Offset(pt.x, pt.y);
}

void	GrRect::Expand(int left, int top, int right, int bottom)
{
	this->left	 -= left;
	this->top	 -= top;
	this->right	 += right;
	this->bottom += bottom;
}

void	GrRect::Expand(GrRect rtExpand)
{
	Expand(rtExpand.left, rtExpand.top, rtExpand.right, rtExpand.bottom);
}

void	GrRect::Contract(int left, int top, int right, int bottom)
{
	this->left	 += left;
	this->top	 += top;
	this->right	 -= right;
	this->bottom -= bottom;
}

void	GrRect::Contract(GrRect rtContract)
{
	Contract(rtContract.left, rtContract.top, rtContract.right, rtContract.bottom);
}

void	GrRect::DoEmpty()
{
	*this = Empty;
}

GrPoint GrRect::GetCenter() const
{
	return GrPoint((left & right) + ((left ^ right) >> 1), (top & bottom) + ((top ^ bottom) >> 1));
	//return GrPoint((left + right)/2, (top + bottom)/2);
}

#if 0
GrRect::operator RECT () 
{ 
	RECT rt; rt.left = left; rt.top = top; rt.right = right; rt.bottom = bottom; return rt; 
}

GrRect::operator RECT () const  
{ 
	RECT rt; rt.left = left; rt.top = top; rt.right = right; rt.bottom = bottom; return rt; 
}
#endif

#ifdef _MANAGED
GrRect::GrRect(System::Drawing::Rectangle% rt) : left(rt.Left), top(rt.Top), right(rt.Right), bottom(rt.Bottom)
{

}

GrRect::GrRect(System::Drawing::RectangleF% rt) : left((int)rt.Left), top((int)rt.Top), right((int)rt.Right), bottom((int)rt.Bottom)
{

}

bool	GrRect::IsIn(System::Drawing::Point% pt) const
{
	if(pt.X < left || pt.Y < top || pt.X >= right || pt.Y >= bottom)
		return false;
	return true;
}
void	GrRect::operator = (System::Drawing::Rectangle% rt)
{
	left = rt.Left; top = rt.Top; right = rt.Right; bottom = rt.Bottom; 
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
{
	this->left = 0;
	this->top = 0;
	this->right = 0;
	this->bottom = 0;
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
	left	= padding.Left;
	top		= padding.Top;
	right	= padding.Right;
	bottom	= padding.Bottom;
}
#endif

const GrColor GrColor::Empty(0, 0, 0, 0);
const GrColor GrColor::AliceBlue(255, 240, 248, 255);
const GrColor GrColor::AntiqueWhite(255, 250, 235, 215);
const GrColor GrColor::Aqua(255, 0, 255, 255);
const GrColor GrColor::Aquamarine(255, 127, 255, 212);
const GrColor GrColor::Azure(255, 240, 255, 255);
const GrColor GrColor::Beige(255, 245, 245, 220);
const GrColor GrColor::Bisque(255, 255, 228, 196);
const GrColor GrColor::Black(255, 0, 0, 0);
const GrColor GrColor::BlanchedAlmond(255, 255, 235, 205);
const GrColor GrColor::Blue(255, 0, 0, 255);
const GrColor GrColor::BlueViolet(255, 138, 43, 226);
const GrColor GrColor::Brown(255, 165, 42, 42);
const GrColor GrColor::BurlyWood(255, 222, 184, 135);
const GrColor GrColor::CadetBlue(255, 95, 158, 160);
const GrColor GrColor::Chartreuse(255, 127, 255, 0);
const GrColor GrColor::Chocolate(255, 210, 105, 30);
const GrColor GrColor::Coral(255, 255, 127, 80);
const GrColor GrColor::CornflowerBlue(255, 100, 149, 237);
const GrColor GrColor::Cornsilk(255, 255, 248, 220);
const GrColor GrColor::Crimson(255, 220, 20, 60);
const GrColor GrColor::Cyan(255, 0, 255, 255);
const GrColor GrColor::DarkBlue(255, 0, 0, 139);
const GrColor GrColor::DarkCyan(255, 0, 139, 139);
const GrColor GrColor::DarkGoldenrod(255, 184, 134, 11);
const GrColor GrColor::DarkGray(255, 169, 169, 169);
const GrColor GrColor::DarkGreen(255, 0, 100, 0);
const GrColor GrColor::DarkKhaki(255, 189, 183, 107);
const GrColor GrColor::DarkMagenta(255, 139, 0, 139);
const GrColor GrColor::DarkOliveGreen(255, 85, 107, 47);
const GrColor GrColor::DarkOrange(255, 255, 140, 0);
const GrColor GrColor::DarkOrchid(255, 153, 50, 204);
const GrColor GrColor::DarkRed(255, 139, 0, 0);
const GrColor GrColor::DarkSalmon(255, 233, 150, 122);
const GrColor GrColor::DarkSeaGreen(255, 143, 188, 139);
const GrColor GrColor::DarkSlateBlue(255, 72, 61, 139);
const GrColor GrColor::DarkSlateGray(255, 47, 79, 79);
const GrColor GrColor::DarkTurquoise(255, 0, 206, 209);
const GrColor GrColor::DarkViolet(255, 148, 0, 211);
const GrColor GrColor::DeepPink(255, 255, 20, 147);
const GrColor GrColor::DeepSkyBlue(255, 0, 191, 255);
const GrColor GrColor::DimGray(255, 105, 105, 105);
const GrColor GrColor::DodgerBlue(255, 30, 144, 255);
const GrColor GrColor::Firebrick(255, 178, 34, 34);
const GrColor GrColor::FloralWhite(255, 255, 250, 240);
const GrColor GrColor::ForestGreen(255, 34, 139, 34);
const GrColor GrColor::Fuchsia(255, 255, 0, 255);
const GrColor GrColor::Gainsboro(255, 220, 220, 220);
const GrColor GrColor::GhostWhite(255, 248, 248, 255);
const GrColor GrColor::Gold(255, 255, 215, 0);
const GrColor GrColor::Goldenrod(255, 218, 165, 32);
const GrColor GrColor::Gray(255, 128, 128, 128);
const GrColor GrColor::Green(255, 0, 128, 0);
const GrColor GrColor::GreenYellow(255, 173, 255, 47);
const GrColor GrColor::Honeydew(255, 240, 255, 240);
const GrColor GrColor::HotPink(255, 255, 105, 180);
const GrColor GrColor::IndianRed(255, 205, 92, 92);
const GrColor GrColor::Indigo(255, 75, 0, 130);
const GrColor GrColor::Ivory(255, 255, 255, 240);
const GrColor GrColor::Khaki(255, 240, 230, 140);
const GrColor GrColor::Lavender(255, 230, 230, 250);
const GrColor GrColor::LavenderBlush(255, 255, 240, 245);
const GrColor GrColor::LawnGreen(255, 124, 252, 0);
const GrColor GrColor::LemonChiffon(255, 255, 250, 205);
const GrColor GrColor::LightBlue(255, 173, 216, 230);
const GrColor GrColor::LightCoral(255, 240, 128, 128);
const GrColor GrColor::LightCyan(255, 224, 255, 255);
const GrColor GrColor::LightGoldenrodYellow(255, 250, 250, 210);
const GrColor GrColor::LightGray(255, 211, 211, 211);
const GrColor GrColor::LightGreen(255, 144, 238, 144);
const GrColor GrColor::LightPink(255, 255, 182, 193);
const GrColor GrColor::LightSalmon(255, 255, 160, 122);
const GrColor GrColor::LightSeaGreen(255, 32, 178, 170);
const GrColor GrColor::LightSkyBlue(255, 135, 206, 250);
const GrColor GrColor::LightSlateGray(255, 119, 136, 153);
const GrColor GrColor::LightSteelBlue(255, 176, 196, 222);
const GrColor GrColor::LightYellow(255, 255, 255, 224);
const GrColor GrColor::Lime(255, 0, 255, 0);
const GrColor GrColor::LimeGreen(255, 50, 205, 50);
const GrColor GrColor::Linen(255, 250, 240, 230);
const GrColor GrColor::Magenta(255, 255, 0, 255);
const GrColor GrColor::Maroon(255, 128, 0, 0);
const GrColor GrColor::MediumAquamarine(255, 102, 205, 170);
const GrColor GrColor::MediumBlue(255, 0, 0, 205);
const GrColor GrColor::MediumOrchid(255, 186, 85, 211);
const GrColor GrColor::MediumPurple(255, 147, 112, 219);
const GrColor GrColor::MediumSeaGreen(255, 60, 179, 113);
const GrColor GrColor::MediumSlateBlue(255, 123, 104, 238);
const GrColor GrColor::MediumSpringGreen(255, 0, 250, 154);
const GrColor GrColor::MediumTurquoise(255, 72, 209, 204);
const GrColor GrColor::MediumVioletRed(255, 199, 21, 133);
const GrColor GrColor::MidnightBlue(255, 25, 25, 112);
const GrColor GrColor::MintCream(255, 245, 255, 250);
const GrColor GrColor::MistyRose(255, 255, 228, 225);
const GrColor GrColor::Moccasin(255, 255, 228, 181);
const GrColor GrColor::NavajoWhite(255, 255, 222, 173);
const GrColor GrColor::Navy(255, 0, 0, 128);
const GrColor GrColor::OldLace(255, 253, 245, 230);
const GrColor GrColor::Olive(255, 128, 128, 0);
const GrColor GrColor::OliveDrab(255, 107, 142, 35);
const GrColor GrColor::Orange(255, 255, 165, 0);
const GrColor GrColor::OrangeRed(255, 255, 69, 0);
const GrColor GrColor::Orchid(255, 218, 112, 214);
const GrColor GrColor::PaleGoldenrod(255, 238, 232, 170);
const GrColor GrColor::PaleGreen(255, 152, 251, 152);
const GrColor GrColor::PaleTurquoise(255, 175, 238, 238);
const GrColor GrColor::PaleVioletRed(255, 219, 112, 147);
const GrColor GrColor::PapayaWhip(255, 255, 239, 213);
const GrColor GrColor::PeachPuff(255, 255, 218, 185);
const GrColor GrColor::Peru(255, 205, 133, 63);
const GrColor GrColor::Pink(255, 255, 192, 203);
const GrColor GrColor::Plum(255, 221, 160, 221);
const GrColor GrColor::PowderBlue(255, 176, 224, 230);
const GrColor GrColor::Purple(255, 128, 0, 128);
const GrColor GrColor::Red(255, 255, 0, 0);
const GrColor GrColor::RosyBrown(255, 188, 143, 143);
const GrColor GrColor::RoyalBlue(255, 65, 105, 225);
const GrColor GrColor::SaddleBrown(255, 139, 69, 19);
const GrColor GrColor::Salmon(255, 250, 128, 114);
const GrColor GrColor::SandyBrown(255, 244, 164, 96);
const GrColor GrColor::SeaGreen(255, 46, 139, 87);
const GrColor GrColor::SeaShell(255, 255, 245, 238);
const GrColor GrColor::Sienna(255, 160, 82, 45);
const GrColor GrColor::Silver(255, 192, 192, 192);
const GrColor GrColor::SkyBlue(255, 135, 206, 235);
const GrColor GrColor::SlateBlue(255, 106, 90, 205);
const GrColor GrColor::SlateGray(255, 112, 128, 144);
const GrColor GrColor::Snow(255, 255, 250, 250);
const GrColor GrColor::SpringGreen(255, 0, 255, 127);
const GrColor GrColor::SteelBlue(255, 70, 130, 180);
const GrColor GrColor::Tan(255, 210, 180, 140);
const GrColor GrColor::Teal(255, 0, 128, 128);
const GrColor GrColor::Thistle(255, 216, 191, 216);
const GrColor GrColor::Tomato(255, 255, 99, 71);
const GrColor GrColor::Turquoise(255, 64, 224, 208);
const GrColor GrColor::Violet(255, 238, 130, 238);
const GrColor GrColor::Wheat(255, 245, 222, 179);
const GrColor GrColor::White(255, 255, 255, 255);
const GrColor GrColor::WhiteSmoke(255, 245, 245, 245);
const GrColor GrColor::Yellow(255, 255, 255, 0);
const GrColor GrColor::YellowGreen(255, 154, 205, 50);



GrColor::GrColor() : value(0)
{
	//RGB(255,255,255)
}

GrColor::GrColor(int clr)
{
	a = 0xff;
	r = GetRValue(clr);
	g = GetGValue(clr);
	b = GetBValue(clr);
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
	return this->value != color.value;
}

bool GrColor::operator == (const GrColor& color) const
{
	return this->value == color.value;
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

#ifdef _MANAGED
GrColor::GrColor(System::Drawing::Color Color)
{
	a = Color.A;
	r = Color.R;
	g = Color.G;
	b = Color.B;
}

GrColor::operator System::Drawing::Color ()
{
	return System::Drawing::Color::FromArgb(a,r,g,b);
}

GrColor::operator System::Drawing::Color () const
{
	return System::Drawing::Color::FromArgb(a,r,g,b);
}
#endif

#define RGB(r,g,b)          ((uintptr_t)(((byte)(r)|((unsigned short)((byte)(g))<<8))|(((uintptr_t)(byte)(b))<<16)))
ulong GrColor::ToRGB() const
{
	return RGB(r,g,b);
}

GrFlag::GrFlag() : m_flag(0)
{

}

GrFlag::GrFlag(ulong flag) : m_flag(flag)
{

}
GrFlag::operator ulong() const
{
	return m_flag;
}

ulong		GrFlag::Add(ulong flag, ulong mask)
{
	flag &= mask;
	ulong returnFlag = flag & ~m_flag;
	m_flag |= flag; 
	return returnFlag;
}

ulong		GrFlag::Remove(ulong flag, ulong mask) 
{
	flag &= mask;
	ulong returnFlag = flag & m_flag;
	m_flag &= ~flag; 
	return returnFlag;
}

void		GrFlag::Clear()
{
	m_flag = 0; 
}
bool		GrFlag::Has(ulong flag, ulong mask) const
{
	flag &= mask;
	return m_flag & flag ? true : false; 
}
ulong		GrFlag::Get(ulong mask) const
{
	return m_flag & mask; 
}

ulong		GrFlag::operator += (ulong flag)
{
	return Add(flag);
}
ulong		GrFlag::operator -= (ulong flag)
{
	return Remove(flag);
}

void		GrFlag::operator = (ulong flag)
{
	m_flag = flag;
}

ulong		GrFlag::operator + (ulong flag) const
{
	return m_flag | flag;
}

ulong		GrFlag::operator - (ulong flag) const
{
	return m_flag & ~flag;
}