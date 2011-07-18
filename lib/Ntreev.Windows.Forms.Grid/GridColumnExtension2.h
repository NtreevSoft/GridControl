#pragma once
#include "GridColumnControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
	using namespace System::Windows::Forms;

	/// <summary>
	/// CheckBox를 지원하는 Column개체를 나타냅니다.
	/// </summary>
	public ref class ColumnCheckBox : ColumnControl<CheckBox^>, IEditByKeyCode
	{
		typedef System::Windows::Forms::VisualStyles::VisualStyleRenderer _VisualStyleRenderer;
	public: // methods
		/// <summary>
		/// <see cref="ColumnCheckBox"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		ColumnCheckBox();

	protected: // methods
		/// <summary>
		/// 셀을 그릴때 호출됩니다.
		/// </summary>
		/// <param name="graphics">그리는데 필요한 개체입니다.</param>
		/// <param name="renderRect">그려질 셀의 안쪽 영역을 나타냅니다.</param>
		/// <param name="clipRect">클립 영역을 나타냅니다.</param>
		/// <param name="style">셀의 배경색 및 전경색, 글꼴을 가져오는 개체입니다.</param>
		/// <param name="value">그려질 셀의 값을 나타냅니다.</param>
		virtual void PaintValue(_Graphics^ graphics, _Rectangle renderRect, _Rectangle clipRect, IStyle^ style, object^ value) override;

		/// <summary>
		/// 컨트롤에서 셀에 적용할 값을 가져올때 호출됩니다.
		/// </summary>
		/// <param name="control">편집에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <returns>셀에 적용할 값의 <see cref="System::Object"/>입니다.</returns>
		virtual object^ GetEditingValue(CheckBox^ control) override;

		/// <summary>
		/// 셀의 값을 컨트롤에 적용할때 호출됩니다.
		/// </summary>
		/// <param name="control">편집 또는 표시에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="value">컨트롤에 적용할 셀의 값입니다.</param>
		virtual void SetEditingValue(CheckBox^ control, object^ value) override;

		/// <summary>
		/// 값이 편집될때 호출됩니다.
		/// </summary>
		/// <param name="e">값이 편집될때 필요한 데이터를 제공하는 <see cref="EditValueEventArgs"/>개체입니다.</param>
		virtual void OnEditValue(EditValueEventArgs^ e) override;

	private: // methods
		virtual bool KeyTest(Keys key) sealed = IEditByKeyCode::KeyTest;
		void checkBox_CheckedChanged(object^ /*sender*/, _EventArgs^ e);

	private:
		_VisualStyleRenderer^	m_rendererChecked;
		_VisualStyleRenderer^	m_rendererUnchecked;
	};
} /*namespace Extensiton*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/