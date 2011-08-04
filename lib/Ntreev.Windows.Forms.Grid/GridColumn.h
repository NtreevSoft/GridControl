#pragma once
#include "GridEvent.h"
#include "GridCellBase.h"
#include "GridUtil.h"
#include "GridStyleProvider.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 셀을 편집하는 방식을 나타냅니다.
	/// </summary>
	enum class EditingType : int
	{
		/// <summary>
		/// 단일 컨트롤을 사용하여 셀을 편집합니다.
		/// </summary>
		Control,

		/// <summary>
		/// DropDown에 컨트롤을 부착시켜 셀을 편집할 수 있도록 합니다.
		/// </summary>
		DropDown,

		/// <summary>
		/// Modal 형태를 사용하여 셀을 편집합니다.
		/// </summary>
		Modal,

		/// <summary>
		/// 그외의 기타 방법을 사용합니다.
		/// </summary>
		Etc,
	};

	typedef Ntreev::Windows::Forms::Grid::EditingType _EditingType;

	/// <summary>
	/// 셀의 편집을 종료하는 방식을 지정합니다.
	/// </summary>
	public enum class EditingResult : int
	{
		/// <summary>
		/// 셀의 편집을 종료하고 값을 갱신합니다.
		/// </summary>
		Ok,

		/// <summary>
		/// 셀의 편집을 종료하지만 값을 갱신하지는 않습니다.
		/// </summary>
		Cancel,
	};

	typedef Ntreev::Windows::Forms::Grid::EditingResult _EditingResult;

	/// <summary> 
	/// 셀 편집을 위해 컨트롤이 그리드 컨트롤이나 DropDown폼에 부착될때 사용되는 이벤트의 데이터를 제공합니다.
	/// </summary>
	/// <typeparam name="TControl">
	/// 편집에 사용되는 컨트롤의 타입입니다.
	/// </typeparam>
	generic<class TControl> where TControl : _Control
	public ref class AttachEventArgs : _EventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="AttachEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="by">어떠한 방식으로 셀 편집이 시작되었는지를 나타내는 <see cref="EditingReason"/>개체입니다.</param>
		/// <param name="value">편집할 값을 나타냅니다.</param>
		/// <param name="control">편집에 사용되는 컨트롤의 인스턴스입니다.</param>
		AttachEventArgs(EditingReason^ by, object^ value, TControl control)
			: m_editingReason(by), m_value(value), m_control(control)
		{

		}

	public: // properties
		/// <summary>
		/// 셀의 편집이 어떠한 방식으로 시작되었는지를 나타내는 <see cref="EditingReason"/>개체를 가져옵니다.
		/// </summary>
		property EditingReason^ By
		{
			EditingReason^ get() { return m_editingReason; }
		}

		/// <summary>
		/// 편집되기 전에 현재의 값을 가져옵니다.
		/// </summary>
		property object^ Value
		{
			object^ get() { return m_value; }
		}

		/// <summary>
		/// 편집에 사용되는 컨트롤의 인스턴스를 가져옵니다.
		/// </summary>
		property TControl Control
		{
			TControl get() { return m_control; }
		}

	private: // variables
		EditingReason^	m_editingReason;
		object^		m_value;
		TControl	m_control;
	};

	/// <summary> 
	/// 셀 편집을 위해 컨트롤이 그리드 컨트롤이나 DropDown폼에 부착될때 사용되는 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	/// <typeparam name="TControl">
	/// 편집에 사용되는 컨트롤의 타입입니다.
	/// </typeparam>
	generic<class TControl> where TControl : _Control
	public delegate void AttachEventHandler(object^ sender, AttachEventArgs<TControl>^ e);

	/// <summary> 
	/// 셀 편집을 위해 사용했던 컨트롤이 그리드 컨트롤이나 DropDown폼에서 제거될때 사용되는 이벤트의 데이터를 제공합니다.
	/// </summary>
	/// <typeparam name="TControl">
	/// 편집에 사용되는 컨트롤의 타입입니다.
	/// </typeparam>
	generic<class TControl> where TControl : _Control
	public ref class DetachEventArgs : _EventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="DetachEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="modified">값이 편집되었는지는 나타냅니다.</param>
		/// <param name="value">편집된 값을 나타냅니다.</param>
		/// <param name="control">편집에 사용되었던 컨트롤의 인스턴스입니다.</param>
		DetachEventArgs(bool modified, object^ value, TControl control)
			: m_modified(modified), m_value(value), m_control(control)
		{

		}

	public: // properties
		/// <summary>
		/// 값의 편집여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 값이 편집되었다면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		property bool Modified
		{
			bool get() { return m_modified; }
		}

		/// <summary>
		/// 편집된 값을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 편집된 값의 <see cref="System::Object"/>입니다.
		/// </returns>
		property object^ Value
		{
			object^ get() { return m_value; }
		}

		/// <summary>
		/// 편집에 사용됬던 컨트롤의 인스턴스입니다.
		/// </summary>
		property TControl Control
		{
			TControl get() { return m_control; }
		}

	private: // variables
		bool		m_modified;
		object^		m_value;
		TControl	m_control;
	};

	/// <summary> 
	/// 셀 편집을 위해 사용했던 컨트롤이 그리드 컨트롤이나 DropDown폼에서 제거될때 사용되는 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	/// <typeparam name="TControl">
	/// 편집에 사용되는 컨트롤의 타입입니다.
	/// </typeparam>
	generic<class TControl> where TControl : _Control
	public delegate void DetachEventHandler(object^ sender, DetachEventArgs<TControl>^ e);

	/// <summary>
	/// 컨트롤을 제공할 수 있는 개체를 나타냅니다.
	/// </summary>
	public interface class IControlProvider
	{
		/// <summary>
		/// 컨트롤을 가져옵니다.
		/// </summary>
		property _Control^ EditingControl
		{
			_Control^ get();
		}
	};

	interface class IControlAttacher : IControlProvider
	{
		void	InvokeAttaching(EditingReason^ by, object^ value);
		void	InvokeAttached(EditingReason^ by, object^ value);
		void	InvokeDetaching(bool modified, object^ value);
		void	InvokeDetached(bool modified, object^ value);

		void	SetValue(object^ value);
		object^ GetValue();
	};

	/// <summary>
	/// 셀의 값을 그릴 수 있는 메서드와 속성을 정의합니다.
	/// </summary>
	public interface class IValuePainter
	{
		/// <summary>
		/// 셀의 값을 그릴 수 있는 기능을 제공하는지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 셀의 값을 그리기 위해 <see cref="PaintValue"/>를 사용하려면 true를, 그렇지 않으면 false를 반환합니다.
		/// </returns>
		property bool PaintValueSupported
		{
			bool get();
		}

		/// <summary>
		/// 셀의 값을 그릴때 호출됩니다.
		/// </summary>
		/// <param name="graphics">그리기에 사용되는 그래픽 개체입니다.</param>
		/// <param name="renderRect">그리기에 대상이 되는 셀의 안쪽 영역입니다.</param>
		/// <param name="clipRect">클립 영역을 나타냅니다.</param>
		/// <param name="style">그리기에 대상이 되는 셀의 스타일 정보를 가져올 수 있는 개체입니다.</param>
		/// <param name="value">그리기에 대상이 되는 셀의 값입니다.</param>
		void PaintValue(_Graphics^ graphics, _Rectangle renderRect, _Rectangle clipRect, IStyle^ style, object^ value);
	};

	interface class IColumnControl : IControlAttacher
	{
		void SetControlLayout(_Control^ control, _Point location, _Size size);

	};

	interface class IColumnDropDown : IControlAttacher
	{
		_Size GetPreferredSize(_Size proposedSize);

		property bool Sizable
		{
			bool get();
		}
	};

	/// <summary>
	/// 숫자 특수 문자와 같은 형태의 키 이벤트로 셀을 편집할 수 있는지를 나타내는 개체입니다.
	/// </summary>
	public interface class IEditByKeyCode
	{
		/// <summary>
		/// 셀편집을 할 수 있는 키인지 확인합니다.
		/// </summary>
		/// <param name="key">키 이벤트로 발생한 키 값입니다.</param>
		/// <returns>
		/// 셀편집을 하려면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		bool KeyTest(System::Windows::Forms::Keys key);
	};

	/// <summary>
	/// 알파벳과 같은 일반적인 문자형태의 키 이벤트로 셀을 편집할 수 있는지를 나타내는 개체입니다.
	/// </summary>
	public interface class IEditByChar
	{

	};

	/// <summary>
	/// ime형태의 키 이벤트로 셀을 편집할 수 있는지를 나타내는 개체입니다.
	/// </summary>
	public interface class IEditByIme
	{

	};

	/// <summary>
	/// 정렬 방식을 지정합니다.
	/// </summary>
	public enum class SortType : int
	{
		/// <summary>
		/// 정렬을 하지 않습니다.
		/// </summary>
		None,

		/// <summary>
		/// 오름차순으로 정렬을 합니다.
		/// </summary>
		Up,

		/// <summary>
		/// 내림차순으로 정렬을 합니다.
		/// </summary>
		Down,
	};

	typedef Ntreev::Windows::Forms::Grid::SortType	_SortType;

	/// <summary>
	/// 셀을 편집하는 이벤트의 데이터를 제공합니다.
	/// </summary>
	public ref class EditValueEventArgs : _EventArgs
	{
	public: // methods
		/// <summary>
		/// <see cref="DetachEventArgs"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="by">어떠한 방식으로 셀 편집이 시작되었는지를 나타내는 <see cref="EditingReason"/>개체입니다.</param>
		/// <param name="value">편집할 값을 나타냅니다.</param>
		EditValueEventArgs(EditingReason^ by, object^ value)
			: m_editingReason(by), m_value(value), m_suppressEditing(false)
		{

		}

	public: // properties
		/// <summary>
		/// 셀의 편집이 어떠한 방식으로 시작되었는지를 나타내는 <see cref="EditingReason"/>개체를 가져옵니다.
		/// </summary>
		property EditingReason^ By
		{
			EditingReason^ get() { return m_editingReason; }
		}

		/// <summary>
		/// 편집될 값을 가져오거나 설정합니다.
		/// </summary>
		property object^ Value
		{
			object^ get() { return m_value; }
			void set(object^ value) { m_value = value; } 
		}

		/// <summary>
		/// 컨트롤을 사용하여 계속 편집을 진행할지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 편집을 종료하려면 true를, 컨트롤을 사용하여 계속 편집을 진행하려면 false를 반환합니다.
		/// </returns>
		property bool SuppressEditing
		{
			bool get() { return m_suppressEditing; }
			void set(bool value) { m_suppressEditing = value; }
		}

	private: // variables
		object^		m_value;
		EditingReason^	m_editingReason;
		bool		m_suppressEditing;
	};

	/// <summary>
	/// 셀을 편집하는 이벤트를 처리하는 메서드를 나타냅니다.
	/// </summary>
	public delegate void EditValueEventHandler(object^ sender, EditValueEventArgs^ e);

	typedef Ntreev::Windows::Forms::Grid::_EditingResult	_EditingResult;

	/// <summary>
	/// 마우스 커서에 따른 열의 상태를 나타냅니다.
	/// </summary>
	public enum class ColumnState : int
	{
		/// <summary>
		/// 마우스 커서가 열의 영역에 있지 않은 일반적인 상태입니다.
		/// </summary>
		Normal,

		/// <summary>
		/// 마우스 커서가 열의 영역에 있고 좌측 버튼이 눌러진 상태입니다.
		/// </summary>
		Pressed,

		/// <summary>
		/// 마우스 커서가 열의 영역에 있는 상태입니다.
		/// </summary>
		Hot,
	};

	typedef Ntreev::Windows::Forms::Grid::ColumnState _ColumnState;

	/// <summary>
	/// 열을 나타내기위한 속성을 정의합니다.
	/// </summary>
	public interface class IColumnDescriptor
	{
		/// <summary>
		/// 표시할 열의 제목을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 표시할 열의 제목을 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		property string^ Title
		{
			string^ get();
		}

		/// <summary>
		/// 열의 이름을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열의 이름을 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		property string^ ColumnName
		{
			string^ get();
		}
		
		/// <summary>
		/// 열의 툴팁을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열의 툴팁을 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		property string^ Tooltip
		{
			string^ get();
		}

		/// <summary>
		/// 열의 고유 아이디를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 고유 아이디를 나타내는 <see cref="System::UInt32"/>형태의 정수값입니다.
		/// </returns>
		property uint ColumnID
		{
			uint get(); 
		}

		/// <summary>
		/// 열의 너비를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열의 너비를 나타내는 <see cref="System::Int32"/>형태의 정수값입니다.
		/// </returns>
		property int Width
		{
			int get();
		}

		/// <summary>
		/// 열의 최소 너비를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열의 최소 너비를 나타내는 <see cref="System::Int32"/>형태의 정수값입니다.
		/// </returns>
		property int MinWidth
		{
			int get();
		}

		/// <summary>
		/// 열의 최대 너비를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열의 최대 너비를 나타내는 <see cref="System::Int32"/>형태의 정수값입니다.
		/// </returns>
		property int MaxWidth
		{
			int get();
		}

		/// <summary>
		/// 열의 표시 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열이 표시되면 true를, 그렇지 않다면 false를 가져옵니다.
		/// </returns>
		property bool IsVisible
		{
			bool get();
		}

		/// <summary>
		/// 열의 위치 이동 여부를 가져옵니다.
		/// </summary>
		/// <remarks>
		/// 이 속성은 마우스를 이용하여 표시되는 열의 위치를 바꿀수 있는지에 대한 여부를 확인하는 것입니다. 
		/// 이 속성의 값이 false이면 마우스를 이용하여 열의 위치 이동을 할 수 없습니다.
		/// </remarks>
		/// <returns>
		/// 열의 위치 이동이 가능하다면 true를, 그렇지 않다면 false를 가져옵니다.
		/// </returns>
		property bool IsMovable
		{
			bool get();
		}

		/// <summary>
		/// 열의 너비 조절 여부를 가져옵니다.
		/// </summary>
		/// <remarks>
		/// 이 속성은 마우스를 이용하여 열의 너비를 조절할 수 있는지에 대한 여부를 확인합니다. 
		/// 이 속성의 값이 false이면 마우스를 이용하여 열의 너비조절을 할 수 없습니다.
		/// </remarks>
		/// <returns>
		/// 열의 너비 조절이 가능하면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		property bool IsResizable
		{
			bool get();
		}

		/// <summary>
		/// 수평 스크롤시 스크롤에 적용 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 수평 스크롤시 스크롤에 적용이 되지 않으면 true, 적용이 되면 false를 반환합니다.
		/// </returns>
		/// <remarks>
		/// 열의 갯수가 많아 화면에 다 표시 되지 못할때는 수평 스크롤을 통해서 보이지 않는 열을 볼 수 있습니다. 
		/// 이 속성의 값이 true이면 분할면을 기준으로 좌측에 배치되어 수평 스크롤을 해도 움직이지 않고 고정되어 항상 화면에 표시할 수 있습니다.
		/// </remarks>
		property bool IsFrozen
		{
			bool get();
		}

		/// <summary>
		/// 열이 읽기 전용인지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열이 읽기 전용이라면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		property bool IsReadOnly
		{
			bool get();
		}

		/// <summary>
		/// 정렬 가능 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 정렬이 가능하다면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		property bool CanBeSorted
		{
			bool get();
		}

		/// <summary>
		/// 소유한 셀들의 선택 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 소유한 셀들중 한개 이상이라도 선택이 되었다면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		property bool IsSelected
		{
			bool get();
		}

		/// <summary>
		/// 소유한 셀들이 모두 선택되어 있는지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 소유한 셀들이 모두 선택되었다면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		property bool IsFullSelected
		{
			bool get(); 
		}

		/// <summary>
		/// 열의 그룹핑 기능이 적용되었는지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열에 그룹핑 기능이 적용되었다면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		property bool IsGrouped
		{
			bool get();
		}

		/// <summary>
		/// 소유한 셀들이 가질 값의 데이터 타입을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 데이터 타입을 나타내는 <see cref="System::Type"/>입니다.
		/// </returns>
		property _Type^ DataType
		{
			_Type^ get();
		}

		/// <summary>
		/// 열의 정렬방식을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열의 정렬방식을 나타내는 <see cref="SortType"/>입니다.
		/// </returns>
		property _SortType SortType
		{
			_SortType get();
		}
		
		/// <summary>
		/// 열이 표시되는 순서를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열이 보여지지 않거나 그리드 컨트롤에 종속되어 있지 안다면 -1을 반환합니다.
		/// </returns>
		property int VisibleIndex
		{
			int get(); 
		}

		/// <summary>
		/// 열이 화면에 표시되는 순서를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열이 화면에 보여지지 않다면 -1을 반환합니다.
		/// </returns>
		property int DisplayIndex
		{
			int get(); 
		}

		/// <summary>
		/// 그리드 컨트롤에 종속된 순서를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 그리드 컨트롤에 종속되었다면 종속된 순서(0부터 시작)를 반환하고, 그렇지 않다면 -1를 반환합니다.
		/// </returns>
		property int Index
		{
			int get();
		}

		/// <summary>
		/// 열의 대한 사용자 정의 데이터를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열의 대한 사용자 정의 데이터를 나타내는 <see cref="System::Object"/>입니다.
		/// </returns>
		property object^ Tag
		{
			object^ get();
		}

		/// <summary>
		/// 마우스 커서에 대한 열의 상태를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 마우스 커서에 대한 열의 상태를 나타내는 <see cref="ColumnState"/>입니다.
		/// </returns>
		property _ColumnState ColumnState
		{
			_ColumnState get();
		}

		/// <summary>
		/// 셀을 편집하는 방식을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 셀의 편집방식을 나타내는 <see cref="EditingType"/>입니다.
		/// </returns>
		property _EditingType EditingType
		{
			_EditingType get(); 
		}
	};

	/// <summary>
	/// 열을 그릴수 있는 개체입니다.
	/// </summary>
	public ref class ColumnPainter abstract
	{
	private: // typedefs
		typedef System::Windows::Forms::VisualStyles::VisualStyleRenderer	_VisualStyleRenderer;
		typedef System::Windows::Forms::VisualStyles::VisualStyleElement	_VisualStyleElement;

		typedef System::Windows::Forms::ControlPaint	_ControlPaint;
		typedef System::Windows::Forms::ScrollButton	_ScrollButton;
		typedef System::Windows::Forms::ButtonState		_ButtonState;

	public: // methods
		/// <summary>
		/// <see cref="ColumnPainter"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		ColumnPainter();

		/// <summary>
		/// 열의 배경을 그립니다.
		/// </summary>
		/// <returns>
		/// 그리기가 성공하여 기본 그리기 수행이 필요 없을때는 true, 그리기가 실패하여 기본 그리기를 수행할 필요가 있다면 false를 반환합니다.
		/// </returns>
		/// <param name="g">그리기에 사용되는 그래픽 개체입니다.</param>
		/// <param name="renderRect">그리기에 대상이 되는 열의 영역입니다.</param>
		/// <param name="columnDescriptor">그리기에 대상이 되는 열의 정보를 가져올 수 있는 개체입니다.</param>
		/// <param name="style">그리기에 대상이 되는 열의 스타일 정보를 가져올 수 있는 개체입니다.</param>
		virtual bool PaintBackground(_Graphics^ g, _Rectangle renderRect, IColumnDescriptor^ columnDescriptor, IStyle^ style) abstract;

		/// <summary>
		/// 열의 배경을 제외한 나머지를 그립니다.
		/// </summary>
		/// <remarks>
		/// 배경을 제외한 텍스트 출력 및 정렬 표시등을 그리고 싶다면 이 메서드를 구현하면 됩니다.
		/// </remarks>
		/// <returns>
		/// 그리기가 성공하여 기본 그리기 수행이 필요 없을때는 true, 그리기가 실패하여 기본 그리기를 수행할 필요가 있다면 false를 반환합니다.
		/// </returns>
		/// <param name="g">그리기에 사용되는 그래픽 개체입니다.</param>
		/// <param name="renderRect">그리기에 대상이 되는 열의 영역입니다.</param>
		/// <param name="columnDescriptor">그리기에 대상이 되는 열의 정보를 가져올 수 있는 개체입니다.</param>
		/// <param name="style">그리기에 대상이 되는 열의 스타일 정보를 가져올 수 있는 개체입니다.</param>
		virtual bool PaintContents(_Graphics^ g, _Rectangle renderRect, IColumnDescriptor^ columnDescriptor, IStyle^ style) abstract;

	protected: // methods
		/// <summary>
		/// 기본 그리기에서 정렬방식을 표현하는 화살표를 그립니다.
		/// </summary>
		/// <param name="g">그리기에 사용되는 그래픽 개체입니다.</param>
		/// <param name="renderRect">화살표를 그리기 위한 영역입니다.</param>
		/// <param name="sortType">정렬방식을 나타내는 <see cref="SortType"/>입니다.</param>
		void DrawSortArrow(_Graphics^ g, _Rectangle renderRect, SortType sortType);

	private: // variables
		_VisualStyleRenderer^	m_sortedDownRenderer;
        _VisualStyleRenderer^	m_sortedUpRenderer;
	};

	/// <summary>
	/// 열을 나타냅니다.
	/// </summary>
	[System::ComponentModel::TypeConverter(System::ComponentModel::ExpandableObjectConverter::typeid)]
	[System::ComponentModel::ToolboxItem(false)]
	[System::ComponentModel::DesignTimeVisible(false)]

	public ref class Column abstract : CellBase, IColumnDescriptor, System::ComponentModel::IComponent, System::ComponentModel::ISite
	{
	internal: // typedefs
		typedef System::ComponentModel::TypeConverter		_TypeConverter;
		typedef Ntreev::Windows::Forms::Grid::ColumnPainter	_ColumnPainter;

	public: // methods
		/// <summary>
		/// <see cref="Column"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		Column();

		/// <summary>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
		/// </summary>
		/// <returns>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		virtual	string^ ToString() override;
		
		/// <summary>
		/// 사용되지 않습니다.
		/// </summary>
		[System::ObsoleteAttribute]
		void RefreshAll();

		/// <summary>
		/// 셀의 전경색을 기본값으로 되돌립니다.
		/// </summary>
		void ResetCellForeColor();

		/// <summary>
		/// 셀의 배경색을 기본값으로 되돌립니다.
		/// </summary>
		void ResetCellBackColor();

		/// <summary>
		/// 셀의 글꼴을 기본값으로 되돌립니다.
		/// </summary>
		void ResetCellFont();

	public: // properties
		/// <summary>
		/// 표시할 열의 제목을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 표시할 열의 제목을 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		property string^ Title
		{
			virtual string^ get() sealed;
			void set(string^); 
		}

		/// <summary>
		/// 열의 이름을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 열의 이름을 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		/// <remarks>
		/// 열의 이름은 일종의 키 값으로 사용됩니다. 따라서 다른 열과 이름이 같을경우 예외가 발생합니다.
		/// </remarks>
		/// <exception cref="System::ArgumentException">이름을 빈 문자열로 설정하거나. <see cref="GridControl"/>에 종속되어 있는 경우 같은 이름을 가진 열이 이미 있을때.</exception>
		property string^ ColumnName
		{
			virtual string^ get() sealed;
			virtual void set(string^); 
		}

		/// <summary>
		/// 열의 툴팁을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 열의 툴팁을 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		[_DefaultValue("")]
		property string^ Tooltip
		{
			virtual string^ get() sealed;
			void set(string^);
		}

		/// <summary>
		/// 열의 고유 아이디를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 고유 아이디를 나타내는 <see cref="System::UInt32"/>형태의 정수값입니다.
		/// </returns>
		[_Browsable(false)]
		property uint ColumnID
		{
			virtual uint get() sealed;
		}

		/// <summary>
		/// 열의 너비를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 최소 너비보가 작거나 최대 너비보다 클 경우 자동으로 값이 수정됩니다.
		/// </remarks>
		/// <returns>
		/// 열의 너비를 나타내는 <see cref="System::Int32"/>형태의 정수값입니다.
		/// </returns>
		/// <exception cref="System::ArgumentException">0보다 작은 수를 설정할때.</exception>
		[_Category("Layout"), _DefaultValue(100)]
		property int Width
		{
			virtual int get() sealed;
			void set(int);
		}

		/// <summary>
		/// 열의 최소 너비를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 열의 최소 너비를 나타내는 <see cref="System::Int32"/>형태의 정수값입니다.
		/// </returns>
		/// <exception cref="System::ArgumentException">0보다 작거나 최대 너비보다 큰 수를 설정할때.</exception>
		[_Category("Layout"), _DefaultValue(30)]
		property int MinWidth
		{
			virtual int get() sealed;
			void set(int); 
		}

		/// <summary>
		/// 열의 최대 너비를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 열의 최대 너비를 나타내는 <see cref="System::Int32"/>형태의 정수값입니다.
		/// </returns>
		/// <exception cref="System::ArgumentException">0보다 작거나 최소 너비보다 작은 수를 설정할때.</exception>
		[_Category("Layout"), _DefaultValue(10000)]
		property int MaxWidth
		{
			virtual int get() sealed;
			void set(int);
		}

		/// <summary>
		/// 열의 표시 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 열이 표시되면 true를, 그렇지 않다면 false를 가져옵니다.
		/// </returns>
		[_Category("Behavior"), _DefaultValue(true)]
		property bool IsVisible
		{
			virtual bool get() sealed;
			void set(bool); 
		}

		/// <summary>
		/// 열의 위치 이동 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성은 마우스를 이용하여 표시되는 열의 위치를 바꿀수 있는지에 대한 여부를 확인하는 것입니다. 
		/// 이 속성의 값이 false이면 마우스를 이용하여 열의 위치 이동을 할 수 없습니다.
		/// </remarks>
		/// <returns>
		/// 열의 위치 이동이 가능하다면 true를, 그렇지 않다면 false를 가져옵니다.
		/// </returns>
		[_Category("Behavior"), _DefaultValue(true)]
		property bool IsMovable
		{
			virtual bool get() sealed;
			void set(bool);
		}

		/// <summary>
		/// 열의 너비 조절 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <remarks>
		/// 이 속성은 마우스를 이용하여 열의 너비를 조절할 수 있는지에 대한 여부를 확인합니다. 
		/// 이 속성의 값이 false이면 마우스를 이용하여 열의 너비조절을 할 수 없습니다.
		/// </remarks>
		/// <returns>
		/// 열의 너비 조절이 가능하면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		[_Category("Behavior"), _DefaultValue(true)]
		property bool IsResizable
		{
			virtual bool get() sealed;
			void set(bool);
		}

		/// <summary>
		/// 수평 스크롤시 스크롤에 적용 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 수평 스크롤시 스크롤에 적용이 되지 않으면 true, 적용이 되면 false를 반환합니다.
		/// </returns>
		/// <remarks>
		/// 열의 갯수가 많아 화면에 다 표시 되지 못할때는 수평 스크롤을 통해서 보이지 않는 열을 볼 수 있습니다. 
		/// 이 속성의 값이 true이면 분할면을 기준으로 좌측에 배치되어 수평 스크롤을 해도 움직이지 않고 고정되어 항상 화면에 표시할 수 있습니다.
		/// </remarks>
		[_Category("Behavior"), _DefaultValue(false)]
		property bool IsFrozen
		{
			virtual bool get() sealed;
			void set(bool);
		}

		/// <summary>
		/// 열이 읽기 전용인지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 열이 읽기 전용이라면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		/// <remarks>
		/// 이 속성의 값이 true이면 열이 소유한 모든 셀이 읽기 전용으로 적용됩니다.
		/// </remarks>
		[_Category("Behavior"), _DefaultValue(false)]
		property bool IsReadOnly
		{
			virtual bool get() sealed;
			void set(bool);
		}

		/// <summary>
		/// 정렬 가능 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 정렬이 가능하다면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		[_Category("Behavior"), _DefaultValue(true)]
		property bool CanBeSorted
		{
			virtual bool get() sealed;
			void set(bool);
		}

		/// <summary>
		/// 소유한 셀들의 선택 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 소유한 셀들중 한개 이상이라도 선택이 되었다면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		/// <param name="value">
		/// 소유한 모든 셀을 선택하려면 true를, 해제하려면 false를 설정합니다.
		/// </param>
		[_Browsable(false)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		property bool IsSelected
		{
			virtual bool get() sealed;
			void set(bool);
		}

		/// <summary>
		/// 소유한 셀들이 모두 선택되어 있는지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 소유한 셀들이 모두 선택되었다면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		[_Browsable(false)]
		property bool IsFullSelected
		{
			virtual bool get() sealed; 
		}

		/// <summary>
		/// 열의 그룹핑 기능이 적용되었는지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 열에 그룹핑 기능이 적용되었다면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		[_DefaultValue(false)]
		property bool IsGrouped
		{
			virtual bool get() sealed;
			void set(bool);
		}

		/// <summary>
		/// 소유한 셀들이 가질 값의 데이터 타입을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 데이터 타입을 나타내는 <see cref="System::Type"/>입니다.
		/// </returns>
		[_DefaultValue(string::typeid)]
		[_EditorAttribute("Ntreev.Windows.Forms.Grid.Design.TypeSelector, Ntreev.Windows.Forms.Grid.Design, Version=1.0.0.1, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", System::Drawing::Design::UITypeEditor::typeid)]
		property _Type^ DataType
		{
			virtual _Type^ get() sealed;
			void set(_Type^);
		}

		/// <summary>
		/// 소유한 셀들이 가질 값의 데이터 타입 변환기를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 데이터 타입 변환기를 나타내는 <see cref="System::ComponentModel::TypeConverter"/>입니다.
		/// </returns>
		/// <remarks>
		/// 이 속성의 기본값은 <see cref="DataType"/>을 기준으로 설정됩니다. 추가적인 타입에 대한 변환를 설정하려면 새로운 타입 변환기를 정의하여 설정하면 됩니다.
		/// </remarks>
		[_Browsable(false)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		property _TypeConverter^ TypeConverter
		{
			_TypeConverter^ get();
			void set(_TypeConverter^);
		}

		/// <summary>
		/// 열의 정렬방식을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 열의 정렬방식을 나타내는 <see cref="SortType"/>입니다.
		/// </returns>
		[_DefaultValue(_SortType::None)]
		property _SortType SortType
		{
			virtual _SortType get() sealed;
			void set(_SortType);
		}

		/// <summary>
		/// 열의 정렬을 하기 위해 비교하는 개체를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 값의 비교를 위한 개체를 나타내는 <see cref="System::Collections::IComparer"/>입니다.
		/// </returns>
		[_Browsable(false), _DefaultValue((string^)nullptr)]
		property System::Collections::IComparer^ SortComparer
		{
			System::Collections::IComparer^ get();
			void set(System::Collections::IComparer^);
		}

		/// <summary>
		/// 열이 표시되는 순서를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열이 보여지지 않거나 그리드 컨트롤에 종속되어 있지 안다면 -1을 반환합니다.
		/// </returns>
		[_Browsable(false)]
		property int VisibleIndex
		{
			virtual int get() sealed; 
		}

		/// <summary>
		/// 열이 화면에 표시되는 순서를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 열이 화면에 보여지지 않다면 -1을 반환합니다.
		/// </returns>
		[_Browsable(false)]
		property int DisplayIndex
		{
			virtual int get() sealed;
		}

		/// <summary>
		/// 그리드 컨트롤에 종속된 순서를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 그리드 컨트롤에 종속되었다면 종속된 순서(0부터 시작)를 반환하고, 그렇지 않다면 -1를 반환합니다.
		/// </returns>
		[_Description("컨트롤에 내에 Column 인덱스를 가져옵니다.")]
		[_Category("Debug")]
		property int Index
		{
			virtual int get() sealed; 
		}

		/// <summary>
		/// 셀의 편집 결과를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 셀의 편집 결과를 나타내는 <see cref="EditingResult"/>입니다.
		/// </returns>
		[_Browsable(false), _DefaultValue(_EditingResult::Ok)]
		property _EditingResult EditingResult
		{
			_EditingResult get();
			void set(_EditingResult);
		}

		/// <summary>
		/// 셀의 기본값을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 셀의 기본값을 나타내는 <see cref="System::Object"/>입니다.
		/// </returns>
		/// <remarks>
		/// 이 속성의 사용 목적은 새로운 행을 추가 하기 위하여 제공되는 <see cref="InsertionRow"/>에 기본값을 제공하기 위해서입니다. 
		/// 데이터 타입에 대한 검사가 없으므로, 가급적 <see cref="DataType"/>과 같은 타입의 개체를 사용하시기 바랍니다.
		/// </remarks>
		[_Browsable(false)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		property object^ DefaultValue
		{
			virtual object^ get();
			virtual void set(object^);
		}

		/// <summary>
		/// 셀의 편집이 비정상적으로 종료되었을때 처리하는 기본 결과를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 셀의 편집 결과를 나타내는 <see cref="EditingResult"/>입니다.
		/// </returns>
		/// <remarks>
		/// 셀 편집시에 창이 비 활성화가 되거나 다른 셀을 클릭하는 등의 비 정상적으로 편집이 종료가 되어야 할 경우에 편집 결과를 어떻게 설정할지 확인합니다.
		/// 예를들면 비정상 종료시 문자열 경우에는 편집 내용이 반영되는 반면에 콤보 박스 경우에는 편집 내용이 반영되지 않습니다.
		/// </remarks>
		[_Browsable(false)]
		property _EditingResult DefaultEditingResult
		{ 
			virtual _EditingResult get();
		}

		/// <summary>
		/// 열을 그릴수 있는 개체를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 열을 그릴수 있는 개체를 나타내는 <see cref="ColumnPainter"/>입니다.
		/// </returns>
		[_Browsable(false)]
		[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Hidden)]
		property _ColumnPainter^ ColumnPainter
		{
			_ColumnPainter^ get();
			void set(_ColumnPainter^);
		}

		/// <summary>
		/// 셀의 값을 문자열로 출력시에 사용하는 수평 정렬방식을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 문자열 정렬 방식을 나타내는 <see cref="System::Drawing::StringAlignment"/>입니다.
		/// </returns>
		[_Category("Cell"), _DefaultValue(_StringAlignment::Near)]
		property _StringAlignment CellAlignment
		{
			_StringAlignment get();
			void set(_StringAlignment);
		}

		/// <summary>
		/// 셀의 값을 문자열로 출력시에 사용하는 수직 정렬방식을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 문자열 정렬 방식을 나타내는 <see cref="System::Drawing::StringAlignment"/>입니다.
		/// </returns>
		[_Category("Cell"), _DefaultValue(_StringAlignment::Near)]
		property _StringAlignment CellLineAlignment
		{
			_StringAlignment get();
			void set(_StringAlignment);
		}

		/// <summary>
		/// 셀의 값을 문자열로 출력시에 자동개행을 적용할 지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 자동개행을 적용하면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		[_Category("Cell"), _DefaultValue(false)]
		property bool CellWordWrap
		{
			bool get();
			void set(bool);
		}

		/// <summary>
		/// 셀의 값을 문자열로 출력시에 멀티라인을 적용할 지에 대한 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 멀티라인 적용하면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		[_Category("Cell"), _DefaultValue(false)]
		property bool CellMultiline
		{
			bool get();
			void set(bool);
		}

		/// <summary>
		/// 셀의 전경색을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 셀의 전경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
		/// </returns>
		[_Category("Cell")]
		property _Color CellForeColor
		{
			_Color get();
			void set(_Color);
		}

		/// <summary>
		/// 셀의 배경색을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 셀의 배경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
		/// </returns>
		[_Category("Cell")]
		property _Color CellBackColor
		{
			_Color get();
			void set(_Color);
		}

		/// <summary>
		/// 셀의 글꼴을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 셀의 글꼴을 나타내는 <see cref="System::Drawing::Font"/>입니다.
		/// </returns>
		[_Category("Cell")]
		property _Font^ CellFont
		{
			_Font^ get();
			void set(_Font^); 
		}

		/// <summary>
		/// 셀을 편집하는 방식을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 셀의 편집방식을 나타내는 <see cref="EditingType"/>입니다.
		/// </returns>
		[_Browsable(false)]
		property _EditingType EditingType
		{
			virtual _EditingType get() abstract; 
		}

	protected: // methods
		/// <summary>
		/// 소멸자 입니다.
		/// </summary>
		~Column();
		

		/// <summary>
		/// 값이 편집될때 호출됩니다.
		/// </summary>
		/// <param name="e">
		/// 값이 편집될때 필요한 데이터를 제공하는 <see cref="EditValueEventArgs"/>개체입니다.
		/// </param>
		virtual void OnEditValue(EditValueEventArgs^ e);

	internal: // methods
		void Invoke_EditValue(EditValueEventArgs^ e) { OnEditValue(e); }

		static Column^ FromNative(const GrColumn* pColumn);

		virtual	void OnEditingResultChanged(object^ sender, ColumnEventArgs^ e);

	internal: // properties
		property _PropertyDescriptor^ PropertyDescriptor
		{
			_PropertyDescriptor^ get();
			void set(_PropertyDescriptor^);
		}

		property GrColumn* NativeRef
		{
			GrColumn* get() { return m_pColumn; }
		}

	internal: // events
		event ColumnEventHandler^ EditingResultChanged
		{
			void add(ColumnEventHandler^ p) { m_eventEditingResultChanged += p; }
			void remove(ColumnEventHandler^ p) { m_eventEditingResultChanged -= p; }
		private:
			void raise(object^ sender, ColumnEventArgs^ e) { if(m_eventEditingResultChanged != nullptr) { m_eventEditingResultChanged->Invoke(sender, e); } }
		}

		event EditValueEventHandler^ EditValue
		{
			void add(EditValueEventHandler^ p) { m_eventEditValue += p; }
			void remove(EditValueEventHandler^ p) { m_eventEditValue -= p; }
		private:
			void raise(object^ sender, EditValueEventArgs^ e) { if(m_eventEditValue != nullptr) { m_eventEditValue->Invoke(sender, e); } }
		}

	private: // methods
		virtual object^ GetService(_Type^ serviceType) sealed = System::IServiceProvider::GetService
		{
			if(m_site != nullptr)
				return m_site->GetService(serviceType);
			return nullptr;
		}

		void ValidateDefaultValue();

		bool ShouldSerializeCellForeColor();
		bool ShouldSerializeCellBackColor();
		bool ShouldSerializeCellFont();
		bool ShouldSerializeTitle();
		
		
	private: // properties
		property object^ Tag_IColumnDescriptor
		{
			virtual object^ get() sealed = IColumnDescriptor::Tag::get { return this->Tag; }
		}

		property _ColumnState ColumnState
		{
			virtual _ColumnState get() sealed = IColumnDescriptor::ColumnState::get;
		}

		property System::ComponentModel::ISite^ Site_IComponent
		{
			virtual System::ComponentModel::ISite^ get() sealed = System::ComponentModel::IComponent::Site::get { return m_site; }
			virtual void set(System::ComponentModel::ISite^ value) sealed  = System::ComponentModel::IComponent::Site::set { m_site = value; }
		}

		property string^ Name_ISite
		{
			virtual string^ get() sealed = System::ComponentModel::ISite::Name::get { return m_siteName; }
			virtual void set(string^ value) sealed = System::ComponentModel::ISite::Name::set { m_siteName = value; }
		}

		property System::ComponentModel::IComponent^ Component_ISite
		{
			virtual System::ComponentModel::IComponent^ get() sealed = System::ComponentModel::ISite::Component::get { return this; }
		}

		property System::ComponentModel::IContainer^ Container_ISite
		{
			virtual System::ComponentModel::IContainer^ get() sealed = System::ComponentModel::ISite::Container::get 
			{
				if(m_site == nullptr)
					return nullptr;

				return m_site->Container; 
			}
		}
	
		property bool DesignMode_ISite
		{
			virtual bool get() sealed = System::ComponentModel::ISite::DesignMode::get
			{
				return ((m_site != nullptr) && m_site->DesignMode);
			}
		}

	private: // events
		event _EventHandler^ Disposed_IComponent
		{
			virtual void add(_EventHandler^ p) sealed = System::ComponentModel::IComponent::Disposed::add { m_eventDisposed += p; }
			virtual void remove(_EventHandler^ p)  sealed = System::ComponentModel::IComponent::Disposed::remove { m_eventDisposed -= p; }
		private:
			virtual void raise(object^ sender, _EventArgs^ e) sealed { if(m_eventDisposed != nullptr) { m_eventDisposed->Invoke(sender, e); } }
		}
	
	private: // variables
		_Type^						m_dataType;
		_TypeConverter^				m_typeConverter;
		string^						m_title;
		string^						m_name;
		string^						m_tooltip;
		_PropertyDescriptor^		m_propertyDescriptor;
		System::Collections::IComparer^ m_sortComparer;

		_EditingResult				m_editingResult;
		object^						m_defaultValue;
		_ColumnPainter^				m_columnPainter;

		GrColumn*					m_pColumn;
		class CustomRender*			m_pCustomRender;

		EditValueEventHandler^		m_eventEditValue;
		ColumnEventHandler^			m_eventEditingResultChanged;

		_EventHandler^				m_eventDisposed;

		System::ComponentModel::ISite^	m_site;

		string^						m_siteName;
	};

	/// <summary>
	/// 모달형식으로 셀을 편집할 수 있는 기능을 제공합니다.
	/// </summary>
	public ref class ColumnModal abstract : Column
	{
	public: // properties
		/// <summary>
		/// 셀을 편집하는 방식을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 셀의 편집방식을 나타내는 <see cref="EditingType"/>입니다.
		/// </returns>
		property _EditingType EditingType
		{ 
			virtual _EditingType get() override sealed { return _EditingType::Modal; } 
		}
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/