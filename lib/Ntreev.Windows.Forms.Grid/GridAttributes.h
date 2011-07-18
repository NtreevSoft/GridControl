#pragma once

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 지정된 속성이 스크롤될 수 없는지 설정합니다.
	/// </summary>
	[System::AttributeUsage(System::AttributeTargets::Property)]
	public ref class FrozenAttribute : System::Attribute
	{
	public:
		/// <summary>
		/// <see cref="FrozenAttribute"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="frozen">스크롤될 수 없는지에 대한 여부를 설정합니다.</param>
		FrozenAttribute(bool frozen)
			: m_frozen(frozen)
		{

		}

	public:
		/// <summary>
		/// 스크롤 될 수 없는지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 스크롤 될 수 없으면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		property bool Frozen
		{
			bool get() { return m_frozen; }
		}

	private:
		bool m_frozen;
	};

	/// <summary>
	/// 지정된 속성이 위치 이동이 되는지 설정합니다.
	/// </summary>
	[System::AttributeUsage(System::AttributeTargets::Property)]
	public ref class MovableAttribute : System::Attribute
	{
	public:
		MovableAttribute(bool movable)
			: m_movable(movable)
		{

		}

	public:
		property bool IsMovable
		{
			bool get() { return m_movable; }
		}

	private:
		bool m_movable;
	};

	[System::AttributeUsage(System::AttributeTargets::Property)]
	public ref class ResizableAttribute : System::Attribute
	{
	public:
		ResizableAttribute(bool resizable) : m_resizable(resizable)
		{

		}

	public:
		property bool Resizable
		{
			bool get() { return m_resizable; }
		}

	private:
		bool m_resizable;
	};

	[System::AttributeUsage(System::AttributeTargets::Property)]
	public ref class SortableAttribute : System::Attribute
	{
	public:
		SortableAttribute(bool sortable) : m_sortable(sortable)
		{

		}

	public:
		property bool Sortable
		{
			bool get() { return m_sortable; }
		}

	private:
		bool m_sortable;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/