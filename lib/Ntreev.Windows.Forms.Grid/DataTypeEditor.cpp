#pragma once
#include "stdafx.h"
#include "DataTypeEditor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    DataTypeEditor::DataTypeEditor(System::Type^ dataType) 
        : TypeEditor(dataType) 
    {
        using namespace System::ComponentModel;
        using namespace System::Drawing::Design;
        m_typeEditor = (UITypeEditor^)TypeDescriptor::GetEditor(dataType, UITypeEditor::typeid);
    }

    System::Object^ DataTypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ /*cell*/, System::Object^ value)
    {
        using namespace System::ComponentModel;
        ITypeDescriptorContext^ context = dynamic_cast<ITypeDescriptorContext^>(editorService->GetService(ITypeDescriptorContext::typeid));
        editorService->IsDropDownResizable = m_typeEditor->IsDropDownResizable;
        return m_typeEditor->EditValue(context, editorService, value);
    }

    Ntreev::Windows::Forms::Grid::Design::EditStyle DataTypeEditor::GetEditStyle()
    {
        using namespace System::Drawing::Design;
        switch(m_typeEditor->GetEditStyle())
        {
        case UITypeEditorEditStyle::DropDown:
            return Ntreev::Windows::Forms::Grid::Design::EditStyle::DropDown;
        case UITypeEditorEditStyle::Modal:
            return Ntreev::Windows::Forms::Grid::Design::EditStyle::Modal;
        default:
            return Ntreev::Windows::Forms::Grid::Design::EditStyle::Control;
        }
    }

    void DataTypeEditor::PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ /*cell*/, System::Object^ value)
    {
        if(value == nullptr || value->ToString() == System::String::Empty)
            return;

        paintRect.Width = 20;
        m_typeEditor->PaintValue(value, graphics, paintRect);
        paintRect.Width--;
        paintRect.Height--;
        graphics->DrawRectangle(System::Drawing::SystemPens::WindowText, paintRect);

        //if(value != nullptr && value->ToString() != System::String::Empty)
        //{
        // System::Drawing::SolidBrush brush(cell->ForeColor);
        // graphics->DrawString(System::String::Format("{0}", value), cell->Font, %brush, paintRect.Right, paintRect.Top);
        //}
    }

    Ntreev::Windows::Forms::Grid::ViewType DataTypeEditor::ViewType::get()
    {
        if(m_typeEditor->GetPaintValueSupported() == true)
            return Ntreev::Windows::Forms::Grid::ViewType::Icon;
        return Ntreev::Windows::Forms::Grid::ViewType::Text; 
    }
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/