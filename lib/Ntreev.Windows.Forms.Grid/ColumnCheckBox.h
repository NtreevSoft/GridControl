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
#include "ColumnControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
    /// <summary>
    /// CheckBox를 지원하는 Column개체를 나타냅니다.
    /// </summary>
    public ref class ColumnCheckBox : Ntreev::Windows::Forms::Grid::ColumnControl<System::Windows::Forms::CheckBox^>
    {
    public: // methods

        /// <summary>
        /// <see cref="ColumnCheckBox"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        ColumnCheckBox();

        /// <summary>
        /// 셀을 그릴때 호출됩니다.
        /// </summary>
        /// <param name="graphics">그리는데 필요한 개체입니다.</param>
        /// <param name="paintRect">그려질 셀의 안쪽 영역을 나타냅니다.</param>
        /// <param name="cell">셀의 배경색 및 전경색, 글꼴을 가져오는 개체입니다.</param>
        /// <param name="value">그려질 셀의 값을 나타냅니다.</param>
        virtual void PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value) override;

    protected: // methods

        /// <summary>
        /// 컨트롤에서 셀에 적용할 값을 가져올때 호출됩니다.
        /// </summary>
        /// <param name="control">편집에 사용되는 컨트롤의 인스턴스입니다.</param>
        /// <returns>셀에 적용할 값의 <see cref="System::Object"/>입니다.</returns>
        virtual System::Object^ GetControlValue(System::Windows::Forms::CheckBox^ control) override;

        /// <summary>
        /// 셀의 값을 컨트롤에 적용할때 호출됩니다.
        /// </summary>
        /// <param name="control">편집 또는 표시에 사용되는 컨트롤의 인스턴스입니다.</param>
        /// <param name="value">컨트롤에 적용할 셀의 값입니다.</param>
        virtual void SetControlValue(System::Windows::Forms::CheckBox^ control, System::Object^ value) override;

        ///// <summary>
        ///// 값이 편집될때 호출됩니다.
        ///// </summary>
        ///// <param name="e">값이 편집될때 필요한 데이터를 제공하는 <see cref="EditValueEventArgs"/>개체입니다.</param>
        //virtual void OnEditValue(EditValueEventArgs^ e);

    private: // methods

        void checkBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e);

    private:

        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_rendererChecked;
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_rendererUnchecked;
    };
} /*namespace Columns*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/