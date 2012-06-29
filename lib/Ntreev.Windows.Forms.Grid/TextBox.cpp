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


#include "StdAfx.h"
#include "TextBox.h"
namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design { namespace Controls
{
    TextBox::TextBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService)
        : m_editorService(editorService)
    {
        this->BorderStyle = System::Windows::Forms::BorderStyle::None;
        //this->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
        this->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::None;
        this->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::CustomSource;
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