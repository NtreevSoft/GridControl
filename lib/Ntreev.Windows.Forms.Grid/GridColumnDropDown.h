//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
#include "Column.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// DropDown형식에 컨트롤을 부착해 편집할 수 있는 기능을 제공합니다.
    /// </summary>
    /// <typeparam name="TControl"> 
    /// 편집할때 사용할 컨트롤의 타입입니다.
    /// </typeparam>
    generic<class TControl> where TControl : System::Windows::Forms::Control
        public ref class ColumnDropDown abstract : Column//, IColumnDropDown, IControlProvider
    {
    public: // methods
        /// <summary>
        /// 컨트롤을 생성하고 <see cref="ColumnDropDown"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        ColumnDropDown();

        /// <summary>
        /// <paramref name="controlArgs"/>를 사용하여 컨트롤을 생성하고 <see cref="ColumnDropDown"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <remarks>
        /// 만약 컨트롤의 특수 생성자가 있을때에는 <paramref name="controlArgs"/>를 사용하여 필요한 인자를 넘길 수 있습니다.
        /// </remarks>
        ColumnDropDown(... cli::array<System::Object^>^ controlArgs);

    public: // properties
        /// <summary>
        /// DropDown에 사용되는 form의 크기를 사용자가 조절할 수 있는지에 대한 여부를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 사용자가 DropDown의 사용되는 form 크기를 조절할 수 있으면 true를 그렇지 않으면 false를 반환합니다.
        /// </returns>
        [System::ComponentModel::DefaultValueAttribute(false)]
        property bool Resizable
        {
            virtual bool get() { return m_resizable; }
            void set(bool value) { m_resizable = value; }
        }

        /// <summary>
        /// 셀이 편집될때 사용되는 컨트롤의 인스턴스를 가져옵니다.
        /// </summary>
        [System::ComponentModel::BrowsableAttribute(false)]
        property TControl Control
        {
            TControl get() { return m_control; }
        }

    protected: // methods
        /// <summary>
        /// 사용할 컨트롤의 인스턴스를 생성합니다.
        /// </summary>
        /// <param name="controlArgs">인스턴스 생성시 필요한 인자들입니다. 인자가 필요없다면 null이 설정됩니다.</param>
        virtual TControl CreateControlInstance(... cli::array<System::Object^>^ controlArgs);

        /// <summary>
        /// 편집용 컨트롤이 생성된후에 호출됩니다.
        /// </summary>
        virtual void OnControlCreated();

        /// <summary>
        /// 컨트롤에서 셀에 적용할 값을 가져올때 호출됩니다.
        /// </summary>
        /// <returns>셀에 적용할 값의 <see cref="System::Object"/>입니다.</returns>
        virtual System::Object^ GetControlValue();

        /// <summary>
        /// 셀의 값을 컨트롤에 적용할때 호출됩니다.
        /// </summary>
        /// <param name="value">컨트롤에 적용할 셀의 값입니다.</param>
        virtual void SetControlValue(System::Object^ value);

        virtual void SetControlLayout(ICell^ cell);

        virtual System::Object^ EditValue(Design::IEditorService^ editorService, ICell^ cell, System::Object^ value) override sealed;

        virtual Design::EditStyle GetEditStyle() override sealed;

        void CloseDropDown();

    private:
        TControl      m_control;
        bool       m_resizable;
        Design::IEditorService^         m_editorService;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/