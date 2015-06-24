using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public class GrStyle
    {

        public GrStyle()
        {
            ForeColor = GrColor.Black;
            BackColor = GrColor.White;
            LineColor = GrColor.DefaultLineColor;
            Padding = GrPadding.Default;
            Font = GrFont.GetDefaultFont();

            SelectedForeColor = GrColor.White;
            SelectedBackColor = GrColor.CornflowerBlue;

            FocusedForeColor = GrColor.White;
            FocusedBackColor = GrColor.LightSkyBlue;

            ColumnForeColor = GrColor.Empty;
            ColumnBackColor = GrColor.Empty;
            ColumnLineColor = GrColor.Empty;
            ColumnFont = null;

            RowForeColor = GrColor.Empty;
            RowBackColor = GrColor.Empty;
            RowLineColor = GrColor.Empty;
            RowFont = null;

            CaptionForeColor = GrColor.Empty;
            CaptionBackColor = GrColor.Empty;
            CaptionLineColor = GrColor.Empty;
            CaptionFont = null;

            GroupPanelForeColor = GrColor.Empty;
            GroupPanelBackColor = GrColor.Empty;
            GroupPanelLineColor = GrColor.Empty;
            GroupPanelFont = null;

            RowHighlightLineColor = GrColor.Black;
            RowHighlightFillColor = GrColor.LightCyan;
        }

        private GrStyle(bool defaultStyle)
        {
            ForeColor = GrColor.Black;
            BackColor = GrColor.White;
            LineColor = GrColor.DefaultLineColor;
            Padding = GrPadding.Default;
            Font = GrFont.GetDefaultFont();

            SelectedForeColor = GrColor.White;
            SelectedBackColor = GrColor.CornflowerBlue;

            FocusedForeColor = GrColor.White;
            FocusedBackColor = GrColor.LightSkyBlue;

            ColumnForeColor = GrColor.Black;
            ColumnBackColor = GrColor.White;
            ColumnLineColor = GrColor.DefaultLineColor;
            ColumnFont = GrFont.GetDefaultFont();

            RowForeColor = GrColor.Black;
            RowBackColor = GrColor.White;
            RowLineColor = GrColor.DefaultLineColor;
            RowFont = GrFont.GetDefaultFont();

            CaptionForeColor = GrColor.Black;
            CaptionBackColor = GrColor.White;
            CaptionLineColor = GrColor.DefaultLineColor;
            CaptionFont = GrFont.GetDefaultFont();

            GroupPanelForeColor = GrColor.Black;
            GroupPanelBackColor = GrColor.White;
            GroupPanelLineColor = GrColor.DefaultLineColor;
            GroupPanelFont = GrFont.GetDefaultFont();

            RowHighlightLineColor = GrColor.Black;
            RowHighlightFillColor = GrColor.LightCyan;

            GroupForeColors = new List<GrColor>();
            GroupForeColors.Add(GrColor.Black);

            GroupBackColors = new List<GrColor>();
            GroupBackColors.Add(GrColor.LightSalmon);
            GroupBackColors.Add(GrColor.PaleGreen);
            GroupBackColors.Add(GrColor.SkyBlue);
            GroupBackColors.Add(GrColor.LemonChiffon);
            GroupBackColors.Add(GrColor.Plum);
            GroupBackColors.Add(GrColor.Bisque);
            GroupBackColors.Add(GrColor.Honeydew);

            GroupLineColors = new List<GrColor>();
            GroupLineColors.Add(GrColor.DefaultLineColor);

            GroupFonts = new List<GrFont>();
            GroupFonts.Add(GrFont.GetDefaultFont());
        }




        public GrColor ForeColor { get; set; }
        public GrColor BackColor { get; set; }
        public GrColor LineColor { get; set; }
        public GrPadding Padding { get; set; }
        public GrFont Font { get; set; }

        public GrColor SelectedForeColor { get; set; }
        public GrColor SelectedBackColor { get; set; }

        public GrColor FocusedForeColor { get; set; }
        public GrColor FocusedBackColor { get; set; }

        public GrColor ColumnForeColor { get; set; }
        public GrColor ColumnBackColor { get; set; }
        public GrColor ColumnLineColor { get; set; }
        public GrFont ColumnFont { get; set; }

        public GrColor RowForeColor { get; set; }
        public GrColor RowBackColor { get; set; }
        public GrColor RowLineColor { get; set; }
        public GrFont RowFont { get; set; }

        public GrColor CaptionForeColor { get; set; }
        public GrColor CaptionBackColor { get; set; }
        public GrColor CaptionLineColor { get; set; }
        public GrFont CaptionFont { get; set; }

        public GrColor GroupPanelForeColor { get; set; }
        public GrColor GroupPanelBackColor { get; set; }
        public GrColor GroupPanelLineColor { get; set; }
        public GrFont GroupPanelFont { get; set; }

        public GrColor RowHighlightLineColor { get; set; }
        public GrColor RowHighlightFillColor { get; set; }

        public List<GrColor> ItemBackColors { get; set; }
        public List<GrColor> ItemForeColors { get; set; }
        public List<GrColor> ItemLineColors { get; set; }
        public List<GrFont> ItemFonts { get; set; }

        public List<GrColor> GroupBackColors { get; set; }
        public List<GrColor> GroupForeColors { get; set; }
        public List<GrColor> GroupLineColors { get; set; }
        public List<GrFont> GroupFonts { get; set; }

        public GrColor GetColumnForeColor()
        {
            if (this.ColumnForeColor == GrColor.Empty)
                return this.ForeColor;
            return this.ColumnForeColor;
        }

        public GrColor GetColumnBackColor()
        {
            if (this.ColumnBackColor == GrColor.Empty)
                return this.BackColor;
            return this.ColumnBackColor;
        }

        public GrColor GetColumnLineColor()
        {
            if (this.ColumnLineColor == GrColor.Empty)
                return this.LineColor;
            return this.ColumnLineColor;
        }

        public GrFont GetColumnFont()
        {
            if (this.ColumnFont == null)
                return this.Font;
            return this.ColumnFont;
        }

        public GrColor GetRowForeColor()
        {
            if (this.RowForeColor == GrColor.Empty)
                return this.ForeColor;
            return this.RowForeColor;
        }

        public GrColor GetRowBackColor()
        {
            if (this.RowBackColor == GrColor.Empty)
                return this.BackColor;
            return this.RowBackColor;
        }

        public GrColor GetRowLineColor()
        {
            if (this.RowLineColor == GrColor.Empty)
                return this.LineColor;
            return this.RowLineColor;
        }

        public GrFont GetRowFont()
        {
            if (this.RowFont == null)
                return this.Font;
            return this.RowFont;
        }

        public GrColor GetCaptionForeColor()
        {
            if (this.CaptionForeColor == GrColor.Empty)
                return this.ForeColor;
            return this.CaptionForeColor;
        }

        public GrColor GetCaptionBackColor()
        {
            if (this.CaptionBackColor == GrColor.Empty)
                return this.BackColor;
            return this.CaptionBackColor;
        }

        public GrColor GetCaptionLineColor()
        {
            if (this.CaptionLineColor == GrColor.Empty)
                return this.LineColor;
            return this.CaptionLineColor;
        }

        public GrFont GetCaptionFont()
        {
            if (this.CaptionFont == null)
                return this.Font;
            return this.CaptionFont;
        }

        public GrColor GetGroupPanelForeColor()
        {
            if (this.GroupPanelForeColor == GrColor.Empty)
                return this.ForeColor;
            return this.GroupPanelForeColor;
        }

        public GrColor GetGroupPanelBackColor()
        {
            if (this.GroupPanelBackColor == GrColor.Empty)
                return this.BackColor;
            return this.GroupPanelBackColor;
        }

        public GrColor GetGroupPanelLineColor()
        {
            if (this.GroupPanelLineColor == GrColor.Empty)
                return this.LineColor;
            return this.GroupPanelLineColor;
        }

        public GrFont GetGroupPanelFont()
        {
            if (this.GroupPanelFont == null)
                return this.Font;
            return this.GroupPanelFont;
        }

        public GrColor GetItemForeColor(int index)
        {
            return this.ItemForeColors[index % ItemForeColors.Count];
        }

        public GrColor GetItemBackColor(int index)
        {
            return this.ItemBackColors[index % ItemBackColors.Count];
        }

        public GrColor GetItemLineColor(int index)
        {
            return this.ItemLineColors[index % ItemLineColors.Count];
        }

        public GrFont GetItemFont(int index)
        {
            return this.ItemFonts[index % ItemFonts.Count];
        }

        public GrColor GetGroupForeColor(int index)
        {
            List<GrColor> colors =
               this.GroupForeColors.Count == 0 ?
               Default.GroupForeColors : this.GroupForeColors;

            return colors[index % colors.Count];
        }

        public GrColor GetGroupBackColor(int index)
        {
            List<GrColor> colors =
               this.GroupBackColors.Count == 0 ?
               Default.GroupBackColors : this.GroupBackColors;

            return colors[index % colors.Count];
        }

        public GrColor GetGroupLineColor(int index)
        {
            List<GrColor> colors =
                this.GroupLineColors.Count == 0 ?
                Default.GroupLineColors : this.GroupLineColors;

            return colors[index % colors.Count];
        }

        public GrFont GetGroupFont(int index)
        {
            List<GrFont> fonts =
               this.GroupFonts.Count == 0 ?
               Default.GroupFonts : this.GroupFonts;

            return fonts[index % fonts.Count];
        }

        public static readonly GrStyle Default = new GrStyle(true);
    }

}
