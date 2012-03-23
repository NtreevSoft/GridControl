﻿//=====================================================================================================================
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

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ref class Cell;
    ref class Column;
    ref class RowBase;
    ref class Row;
    ref class CaptionRow;
    ref class GroupRow;
    ref class GridControl;

    ref class FromNative
    {
    public: // methods

        static Ntreev::Windows::Forms::Grid::Cell^ Get(GrItem* pItem);

        static Ntreev::Windows::Forms::Grid::Cell^ Get(GrItem* pItem, Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

        static Ntreev::Windows::Forms::Grid::Column^ Get(GrColumn* pColumn);

        static Ntreev::Windows::Forms::Grid::RowBase^ Get(IDataRow* pDataRow);

        static Ntreev::Windows::Forms::Grid::GroupRow^ Get(GrGroupRow* pGroupRow);

        static Ntreev::Windows::Forms::Grid::Row^ Get(GrDataRow* pDataRow);

        static Ntreev::Windows::Forms::Grid::CaptionRow^ Get(GrCaption* pCaption);
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/