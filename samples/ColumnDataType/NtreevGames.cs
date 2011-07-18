using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace ColumnAdvancedExtension
{
    [Flags]
    enum NtreevGames : int
    {
        Panya = 0x00000001,
        Trickster = 0x00000002,
        BaseballManager = 0x00000004,
        Arche = 0x00000010,
        Alicia = 0x00000008,
    }
}
