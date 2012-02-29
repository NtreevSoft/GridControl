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


#include "StdAfx.h"
#include "GridBase.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    GridObject::GridObject()
    {

    }

    GridObject::GridObject(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : m_gridControl(gridControl), 
        m_pGridCore(gridControl->GridCore), 
        m_pGridPainter(gridControl->GridPainter), 
        m_pItemSelector(m_pGridCore->GetItemSelector()),
        m_pFocuser(m_pGridCore->GetFocuser())
    {


    }

    void GridObject::GridControl::set(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
    {
        if(gridControl != nullptr)
        {
            m_gridControl = gridControl;
            m_pGridCore  = gridControl->GridCore;
            m_pGridPainter  = gridControl->GridPainter;
            m_pItemSelector = m_pGridCore->GetItemSelector();
            m_pFocuser  = m_pGridCore->GetFocuser();

            OnGridControlAttachedInternal();
        }
        else
        {
            OnGridControlDetachedInternal();

            m_gridControl = nullptr;
            m_pGridCore  = nullptr;
            m_pGridPainter  = nullptr;
            m_pItemSelector = nullptr;
            m_pFocuser  = nullptr;
        }
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/