//=====================================================================================================================
// Ntreev Grid for .Net 1.0.4300.26762
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


#pragma once
#define _SECURE_SCL 0
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <algorithm>

typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned char	byte;

#define DEF_GROUP_WIDTH		15
#define DEF_CONTROL_WIDTH	17
#define DEF_SPLITTER_SIZE	5

#define DEF_LEADING			5

#ifdef _MANAGED
#define _Exception(_message) gcnew System::Exception(_message)
#else
#define _Exception(_message) new std::exception(_message)
#endif

#ifdef _MANAGED
#define _Exception_(_type, _message) gcnew System::_type(_message)
#else
#define _Exception_(_type, _message) new std::exception(_message)
#endif

template<typename T>
class GrEnumerator
{
public:
	typedef typename T::value_type	ValueType;
	typedef typename T::size_type	SizeType;
	typedef typename T::const_iterator	IteratorType;

public:
	GrEnumerator(const T& container)
		: m_container(&container), m_index((SizeType)-1), m_iterator(m_container->begin())
	{
		
	}

	ValueType& GetValue()
	{
		return m_value;
	}

	operator ValueType& ()
	{
		return m_value;
	}

	ValueType& operator ->()
	{
		return m_value;
	}

	bool Next()
	{
		if(m_iterator == m_container->end())
			return false;
		m_value = *m_iterator;
		m_iterator++;
		m_index++;
		return true;
	}

	SizeType GetIndex() const
	{
		return m_index;
	}

private:
	const T*		m_container;
	IteratorType	m_iterator;
	ValueType		m_value;
	SizeType		m_index;
};

template<typename T, typename U>
class GrEnumerator< std::map<T, U> >
{
public:
	typedef typename std::map<T, U>				ContainerType;
	typedef typename ContainerType::key_type	KeyType;
	typedef typename ContainerType::mapped_type	ValueType;
	typedef typename ContainerType::size_type	SizeType;
	typedef typename ContainerType::iterator	IteratorType;

public:
	GrEnumerator(ContainerType& container)
		: m_container(&container), m_index((SizeType)-1), m_iterator(m_container->begin())
	{
		
	}

	operator ValueType& ()
	{
		return (*m_value).second;
	}

	ValueType& operator ->() 
	{
		return (*m_value).second;
	}

	bool Next()
	{
		if(m_iterator == m_container->end())
			return false;
		m_value = m_iterator;
		m_iterator++;
		m_index++;
		return true;
	}

	KeyType& GetKey() const
	{
		return (*m_value).first;
	}

	ValueType& GetValue() const
	{
		return (*m_value).second;
	}

	SizeType GetIndex() const
	{
		return m_index;
	}

private:
	ContainerType*	m_container;
	IteratorType	m_value;
	IteratorType	m_iterator;
	SizeType		m_index;
};

#define for_each(_container_type, _container_instance, _value_name) \
	for(GrEnumerator< _container_type > _value_name(_container_instance) ; _value_name.Next() == true ; )

#define for_each_map(_key_type, _value_type, _container_instance, _value_name) \
	for(GrEnumerator< std::map<_key_type, _value_type> > _value_name(_container_instance) ; _value_name.Next() == true ; )

enum GrState
{
	GrState_Normal,
	GrState_MouseOver,
	GrState_Pressed,
	GrState_Selected,

	GrState_Count,
}; 

enum GrCellType
{
	GrCellType_Root,
	GrCellType_Column,
	GrCellType_Row,
	GrCellType_Item,
	GrCellType_GroupingInfo,
	GrCellType_Grouping,
	GrCellType_Splitter,

	GrCellType_Unknown,

	GrCellType_Count,
};

enum GrRowType
{
	GrRowType_Caption,
	GrRowType_GroupingList,
	GrRowType_InsertionRow,
	GrRowType_DataRow,
	GrRowType_Row,
	GrRowType_ColumnList,
	GrRowType_DataRowList,
	GrRowType_GroupingRow,
	GrRowType_Splitter,

	GrRowType_Dummy,

	GrRowType_Unknown,

	GrRowType_Count,
};

enum GrSort
{
	GrSort_None,
	GrSort_Up,
	GrSort_Down,

	GrSort_Count,
};

class GrColumn;
class GrDataRow;
typedef std::set<GrColumn*>			GrColumns;
typedef std::set<GrDataRow*>		GrDataRows;

