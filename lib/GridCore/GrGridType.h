﻿#pragma once
#define _SECURE_SCL 0
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <algorithm>

#pragma warning(disable:4201) // warning C4201: 비표준 확장이 사용됨 : 구조체/공용 구조체의 이름이 없습니다.
#pragma warning(disable:4481) // warning C4481: 비표준 확장이 사용되었습니다. 'sealed' 지정자를 재정의합니다.

typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned char	byte;

#define SPLITTER_ROW_START	(ETC_ROW)
#define MAX_SPLITTER_ROW	(1000)
#define SPLITTER_ROW_END	(ETC_ROW - MAX_SPLITTER_ROW)

#define GROUPING_ROW_START	(SPLITTER_ROW_END)

#define DEF_GROUP_WIDTH		15
#define DEF_MARGIN			1
#define DEF_CONTROL_WIDTH	17
#define DEF_SPLITTER_SIZE	5

#define DEF_LEADING			5

#ifdef _MANAGED
#define Thow_Exception(_message) throw gcnew System::Exception(_message)
#else
#define Thow_Exception(_message) throw new std::exception(_message)
#endif

#ifdef _MANAGED
#define Thow_Exception_(_type, _message) throw gcnew System::_type(_message)
#else
#define Thow_Exception_(_type, _message) throw new std::exception(_message)
#endif

#define for_stl_const(_type, _var, _itor) for(_type::const_iterator _itor = (_var).begin() ; _itor != (_var).end() ; _itor++)
#define for_stl(type, var, itor) for(type::iterator itor = (var).begin() ; itor != (var).end() ; itor++)
#define for_stl_const_ptr(type, var_ptr, itor) for(type::const_iterator itor = var_ptr->begin() ; itor != var_ptr->end() ; itor++)
#define for_stl_ptr(type, var_ptr, itor) for(type::iterator itor = var_ptr->begin() ; itor != var_ptr->end() ; itor++)

#define for_stl_const_reverse(type, var, itor) for(type::const_reverse_iterator itor = (var).rbegin() ; itor != (var).rend() ; itor++)
#define for_stl_reverse(type, var, itor) for(type::reverse_iterator itor = (var).rbegin() ; itor != (var).rend() ; itor++)
#define for_stl_const_reverse_ptr(type, var_ptr, itor) for(type::const_reverse_iterator itor = var_ptr->rbegin() ; itor != var_ptr->rend() ; itor++)
#define for_stl_reverse_ptr(type, var_ptr, itor) for(type::reverse_iterator itor = var_ptr->rbegin() ; itor != var_ptr->rend() ; itor++)

#define for_stl_map(key, type, var, itor) for(map<key, type>::iterator itor = (var).begin() ; itor != (var).end() ; itor++)
#define for_stl_map_const(key, type, var, itor) for(map<key, type>::const_iterator itor = (var).begin() ; itor != (var).end() ; itor++)
#define for_stl_map_ptr(key, type, var_ptr, itor) for(map<key, type>::iterator itor = var_ptr->begin() ; itor != var_ptr->end() ; itor++)
#define for_stl_map_const_ptr(key, type, var_ptr, itor) for(map<key, type>::const_iterator itor = var_ptr->begin() ; itor != var_ptr->end() ; itor++)

class GrState
{
public:
	enum Type
	{
		Normal,
		MouseOver,
		Pressed,
		Selected,

		Count,
	};
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

class GrSort
{
public:
	enum Type
	{
		None,
		Up,
		Down,

		Count,
	};
};

typedef unsigned int GrColID;
typedef unsigned int GrRowID;

class GrColumn;
class GrDataRow;
typedef std::set<GrColumn*>			GrColumns;
typedef std::set<GrDataRow*>		GrDataRows;

struct GrID
{
public:
	bool operator < (const GrID& id) const
	{
		if(id0 == id.id0)
			return id1 < id.id1;
		return id0 < id.id0;
	}

	uint	id0;
	uint	id1;
};

//class GrCellID
//{
//public:
//	GrCellID() {}
//	GrCellID(uint c, uint r) : col(c), row(r) {}
//
//	bool operator < (const GrCellID& n) const;
//	bool operator == (const GrCellID& n) const;
//	bool operator != (const GrCellID& n) const;
//
//	GrColID		GetColumnID() const { return col; }
//	void		SetColumnID(GrColID columnID) { col = columnID; }
//	GrRowID		GetRowID() const { return row; }
//	void		SetRowID(GrRowID rowID) { row = rowID; }
//
//	GrColID		col;
//	GrRowID		row;
//
//#ifdef _MANAGED1
//	typedef Ntreev::Windows::Forms::Grid::CellID _CellID;
//	GrCellID(_CellID Index);
//	operator _CellID ();
//	operator _CellID () const;
//	void operator = (_CellID Index);
//#endif
//};

class GrPoint
{
public:
	GrPoint(){}
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

	static const GrPoint	Empty;
};

class GrSize
{
public:
	GrSize() {}
	GrSize(int w, int h) : width(w), height(h) {}

#ifdef _MANAGED
	GrSize(System::Drawing::Size size) : width(size.Width), height(size.Height) {}
#endif

	int width;
	int height;

	static const GrSize		Empty;
};

class GrRect
{
public:
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

	static const GrRect			Empty;
};

class GrPadding
{
public:
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

class GrColor
{
public:
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
		struct 
		{
			byte		b;
			byte		g;
			byte		r;
			byte		a;
		};
		int	value;
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

class GrFlag
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
class GrRange
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