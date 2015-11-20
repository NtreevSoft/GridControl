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
#include "Column.h"
#include "IControlPainter.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 컨트롤을 이용해 편집할 수 있고 셀을 그릴 수 있는 기능을 제공합니다.
    /// </summary>
    /// <typeparam name="TControl"> 
    /// 편집할때 사용할 컨트롤의 타입입니다.
    /// </typeparam>
    generic<class TControl> where TControl : System::Windows::Forms::Control
        public ref class ColumnControl abstract : Ntreev::Windows::Forms::Grid::Column
    {
    public: // methods

        /// <summary>
        /// 컨트롤을 생성하고 <see cref="ColumnControl"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        ColumnControl();

        /// <summary>
        /// <paramref name="controlArgs"/>를 사용하여 컨트롤을 생성하고 <see cref="ColumnControl"/>클래스의 새 인스턴스를 초기화합니다.
        /// </summary>
        /// <remarks>
        /// 만약 컨트롤의 특수 생성자가 있을때에는 <paramref name="controlArgs"/>를 사용하여 필요한 인자를 넘길 수 있습니다.
        /// </remarks>
        ColumnControl(... cli::array<System::Object^>^ controlArgs);

        /// <summary>
        /// 셀을 그릴때 호출됩니다.
        /// </summary>
        /// <param name="graphics">그리는데 필요한 개체입니다.</param>
        /// <param name="paintRect">그려질 셀의 안쪽 영역을 나타냅니다.</param>
        /// <param name="cell">셀의 배경색 및 전경색, 글꼴을 가져오는 개체입니다.</param>
        /// <param name="value">그려질 셀의 값을 나타냅니다.</param>
        virtual void PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value) override;

    public: // properties

        /// <summary>
        /// 셀이 편집될때 사용되는 컨트롤의 인스턴스를 가져옵니다.
        /// </summary>
        [System::ComponentModel::BrowsableAttribute(false)]
        property TControl Control
        {
            TControl get();
        }

        /// <summary>
        /// 별도의 셀 렌더링 기능을 지원하는지에 대한 여부를 가져옵니다.
        /// </summary>
        /// <remarks>
        /// <see cref="ColumnControl"/>클래스의 인스턴스 생성시 이 속성이 true이면 셀을 표시하기 위한 별도의 컨트롤을 생성합니다.
        /// </remarks>
        property Ntreev::Windows::Forms::Grid::ViewType ViewType
        {
            virtual Ntreev::Windows::Forms::Grid::ViewType get() override;
        }

    protected: // methods

        /// <summary>
        /// 사용할 컨트롤의 인스턴스를 생성합니다.
        /// </summary>
        /// <param name="controlArgs">인스턴스 생성시 필요한 인자들입니다. 인자가 필요없다면 null이 설정됩니다.</param>
        virtual TControl CreateControlInstance(... cli::array<System::Object^>^ controlArgs);

        /// <summary>
        /// 컨트롤에서 셀에 적용할 값을 가져올때 호출됩니다.
        /// </summary>
        /// <param name="control">편집에 사용되는 컨트롤의 인스턴스입니다.</param>
        /// <returns>셀에 적용할 값의 <see cref="System::Object"/>입니다.</returns>
        virtual System::Object^ GetControlValue(TControl control);

        /// <summary>
        /// 셀의 값을 컨트롤에 적용할때 호출됩니다.
        /// </summary>
        /// <param name="control">편집 또는 표시에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="cell">편집 또는 표시에 사용되는 셀의 인스턴스입니다.</param>
        /// <param name="value">컨트롤에 적용할 셀의 값입니다.</param>
        virtual void SetControlValue(TControl control, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value);

        /// <summary>
        /// 컨트롤을 셀의 위치로 이동하고 셀의 크기로 조정할때 호출됩니다.
        /// </summary>
        /// <param name="cell">셀의 정보를 나타냅니다..</param>
        virtual void SetControlLayout(TControl control, Ntreev::Windows::Forms::Grid::ICell^ cell);

        virtual System::Object^ EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value) override;

        virtual bool CanEdit(Ntreev::Windows::Forms::Grid::ICell^ cell, Ntreev::Windows::Forms::Grid::EditingReason reason) override;

        void CloseControl();

        ~ColumnControl();

        virtual void OnGridControlAttached() override;

        virtual void OnGridControlDetached() override;

    internal: // methods

        virtual Ntreev::Windows::Forms::Grid::Design::EditStyle GetEditStyle() override sealed;

		virtual void OnCellBoundUpdate(Cell^ cell, System::Object^ value) override;

    private: // methods

        void gridControl_CellMouseMove(System::Object^ sender, Ntreev::Windows::Forms::Grid::CellMouseEventArgs^ e);
		void viewControl_Invalidated(System::Object^ sender, System::Windows::Forms::InvalidateEventArgs^ e);

    private: // variables

        TControl m_control;
        TControl m_viewControl;

        Ntreev::Windows::Forms::Grid::IControlPainter^ m_controlPainter;
        Ntreev::Windows::Forms::Grid::Design::IEditorService^ m_editorService;

        //System::Windows::Forms::Form^ m_form;
        Ntreev::Windows::Forms::Grid::CellMouseEventHandler^ m_eventCellMouseMove;
		System::Windows::Forms::InvalidateEventHandler^ m_viewControlInvalidate;
		Cell^ m_viewedCell;


		ref class MessageFilter : System::Windows::Forms::IMessageFilter
        {
		public:
			MessageFilter(TControl control) : m_control(control) {}

        private:
            virtual bool PreFilterMessage(System::Windows::Forms::Message% m) sealed = System::Windows::Forms::IMessageFilter::PreFilterMessage;

		private:
			TControl m_control;
        };

		MessageFilter^ messageFilter;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/