struct GrPoint
{
	GrPoint() {}
	GrPoint(int x, int y) : x(x), y(y) {}

	void operator += (const GrPoint& pt) { x += pt.x; y -= pt.y; }
	void operator -= (const GrPoint& pt) { x += pt.x; y -= pt.y; }

	GrPoint	operator + (const GrPoint& pt) { return GrPoint(this->x + pt.x, this->y + pt.y); }
	GrPoint	operator - (const GrPoint& pt) { return GrPoint(this->x - pt.x, this->y - pt.y); }

	GrPoint operator - () const { return GrPoint(-this->x, -this->y); }
	GrPoint operator + () const { return GrPoint(this->x, this->y); }

#ifdef _MANAGED
	GrPoint(System::Drawing::Point pt) : x(pt.X), y(pt.Y) {}
	void operator = (System::Drawing::Point pt) { x = pt.X; y = pt.Y; }
	operator System::Drawing::Point () { return System::Drawing::Point(x, y); }
	operator System::Drawing::Point () const { return  System::Drawing::Point(x, y); }
#endif

	int x;
	int y;

	static const GrPoint Empty;
};

struct GrSize
{
	GrSize() {}
	GrSize(int w, int h) : width(w), height(h) {}

#ifdef _MANAGED
	GrSize(System::Drawing::Size size) : width(size.Width), height(size.Height) {}
#endif

	int width;
	int height;

	static const GrSize Empty;
};

struct GrRect
{
	GrRect();
	GrRect(int l, int t, int r, int b);
	GrRect(GrPoint pt, GrSize sz);
	GrRect(GrPoint pt1, GrPoint pt2);

	bool		IsIn(GrPoint pt) const;
	bool		IsIn(const GrRect* pRect) const;
	bool		IsEmpty() const;

	int			GetWidth() const;
	int			GetHeight() const;

	GrSize		GetSize() const;
	void		SetSize(GrSize size);

	GrPoint		GetCenter() const;
	GrPoint		GetLocation() const;
	void		SetLocation(GrPoint pt);

	void		Offset(int x, int y);
	void		Offset(GrPoint pt);

	void		Expand(int left, int top, int right, int bottom);
	void		Expand(GrRect rtExpand);
	void		Contract(int left, int top, int right, int bottom);
	void		Contract(GrRect rtContract);

	void		DoEmpty();

	void 		operator += (const GrRect& rt);
	bool 		operator == (const GrRect& rt) const;
	bool 		operator != (const GrRect& rt) const;
	GrRect		operator + (const GrPoint& p) const;
	GrRect		operator - (const GrPoint& p) const;

#ifdef _MANAGED
	GrRect(System::Drawing::Rectangle% rt);
	GrRect(System::Drawing::RectangleF% rt);

	bool		IsIn(System::Drawing::Point% pt) const;
	operator System::Drawing::Rectangle ();
	operator System::Drawing::Rectangle () const;
	void operator = (System::Drawing::Rectangle% rt);
#endif

	int left, top, right, bottom;

	static const GrRect Empty;
};

struct GrPadding
{
	GrPadding();
	GrPadding(int l, int t, int r, int b);

	int				GetHorizontal() const;
	int				GetVertical() const;

	operator		GrRect();
	operator		GrRect() const;

	bool		operator != (const GrPadding& padding) const;
	bool		operator == (const GrPadding& padding) const;

#ifdef _MANAGED
	GrPadding(System::Windows::Forms::Padding% padding);

	operator		System::Windows::Forms::Padding ();
	operator		System::Windows::Forms::Padding () const;
	void operator = (System::Windows::Forms::Padding% padding);
#endif

	int left, top, right, bottom;

	static const GrPadding		Empty;
	static const GrPadding		Default;
};

struct GrColor
{
	struct GrColorByte
	{
		byte		b;
		byte		g;
		byte		r;
		byte		a;
	};

	GrColor();
	GrColor(int clr);
	GrColor(int a, int r, int g, int b);
	GrColor(int r, int g, int b);

#ifdef _MANAGED
	GrColor(System::Drawing::Color Color);
	operator System::Drawing::Color ();
	operator System::Drawing::Color () const;
#endif
	void		operator *= (float f);
	GrColor		operator * (float f) const;
	GrColor		operator * (const GrColor& color) const;
	bool		operator != (const GrColor& color) const;
	bool		operator == (const GrColor& color) const;

