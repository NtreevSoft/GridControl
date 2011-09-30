//=====================================================================================================================
// Ntreev Grid for .Net 1.0
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
#include "GridColumnControl.h"
#include "GridColumnDropDown.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
	/// <summary>
	/// 문자열 형태의 편집을 지원하는 Column개체를 나타냅니다.
	/// </summary>
	public ref class ColumnTextBox : ColumnControl<System::Windows::Forms::TextBox^>, IEditByChar, IEditByIme
	{
	public: // methods
		/// <summary>
		/// <see cref="ColumnTextBox"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		ColumnTextBox();

	public: // properties
		/// <summary>
		/// 별도의 셀 렌더링 기능을 지원하는지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <remarks>
		/// <see cref="ColumnControl"/>클래스의 인스턴스 생성시 이 속성이 true이면 셀을 표시하기 위한 별도의 컨트롤을 생성합니다.
		/// </remarks>
		property bool PaintValueSupported
		{
			virtual bool get() override sealed { return false; }
		}

	protected: // methods
		/// <summary>
		/// 편집용 컨트롤이 생성된후에 호출됩니다.
		/// </summary>
		/// <param name="control">생성된 컨트롤의 인스턴스입니다.</param>
		virtual void OnControlCreated(System::Windows::Forms::TextBox^ control) override;

		/// <summary>
		/// 컨트롤에서 셀에 적용할 값을 가져올때 호출됩니다.
		/// </summary>
		/// <param name="control">편집에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <returns>셀에 적용할 값의 <see cref="System::Object"/>입니다.</returns>
		virtual object^ GetEditingValue(System::Windows::Forms::TextBox^ control) override;

		/// <summary>
		/// 셀의 값을 컨트롤에 적용할때 호출됩니다.
		/// </summary>
		/// <param name="control">편집 또는 표시에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="value">컨트롤에 적용할 셀의 값입니다.</param>
		virtual void SetEditingValue(System::Windows::Forms::TextBox^ control, object^ value) override;

		/// <summary>
		/// <see cref="Attaching"/>이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">이벤트 데이터가 들어있는 <see cref="AttachEventArgs"/>입니다.</param>
		virtual void OnAttaching(AttachEventArgs^ e) override;

		/// <summary>
		/// <see cref="Attached"/>이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">이벤트 데이터가 들어있는 <see cref="AttachEventArgs"/>입니다.</param>
		virtual void OnAttached(AttachEventArgs^ e) override;

		/// <summary>
		/// <see cref="Detaching"/>이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">이벤트 데이터가 들어있는 <see cref="DetachEventArgs"/>입니다.</param>
		virtual void OnDetaching(DetachEventArgs^ e) override;

		/// <summary>
		/// 컨트롤을 셀의 위치로 이동하고 셀의 크기로 조정할때 호출됩니다.
		/// </summary>
		/// <param name="control">편집 또는 표시에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="location">셀의 안쪽영역의 위치입니다.</param>
		/// <param name="size">셀의 안쪽영역의 크기입니다.</param>
		virtual void SetControlLayout(System::Windows::Forms::TextBox^ control, _Point location, _Size size) override;

	private: // variables
		void textBox_OnKeyPress(object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);
	};

	/// <summary>
	/// ComboBox형태의 컨트롤을 지원하는 Column개체를 나타냅니다.
	/// </summary>
	public ref class ColumnComboBox : ColumnDropDown<System::Windows::Forms::ListBox^>, IEditByKeyCode
	{
	private:
		typedef System::Windows::Forms::MouseEventArgs	_MouseEventArgs;
	public: // methods
		/// <summary>
		/// <see cref="ColumnComboBox"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		ColumnComboBox();

		virtual _Size		GetPreferredSize(_Size proposedSize) override;

	public: // properties
		/// <summary>
		/// 펼쳐질 목록에 표시할 데이터 소스를 가져오거나 설정합니다.
		/// </summary>
		/// <param name="value">
		/// <see cref="System::Array"/>같은 <see cref="System::Collections::IList"/> 인터페이스를 구현하는 개체입니다. 
		/// </param>
		[System::ComponentModel::Category("Data")]
		[System::ComponentModel::RefreshProperties(System::ComponentModel::RefreshProperties::Repaint)]
		[System::ComponentModel::AttributeProvider(System::ComponentModel::IListSource::typeid)]
		[_DefaultValue((string^)nullptr)]
		property object^ DataSource
		{
			object^ get();
			void set(object^); 
		}

		[System::ComponentModel::Category("Data")]
		[System::ComponentModel::Editor("System.Windows.Forms.Design.ListControlStringCollectionEditor, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a", System::Drawing::Design::UITypeEditor::typeid)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Content)]
		[System::ComponentModel::MergableProperty(false)]
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
		/// <param name="control">편집에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <returns>셀에 적용할 값의 <see cref="System::Object"/>입니다.</returns>
		virtual object^ GetEditingValue(System::Windows::Forms::ListBox^ control) override;

		/// <summary>
		/// 셀의 값을 컨트롤에 적용할때 호출됩니다.
		/// </summary>
		/// <param name="control">편집 또는 표시에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="value">컨트롤에 적용할 셀의 값입니다.</param>
		virtual void SetEditingValue(System::Windows::Forms::ListBox^ control, object^ value) override;

	private: // methods
		virtual bool KeyTest(System::Windows::Forms::Keys key) sealed = IEditByKeyCode::KeyTest;
		void listBox_OnMouseMove(object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void listBox_OnMouseClick(object^ sender, System::Windows::Forms::MouseEventArgs^ e);

		bool ShouldSerializeMaxDropDownItems();

	public: // variables
		/// <summary>
		/// 펼쳐질 목록에 보여질 아이템 갯수의 기본값입니다.
		/// </summary>
		static int	DefaultMaxDropDownItems = 8;
	private: // variables
		int					m_nMaxDropDownItems;
		_Size				m_size;
	};

	/// <summary>
	/// Modal형식으로 경로 데이터를 편집할 수 있는 Column개체를 나타냅니다.
	/// </summary>
	public ref class ColumnPathSelector : ColumnModal
	{
	public: // methods
		/// <summary>
		/// <see cref="ColumnTextBox"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		ColumnPathSelector();

	protected: // methods
		/// <summary>
		/// 값이 편집될때 호출됩니다.
		/// </summary>
		/// <param name="e">값이 편집될때 필요한 데이터를 제공하는 <see cref="EditValueEventArgs"/>개체입니다.</param>
		virtual void OnEditValue(EditValueEventArgs^ e) override;

	private: // variables
		System::Windows::Forms::FolderBrowserDialog^ m_folderBrowerDialog;
	};

	Column^ CreateCheckBox();
} /*namespace Extensiton*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/