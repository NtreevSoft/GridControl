#pragma once
#include "IColumn.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// ���� �׸��� �ִ� ��ü�Դϴ�.
    /// </summary>
    [System::ComponentModel::ToolboxItem(true)]
    [System::Drawing::ToolboxBitmap(ColumnPainter::typeid)]
    public ref class ColumnPainter abstract : System::ComponentModel::Component
    {
    public: // methods
        /// <summary>
        /// <see cref="ColumnPainter"/>Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�.
        /// </summary>
        ColumnPainter();

        /// <summary>
        /// ���� ����� �׸��ϴ�.
        /// </summary>
        /// <returns>
        /// �׸��Ⱑ �����Ͽ� �⺻ �׸��� ������ �ʿ� �������� true, �׸��Ⱑ �����Ͽ� �⺻ �׸��⸦ ������ �ʿ䰡 �ִٸ� false�� ��ȯ�մϴ�.
        /// </returns>
        /// <param name="g">�׸��⿡ ���Ǵ� �׷��� ��ü�Դϴ�.</param>
        /// <param name="paintRect">�׸��⿡ ����� �Ǵ� ���� �����Դϴ�.</param>
        /// <param name="column">�׸��⿡ ����� �Ǵ� ���� ������ ������ �� �ִ� ��ü�Դϴ�.</param>
        virtual bool PaintBackground(System::Drawing::Graphics^ g, System::Drawing::Rectangle paintRect, IColumn^ column) abstract;

        /// <summary>
        /// ���� ����� ������ �������� �׸��ϴ�.
        /// </summary>
        /// <remarks>
        /// ����� ������ �ؽ�Ʈ ��� �� ���� ǥ�õ��� �׸��� �ʹٸ� �� �޼��带 �����ϸ� �˴ϴ�.
        /// </remarks>
        /// <returns>
        /// �׸��Ⱑ �����Ͽ� �⺻ �׸��� ������ �ʿ� �������� true, �׸��Ⱑ �����Ͽ� �⺻ �׸��⸦ ������ �ʿ䰡 �ִٸ� false�� ��ȯ�մϴ�.
        /// </returns>
        /// <param name="g">�׸��⿡ ���Ǵ� �׷��� ��ü�Դϴ�.</param>
        /// <param name="paintRect">�׸��⿡ ����� �Ǵ� ���� �����Դϴ�.</param>
        /// <param name="column">�׸��⿡ ����� �Ǵ� ���� ������ ������ �� �ִ� ��ü�Դϴ�.</param>
        virtual bool PaintContents(System::Drawing::Graphics^ g, System::Drawing::Rectangle paintRect, IColumn^ column) abstract;

    protected: // methods
        /// <summary>
        /// �⺻ �׸��⿡�� ���Ĺ���� ǥ���ϴ� ȭ��ǥ�� �׸��ϴ�.
        /// </summary>
        /// <param name="g">�׸��⿡ ���Ǵ� �׷��� ��ü�Դϴ�.</param>
        /// <param name="paintRect">ȭ��ǥ�� �׸��� ���� �����Դϴ�.</param>
        /// <param name="sortType">���Ĺ���� ��Ÿ���� <see cref="SortType"/>�Դϴ�.</param>
        void DrawSortArrow(System::Drawing::Graphics^ g, System::Drawing::Rectangle paintRect, SortType sortType);

    private: // variables
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_sortedDownRenderer;
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_sortedUpPainter;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/