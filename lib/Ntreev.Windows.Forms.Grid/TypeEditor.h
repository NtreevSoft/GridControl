//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4461.30274
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
#include "ICell.h"
#include "IEditorService.h"
#include "EditingReason.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    public ref class TypeEditor abstract
    {
    public: // methods

        TypeEditor(System::Type^ dataType);

        virtual bool CanEdit(Ntreev::Windows::Forms::Grid::ICell^ cell, Ntreev::Windows::Forms::Grid::EditingReason reason);

        virtual System::Object^ EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value);

        virtual void PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value);

        virtual Ntreev::Windows::Forms::Grid::Design::EditStyle GetEditStyle();

        static Ntreev::Windows::Forms::Grid::Design::TypeEditor^ FromDataType(System::Type^ dataType);

    public: // properties

        property Ntreev::Windows::Forms::Grid::ViewType ViewType
        {
            virtual Ntreev::Windows::Forms::Grid::ViewType get();
        }

    protected: // properties

        property System::Type^ DataType
        {
            System::Type^ get();
        }

    private: // variables

        System::Type^ m_dataType;
    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/