#pragma once
#include "GridCellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	public ref class RowBase : CellBase
	{
	public: // properties
		/// <summary>
		/// 행의 높이를 가져오거나 설정합니다.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 0보다 작을 경우
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 <see cref="MinHeight"/>보다 작거나 <see cref="MaxHeight"/>보다 클경우
		/// </exception>
		[_Description("열의 높이입니다.")]
		[_Category("Layout")]
		[_DefaultValue(21)]
		property int Height
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// 행의 최소높이를 가져오거나 설정합니다.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 0보다 작을 경우
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 <see cref="MaxHeight"/>보다 클경우
		/// </exception>
		[_Description("열의 최소높이입니다.")]
		[_Category("Layout")]
		[_DefaultValue(0)]
		property int MinHeight
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// 행의 최대높이를 가져오거나 설정합니다.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 0보다 작을 경우
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 <see cref="MinHeight"/>보다 작을경우
		/// </exception>
		[_Description("열의 최대높이입니다.")]
		[_Category("Layout")]
		[_DefaultValue(10000)]
		property int MaxHeight
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// 표시되고 있는지의 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 표시되고 있다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		[_Description("표시되고 있는지의 여부를 가져오거나 설정합니다.")]
		[_Category("Appearance")]
		[_DefaultValue(true)]
		property bool IsVisible 
		{
			bool get();

		}

		/// <summary>
		/// 표시되는 인덱스를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 표시가 된다면 인덱스(0부터 시작)를 반환하고, 그렇지 않다면 0xffffffff를 반환합니다.
		/// </returns>
		/// <remarks>
		/// 표시 여부를 확인하기 위해서는 <see cref="IsVisible"/>를 확인하면 됩니다.
		/// </remarks>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("표시되는 인덱스입니다.")]
#else
		[_Browsable(false)]
#endif
		property uint VisibleIndex
		{
			uint get(); 
		}

		/// <summary>
		/// 화면에 표시되는 인덱스를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 화면에 표시가 된다면 인덱스(0부터 시작)를 반환하고, 그렇지 않다면 0xffffffff를 반환합니다.
		/// </returns>
		/// <remarks>
		/// 화면에 표시 여부를 확인하기 위해서는 <see cref="IsDisplayable"/>를 확인하면 됩니다.
		/// </remarks>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("화면에 표시되는 인덱스입니다")]
#else
		[_Browsable(false)]
#endif
		property uint DisplayIndex
		{
			uint get(); 
		}

	internal: // methods
		RowBase(_GridControl^ gridControl, IDataRow* pDataRow);

	private: // variables
		IDataRow*		m_pDataRow;
	};

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/