//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5792.31442
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
#include "GridObject.h"
#include "NativeCollection.h"
#include "GrGridType.h"
#include "StyleFontCollection.h"
#include "StyleColorCollection.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    [System::Drawing::ToolboxBitmapAttribute(Style::typeid)]
	[System::ComponentModel::TypeConverterAttribute(System::ComponentModel::ExpandableObjectConverter::typeid)]
	public ref class Style
		: Ntreev::Windows::Forms::Grid::GridObject
		, System::ComponentModel::IComponent
		, System::IServiceProvider
	{
	public: // methods

		Style();

		void ResetForeColor();
		void ResetBackColor();
		void ResetLineColor();
		void ResetFont();

		void ResetSelectedForeColor();
		void ResetSelectedBackColor();

		void ResetFocusedForeColor();
		void ResetFocusedBackColor();

		void ResetColumnForeColor();
		void ResetColumnBackColor();
		void ResetColumnLineColor();
		void ResetColumnFont();

		void ResetRowForeColor();
		void ResetRowBackColor();
		void ResetRowLineColor();
		void ResetRowFont();

        void ResetCaptionForeColor();
		void ResetCaptionBackColor();
		void ResetCaptionLineColor();
		void ResetCaptionFont();

        void ResetGroupPanelForeColor();
		void ResetGroupPanelBackColor();
		void ResetGroupPanelLineColor();
		void ResetGroupPanelFont();

		void ResetRowHighlightLineColor();
		void ResetRowHighlightFillColor();

		void ResetCellForeColors();
		void ResetCellBackColors();
		void ResetCellLineColors();
		void ResetCellFonts();

        void ResetGroupForeColors();
		void ResetGroupBackColors();
		void ResetGroupLineColors();
		void ResetGroupFonts();

	public: // properties

		[System::ComponentModel::CategoryAttribute("Appearance")]
		property System::Drawing::Color ForeColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Appearance")]
		property System::Drawing::Color BackColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Appearance")]
		property System::Drawing::Color LineColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Appearance")]
		property System::Drawing::Font^ Font
		{
			System::Drawing::Font^ get();
			void set(System::Drawing::Font^);
		}

		[System::ComponentModel::CategoryAttribute("Column")]
		property System::Drawing::Color ColumnForeColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Column")]
		property System::Drawing::Color ColumnBackColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Column")]
		property System::Drawing::Color ColumnLineColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Column")]
		property System::Drawing::Font^ ColumnFont
		{
			System::Drawing::Font^ get();
			void set(System::Drawing::Font^);
		}

		[System::ComponentModel::CategoryAttribute("Row")]
		property System::Drawing::Color RowForeColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Row")]
		property System::Drawing::Color RowBackColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Row")]
		property System::Drawing::Color RowLineColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Row")]
		property System::Drawing::Font^ RowFont
		{
			System::Drawing::Font^ get();
			void set(System::Drawing::Font^);
		}

        [System::ComponentModel::CategoryAttribute("Caption")]
		property System::Drawing::Color CaptionForeColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Caption")]
		property System::Drawing::Color CaptionBackColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Caption")]
		property System::Drawing::Color CaptionLineColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Caption")]
		property System::Drawing::Font^ CaptionFont
		{
			System::Drawing::Font^ get();
			void set(System::Drawing::Font^);
		}

        [System::ComponentModel::CategoryAttribute("Group")]
		property System::Drawing::Color GroupPanelForeColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Group")]
		property System::Drawing::Color GroupPanelBackColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Group")]
		property System::Drawing::Color GroupPanelLineColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Group")]
		property System::Drawing::Font^ GroupPanelFont
		{
			System::Drawing::Font^ get();
			void set(System::Drawing::Font^);
		}

		[System::ComponentModel::CategoryAttribute("Cell")]
		property System::Drawing::Color SelectedForeColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Cell")]
		property System::Drawing::Color SelectedBackColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Cell")]
		property System::Drawing::Color FocusedForeColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Cell")]
		property System::Drawing::Color FocusedBackColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Row")]
		property System::Drawing::Color RowHighlightLineColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Row")]
		property System::Drawing::Color RowHighlightFillColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color);
		}

		[System::ComponentModel::CategoryAttribute("Cell")]
		[System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property StyleColorCollection^ CellForeColors
		{
			StyleColorCollection^ get(); 
		}

		[System::ComponentModel::CategoryAttribute("Cell")]
		[System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property StyleColorCollection^ CellBackColors
		{
			StyleColorCollection^ get(); 
		}

		[System::ComponentModel::CategoryAttribute("Cell")]
		[System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property StyleColorCollection^ CellLineColors
		{
			StyleColorCollection^ get(); 
		}

		[System::ComponentModel::CategoryAttribute("Cell")]
		[System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property StyleFontCollection^ CellFonts
		{ 
			StyleFontCollection^ get();
		}

		[System::ComponentModel::CategoryAttribute("Group")]
		[System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property StyleColorCollection^ GroupForeColors
		{
			StyleColorCollection^ get();
		}

        [System::ComponentModel::CategoryAttribute("Group")]
		[System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property StyleColorCollection^ GroupBackColors
		{
			StyleColorCollection^ get(); 
		}

        [System::ComponentModel::CategoryAttribute("Group")]
		[System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property StyleColorCollection^ GroupLineColors
		{
			StyleColorCollection^ get(); 
		}

		[System::ComponentModel::CategoryAttribute("Group")]
		[System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property StyleFontCollection^ GroupFonts
		{
			StyleFontCollection^ get();
		}

		/// <summary>
		/// 열의 <see cref="System::ComponentModel::ISite"/>를 가져오거나 설정합니다.
		/// </summary>
		[System::ComponentModel::BrowsableAttribute(false)]
		property System::ComponentModel::ISite^ Site
		{
			virtual System::ComponentModel::ISite^ get() sealed;
			virtual void set(System::ComponentModel::ISite^ value) sealed;
		}

	public: // events

		/// <summary>
		/// 구성 요소가 삭제되는 경우 발생합니다.
		/// </summary>
		event System::EventHandler^ Disposed
		{
			virtual void add(System::EventHandler^ p) sealed { m_eventDisposed += p; }
			virtual void remove(System::EventHandler^ p) sealed { m_eventDisposed -= p; }
		private:
			virtual void raise(System::Object^ sender, System::EventArgs^ e) sealed { if(m_eventDisposed != nullptr) { m_eventDisposed->Invoke(sender, e); } }
		}

	protected:
		~Style();

	internal: // methods

		virtual void OnGridControlAttachedInternal() override;

		virtual void OnGridControlDetachedInternal() override;

	internal: // properties

		property GrStyle* NativeStyle
		{
			GrStyle* get(); 
		}

	private: // methods
		bool ShouldSerializeForeColor();
		bool ShouldSerializeBackColor();
		bool ShouldSerializeLineColor();
		bool ShouldSerializeFont();

		bool ShouldSerializeSelectedForeColor();
		bool ShouldSerializeSelectedBackColor();

		bool ShouldSerializeFocusedForeColor();
		bool ShouldSerializeFocusedBackColor();

		bool ShouldSerializeColumnForeColor();
		bool ShouldSerializeColumnBackColor();
		bool ShouldSerializeColumnLineColor();
		bool ShouldSerializeColumnFont();

		bool ShouldSerializeRowForeColor();
		bool ShouldSerializeRowBackColor();
		bool ShouldSerializeRowLineColor();
		bool ShouldSerializeRowFont();

        bool ShouldSerializeCaptionForeColor();
		bool ShouldSerializeCaptionBackColor();
		bool ShouldSerializeCaptionLineColor();
		bool ShouldSerializeCaptionFont();

        bool ShouldSerializeGroupPanelForeColor();
		bool ShouldSerializeGroupPanelBackColor();
		bool ShouldSerializeGroupPanelLineColor();
		bool ShouldSerializeGroupPanelFont();

		bool ShouldSerializeRowHighlightLineColor();
		bool ShouldSerializeRowHighlightFillColor();

		bool ShouldSerializeCellForeColors();
		bool ShouldSerializeCellBackColors();
		bool ShouldSerializeCellLineColors();
		bool ShouldSerializeCellFonts();

        bool ShouldSerializeGroupForeColors();
		bool ShouldSerializeGroupBackColors();
		bool ShouldSerializeGroupLineColors();
		bool ShouldSerializeGroupFonts();

		virtual System::Object^ GetService(System::Type^ serviceType) sealed = System::IServiceProvider::GetService;

	private: // variables

		StyleColorCollection^ m_cellForeColors;
		StyleColorCollection^ m_cellBackColors;
		StyleColorCollection^ m_cellLineColors;
		StyleFontCollection^ m_cellFonts;

		StyleColorCollection^ m_groupForeColors;
		StyleColorCollection^ m_groupBackColors;
        StyleColorCollection^ m_groupLineColors;
		StyleFontCollection^ m_groupFonts;

		GrStyle* m_pNativeStyle;

		System::EventHandler^ m_eventDisposed;
		System::ComponentModel::ISite^ m_site;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/