#include "StdAfx.h"
#include "TextBox.h"
namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design { namespace Controls
{
    TextBox::TextBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService)
        : m_editorService(editorService)
    {
        this->BorderStyle = System::Windows::Forms::BorderStyle::None;
    }

    void TextBox::OnKeyDown(System::Windows::Forms::KeyEventArgs^ e)
    {
        System::Windows::Forms::TextBox::OnKeyDown(e);
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

} /*namespace Controls*/ } /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/