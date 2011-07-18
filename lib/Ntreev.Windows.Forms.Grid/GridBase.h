#pragma once

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// GridControl 속성을 얻을수 있는 인터페이스를 제공합니다.
	/// </summary>
	public interface class IGridControlProvider
	{
	public: // properties
		property _GridControl^ GridControl
		{
			_GridControl^ get();
		}
	};

	/// <summary>
	/// GridControl에 종속되어 사용되는 개체를 나타냅니다.
	/// </summary>
	public ref class GridObject abstract : IGridControlProvider//, System::IServiceProvider
	{
	public: // methods
		/// <summary>
		/// <see cref="GridObject"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		GridObject();

		/// <summary>
		/// <see cref="GridObject"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <param name="gridControl">종속될 그리드 컨트롤의 인스턴스입니다.</param>
		GridObject(_GridControl^ gridControl);

	public: // properties
		/// <summary>
		/// 종속된 그리드 컨트롤의 인스턴스를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 종속되었다면 그리드 컨트롤의 인스턴스를, 그렇지 않다면 null을 반환합니다.
		/// </returns>
#ifdef _DEBUG
		[_Category("Debug")]
#else
		[_Browsable(false)]
#endif
		property _GridControl^ GridControl 
		{
			virtual _GridControl^ get() sealed { return m_gridControl; }
		internal:
			void set(_GridControl^ value);
		}

	internal: // methods
		void Invalidate();

		//void InvalidateCell(const GrCell* pCell);

		//void Invalidate(_Rectangle rectangle);

		void Invalidate(int left, int top, int right, int bottom);

	internal: // properties
		property GrGridCore* GridCore
		{
			GrGridCore* get() { return m_pGridCore; }
		}

		property GrGridRenderer* GridRenderer 
		{
			GrGridRenderer* get() { return m_pGridRenderer; }
		}

		property GrItemSelector* Selector
		{
			GrItemSelector* get() { return m_pItemSelector; }
		}

		property GrFocuser* Focuser
		{
			GrFocuser* get() { return m_pFocuser; }
		}

	protected: // methods
		/// <summary>
		/// 지정된 형식의 서비스 개체를 가져옵니다.
		/// </summary>
		/// <param name="serviceType">가져올 서비스 개체의 형식을 지정하는 개체입니다. </param>
		/// <returns>
		/// serviceType 형식의 서비스 개체입니다.- 또는 - serviceType 형식의 서비스 개체가 없으면 null입니다.
		/// </returns>
		//virtual object^ GetService(_Type^ serviceType) sealed = System::IServiceProvider::GetService;


	private: // variables
		[System::NonSerialized]
		_GridControl^		m_gridControl;
		GrGridCore*			m_pGridCore;
		GrGridRenderer*		m_pGridRenderer;
		GrItemSelector*		m_pItemSelector;
		GrFocuser*			m_pFocuser;
	};

	generic<typename T>
	ref class Rotator : IGridControlProvider, System::Collections::Generic::List<T>
	{
	public: // methods
		Rotator(_GridControl^ gridControl)
			: m_gridControl(gridControl), m_previousIndex(0), m_currentIndex(0)
		{
				
		}

		void MoveNext()
		{
			if(Count == 0)
				throw gcnew System::NullReferenceException();
			m_previousIndex = m_currentIndex;
			m_currentIndex  = (m_currentIndex + 1) % Count;
		}

	public: // properties
		property T Current
		{
			T get() { return default[m_currentIndex]; }
		}

		property T Previous
		{
			T get() { return default[m_previousIndex]; }
		}

		property _GridControl^ GridControl
		{
			virtual _GridControl^ get() { return m_gridControl; }
		}

	private: // variables
		int				m_previousIndex;
		int				m_currentIndex;
		_GridControl^	m_gridControl;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/