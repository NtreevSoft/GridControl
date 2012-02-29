#include "StdAfx.h"
#include "GridListBox.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    GridListBox::GridListBox(IEditorService^ editorService)
        : m_editorService(editorService)
    {
        this->BorderStyle = System::Windows::Forms::BorderStyle::None;
    }

    GridListBox::GridListBox(IEditorService^ editorService, System::Type^ dataType, System::Object^ value)
        : m_editorService(editorService), m_value(value)
    {
        this->BorderStyle = System::Windows::Forms::BorderStyle::None;

        if(dataType->IsEnum == true)
        {
            for each(System::Object^ item in System::Enum::GetValues(dataType))
            {
                this->Items->Add(item);
            }
        }
        else if(dataType->IsArray == true)
        {

        }
        this->SelectedItem = value;
    }

    System::Object^ GridListBox::Value::get()
    {
        return m_value;
    }

    void GridListBox::OnMouseMove(System::Windows::Forms::MouseEventArgs^ e)
    {
        ListBox::OnMouseMove(e);
        int selIndex = this->IndexFromPoint(e->Location);
        this->SelectedIndex = selIndex;
    }

    void GridListBox::OnMouseClick(System::Windows::Forms::MouseEventArgs^ e)
    {
        ListBox::OnMouseClick(e);
        if(e->Button == System::Windows::Forms::MouseButtons::Left)
        {
            UpdateValue();
            m_editorService->Close();
        }
    }

    void GridListBox::OnKeyDown(System::Windows::Forms::KeyEventArgs^ e)
    {
        ListBox::OnKeyDown(e);
        switch(e->KeyCode)
        {
        case System::Windows::Forms::Keys::Enter:
            {
                UpdateValue();
                m_editorService->Close();
            }
            break;
        }
    }

    void GridListBox::UpdateValue()
    {
        if(this->SelectedItem != nullptr)
            m_value = this->SelectedItem;
    }
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/