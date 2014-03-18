//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5190.32793
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


#include "StdAfx.h"
#include "FromNative.h"
#include "Cell.h"
#include "Column.h"
#include "Row.h"
#include "GroupRow.h"
#include "CaptionRow.h"
#include "GridRow.h"
#include "NativeGridRow.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    Cell^ FromNative::Get(GrItem* pItem)
    {
        System::Object^ ref = pItem->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Cell^>(ref);
    }

    Column^ FromNative::Get(GrColumn* pColumn)
    {
        if(pColumn == nullptr)
            return nullptr;
        System::Object^ ref = pColumn->ManagedRef;
        return safe_cast<Column^>(ref);
    }

    RowBase^ FromNative::Get(IDataRow* pDataRow)
    {
        if(pDataRow == nullptr)
        {
            return nullptr;
        }

        Native::GrGridRow* pGridRow = dynamic_cast<Native::GrGridRow*>(pDataRow);
        if(pGridRow != nullptr)
        {
            return Get(pGridRow);
        }
        else if(pDataRow->GetRowType() == GrRowType_GroupRow)
        {
            return Get((GrGroupRow*)pDataRow);
        }
        else if(pDataRow->GetRowType() == GrRowType_DataRow || pDataRow->GetRowType() == GrRowType_InsertionRow)
        {
            return Get((GrDataRow*)pDataRow);
        }

        return nullptr;
    }

    GroupRow^ FromNative::Get(GrGroupRow* pGroupRow)
    {
        System::Object^ ref = pGroupRow->ManagedRef;
        if(ref == nullptr)
        {
            ref = gcnew GroupRow(pGroupRow);
            pGroupRow->ManagedRef = ref;
        }

        return safe_cast<GroupRow^>(ref);
    }

    Row^ FromNative::Get(GrDataRow* pDataRow)
    {
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<Row^>(ref);
    }

    CaptionRow^ FromNative::Get(GrCaption* pCaption)
    {
        System::Object^ ref = pCaption->ManagedRef;
        return safe_cast<CaptionRow^>(ref);
    }

    GridRow^ FromNative::Get(Native::GrGridRow* pGridRow)
    {
        System::Object^ ref = pGridRow->ManagedRef;
        if(ref == nullptr)
        {
            ref = gcnew GridRow(pGridRow);
            pGridRow->ManagedRef = ref;
        }

        return safe_cast<GridRow^>(ref);
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/