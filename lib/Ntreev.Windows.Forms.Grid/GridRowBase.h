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

	private: // methods
		bool ShouldSerializeHeight();
		bool ShouldSerializeMinHeight();
		bool ShouldSerializeMaxHeight();

	private: // variables
		IDataRow*		m_pDataRow;
	};

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/