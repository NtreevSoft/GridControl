//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5792.31442
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
#include "ColumnDropDown.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
    /// <summary>
    /// ComboBox형태의 컨트롤을 지원하는 Column개체를 나타냅니다.
    /// </summary>
    public ref class ColumnComboBox : Ntreev::Windows::Forms::Grid::ColumnDropDown<System::Windows::Forms::ListBox^>
    {
    public: // methods

        /// <summary>
        /// <see cref="ColumnComboBox"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        ColumnComboBox();

    public: // properties

        /// <summary>
        /// 펼쳐질 목록에 표시할 데이터 소스를 가져오거나 설정합니다.
        /// </summary>
        /// <param name="value">
        /// <see cref="System::Array"/>같은 <see cref="System::Collections::IList"/> 인터페이스를 구현하는 개체입니다. 
        /// </param>
        [System::ComponentModel::CategoryAttribute("Data")]
        [System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::Repaint)]
        [System::ComponentModel::AttributeProviderAttribute(System::ComponentModel::IListSource::typeid)]
        [System::ComponentModel::DefaultValueAttribute((System::String^)nullptr)]
        property System::Object^ DataSource
        {
            System::Object^ get();
            void set(System::Object^); 
        }

        [System::ComponentModel::CategoryAttribute("Data")]
        [System::ComponentModel::EditorAttribute("System.Windows.Forms.Design.ListControlStringCollectionEditor, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a", System::Drawing::Design::UITypeEditor::typeid)]
        [System::ComponentModel::DesignerSerializationVisibilityAttribute(System::ComponentModel::DesignerSerializationVisibility::Content)]
        [System::ComponentModel::MergablePropertyAttribute(false)]
        property System::Windows::Forms::ListBox::ObjectCollection^ Items
        {
            System::Windows::Forms::ListBox::ObjectCollection^ get();
        }

        /// <summary>
        /// 펼쳐질 목록에 보여질 최대 아이템 갯수를 가져오거나 설정합니다.
        /// </summary>
        /// <returns>
        /// 펼쳐질 목록에 보여질 최대 아이템 갯수를 나타내는 <see cref="System::Int32"/>형태의 정수값 입니다. 기본 값은 8입니다.(<see cref="DefaultMaxDropDownItems"/>)
        /// </returns>
        property int MaxDropDownItems
        { 
            int get();
            void set(int); 
        }

    protected: // methods

        /// <summary>
        /// 컨트롤에서 셀에 적용할 값을 가져올때 호출됩니다.
        /// </summary>
        /// <returns>셀에 적용할 값의 <see cref="System::Object"/>입니다.</returns>
        virtual System::Object^ GetControlValue() override;

        /// <summary>
        /// 셀의 값을 컨트롤에 적용할때 호출됩니다.
        /// </summary>
        /// <param name="value">컨트롤에 적용할 셀의 값입니다.</param>
        virtual void SetControlValue(System::Object^ value) override;

        virtual void SetControlLayout(Ntreev::Windows::Forms::Grid::ICell^ cell) override;

    private: // methods

        void listBox_PreviewKeyDown(System::Object^ sender, System::Windows::Forms::PreviewKeyDownEventArgs^ e);
        void listBox_OnMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        void listBox_OnMouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        void listBox_OnVisibleChanged(System::Object^ sender, System::EventArgs^ e);

        bool ShouldSerializeMaxDropDownItems();

    public: // variables

        /// <summary>
        /// 펼쳐질 목록에 보여질 아이템 갯수의 기본값입니다.
        /// </summary>
        static int DefaultMaxDropDownItems = 8;

    private: // variables

        int m_nMaxDropDownItems;
        System::Drawing::Size m_size;
        System::Object^ m_value;
        System::Object^ m_dataSource;
    };
} /*namespace Extensiton*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/