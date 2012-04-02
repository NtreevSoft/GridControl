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


#include "StdAfx.h"
#include "FromNative.h"
#include "Cell.h"
#include "Column.h"
#include "Row.h"
#include "GroupRow.h"
#include "CaptionRow.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    Ntreev::Windows::Forms::Grid::Cell^ FromNative::Get(GrItem* pItem)
    {
        System::Object^ ref = pItem->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Cell^>(ref);
    }

    //Ntreev::Windows::Forms::Grid::Cell^ FromNative::Get(GrItem* pItem, Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
    //{
    //    Ntreev::Windows::Forms::Grid::Cell^ cell = Get(pItem);
    //    if(cell == nullptr)
    //        cell = gcnew Ntreev::Windows::Forms::Grid::Cell(gridControl, pItem);
    //    return cell;
    //}

    Ntreev::Windows::Forms::Grid::Column^ FromNative::Get(GrColumn* pColumn)
    {
        if(pColumn == nullptr)
            return nullptr;
        System::Object^ ref = pColumn->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Column^>(ref);
    }

    Ntreev::Windows::Forms::Grid::RowBase^ FromNative::Get(IDataRow* pDataRow)
    {
        if(pDataRow == nullptr)
        {
            return nullptr;
        }

        if(pDataRow->GetRowType() == GrRowType_GroupRow)
        {
            return Get((GrGroupRow*)pDataRow);
        }
        else if(pDataRow->GetRowType() == GrRowType_DataRow)
        {
            return Get((GrDataRow*)pDataRow);
        }

        return nullptr;
    }

    Ntreev::Windows::Forms::Grid::GroupRow^ FromNative::Get(GrGroupRow* pGroupRow)
    {
        System::Object^ ref = pGroupRow->ManagedRef;
        if(ref == nullptr)
        {
            ref = gcnew Ntreev::Windows::Forms::Grid::GroupRow(pGroupRow);
            pGroupRow->ManagedRef = ref;
        }

        return safe_cast<Ntreev::Windows::Forms::Grid::GroupRow^>(ref);
    }

    Ntreev::Windows::Forms::Grid::Row^ FromNative::Get(GrDataRow* pDataRow)
    {
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Row^>(ref);
    }

    Ntreev::Windows::Forms::Grid::CaptionRow^ FromNative::Get(GrCaption* pCaption)
    {
        System::Object^ ref = pCaption->ManagedRef;
        return safe_cast<CaptionRow^>(ref);
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/