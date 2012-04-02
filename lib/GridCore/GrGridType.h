//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4475.19551
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
#ifndef _SECURE_SCL
#define _SECURE_SCL 0
#endif
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <exception>


#ifdef _WIN64
typedef unsigned __int64 uint;
#else
typedef unsigned __int32 uint;
#endif

typedef unsigned long ulong;
typedef unsigned char byte;

static const int DEF_GROUP_WIDTH = 15;
static const int DEF_CONTROL_WIDTH = 17;
static const int DEF_ICON_SIZE = 20;


#ifdef _MANAGED
#define _Exception(_message) gcnew System::Exception(_message)
#else
#ifdef _MSC_VER 
#define _Exception(_message) new std::exception(_message)
#else
#define _Exception(_message) new std::exception()
#endif
#endif

#ifdef _MANAGED
#define _Exception_(_type, _message) gcnew System::_type(_message)
#else
#ifdef _MSC_VER 
#define _Exception_(_type, _message) new std::exception(_message)
#else
#define _Exception_(_type, _message) new std::exception()
#endif

#endif

template<typename T>
class GrEnumerator 
{
public:
    typedef typename T::value_type ValueType;
    typedef typename T::size_type SizeType;
    typedef typename T::const_iterator IteratorType;

public:
    GrEnumerator(const T& container)
        : m_container(&container), m_iterator(m_container->begin())
    {
        m_index = (SizeType)-1;
    }

    GrEnumerator(const GrEnumerator<T>& instance)
        : m_container(instance.m_container), m_iterator(m_container->begin())
    {
        m_index = (SizeType)-1;
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
    const T* m_container;
    IteratorType m_iterator;
    ValueType m_value;
    SizeType m_index;
}; 

template<typename T, typename U>
class GrEnumerator< std::map<T, U> >
{
public:
    typedef typename std::map<T, U> ContainerType;
    typedef typename ContainerType::key_type KeyType;
    typedef typename ContainerType::mapped_type ValueType;
    typedef typename ContainerType::size_type SizeType;
    typedef typename ContainerType::iterator IteratorType;

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
    ContainerType* m_container;
    IteratorType m_value;
    IteratorType m_iterator;
    SizeType m_index;
};

#define for_each(_container_type, _container_instance, _value_name) \
    for(GrEnumerator< _container_type > _value_name(_container_instance) ; _value_name.Next() == true ; )

#define for_each_map(_key_type, _value_type, _container_instance, _value_name) \
    for(GrEnumerator< std::map<_key_type, _value_type> > _value_name(_container_instance) ; _value_name.Next() == true ; )

enum GrControlState
{
    GrControlState_Normal,
    GrControlState_Hot,
    GrControlState_Pressed,
    GrControlState_Disabled,

    GrControlState_Count,
}; 

enum GrCellType
{
    GrCellType_Root,
    GrCellType_Column,
    GrCellType_Row,
    GrCellType_Item,
    GrCellType_Group,
    GrCellType_GroupHeader,
    GrCellType_Splitter,

    GrCellType_Unknown,

    GrCellType_Count,
};

enum GrRowType
{
    GrRowType_Caption,
    GrRowType_GroupPanel,
    GrRowType_InsertionRow,
    GrRowType_DataRow,
    GrRowType_Row,
    GrRowType_ColumnList,
    GrRowType_DataRowList,
    GrRowType_GroupRow,
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

struct GrPoint
{
    GrPoint();
    GrPoint(int x, int y);

    bool operator == (const GrPoint& point) const;
    bool operator != (const GrPoint& point) const;

    void operator += (const GrPoint& point);
    void operator -= (const GrPoint& point);

    GrPoint operator + (const GrPoint& point);
    GrPoint operator - (const GrPoint& point);

    GrPoint operator + (const GrPoint& point) const;
    GrPoint operator - (const GrPoint& point) const;

    GrPoint operator - () const;
    GrPoint operator + () const;

#ifdef _MANAGED
    GrPoint(System::Drawing::Point point);
    void operator = (System::Drawing::Point point);
    operator System::Drawing::Point ();
    operator System::Drawing::Point () const;
#endif

    int x;
    int y;

    static const GrPoint Empty;
};

struct GrSize
{
    GrSize();
    GrSize(int width, int height);

#ifdef _MANAGED
    GrSize(System::Drawing::Size size);
    void operator = (System::Drawing::Size size);
    operator System::Drawing::Size ();
    operator System::Drawing::Size () const;
#endif

    int width;
    int height;

