//=====================================================================================================================
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
#include "GridCollection.h"
#include "GridNativeCollection.h"
#include "GrGridType.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
	class FontConverter
	{
	public:
		static _Font^ StoT(GrFont* pFont);
		static GrFont* TtoS(_Font^ font);
	};

	[System::ComponentModel::TypeConverter(System::ComponentModel::ExpandableObjectConverter::typeid)]
	public ref class Style : object
	{
		typedef Ntreev::Windows::Forms::Grid::Private::NativeCollection<_Color, GrColor>	_Colors;
		typedef System::Collections::Generic::ICollection<_Color>	_Colors2;

		typedef Ntreev::Windows::Forms::Grid::Private::NativeCollection<_Font^, GrFont*, FontConverter>	_Fonts;
		typedef System::Collections::Generic::ICollection<_Font^>	_Fonts2;

		//typedef System::Collections::Specialized::NotifyCollectionChangedEventArgs	_NotifyCollectionChangedEventArgs;

	public:
		Style();

		void					ResetRowCellForeColors();
		void					ResetRowCellBackColors();
		void					ResetRowCellFonts();

		void					ResetSelectedForeColor();
		void					ResetSelectedBackColor();

		void					ResetFocusedForeColor();
		void					ResetFocusedBackColor();

		void					ResetCellForeColor();
		void					ResetCellBackColor();
		void					ResetCellFont();

		void					ResetRowHighlightForeColor();
		void					ResetRowHighlightBackColor();


		property _Color CellForeColor
		{
			_Color get();
			void set(_Color);
		}

		property _Color CellBackColor
		{
			_Color get();
			void set(_Color);
		}

		property _Font^ CellFont
		{
			_Font^ get();
			void set(_Font^);
		}

		property _Color			SelectedForeColor	{ _Color get(); void set(_Color); }
		property _Color			SelectedBackColor	{ _Color get(); void set(_Color); }

		property _Color			FocusedForeColor	{ _Color get(); void set(_Color); }
		property _Color			FocusedBackColor	{ _Color get(); void set(_Color); }

		property _Color			RowHighlightForeColor{ _Color get(); void set(_Color); }
		property _Color			RowHighlightBackColor{ _Color get(); void set(_Color); }

		property _Colors2^		RowCellForeColors	{ _Colors2^	get(); }
		property _Colors2^		RowCellBackColors	{ _Colors2^	get(); }
		property _Fonts2^		RowCellFonts		{ _Fonts2^	get(); }

		property _Colors2^		GroupingBackColors	{ _Colors2^	get(); }
		property _Colors2^		GroupingForeColors	{ _Colors2^	get(); }
		property _Fonts2^		GroupingFonts		{ _Fonts2^	get(); }

	//private:
	//	void					OnRowCellForeColorChanged(object^ /*sender*/, _NotifyCollectionChangedEventArgs^ e);
	//	void					OnRowCellBackColorChanged(object^ /*sender*/, _NotifyCollectionChangedEventArgs^ e);

	internal:
		property GrStyle*		NativeStyle			{ GrStyle* get(); }

	private:
		_Colors^				m_rowCellForeColors;
		_Colors^				m_rowCellBackColors;
		_Fonts^					m_rowCellFonts;

		_Colors^				m_groupingForeColors;
		_Colors^				m_groupingBackColors;
		_Fonts^					m_groupingFonts;

		GrStyle*				m_pNativeStyle;
	};
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/