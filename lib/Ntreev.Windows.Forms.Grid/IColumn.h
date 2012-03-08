#pragma once
#include "ICellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// ���� ��Ÿ�������� �Ӽ��� �����մϴ�.
    /// </summary>
    public interface class IColumn : ICellBase
    {
        /// <summary>
        /// ǥ���� ���� ������ �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ǥ���� ���� ������ ��Ÿ���� <see cref="System::String"/>�Դϴ�.
        /// </returns>
        property System::String^ Title
        {
            System::String^ get();
        }

        /// <summary>
        /// ���� �̸��� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� �̸��� ��Ÿ���� <see cref="System::String"/>�Դϴ�.
        /// </returns>
        property System::String^ ColumnName
        {
            System::String^ get();
        }

        /// <summary>
        /// ���� ������ �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� ������ ��Ÿ���� <see cref="System::String"/>�Դϴ�.
        /// </returns>
        property System::String^ Tooltip
        {
            System::String^ get();
        }

        /// <summary>
        /// ���� ���� ���̵� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� ���̵� ��Ÿ���� <see cref="System::UInt32"/>������ �������Դϴ�.
        /// </returns>
        property unsigned int ColumnID
        {
            unsigned int get(); 
        }

        /// <summary>
        /// ���� �ּ� �ʺ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� �ּ� �ʺ� ��Ÿ���� <see cref="System::Int32"/>������ �������Դϴ�.
        /// </returns>
        property int MinWidth
        {
            int get();
        }

        /// <summary>
        /// ���� �ִ� �ʺ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� �ִ� �ʺ� ��Ÿ���� <see cref="System::Int32"/>������ �������Դϴ�.
        /// </returns>
        property int MaxWidth
        {
            int get();
        }

        /// <summary>
        /// ���� ǥ�� ���θ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� ǥ�õǸ� true��, �׷��� �ʴٸ� false�� �����ɴϴ�.
        /// </returns>
        property bool IsVisible
        {
            bool get();
        }

        /// <summary>
        /// ���� ��ġ �̵� ���θ� �����ɴϴ�.
        /// </summary>
        /// <remarks>
        /// �� �Ӽ��� ���콺�� �̿��Ͽ� ǥ�õǴ� ���� ��ġ�� �ٲܼ� �ִ����� ���� ���θ� Ȯ���ϴ� ���Դϴ�. 
        /// �� �Ӽ��� ���� false�̸� ���콺�� �̿��Ͽ� ���� ��ġ �̵��� �� �� �����ϴ�.
        /// </remarks>
        /// <returns>
        /// ���� ��ġ �̵��� �����ϴٸ� true��, �׷��� �ʴٸ� false�� �����ɴϴ�.
        /// </returns>
        property bool IsMovable
        {
            bool get();
        }

        /// <summary>
        /// ���� �ʺ� ���� ���θ� �����ɴϴ�.
        /// </summary>
        /// <remarks>
        /// �� �Ӽ��� ���콺�� �̿��Ͽ� ���� �ʺ� ������ �� �ִ����� ���� ���θ� Ȯ���մϴ�. 
        /// �� �Ӽ��� ���� false�̸� ���콺�� �̿��Ͽ� ���� �ʺ������� �� �� �����ϴ�.
        /// </remarks>
        /// <returns>
        /// ���� �ʺ� ������ �����ϸ� true��, �׷��� �ʴٸ� false�� ��ȯ�մϴ�.
        /// </returns>
        property bool IsResizable
        {
            bool get();
        }

        /// <summary>
        /// ���� ��ũ�ѽ� ��ũ�ѿ� ���� ���θ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� ��ũ�ѽ� ��ũ�ѿ� ������ ���� ������ true, ������ �Ǹ� false�� ��ȯ�մϴ�.
        /// </returns>
        /// <remarks>
        /// ���� ������ ���� ȭ�鿡 �� ǥ�� ���� ���Ҷ��� ���� ��ũ���� ���ؼ� ������ �ʴ� ���� �� �� �ֽ��ϴ�. 
        /// �� �Ӽ��� ���� true�̸� ���Ҹ��� �������� ������ ��ġ�Ǿ� ���� ��ũ���� �ص� �������� �ʰ� �����Ǿ� �׻� ȭ�鿡 ǥ���� �� �ֽ��ϴ�.
        /// </remarks>
        property bool IsFrozen
        {
            bool get();
        }

        /// <summary>
        /// ���� �б� ���������� ���� ���θ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� �б� �����̶�� true��, �׷��� �ʴٸ� false�� ��ȯ�մϴ�.
        /// </returns>
        property bool IsReadOnly
        {
            bool get();
        }

        /// <summary>
        /// ���� ���� ���θ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ������ �����ϴٸ� true��, �׷��� �ʴٸ� false�� ��ȯ�մϴ�.
        /// </returns>
        property bool CanBeSorted
        {
            bool get();
        }

        ///// <summary>
        ///// ���� ������ �ҽ��� ���ε� �� �� �ִ����� ���� ���θ� �������ų� �����մϴ�.
        ///// </summary>
        ///// <returns>
        ///// ���� ������ �ҽ��� ���ε� �� �� �ִٸ� true��, �׷��� �ʴٸ� false�� ��ȯ�մϴ�.
        ///// </returns>
        //property bool IsBindable
        //{
        // bool get();
        //}

        /// <summary>
        /// ������ ������ ���� ���θ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ������ ������ �Ѱ� �̻��̶� ������ �Ǿ��ٸ� true��, �׷��� �ʴٸ� false�� ��ȯ�մϴ�.
        /// </returns>
        property bool IsSelected
        {
            bool get();
        }

        /// <summary>
        /// ������ ������ ��� ���õǾ� �ִ����� ���� ���θ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ������ ������ ��� ���õǾ��ٸ� true��, �׷��� �ʴٸ� false�� ��ȯ�մϴ�.
        /// </returns>
        property bool IsFullSelected
        {
            bool get(); 
        }

        /// <summary>
        /// ���� �׷��� ����� ����Ǿ������� ���� ���θ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� �׷��� ����� ����Ǿ��ٸ� true��, �׷��� �ʴٸ� false�� ��ȯ�մϴ�.
        /// </returns>
        property bool IsGrouped
        {
            bool get();
        }

        /// <summary>
        /// ���� �׷��� ����� ������ �� �ִ����� ���� ���θ� �������ų� �����մϴ�.
        /// </summary>
        /// <returns>
        /// ���� �׷��� ����� ������ �� �ִٸ� true��, �׷��� �ʴٸ� false�� ��ȯ�մϴ�.
        /// </returns>
        property bool CanBeGrouping
        {
            bool get();
        }

        /// <summary>
        /// ������ ������ ���� ���� ������ Ÿ���� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ������ Ÿ���� ��Ÿ���� <see cref="System::Type"/>�Դϴ�.
        /// </returns>
        property System::Type^ DataType
        {
            System::Type^ get();
        }

        /// <summary>
        /// ���� ���Ĺ���� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� ���Ĺ���� ��Ÿ���� <see cref="SortType"/>�Դϴ�.
        /// </returns>
        property Ntreev::Windows::Forms::Grid::SortType SortType
        {
            Ntreev::Windows::Forms::Grid::SortType get();
        }

        /// <summary>
        /// ���� ǥ�õǴ� ������ �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� �������� �ʰų� �׸��� ��Ʈ�ѿ� ���ӵǾ� ���� �ȴٸ� -1�� ��ȯ�մϴ�.
        /// </returns>
        property int VisibleIndex
        {
            int get(); 
        }

        /// <summary>
        /// ���� ȭ�鿡 ǥ�õǴ� ������ �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� ȭ�鿡 �������� �ʴٸ� -1�� ��ȯ�մϴ�.
        /// </returns>
        property int DisplayIndex
        {
            int get(); 
        }

        /// <summary>
        /// �׸��� ��Ʈ�ѿ� ���ӵ� ������ �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// �׸��� ��Ʈ�ѿ� ���ӵǾ��ٸ� ���ӵ� ����(0���� ����)�� ��ȯ�ϰ�, �׷��� �ʴٸ� -1�� ��ȯ�մϴ�.
        /// </returns>
        property int Index
        {
            int get();
        }

        /// <summary>
        /// ���� ���� ����� ���� �����͸� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���� ���� ����� ���� �����͸� ��Ÿ���� <see cref="System::Object"/>�Դϴ�.
        /// </returns>
        property System::Object^ Tag
        {
            System::Object^ get();
        }

        /// <summary>
        /// ���콺 Ŀ���� ���� ���� ���¸� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ���콺 Ŀ���� ���� ���� ���¸� ��Ÿ���� <see cref="ColumnState"/>�Դϴ�.
        /// </returns>
        property Ntreev::Windows::Forms::Grid::ColumnState ColumnState
        {
            Ntreev::Windows::Forms::Grid::ColumnState get();
        }
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/