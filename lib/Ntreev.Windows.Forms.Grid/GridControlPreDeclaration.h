//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4646.22417
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
    ref class GridControl;

    interface class ICellBase;
    interface class ICell;
    interface class IColumn;

    ref class GridObject;

    ref class CellBase;
    ref class Cell;
    ref class RowBase;
    ref class Row;
    ref class CaptionRow;
    ref class GroupRow;
    ref class GridRow;
    ref class GroupPanel;
    ref class Column;

    ref class CellTagCollection;
    ref class CellCollection;
    ref class ColumnCollection;
    ref class VisibleColumnCollection;
    ref class DisplayableColumnCollection;
    ref class FrozenColumnCollection;
    ref class UnfrozenColumnCollection;
    ref class RowCollection;
    ref class VisibleRowCollection;
    ref class DisplayableRowCollection;
    ref class SelectedRowCollection;
    ref class SelectedColumnCollection;
    ref class GroupRowCollection;

    ref class ErrorDescriptor;
    ref class ToolTip;
    ref class Style;
    

    ref class RowBuilder;
    ref class CellBuilder;

    enum class RowHighlightType;
    enum class ColumnBindingCreation;
    enum class ViewType;

    value class EditingReason;

    namespace Native
    {
        class WinFormWindow;
        class WinFormGridCore;
        class GrGridRow;
    }

    typedef GridControl _GridControl;
    typedef GridObject _GridObject;

    typedef ToolTip _ToolTip;
    typedef Style _Style;
    typedef CaptionRow _CaptionRow;
    typedef GroupPanel _GroupPanel;
    typedef ErrorDescriptor _ErrorDescriptor;

    typedef RowHighlightType _RowHighlightType;
    typedef ColumnBindingCreation _ColumnBindingCreation;
    typedef ViewType _ViewType;
    typedef Column _Column;
    typedef Row _Row;

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/