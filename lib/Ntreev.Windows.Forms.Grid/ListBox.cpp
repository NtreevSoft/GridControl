#include "StdAfx.h"
#include "ListBox.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design { namespace Controls
{
    ListBox::ListBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService)
        : m_editorService(editorService)
    {
        this->BorderStyle = System::Windows::Forms::BorderStyle::None;
    }

    ListBox::ListBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, System::Type^ dataType, System::Object^ value)
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

    System::Object^ ListBox::Value::get()
    {
        return m_value;
    }

    void ListBox::OnMouseMove(System::Windows::Forms::MouseEventArgs^ e)
    {
        System::Windows::Forms::ListBox::OnMouseMove(e);
        int selIndex = this->IndexFromPoint(e->Location);
        this->SelectedIndex = selIndex;
    }

    void ListBox::OnMouseClick(System::Windows::Forms::MouseEventArgs^ e)
    {
        System::Windows::Forms::ListBox::OnMouseClick(e);
        if(e->Button == System::Windows::Forms::MouseButtons::Left)
        {
            UpdateValue();
            m_editorService->Close();
        }
    }

    void ListBox::OnKeyDown(System::Windows::Forms::KeyEventArgs^ e)
    {
        System::Windows::Forms::ListBox::OnKeyDown(e);
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

    void ListBox::UpdateValue()
    {
        if(this->SelectedItem != nullptr)
            m_value = this->SelectedItem;
    }
} /*namespace Controls*/ } /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/