	union
	{
		GrColorByte bytes;
		int			value;
	};

	ulong		ToRGB() const;

	float	A() const;
	float	R() const;
	float	G() const;
	float	B() const;

	void	A(float value);
	void	R(float value);
	void	G(float value);
	void	B(float value);

	static const GrColor Empty;
	static const GrColor AliceBlue;
	static const GrColor AntiqueWhite;
	static const GrColor Aqua;
	static const GrColor Aquamarine;
	static const GrColor Azure;
	static const GrColor Beige;
	static const GrColor Bisque;
	static const GrColor Black;
	static const GrColor BlanchedAlmond;
	static const GrColor Blue;
	static const GrColor BlueViolet;
	static const GrColor Brown;
	static const GrColor BurlyWood;
	static const GrColor CadetBlue;
	static const GrColor Chartreuse;
	static const GrColor Chocolate;
	static const GrColor Coral;
	static const GrColor CornflowerBlue;
	static const GrColor Cornsilk;
	static const GrColor Crimson;
	static const GrColor Cyan;
	static const GrColor DarkBlue;
	static const GrColor DarkCyan;
	static const GrColor DarkGoldenrod;
	static const GrColor DarkGray;
	static const GrColor DarkGreen;
	static const GrColor DarkKhaki;
	static const GrColor DarkMagenta;
	static const GrColor DarkOliveGreen;
	static const GrColor DarkOrange;
	static const GrColor DarkOrchid;
	static const GrColor DarkRed;
	static const GrColor DarkSalmon;
	static const GrColor DarkSeaGreen;
	static const GrColor DarkSlateBlue;
	static const GrColor DarkSlateGray;
	static const GrColor DarkTurquoise;
	static const GrColor DarkViolet;
	static const GrColor DeepPink;
	static const GrColor DeepSkyBlue;
	static const GrColor DimGray;
	static const GrColor DodgerBlue;
	static const GrColor Firebrick;
	static const GrColor FloralWhite;
	static const GrColor ForestGreen;
	static const GrColor Fuchsia;
	static const GrColor Gainsboro;
	static const GrColor GhostWhite;
	static const GrColor Gold;
	static const GrColor Goldenrod;
	static const GrColor Gray;
	static const GrColor Green;
	static const GrColor GreenYellow;
	static const GrColor Honeydew;
	static const GrColor HotPink;
	static const GrColor IndianRed;
	static const GrColor Indigo;
	static const GrColor Ivory;
	static const GrColor Khaki;
	static const GrColor Lavender;
	static const GrColor LavenderBlush;
	static const GrColor LawnGreen;
	static const GrColor LemonChiffon;
	static const GrColor LightBlue;
	static const GrColor LightCoral;
	static const GrColor LightCyan;
	static const GrColor LightGoldenrodYellow;
	static const GrColor LightGray;
	static const GrColor LightGreen;
	static const GrColor LightPink;
	static const GrColor LightSalmon;
	static const GrColor LightSeaGreen;
	static const GrColor LightSkyBlue;
	static const GrColor LightSlateGray;
	static const GrColor LightSteelBlue;
	static const GrColor LightYellow;
	static const GrColor Lime;
	static const GrColor LimeGreen;
	static const GrColor Linen;
	static const GrColor Magenta;
	static const GrColor Maroon;
	static const GrColor MediumAquamarine;
	static const GrColor MediumBlue;
	static const GrColor MediumOrchid;
	static const GrColor MediumPurple;
	static const GrColor MediumSeaGreen;
	static const GrColor MediumSlateBlue;
	static const GrColor MediumSpringGreen;
	static const GrColor MediumTurquoise;
	static const GrColor MediumVioletRed;
	static const GrColor MidnightBlue;
	static const GrColor MintCream;
	static const GrColor MistyRose;
	static const GrColor Moccasin;
	static const GrColor NavajoWhite;
	static const GrColor Navy;
	static const GrColor OldLace;
	static const GrColor Olive;
	static const GrColor OliveDrab;
	static const GrColor Orange;
	static const GrColor OrangeRed;
	static const GrColor Orchid;
	static const GrColor PaleGoldenrod;
	static const GrColor PaleGreen;
	static const GrColor PaleTurquoise;
	static const GrColor PaleVioletRed;
	static const GrColor PapayaWhip;
	static const GrColor PeachPuff;
	static const GrColor Peru;
	static const GrColor Pink;
	static const GrColor Plum;
	static const GrColor PowderBlue;
	static const GrColor Purple;
	static const GrColor Red;
	static const GrColor RosyBrown;
	static const GrColor RoyalBlue;
	static const GrColor SaddleBrown;
	static const GrColor Salmon;
	static const GrColor SandyBrown;
	static const GrColor SeaGreen;
	static const GrColor SeaShell;
	static const GrColor Sienna;
	static const GrColor Silver;
	static const GrColor SkyBlue;
	static const GrColor SlateBlue;
	static const GrColor SlateGray;
	static const GrColor Snow;
	static const GrColor SpringGreen;
	static const GrColor SteelBlue;
	static const GrColor Tan;
	static const GrColor Teal;
	static const GrColor Thistle;
	static const GrColor Tomato;
	static const GrColor Turquoise;
	static const GrColor Violet;
	static const GrColor Wheat;
	static const GrColor White;
	static const GrColor WhiteSmoke;
	static const GrColor Yellow;
	static const GrColor YellowGreen;
};

