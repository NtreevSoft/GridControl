//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4478.19833
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
#include "stdafx.h"
#include "BindingListTypeEditor.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
#ifdef PAINT_MODAL
    BindingListTypeEditor::BindingListTypeEditor(System::Type^ dataType)
        : TypeEditor(dataType)
    {
        
    }

    System::Object^ BindingListTypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        if(ValueChecker::IsNullOrDBNull(value) == true)
            return value;

        Ntreev::Windows::Forms::Grid::GridControl^ gridControl = gcnew Ntreev::Windows::Forms::Grid::GridControl();
        gridControl->Dock = System::Windows::Forms::DockStyle::Fill;
        gridControl->Caption = cell->Column->Title;
        gridControl->DataSource = dynamic_cast<System::ComponentModel::IBindingList^>(value);

        editorService->ShowDialog(gridControl);
        return value;
    }

    Ntreev::Windows::Forms::Grid::Design::EditStyle BindingListTypeEditor::GetEditStyle() 
    {
        return Ntreev::Windows::Forms::Grid::Design::EditStyle::Modal; 
    }
#else

    BindingListTypeEditor::BindingListTypeEditor(System::Type^ dataType)
        : TypeEditor(dataType)
    {
        
    }

    System::Object^ BindingListTypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        if(ValueChecker::IsNullOrDBNull(value) == true)
            return value;

        Ntreev::Windows::Forms::Grid::GridControl^ gridControl = GetGridControl(cell);
        if(gridControl->DataSource == nullptr)
            gridControl->DataSource = dynamic_cast<System::ComponentModel::IBindingList^>(value);
        editorService->ShowControl(gridControl);
        return nullptr;
    }

    Ntreev::Windows::Forms::Grid::Design::EditStyle BindingListTypeEditor::GetEditStyle() 
    {
        return Ntreev::Windows::Forms::Grid::Design::EditStyle::Control; 
    }

    void BindingListTypeEditor::PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        System::Drawing::Bitmap^ bitmap = gcnew System::Drawing::Bitmap(paintRect.Width, paintRect.Height, graphics);
        Ntreev::Windows::Forms::Grid::GridControl^ gridControl = GetGridControl(cell);
        if(gridControl->DataSource == nullptr)
        gridControl->DataSource = value;
        gridControl->Bounds = System::Drawing::Rectangle(0, 0, paintRect.Width, paintRect.Height);

        gridControl->DrawToBitmap(bitmap, System::Drawing::Rectangle(0, 0, paintRect.Width, paintRect.Height));


        graphics->DrawImage(bitmap, paintRect.Location);

        //m_gridControl->DataSource = value;
        
        //m_gridControl->DrawToBitmap(
    }

    Ntreev::Windows::Forms::Grid::ViewType BindingListTypeEditor::ViewType::get()
    {
        return Ntreev::Windows::Forms::Grid::ViewType::Custom;
    }

    Ntreev::Windows::Forms::Grid::GridControl^ BindingListTypeEditor::GetGridControl(Ntreev::Windows::Forms::Grid::ICell^ cell)
    {
        Ntreev::Windows::Forms::Grid::GridControl^ gridControl;
        if(m_gridControls.TryGetValue(cell, gridControl) == false)
        {
            gridControl = gcnew Ntreev::Windows::Forms::Grid::GridControl();
            gridControl->IsCaptionRowVisible = false;
            gridControl->IsFrozingSplitterVisible = false;
            gridControl->IsGroupPanelVisible = false;
            gridControl->Dock = System::Windows::Forms::DockStyle::Fill;
            gridControl->Caption = cell->Column->Title;

            m_gridControls.Add(cell, gridControl);
        }

        return gridControl;
    }
#endif
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/