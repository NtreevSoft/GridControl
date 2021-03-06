﻿//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5190.32793
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
#include "IEditorService.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design { namespace Controls
{
    ref class ListBox : public System::Windows::Forms::ListBox
    {
    public: // methods

        ListBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService);

        ListBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, System::Type^ dataType, System::Object^ value);

    public: // properties

        property System::Object^ Value
        {
            System::Object^ get();
        }

    protected: // methods

        virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) override;

        virtual void OnMouseClick(System::Windows::Forms::MouseEventArgs^ e) override;

        virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs^ e) override;

    private: // methods

        void UpdateValue();

    private: // variables

        Ntreev::Windows::Forms::Grid::Design::IEditorService^ m_editorService;
        System::Object^ m_value;
        System::Type^ m_dataType;
    };
} /*namespace Controls*/ } /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/