    static const GrSize Empty;
};

struct GrPadding;


struct GrRect
{
    GrRect();
    GrRect(int left, int top, int right, int bottom);
    GrRect(const GrPoint& location, const GrSize& size);
    GrRect(const GrPoint& pt1, const GrPoint& pt2);

    bool Contains(const GrPoint& location) const;
    bool Contains(const GrRect& rect) const;
    bool IsEmpty() const;

    int GetWidth() const;
    int GetHeight() const;

    GrSize GetSize() const;
    void SetSize(const GrSize& size);
    void SetSize(int width, int height);


    GrPoint GetCenter() const;
    GrPoint GetLocation() const;
    void SetLocation(const GrPoint& location);
    void SetLocation(int x, int y);

    void Offset(int x, int y);
    void Offset(const GrPoint& offset);

    void Expand(int left, int top, int right, int bottom);
    void Expand(const GrPadding& padding);
    void Contract(int left, int top, int right, int bottom);
    void Contract(const GrPadding& padding);

    void DoEmpty();

    void operator += (const GrRect& rect);
    bool operator == (const GrRect& rect) const;
    bool operator != (const GrRect& rect) const;
    GrRect operator + (const GrPoint& point) const;
    GrRect operator - (const GrPoint& point) const;

    GrRect operator + (const GrPadding& padding) const;
    GrRect operator - (const GrPadding& padding) const;
    void operator += (const GrPadding& padding);
    void operator -= (const GrPadding& padding);

#ifdef _MANAGED
    GrRect(System::Drawing::Rectangle% rect);
    GrRect(System::Drawing::RectangleF% rect);

    bool Contains(System::Drawing::Point% location) const;
    operator System::Drawing::Rectangle ();
    operator System::Drawing::Rectangle () const;
    void operator = (System::Drawing::Rectangle% rect);
#endif

    int left, top, right, bottom;

    static const GrRect Empty;
};

struct GrPadding
{
    GrPadding();
    GrPadding(int left, int top, int right, int bottom);

    int GetHorizontal() const;
    int GetVertical() const;

    operator GrRect();
    operator GrRect() const;

    bool operator != (const GrPadding& padding) const;
    bool operator == (const GrPadding& padding) const;

#ifdef _MANAGED
    GrPadding(System::Windows::Forms::Padding% padding);

    operator System::Windows::Forms::Padding ();
    operator System::Windows::Forms::Padding () const;
    void operator = (System::Windows::Forms::Padding% padding);
#endif

    int left, top, right, bottom;

    static const GrPadding Empty;
    static const GrPadding Default;
};

struct GrColor
{
    GrColor();
    GrColor(int argb);
    GrColor(int a, int r, int g, int b);
    GrColor(int r, int g, int b);

#ifdef _MANAGED
    GrColor(System::Drawing::Color color);
    operator System::Drawing::Color ();
    operator System::Drawing::Color () const;
#endif
    void operator *= (float f);
    GrColor operator * (float f) const;
    GrColor operator * (const GrColor& color) const;
    bool operator != (const GrColor& color) const;
    bool operator == (const GrColor& color) const;

#pragma warning(push)
#pragma warning(disable:4201)
    union
    {
        struct
        {
            byte b, g, r, a;
        };
        int value;
    };
#pragma warning(pop)

    float A() const;
    float R() const;
    float G() const;
    float B() const;

    void A(float value);
    void R(float value);
    void G(float value);
    void B(float value);

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
    ulong Add(ulong flag, ulong mask = 0xffffffff);
    ulong Remove(ulong flag, ulong mask = 0xffffffff);
    void Clear();
    bool Has(ulong flag, ulong mask = 0xffffffff) const;
    ulong Get(ulong mask = 0xffffffff) const;

    ulong operator += (ulong flag);
    ulong operator -= (ulong flag);
    void operator = (ulong flag);

    ulong operator + (ulong flag) const;
    ulong operator - (ulong flag) const;

    operator ulong() const;

private:
    ulong m_flag;
};

enum GrSelectionRange
{
    GrSelectionRange_One,
    GrSelectionRange_Multi,
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

template<typename T>
struct GrRange
{
public:
    GrRange() : m_minValue(0), m_maxValue(0)
    {
    }

    GrRange(T minValue, T maxValue)
    {
        SetRange(minValue, maxValue); 
    }

    inline bool IsIn(T value) const
    {
        if(value < m_minValue || value >= m_maxValue)
            return false;
        return true;
    }

    T GetLength() const { return m_maxValue - m_minValue; }

    bool operator == (const GrRange<T>& range) const
    {
        if(m_minValue != range.m_minValue || m_maxValue != range.m_maxValue)
            return false;
        return true;
    }

