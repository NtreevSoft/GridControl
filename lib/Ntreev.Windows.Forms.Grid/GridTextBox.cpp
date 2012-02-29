#include "StdAfx.h"
#include "GridTextBox.h"
namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    GridTextBox::GridTextBox(IEditorService^ editorService)
        : m_editorService(editorService)
    {
        this->BorderStyle = System::Windows::Forms::BorderStyle::None;
    }

    void GridTextBox::OnKeyDown(System::Windows::Forms::KeyEventArgs^ e)
    {
        TextBox::OnKeyDown(e);
        switch(e->KeyCode)
        {
        case System::Windows::Forms::Keys::Enter:
            if(this->Multiline == true)
            {
                if(e->Control == true)
                {
                    m_editorService->Close();
                    e->SuppressKeyPress = true;
                }
            }
            else
            {
                m_editorService->Close();
                e->SuppressKeyPress = true;
            }
            
            break;
        }
    }

} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/