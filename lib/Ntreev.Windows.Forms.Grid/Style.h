//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4464.32161
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
#include "NativeCollection.h"
#include "GrGridType.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    class FontConverter
    {
    public:
        static System::Drawing::Font^ StoT(GrFont* pFont);
        static GrFont* TtoS(System::Drawing::Font^ font);
    };

    [System::ComponentModel::TypeConverter(System::ComponentModel::ExpandableObjectConverter::typeid)]
    public ref class Style
    {
    public: // methods

        Style();

        void ResetRowCellForeColors();
        void ResetRowCellBackColors();
        void ResetRowCellFonts();

        void ResetSelectedForeColor();
        void ResetSelectedBackColor();

        void ResetFocusedForeColor();
        void ResetFocusedBackColor();

        void ResetCellForeColor();
        void ResetCellBackColor();
        void ResetCellFont();

        void ResetRowHighlightForeColor();
        void ResetRowHighlightBackColor();


    public: // properties

        property System::Drawing::Color CellForeColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color);
        }

        property System::Drawing::Color CellBackColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color);
        }

        property System::Drawing::Font^ CellFont
        {
            System::Drawing::Font^ get();
            void set(System::Drawing::Font^);
        }

        property System::Drawing::Color SelectedForeColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color);
        }

        property System::Drawing::Color SelectedBackColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color);
        }

        property System::Drawing::Color FocusedForeColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color);
        }

        property System::Drawing::Color FocusedBackColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color);
        }

        property System::Drawing::Color RowHighlightForeColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color);
        }

        property System::Drawing::Color RowHighlightBackColor
        {
            System::Drawing::Color get();
            void set(System::Drawing::Color);
        }

        property System::Collections::Generic::ICollection<System::Drawing::Color>^ RowCellForeColors
        {
            System::Collections::Generic::ICollection<System::Drawing::Color>^ get(); 
        }

        property System::Collections::Generic::ICollection<System::Drawing::Color>^ RowCellBackColors
        {
            System::Collections::Generic::ICollection<System::Drawing::Color>^ get(); 
        }

        property System::Collections::Generic::ICollection<System::Drawing::Font^>^ RowCellFonts
        { 
            System::Collections::Generic::ICollection<System::Drawing::Font^>^ get();
        }

        property System::Collections::Generic::ICollection<System::Drawing::Color>^ GroupingBackColors
        {
            System::Collections::Generic::ICollection<System::Drawing::Color>^ get(); 
        }

        property System::Collections::Generic::ICollection<System::Drawing::Color>^ GroupingForeColors
        {
            System::Collections::Generic::ICollection<System::Drawing::Color>^ get();
        }

        property System::Collections::Generic::ICollection<System::Drawing::Font^>^ GroupingFonts
        {
            System::Collections::Generic::ICollection<System::Drawing::Font^>^ get();
        }

    internal: // properties

        property GrStyle* NativeStyle
        {
            GrStyle* get(); 
        }

    private: // methods

        Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Color, GrColor>^ m_rowCellForeColors;
        Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Color, GrColor>^ m_rowCellBackColors;
        Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Font^, GrFont*, FontConverter>^ m_rowCellFonts;

        Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Color, GrColor>^ m_groupingForeColors;
        Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Color, GrColor>^ m_groupingBackColors;
        Ntreev::Windows::Forms::Grid::NativeCollection<System::Drawing::Font^, GrFont*, FontConverter>^ m_groupingFonts;

        GrStyle* m_pNativeStyle;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/