    bool operator != (const GrRange<T>& range) const
    {
        if(m_minValue == range.m_minValue && m_maxValue == range.m_maxValue)
            return false;
        return true;
    }

    T GetMinValue() const
    {
        return m_minValue; 
    }

    T GetMaxValue() const
    {
        return m_maxValue; 
    }

    void SetMinValue(T value)
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

    void SetMaxValue(T value)
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

    void SetRange(T minValue, T maxValue)
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
    static GrRange<T> Empty;

private:
    T m_minValue;
    T m_maxValue;
};

template<typename T>
GrRange<T> GrRange<T>::Empty;

typedef GrRange<int> GrIntRange;
typedef GrRange<uint> GrIndexRange;

#define foreach_range(_class_type, _instance, _value) \
    for(_class_type::value_type _value = _instance.GetMinValue() ; _value < _instance.GetMaxValue() ; _value++)

struct GrHitTest
{
    GrHitTest();

    class GrCell* pHitted;
    GrPoint localHit;

    static GrHitTest Empty;
};

enum GrKeys
{
    GrKeys_Modifiers = -65536,
    GrKeys_None = 0,
    GrKeys_LButton = 1,
    GrKeys_RButton = 2,
    GrKeys_Cancel = 3,
    GrKeys_MButton = 4,
    GrKeys_XButton1 = 5,
    GrKeys_XButton2 = 6,
    GrKeys_Back = 8,
    GrKeys_Tab = 9,
    GrKeys_LineFeed = 10,
    GrKeys_Clear = 12,
    GrKeys_Enter = 13,
    GrKeys_Return = 13,
    GrKeys_ShiftKey = 16,
    GrKeys_ControlKey = 17,
    GrKeys_Menu = 18,
    GrKeys_Pause = 19,
    GrKeys_CapsLock = 20,
    GrKeys_Capital = 20,
    GrKeys_KanaMode = 21,
    GrKeys_HanguelMode = 21,
    GrKeys_HangulMode = 21,
    GrKeys_JunjaMode = 23,
    GrKeys_FinalMode = 24,
    GrKeys_KanjiMode = 25,
    GrKeys_HanjaMode = 25,
    GrKeys_Escape = 27,
    GrKeys_IMEConvert = 28,
    GrKeys_IMENonconvert = 29,
    GrKeys_IMEAceept = 30,
    GrKeys_IMEAccept = 30,
    GrKeys_IMEModeChange = 31,
    GrKeys_Space = 32,
    GrKeys_Prior = 33,
    GrKeys_PageUp = 33,
    GrKeys_Next = 34,
    GrKeys_PageDown = 34,
    GrKeys_End = 35,
    GrKeys_Home = 36,
    GrKeys_Left = 37,
    GrKeys_Up = 38,
    GrKeys_Right = 39,
    GrKeys_Down = 40,
    GrKeys_Select = 41,
    GrKeys_Print = 42,
    GrKeys_Execute = 43,
    GrKeys_PrintScreen = 44,
    GrKeys_Snapshot = 44,
    GrKeys_Insert = 45,
    GrKeys_Delete = 46,
    GrKeys_Help = 47,
    GrKeys_D0 = 48,
    GrKeys_D1 = 49,
    GrKeys_D2 = 50,
    GrKeys_D3 = 51,
    GrKeys_D4 = 52,
    GrKeys_D5 = 53,
    GrKeys_D6 = 54,
    GrKeys_D7 = 55,
    GrKeys_D8 = 56,
    GrKeys_D9 = 57,
    GrKeys_A = 65,
    GrKeys_B = 66,
    GrKeys_C = 67,
    GrKeys_D = 68,
    GrKeys_E = 69,
    GrKeys_F = 70,
    GrKeys_G = 71,
    GrKeys_H = 72,
    GrKeys_I = 73,
    GrKeys_J = 74,
    GrKeys_K = 75,
    GrKeys_L = 76,
    GrKeys_M = 77,
    GrKeys_N = 78,
    GrKeys_O = 79,
    GrKeys_P = 80,
    GrKeys_Q = 81,
    GrKeys_R = 82,
    GrKeys_S = 83,
    GrKeys_T = 84,
    GrKeys_U = 85,
    GrKeys_V = 86,
    GrKeys_W = 87,
    GrKeys_X = 88,
    GrKeys_Y = 89,
    GrKeys_Z = 90,
    GrKeys_LWin = 91,
    GrKeys_RWin = 92,
    GrKeys_Apps = 93,
    GrKeys_Sleep = 95,
    GrKeys_NumPad0 = 96,
    GrKeys_NumPad1 = 97,
    GrKeys_NumPad2 = 98,
    GrKeys_NumPad3 = 99,
    GrKeys_NumPad4 = 100,
    GrKeys_NumPad5 = 101,
    GrKeys_NumPad6 = 102,
    GrKeys_NumPad7 = 103,
    GrKeys_NumPad8 = 104,
    GrKeys_NumPad9 = 105,
    GrKeys_Multiply = 106,
    GrKeys_Add = 107,
    GrKeys_Separator = 108,
    GrKeys_Subtract = 109,
    GrKeys_Decimal = 110,
    GrKeys_Divide = 111,
    GrKeys_F1 = 112,
    GrKeys_F2 = 113,
    GrKeys_F3 = 114,
    GrKeys_F4 = 115,
    GrKeys_F5 = 116,
    GrKeys_F6 = 117,
    GrKeys_F7 = 118,
    GrKeys_F8 = 119,
    GrKeys_F9 = 120,
    GrKeys_F10 = 121,
    GrKeys_F11 = 122,
    GrKeys_F12 = 123,
    GrKeys_F13 = 124,
    GrKeys_F14 = 125,
    GrKeys_F15 = 126,
    GrKeys_F16 = 127,
    GrKeys_F17 = 128,
    GrKeys_F18 = 129,
    GrKeys_F19 = 130,
    GrKeys_F20 = 131,
    GrKeys_F21 = 132,
    GrKeys_F22 = 133,
    GrKeys_F23 = 134,
    GrKeys_F24 = 135,
    GrKeys_NumLock = 144,
    GrKeys_Scroll = 145,
    GrKeys_LShiftKey = 160,
    GrKeys_RShiftKey = 161,
    GrKeys_LControlKey = 162,
    GrKeys_RControlKey = 163,
    GrKeys_LMenu = 164,
    GrKeys_RMenu = 165,
    GrKeys_BrowserBack = 166,
    GrKeys_BrowserForward = 167,
    GrKeys_BrowserRefresh = 168,
    GrKeys_BrowserStop = 169,
    GrKeys_BrowserSearch = 170,
    GrKeys_BrowserFavorites = 171,
    GrKeys_BrowserHome = 172,
    GrKeys_VolumeMute = 173,
    GrKeys_VolumeDown = 174,
    GrKeys_VolumeUp = 175,
    GrKeys_MediaNextTrack = 176,
    GrKeys_MediaPreviousTrack = 177,
    GrKeys_MediaStop = 178,
    GrKeys_MediaPlayPause = 179,
    GrKeys_LaunchMail = 180,
    GrKeys_SelectMedia = 181,
    GrKeys_LaunchApplication1 = 182,
    GrKeys_LaunchApplication2 = 183,
    GrKeys_Oem1 = 186,
    GrKeys_OemSemicolon = 186,
    GrKeys_Oemplus = 187,
    GrKeys_Oemcomma = 188,
    GrKeys_OemMinus = 189,
    GrKeys_OemPeriod = 190,
    GrKeys_OemQuestion = 191,
    GrKeys_Oem2 = 191,
    GrKeys_Oemtilde = 192,
    GrKeys_Oem3 = 192,
    GrKeys_Oem4 = 219,
    GrKeys_OemOpenBrackets = 219,
    GrKeys_OemPipe = 220,
    GrKeys_Oem5 = 220,
    GrKeys_Oem6 = 221,
    GrKeys_OemCloseBrackets = 221,
    GrKeys_Oem7 = 222,
    GrKeys_OemQuotes = 222,
    GrKeys_Oem8 = 223,
    GrKeys_Oem102 = 226,
    GrKeys_OemBackslash = 226,
    GrKeys_ProcessKey = 229,
    GrKeys_Packet = 231,
    GrKeys_Attn = 246,
    GrKeys_Crsel = 247,
    GrKeys_Exsel = 248,
    GrKeys_EraseEof = 249,
    GrKeys_Play = 250,
    GrKeys_Zoom = 251,
    GrKeys_NoName = 252,
    GrKeys_Pa1 = 253,
    GrKeys_OemClear = 254,
    GrKeys_KeyCode = 65535,
    GrKeys_Shift = 65536,
    GrKeys_Control = 131072,
    GrKeys_Alt = 262144,
};

enum GrAutoFitColumnType
{
    GrAutoFitColumnType_ItemOnly,
    GrAutoFitColumnType_ColumnIncluded,
};

enum GrDataRowInsertType
{
    GrDataRowInsertType_New,
    GrDataRowInsertType_Insertion,
};