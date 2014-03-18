//=====================================================================================================================
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


#include "StdAfx.h"
#include "TypeEditor.h"
#include "BindingListTypeEditor.h"
#include "DataTypeEditor.h"
#include "BooleanTypeEditor.h"
#include "FlagTypeEditor.h"
#include "EnumTypeEditor.h"
#include "ListTypeEditor.h"
#include "StringTypeEditor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    TypeEditor::TypeEditor(System::Type^ dataType)
        : m_dataType(dataType)
    {

    }

    bool TypeEditor::CanEdit(Ntreev::Windows::Forms::Grid::ICell^ /*cell*/, Ntreev::Windows::Forms::Grid::EditingReason reason)
    {
        if(reason.ReasonType == Ntreev::Windows::Forms::Grid::EditingReasonType::Mouse)
            return true;
        return false; 
    }

    System::Object^ TypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ /*editorService*/, Ntreev::Windows::Forms::Grid::ICell^ /*cell*/, System::Object^ value)
    {
        return value; 
    }

    void TypeEditor::PaintValue(System::Drawing::Graphics^ /*graphics*/, System::Drawing::Rectangle /*paintRect*/, Ntreev::Windows::Forms::Grid::ICell^ /*cell*/, System::Object^ /*value*/)
    {

    }

    Ntreev::Windows::Forms::Grid::Design::EditStyle TypeEditor::GetEditStyle()
    {
        return Ntreev::Windows::Forms::Grid::Design::EditStyle::Control;
    }

    System::Type^ TypeEditor::DataType::get()
    {
        return m_dataType; 
    }

    Ntreev::Windows::Forms::Grid::ViewType TypeEditor::ViewType::get()
    {
        return Ntreev::Windows::Forms::Grid::ViewType::Text; 
    }

    Ntreev::Windows::Forms::Grid::Design::TypeEditor^ TypeEditor::FromDataType(System::Type^ dataType)
    {
        if(System::ComponentModel::IBindingList::typeid->IsAssignableFrom(dataType) == true)
        {
            return gcnew Ntreev::Windows::Forms::Grid::Design::BindingListTypeEditor(dataType);
        }
        else
        {
            System::Drawing::Design::UITypeEditor^ typeEditor 
                = (System::Drawing::Design::UITypeEditor^)System::ComponentModel::TypeDescriptor::GetEditor(dataType, System::Drawing::Design::UITypeEditor::typeid);

            if(typeEditor != nullptr)
                return gcnew Ntreev::Windows::Forms::Grid::Design::DataTypeEditor(dataType);

            if(dataType == bool::typeid)
            {
                return gcnew Ntreev::Windows::Forms::Grid::Design::BooleanTypeEditor(dataType);
            }
            else if(dataType->IsEnum == true)
            {
                if(dataType->GetCustomAttributes(System::FlagsAttribute::typeid, true)->Length != 0)
                {
                    return gcnew Ntreev::Windows::Forms::Grid::Design::FlagTypeEditor(dataType);
                }
                return gcnew Ntreev::Windows::Forms::Grid::Design::EnumTypeEditor(dataType);
            }
            else if(dataType->IsArray == true)
            {
                return gcnew Ntreev::Windows::Forms::Grid::Design::ListTypeEditor(dataType);
            }
        }
        return gcnew Ntreev::Windows::Forms::Grid::Design::StringTypeEditor(dataType);
    }
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/