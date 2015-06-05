using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace Ntreev.Windows.Forms.Grid
{
    public interface IColumn : ICellBase
    {
        string Title
        {
            get;
        }

        string ColumnName
        {
            get;
        }

        string Tooltip
        {
            get;
        }

        int ColumnID
        {
            get;
        }

        int MinWidth
        {
            get;
        }

        int MaxWidth
        {
            get;
        }

        bool IsVisible
        {
            get;
        }

        bool IsMovable
        {
            get;
        }

        bool IsResizable
        {
            get;
        }

        bool IsFrozen
        {
            get;
        }

        bool IsReadOnly
        {
            get;
        }

        bool IsSortable
        {
            get;
        }

        bool IsSelected
        {
            get;
        }

        bool IsFullSelected
        {
            get;
        }

        bool IsGrouped
        {
            get;
        }

        bool IsGroupable
        {
            get;
        }

        bool IsBinded
        {
            get;
        }

        Type SourceType
        {
            get;
        }

        Type DataType
        {
            get;
        }

        TypeConverter TypeConverter
        {
            get;
        }

        SortType SortType
        {
            get;
        }

        int VisibleIndex
        {
            get;
        }

        int DisplayIndex
        {
            get;
        }

        int Index
        {
            get;
        }

        object Tag
        {
            get;
        }

        ColumnState ColumnState
        {
            get;
        }
    };
}
