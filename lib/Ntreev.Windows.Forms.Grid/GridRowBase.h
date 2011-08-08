#pragma once
#include "GridCellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	public ref class RowBase : CellBase
	{
	public: // properties
		/// <summary>
		/// ���� ���̸� �������ų� �����մϴ�.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// �Ű������� 0���� ���� ���
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// �Ű������� <see cref="MinHeight"/>���� �۰ų� <see cref="MaxHeight"/>���� Ŭ���
		/// </exception>
		[_Description("���� �����Դϴ�.")]
		[_Category("Layout")]
		[_DefaultValue(21)]
		property int Height
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// ���� �ּҳ��̸� �������ų� �����մϴ�.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// �Ű������� 0���� ���� ���
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// �Ű������� <see cref="MaxHeight"/>���� Ŭ���
		/// </exception>
		[_Description("���� �ּҳ����Դϴ�.")]
		[_Category("Layout")]
		[_DefaultValue(0)]
		property int MinHeight
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// ���� �ִ���̸� �������ų� �����մϴ�.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// �Ű������� 0���� ���� ���
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// �Ű������� <see cref="MinHeight"/>���� �������
		/// </exception>
		[_Description("���� �ִ�����Դϴ�.")]
		[_Category("Layout")]
		[_DefaultValue(10000)]
		property int MaxHeight
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// ǥ�õǰ� �ִ����� ���θ� �������ų� �����մϴ�.
		/// </summary>
		/// <returns>
		/// ǥ�õǰ� �ִٸ� true�� ��ȯ�ϰ�, �׷��� �ʴٸ� false�� ��ȯ�մϴ�.
		/// </returns>
		[_Description("ǥ�õǰ� �ִ����� ���θ� �������ų� �����մϴ�.")]
		[_Category("Appearance")]
		[_DefaultValue(true)]
		property bool IsVisible 
		{
			bool get();

		}

		/// <summary>
		/// ǥ�õǴ� �ε����� �����ɴϴ�.
		/// </summary>
		/// <returns>
		/// ǥ�ð� �ȴٸ� �ε���(0���� ����)�� ��ȯ�ϰ�, �׷��� �ʴٸ� 0xffffffff�� ��ȯ�մϴ�.
		/// </returns>
		/// <remarks>
		/// ǥ�� ���θ� Ȯ���ϱ� ���ؼ��� <see cref="IsVisible"/>�� Ȯ���ϸ� �˴ϴ�.
		/// </remarks>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("ǥ�õǴ� �ε����Դϴ�.")]
#else
		[_Browsable(false)]
#endif
		property uint VisibleIndex
		{
			uint get(); 
		}

		/// <summary>
		/// ȭ�鿡 ǥ�õǴ� �ε����� �����ɴϴ�.
		/// </summary>
		/// <returns>
		/// ȭ�鿡 ǥ�ð� �ȴٸ� �ε���(0���� ����)�� ��ȯ�ϰ�, �׷��� �ʴٸ� 0xffffffff�� ��ȯ�մϴ�.
		/// </returns>
		/// <remarks>
		/// ȭ�鿡 ǥ�� ���θ� Ȯ���ϱ� ���ؼ��� <see cref="IsDisplayable"/>�� Ȯ���ϸ� �˴ϴ�.
		/// </remarks>
#ifdef _DEBUG
		[_Category("Debug")]
		[_Description("ȭ�鿡 ǥ�õǴ� �ε����Դϴ�")]
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