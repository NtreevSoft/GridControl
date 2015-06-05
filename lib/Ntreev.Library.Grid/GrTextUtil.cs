using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ntreev.Library.Grid
{
    static class GrTextUtil
    {
        private static List<GrWordDesc> m_sList = new List<GrWordDesc>();

        public static void SingleLine(ref GrLineDesc pLine, string cellText, GrFont pFont)
        {
            //memset(pLine, 0, sizeof(GrLineDesc));
            pLine.length = (int)cellText.Length;
            pLine.width = pFont.GetStringWidth(cellText);
        }

        public static void MultiLine(List<GrLineDesc> pLines, string cellText, int cellWidth, GrFont pFont, bool wordWrap)
        {
            if (wordWrap == true)
                DoMultilineWordWrap(pLines, cellText, cellWidth, pFont);
            else
                DoMultiline(pLines, cellText, pFont);
        }

        private static void DoMultiline(List<GrLineDesc> pLines, string cellText, GrFont pFont)
        {
            int pos = 0;
            GrLineDesc cl = new GrLineDesc();
            cl.width = 0;
            cl.length = 0;
            cl.textBegin = 0;

            while (pos != cellText.Length)
            {
                char s = cellText[pos++];
                int charWidth = pFont.GetCharacterWidth(s);
                if (s == '\n')
                {
                    pLines.Add(cl);
                    cl.textBegin = pos;
                    cl.length = 0;
                    cl.width = 0;
                }
                else
                {
                    cl.width += charWidth;
                    cl.length++;
                }
            }

            pLines.Add(cl);
        }

        private static void WordWrap(List<GrWordDesc> pList, string cellText, GrFont pFont, int cellWidth)
        {
            bool wordBreak = false;

            
            int pos = 0;
            //memset(&wd, 0, sizeof(GrWordDesc));

            GrWordDesc wd = new GrWordDesc();
            while (pos != cellText.Length)
            {
                
                char s = cellText[pos];
                int width = pFont.GetCharacterWidth(s);

                if ((wordBreak == true && s != ' ') || wd.width + width > cellWidth || s > 0xff || s == '\n')
                {
                    pList.Add(wd);
                    //memset(&wd, 0, sizeof(GrWordDesc));
                    wd.pos = pos;
                    wordBreak = false;
                }

                wd.length++;
                wd.width += width;

                if (s == ' ')
                    wordBreak = true;
                else
                    wd.validWidth += width;

                pos++;
            }

            if (wd.length != 0)
            {
                pList.Add(wd);
            }
        }

        private static void DoMultilineWordWrap(List<GrLineDesc> pLines, string cellText, int cellWidth, GrFont pFont)
        {
            List<GrWordDesc> words = new List<GrWordDesc>();
            WordWrap(words, cellText, pFont, cellWidth);

            int pos = 0;

            GrLineDesc cl = new GrLineDesc();
            //memset(&cl, 0, sizeof(GrLineDesc));
            foreach (var value in words)
            {
                
                if (cl.width + value.validWidth > cellWidth || cellText[value.pos] == '\n')
                {
                    pLines.Add(cl);
                    //memset(&cl, 0, sizeof(GrLineDesc));
                    cl.textBegin = pos;
                }

                cl.width += value.width;
                cl.length += value.length;
                pos += value.length;
            }

            pLines.Add(cl);
        }



    }
}
