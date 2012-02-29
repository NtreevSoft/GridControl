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
#include "GridColumnUITypeEditor.h"
#include "GridControl.h"
#include "GridCell.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
    //ColumnUITypeEditor::ColumnUITypeEditor()
    //{

    //}

    //System::Drawing::Design::UITypeEditor^ ColumnUITypeEditor::UITypeEditor::get()
    //{
    //    using namespace System::ComponentModel;
    //    using namespace System::Drawing::Design;

    //    if(m_typeEditor == nullptr)
    //    {
    //        m_typeEditor = (System::Drawing::Design::UITypeEditor^)TypeDescriptor::GetEditor(this->DataType, System::Drawing::Design::UITypeEditor::typeid);
    //        
    //        switch(m_typeEditor->GetEditStyle())
    //        {
    //        case UITypeEditorEditStyle::None:
    //            m_pColumn->SetItemType(GrItemType_Control);
    //            break;
    //        case UITypeEditorEditStyle::DropDown:
    //            m_pColumn->SetItemType(GrItemType_DropDown);
    //            break;
    //        case UITypeEditorEditStyle::Modal:
    //            m_pColumn->SetItemType(GrItemType_Modal);
    //            break;
    //        }
    //    }

    //    return m_typeEditor;
    //}

    //bool ColumnUITypeEditor::PaintValueSupported::get()
    //{
    //    if(this->UITypeEditor == nullptr)
    //        return false;
    //    return UITypeEditor->GetPaintValueSupported();
    //}

    //void ColumnUITypeEditor::PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, System::Drawing::Rectangle clipRect, ICell^ /*style*/, System::Object^ value)
    //{
    //    UITypeEditor->PaintValue(gcnew System::Drawing::Design::PaintValueEventArgs(nullptr, value, graphics, clipRect));
    //    clipRect.Width--;
    //    clipRect.Height--;
    //    graphics->DrawRectangle(System::Drawing::Pens::Black, clipRect);
    //}

    //void ColumnUITypeEditor::OnEditValue(EditValueEventArgs^ e)
    //{
    //    using namespace System::ComponentModel;
    //    using namespace System::Drawing::Design;
    //    using namespace System::Windows::Forms;

    //    if(this->UITypeEditor == nullptr)
    //        return;

    //    Form^ owner = GridControl->FindForm();
    //    while(owner->ParentForm != nullptr)
    //        owner = owner->ParentForm;

    //    if(dynamic_cast<IBindingList^>(e->Value) != nullptr)
    //    {
    //        IBindingList^ bindingList = dynamic_cast<IBindingList^>(e->Value);

    //        Form form;
    //        Ntreev::Windows::Forms::Grid::GridControl^ gridControl = gcnew Ntreev::Windows::Forms::Grid::GridControl();
    //        gridControl->Dock = DockStyle::Fill;
    //        gridControl->DataSource = bindingList;

    //        form.Controls->Add(gridControl);

    //        form.ShowDialog(owner);
    //        e->CancelEdit = true;
    //    }
    //    else
    //    {

    //    }
    //    e->SuppressEditing = true;
    //}

} /*namespace Columns*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/