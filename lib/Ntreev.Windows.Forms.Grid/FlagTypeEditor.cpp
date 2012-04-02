//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4475.19551
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
#include "FlagTypeEditor.h"
#include "FlagControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    FlagTypeEditor::FlagTypeEditor(System::Type^ dataType) 
        : TypeEditor(dataType)
    {

    }

    System::Object^ FlagTypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        Ntreev::Windows::Forms::Grid::Design::Controls::FlagControl^ flagControl = gcnew Ntreev::Windows::Forms::Grid::Design::Controls::FlagControl(editorService, this->DataType);

        flagControl->Value = value;
        flagControl->BackColor = cell->BackColor;
        flagControl->ForeColor = cell->ForeColor;
        flagControl->Font = cell->Font;

        editorService->DropDownControl(flagControl);
        return flagControl->Value;
    }

    Ntreev::Windows::Forms::Grid::Design::EditStyle FlagTypeEditor::GetEditStyle()
    {
        return Ntreev::Windows::Forms::Grid::Design::EditStyle::DropDown; 
    }
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/