struct GrFlag
{
public:
	GrFlag();
	GrFlag(ulong flag);
	ulong		Add(ulong flag, ulong mask = 0xffffffff);
	ulong		Remove(ulong flag, ulong mask = 0xffffffff);
	void		Clear();
	bool		Has(ulong flag, ulong mask = 0xffffffff) const;
	ulong		Get(ulong mask = 0xffffffff) const;

	ulong		operator += (ulong flag);
	ulong		operator -= (ulong flag);
	void		operator = (ulong flag);

	ulong		operator + (ulong flag) const;
	ulong		operator - (ulong flag) const;

	operator	ulong() const;

private:
	ulong		m_flag;
};

enum GrNotify
{
	GrNotify_TextChanged,
	GrNotify_WidthChanged,
	GrNotify_HeightChanged,
	GrNotify_AlignChanged,

	GrNotify_Count,
};

enum GrSelectionType
{
	GrSelectionType_Normal,
	GrSelectionType_Add,
	GrSelectionType_Remove,
};

enum GrRowHighlightType
{
	GrRowHighlightType_Fill,
	GrRowHighlightType_Line,
	GrRowHighlightType_Both,
};

template<typename T, T V>
struct GrRange
{
public:
	typename typedef T value_type;
	GrRange() : m_minValue(V), m_maxValue(V)
	{
	}

	GrRange(T minValue, T maxValue)
	{
		SetRange(minValue, maxValue);	
	}

	inline bool		IsIn(T value) const
	{
		if(value < m_minValue || value >= m_maxValue)
			return false;
		return true;
	}

	T			GetLength() const { return m_maxValue - m_minValue; }

	bool		operator == (const GrRange<T, V>& range) const
	{
		if(m_minValue != range.m_minValue || m_maxValue != range.m_maxValue)
			return false;
		return true;
	}

	bool		operator != (const GrRange<T, V>& range) const
	{
		if(m_minValue == range.m_minValue && m_maxValue == range.m_maxValue)
			return false;
		return true;
	}

	T			GetMinValue() const
	{
		return m_minValue; 
	}

	T			GetMaxValue() const
	{
		return m_maxValue; 
	}

	void		SetMinValue(T value)
	{
		if(value > m_maxValue)
		{
			m_minValue = m_maxValue;
			m_maxValue = value;
		}
		else
		{
			m_minValue = value;
		}
	}

	void		SetMaxValue(T value)
	{
		if(value < m_minValue)
		{
			m_maxValue = m_minValue;
			m_minValue = value;
		}
		else
		{
			m_maxValue = value;
		}
	}

	void		SetRange(T minValue, T maxValue)
	{
		if(minValue > maxValue)
		{
			m_minValue = maxValue;
			m_maxValue = minValue;
		}
		else
		{
			m_minValue = minValue;
			m_maxValue = maxValue;
		}
	}

public:
	static GrRange<T, V> Empty;

private:
	T	m_minValue;
	T	m_maxValue;
};

template<typename T, T V>
GrRange<T, V> GrRange<T, V>::Empty;

typedef GrRange<int, 0>		GrIntRange;
typedef GrRange<uint, 0>	GrIndexRange;

#define foreach_range(_class_type, _instance, _value) \
		for(_class_type::value_type _value = _instance.GetMinValue() ; _value < _instance.GetMaxValue() ; _value++)