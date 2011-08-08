
typedef System::Object					object;
typedef System::String					string;
typedef System::Type					_Type;

typedef System::EventHandler			_EventHandler;
typedef System::EventArgs				_EventArgs;

typedef System::Drawing::Rectangle		_Rectangle;
typedef System::Drawing::Graphics		_Graphics;
typedef System::Drawing::Color			_Color;
typedef System::Drawing::Font			_Font;
typedef System::Drawing::Point			_Point;
typedef System::Drawing::Size			_Size;
typedef System::Drawing::Bitmap			_Bitmap;

typedef System::Windows::Forms::Padding	_Padding;
typedef System::Windows::Forms::Control	_Control;
typedef System::Windows::Forms::CurrencyManager _CurrencyManager;
typedef System::ComponentModel::PropertyDescriptor _PropertyDescriptor;

typedef System::ComponentModel::CategoryAttribute		_CategoryAttribute;
typedef System::ComponentModel::DefaultValueAttribute	_DefaultValueAttribute;
typedef System::ComponentModel::BrowsableAttribute		_BrowsableAttribute;
typedef System::ComponentModel::DescriptionAttribute	_DescriptionAttribute;
typedef System::ComponentModel::EditorAttribute			_EditorAttribute;

#include "GridType.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	ref class GridControl;
	ref class CaptionRow;
	ref class GroupingRow;
	ref class Column;
	ref class Cell;
	ref class RowBase;
	ref class Row;
	ref class InsertionRow;
	ref class EditingReason;
	
	interface class IToolTip;

	value struct HitTest;

	ref class ColumnCollection;
	ref class VisibleColumnCollection;
	ref class DisplayableColumnCollection;
	ref class FrozenColumnCollection;
	ref class UnfrozenColumnCollection;
	ref class RowCollection;
	ref class VisibleRowCollection;
	ref class DisplayableRowCollection;
	ref class SelectedRowCollection;
	ref class SelectedColumnCollection;
	ref class CellCollection;
	ref class CellIterator;

	//ref class ColumnEventArgs;
	//ref class CellEventArgs;

	ref class GridTooltip;

	namespace Private
	{
		ref class ColumnDropDownForm;
		ref class ColumnCheckedListBox;
		ref class ErrorDescriptor;
		ref class SelectionTimer;
	}

	namespace GridState
	{
		ref class StateManager;
	}

	namespace Win32
	{
		ref class BitBlt;
	}

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/

typedef Ntreev::Windows::Forms::Grid::GridControl	_GridControl;
typedef Ntreev::Windows::Forms::Grid::Column		_Column;
typedef Ntreev::Windows::Forms::Grid::RowBase		_RowBase;
typedef Ntreev::Windows::Forms::Grid::Row			_Row;
typedef Ntreev::Windows::Forms::Grid::Cell			_Cell;
typedef Ntreev::Windows::Forms::Grid::CaptionRow	_CaptionRow;
typedef Ntreev::Windows::Forms::Grid::GroupingRow	_GroupingRow;
typedef Ntreev::Windows::Forms::Grid::InsertionRow	_InsertionRow;
typedef Ntreev::Windows::Forms::Grid::CellIterator	_CellIterator;