﻿//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
#include "GridBase.h"
#include "GridStyle.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    [System::FlagsAttribute]
    public enum class CellState : int
    {
        Normal = 0,
        Focused = 1,
        Hot = 2,
        Selected = 4,
        Pressed = 8,

        All = 0x0000000f,
    };

    /// <summary>
	/// 개체의 스타일 속성을 정의합니다.
	/// </summary>
	public interface class ICellBase
	{
		/// <summary>
		/// 개체의 전경색을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 전경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
		/// </returns>
		property System::Drawing::Color ForeColor
		{
			System::Drawing::Color get();
		}

		/// <summary>
		/// 개체의 배경색을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 배경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
		/// </returns>
		property System::Drawing::Color BackColor
		{
			System::Drawing::Color get();
		}

		/// <summary>
		/// 개체의 글꼴을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 글꼴색을 나타내는 <see cref="System::Drawing::Font"/>입니다.
		/// </returns>
		property System::Drawing::Font^ Font
		{
			System::Drawing::Font^ get();
		}

        property System::Drawing::Color PaintingForeColor
        {
            System::Drawing::Color get();
        }

        property System::Drawing::Color PaintingBackColor
        {
            System::Drawing::Color get();
        }

        property System::Drawing::Rectangle Bounds
        {
            System::Drawing::Rectangle get();
        }

        property int X
        {
            int get();
        }

        property int Y
        {
            int get();
        }

        property int Width
        {
            int get();
        }
    
        property int Height
        {
            int get();
        }

        property System::Drawing::Point Location
        {
            System::Drawing::Point get();
        }

        property System::Drawing::Size Size
        {
            System::Drawing::Size get();
        }

        property int Left
        {
            int get();
        }

        property int Top
        {
            int get();
        }

        property int Right
        {
            int get();
        }

        property int Bottom
        {
            int get();
        }

        property System::Drawing::Rectangle ClientRectangle
        {
            System::Drawing::Rectangle get();
        }

        property System::Windows::Forms::Padding Padding
        {
            System::Windows::Forms::Padding get();
        }

        property bool Multiline
        {
            bool get();
        }

        property bool WordWrap
        {
            bool get();
        }

        /// <summary>
		/// 셀의 수평 정렬방식을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 수평 정렬방식을 나타내는 <see cref="System::Drawing::StringAlignment"/>입니다.
		/// </returns>
		property System::Drawing::StringAlignment Alignment
		{
			System::Drawing::StringAlignment get(); 
		}

		/// <summary>
		/// 셀의 수직 정렬방식을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 수직 정렬방식을 나타내는 <see cref="System::Drawing::StringAlignment"/>입니다.
		/// </returns>
		property System::Drawing::StringAlignment LineAlignment
		{
			System::Drawing::StringAlignment get();
		}

        property CellState State
        {
            CellState get();
        }
	};

	/// <summary>
	/// 셀을 표시하기 위한 기본 방법을 제공합니다.
	/// </summary>
	public ref class CellBase abstract : GridObject, ICellBase
	{
	public: // methods
		/// <summary>
		/// 셀의 전경색을 기본값으로 되돌립니다.
		/// </summary>
		/// <remarks>
		/// 기본값은 <see cref="Design::Style::CellForeColor"/> 속성의 값입니다.
		/// </remarks>
		void ResetForeColor();

		/// <summary>
		/// 셀의 배경색을 기본값으로 되돌립니다.
		/// </summary>
		/// <remarks>
		/// 기본값은 <see cref="Design::Style::CellBackColor"/> 속성의 값입니다.
		/// </remarks>
		void ResetBackColor();
		
		/// <summary>
		/// 셀의 배경색을 기본값으로 되돌립니다.
		/// </summary>
		/// <remarks>
		/// 기본값은 <see cref="Design::Style::CellBackColor"/> 속성의 값입니다.
		/// </remarks>
		void ResetFont();

	public: // properties
		/// <summary>
		/// 셀의 전경색을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		///	셀의 전경색을 나타내는 <see cref="System::Drawing::Color"/>입니다. 기본값은 <see cref="Design::Style::CellForeColor"/> 속성의 값입니다.
		/// </returns>
		/// <remarks>
		/// 일반적으로 문자열의 색상을 나타냅니다.
		/// </remarks>
		[System::ComponentModel::DescriptionAttribute("셀의 전경색을 가져오거나 설정합니다.")]
		[System::ComponentModel::CategoryAttribute("Appearance")]
		property System::Drawing::Color ForeColor
		{
			virtual System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		/// <summary>
		/// 셀의 배경색을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		///	셀의 배경색을 나타내는 <see cref="System::Drawing::Color"/>입니다. 기본값은 <see cref="Design::Style::CellBackColor"/> 속성의 값입니다.
		/// </returns>
		[System::ComponentModel::DescriptionAttribute("셀의 배경색을 가져오거나 설정합니다.")]
		[System::ComponentModel::CategoryAttribute("Appearance")]
		property System::Drawing::Color BackColor
		{
			virtual System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		/// <summary>
		/// 셀의 글꼴을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		///	셀의 글꼴을 나타내는 <see cref="System::Drawing::Font"/>입니다. 기본값은 <see cref="Design::Style::CellFont"/> 속성의 값입니다.
		/// </returns>
		[System::ComponentModel::CategoryAttribute("Appearance"), System::ComponentModel::AmbientValue((System::String^)nullptr)]
		property System::Drawing::Font^ Font
		{ 
			virtual System::Drawing::Font^ get();
			void set(System::Drawing::Font^);
		}

		/// <summary>
		/// 화면에 표시되고 있는지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 화면상에 표시되고 있다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
		/// </returns>
#ifdef _DEBUG
		[System::ComponentModel::CategoryAttribute("Debug")]
#else
		[System::ComponentModel::BrowsableAttribute(false)]
#endif
		property bool IsDisplayable
		{ 
			bool get(); 
		}

		/// <summary>
		/// 셀에 대한 데이터가 들어 있는 개체를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 셀에 대한 데이터가 들어 있는 <see cref="System::Object"/>입니다. 기본값은 null입니다.
		/// </returns>
		[System::ComponentModel::TypeConverter(System::ComponentModel::StringConverter::typeid)]
		[System::ComponentModel::CategoryAttribute("Data"), System::ComponentModel::DefaultValueAttribute((System::String^)nullptr)]
		virtual property System::Object^ Tag;

		
		/// <summary>
		/// 화면에 표시되고 있는 셀의 영역을 가져옵니다.
		/// </summary>
		/// <remarks>
		/// 화면에 표시되지 않아도 셀의 영역을 가져올수 있습니다.
		/// </remarks>
		/// <returns>
		/// 화면에 표시되는 영역을 나타내는 <see cref="System::Drawing::Rectangle"/>입니다.
		/// </returns>
#ifdef _DEBUG
		[System::ComponentModel::CategoryAttribute("Debug")]
#else
		[System::ComponentModel::BrowsableAttribute(false)]
#endif
		property System::Drawing::Rectangle Bounds
		{
			virtual System::Drawing::Rectangle get() sealed;
		}

        property int X
        {
            virtual int get();
        }

        property int Y
        {
            virtual int get();
        }

    private:
        property int Width_ICellBase
        {
            virtual int get() sealed = ICellBase::Width::get;
        }
    public:

        property int Height
        {
            virtual int get();
        }

        property System::Drawing::Point Location
        {
            virtual System::Drawing::Point get();
        }

        property System::Drawing::Size Size
        {
            virtual System::Drawing::Size get();
        }

        property int Left
        {
            virtual int get();
        }

        property int Top
        {
            virtual int get();
        }

        property int Right
        {
            virtual int get();
        }

        property int Bottom
        {
            virtual int get();
        }

		/// <summary>
		/// 화면에 표시되고 있는 셀의 안쪽 영역을 가져옵니다.
		/// </summary>
		/// <remarks>
		/// 화면에 표시되지 않아도 셀의 안쪽 영역을 가져올수 있습니다.
		/// </remarks>
		/// <returns>
		/// 화면에 표시되는 안쪽 영역을 나타내는 <see cref="System::Drawing::Rectangle"/>입니다.
		/// </returns>
#ifdef _DEBUG
		[System::ComponentModel::CategoryAttribute("Debug")]
#else
		[System::ComponentModel::BrowsableAttribute(false)]
#endif
		property System::Drawing::Rectangle ClientRectangle
		{
			virtual System::Drawing::Rectangle get() sealed;
		}

		/// <summary>
		/// 셀의 내부 간격을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		///	셀의 내부 간격 특성을 나타내는 <see cref="System::Windows::Forms::Padding"/>입니다.
		/// </returns>
		[System::ComponentModel::DescriptionAttribute("셀의 안쪽 여백을 가져오거나 설정합니다.")]
		[System::ComponentModel::CategoryAttribute("Layout")]
		property System::Windows::Forms::Padding Padding 
		{
			virtual System::Windows::Forms::Padding get();
			virtual void set(System::Windows::Forms::Padding);
		}

	public: // events
		/// <summary>
		/// <see cref="ForeColor"/> 속성 값이 변경되면 발생합니다.
		/// </summary>
		[System::ComponentModel::DescriptionAttribute("전경색이 변경되면 발생합니다.")]
		event System::EventHandler^		ForeColorChanged
		{
			void add(System::EventHandler^ p);
			void remove(System::EventHandler^ p);
		private:
			void raise(System::Object^ sender, System::EventArgs^ e);
		}

		/// <summary>
		/// <see cref="BackColor"/> 속성 값이 변경되면 발생합니다.
		/// </summary>
		[System::ComponentModel::DescriptionAttribute("배경색이 변경되면 발생합니다.")]
		event System::EventHandler^		BackColorChanged
		{
			void add(System::EventHandler^ p);
			void remove(System::EventHandler^ p);
		private:
			void raise(System::Object^ sender, System::EventArgs^ e);
		}

		/// <summary>
		/// <see cref="Font"/> 속성 값이 변경되면 발생합니다.
		/// </summary>
		[System::ComponentModel::DescriptionAttribute("글꼴이 변경되면 발생합니다.")]
		event System::EventHandler^		FontChanged
		{
			void add(System::EventHandler^ p);
			void remove(System::EventHandler^ p);
		private:
			void raise(System::Object^ sender, System::EventArgs^ e);
		}

	internal: // methods
		CellBase(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrCell* pCell);

		CellBase(GrCell* pCell);

	internal: // properties
		property System::Drawing::Color DisplayForeColor
		{
			System::Drawing::Color get();
		}

		property System::Drawing::Color DisplayBackColor
		{
			System::Drawing::Color get();
		}

	protected: // methods
		/// <summary>
		/// <see cref="ForeColorChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnForeColorChanged(System::EventArgs^ e);

		/// <summary>
		/// <see cref="BackColorChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnBackColorChanged(System::EventArgs^ e);

		/// <summary>
		/// <see cref="FontChanged"/> 이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">
		/// 이벤트 데이터가 들어 있는 <see cref="System::EventArgs"/>입니다.
		/// </param>
		virtual void OnFontChanged(System::EventArgs^ e);

	private: // methods
		bool ShouldSerializeForeColor();
		bool ShouldSerializeBackColor();
		bool ShouldSerializeFont();
		bool ShouldSerializePadding();

		void ResetPadding();

    private: // properties
        property System::Drawing::Color PaintingForeColor_ICellBase
        {
            virtual System::Drawing::Color get() sealed = ICellBase::PaintingForeColor::get;
        }

        property System::Drawing::Color PaintingBackColor_ICellBase
        {
            virtual System::Drawing::Color get() sealed = ICellBase::PaintingBackColor::get;
        }

        property bool Multiline_ICellBase
        {
            virtual bool get() sealed = ICellBase::Multiline::get;
        }

        property bool WordWrap_ICellBase
        {
            virtual bool get() sealed = ICellBase::WordWrap::get;
        }

		property System::Drawing::StringAlignment Alignment_ICellBase
		{
			virtual System::Drawing::StringAlignment get() sealed = ICellBase::Alignment::get;
		}

		property System::Drawing::StringAlignment LineAlignment_ICellBase
		{
            virtual System::Drawing::StringAlignment get() sealed = ICellBase::LineAlignment::get;
		}

        property CellState State_ICellBase
        {
            virtual CellState get() sealed = ICellBase::State::get;
        }

	private: // variables
		GrCell*						m_pCell;
		System::EventHandler^				m_eventForeColorChanged;
		System::EventHandler^				m_eventBackColorChanged;
		System::EventHandler^				m_eventFontChanged;
	};

    public interface class ICell : ICellBase
    {
        property System::Object^ Value
        {
            System::Object^ get();
        }

        property System::Object^ Tag
        {
            System::Object^ get();
        }
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/