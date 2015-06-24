using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Ntreev.Library.Grid
{
    public struct GrColor
    {
        private byte b, g, r, a;

        private string name;
        private bool isNamed;
        private static Dictionary<string, GrColor> namedColor = new Dictionary<string, GrColor>();

        public GrColor(int argb)
            : this(((argb >> 24) & 0xFF), ((argb >> 16) & 0xFF), ((argb >> 8) & 0xFF), ((argb) & 0xFF))
        {

        }

        public GrColor(int a, int r, int g, int b)
        {
            this.a = (byte)Math.Min(a, byte.MaxValue);
            this.r = (byte)Math.Min(r, byte.MaxValue);
            this.g = (byte)Math.Min(g, byte.MaxValue);
            this.b = (byte)Math.Min(b, byte.MaxValue);
            this.isNamed = false;
            this.name = string.Format("{0:2x}{1:2x}{2:2x}{3:2x}", this.a, this.r, this.g, this.b);
        }

        public GrColor(int r, int g, int b)
            : this(255, r, g, b)
        {

        }

        public GrColor(float a, float r, float g, float b)
            : this((int)(a * 255.0f), (int)(r * 255.0f), (int)(g * 255.0f), (int)(b * 255.0f))
        {

        }

        public GrColor(float r, float g, float b)
            : this(1.0f, r, g, b)
        {

        }

        private GrColor(int a, int r, int g, int b, string name)
        {
            this.a = (byte)Math.Min(a, byte.MaxValue);
            this.r = (byte)Math.Min(r, byte.MaxValue);
            this.g = (byte)Math.Min(g, byte.MaxValue);
            this.b = (byte)Math.Min(b, byte.MaxValue);
            this.name = name;
            this.isNamed = true;
        }

        public override bool Equals(object obj)
        {
            if (obj is GrColor == false)
                return false;
            return this == (GrColor)obj;
        }

        public override int GetHashCode()
        {
            return GrGridUtility.GetHashCode(this.b) ^
                GrGridUtility.GetHashCode(this.g) ^
                GrGridUtility.GetHashCode(this.b) ^
                GrGridUtility.GetHashCode(this.a) ^
                GrGridUtility.GetHashCode(this.name) ^
                GrGridUtility.GetHashCode(this.isNamed);
        }

        public static GrColor operator *(GrColor color, float f)
        {
            return new GrColor(color.Af * f, color.Rf * f, color.Gf * f, color.Bf * f);
        }

        public static bool operator !=(GrColor color1, GrColor color2)
        {
            return !(color1 == color2);
        }

        public static bool operator ==(GrColor color1, GrColor color2)
        {
            return color1.a == color2.a &&
                color1.r == color2.r &&
                color1.g == color2.g &&
                color1.b == color2.b;
        }

        public int ToARGB()  
        {
            byte[] bytes = new byte[] { b, g, r, a, };
            return BitConverter.ToInt32(bytes, 0); 
        }

        public static GrColor SetBrightness(GrColor c, float brightness)
        {
            HSL hsl = RGB_to_HSL(c);
            hsl.L(brightness);
            return HSL_to_RGB(hsl);
        }

        public static GrColor ModifyBrightness(GrColor c, float brightness)
        {
            HSL hsl = RGB_to_HSL(c);
            hsl.L(hsl.L() * brightness);
            return HSL_to_RGB(hsl);
        }

        public static GrColor SetSaturation(GrColor c, float Saturation)
        {
            HSL hsl = RGB_to_HSL(c);
            hsl.S(Saturation);
            return HSL_to_RGB(hsl);
        }

        public static GrColor ModifySaturation(GrColor c, float Saturation)
        {
            HSL hsl = RGB_to_HSL(c);
            hsl.S(hsl.S() * Saturation);
            return HSL_to_RGB(hsl);
        }

        public static GrColor SetHue(GrColor c, float Hue)
        {
            HSL hsl = RGB_to_HSL(c);
            hsl.H(Hue);
            return HSL_to_RGB(hsl);
        }

        public static GrColor ModifyHue(GrColor c, float Hue)
        {
            HSL hsl = RGB_to_HSL(c);
            hsl.H(hsl.H() * Hue);
            return HSL_to_RGB(hsl);
        }

        private static GrColor HSL_to_RGB(HSL hsl)
        {
            float r = 0, g = 0, b = 0;
            float temp1, temp2;

            if (hsl.L() == 0)
            {
                r = g = b = 0;
            }
            else
            {
                if (hsl.S() == 0)
                {
                    r = g = b = hsl.L();
                }
                else
                {
                    temp2 = ((hsl.L() <= 0.5f) ? hsl.L() * (1.0f + hsl.S()) : hsl.L() + hsl.S() - (hsl.L() * hsl.S()));
                    temp1 = 2.0f * hsl.L() - temp2;

                    float[] t3 = new float[] { hsl.H() + 1.0f / 3.0f, hsl.H(), hsl.H() - 1.0f / 3.0f };
                    float[] clr = new float[] { 0.0f, 0.0f, 0.0f };
                    for (int i = 0; i < 3; i++)
                    {
                        if (t3[i] < 0)
                            t3[i] += 1.0f;
                        if (t3[i] > 1)
                            t3[i] -= 1.0f;

                        if (6.0f * t3[i] < 1.0f)
                            clr[i] = temp1 + (temp2 - temp1) * t3[i] * 6.0f;
                        else if (2.0f * t3[i] < 1.0f)
                            clr[i] = temp2;
                        else if (3.0f * t3[i] < 2.0f)
                            clr[i] = (temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - t3[i]) * 6.0f);
                        else
                            clr[i] = temp1;
                    }
                    r = clr[0];
                    g = clr[1];
                    b = clr[2];

                }
            }

            return new GrColor((int)(255 * r), (int)(255 * g), (int)(255 * b));
        }

        private static HSL RGB_to_HSL(GrColor c)
        {
            HSL hsl = new HSL();

            hsl.H(c.GetHue() / 360.0f); // we store hue as 0-1 as opposed to 0-360
            hsl.L(c.GetBrightness());
            hsl.S(c.GetSaturation());

            return hsl;
        }

        public float GetBrightness()
        {
            throw new NotImplementedException();
        }

        public float GetHue()
        {
            throw new NotImplementedException();
        }

        public float GetSaturation()
        {
            throw new NotImplementedException();
        }

        public int Value
        {
            get
            {
                byte[] values = new byte[] { this.b, this.g, this.r, this.a, };
                return BitConverter.ToInt32(values, 0);
            }
        }

        public int A
        {
            get { return this.a; }
            set { this.a = (byte)Math.Min(value, byte.MaxValue); }
        }

        public int R
        {
            get { return this.r; }
            set { this.r = (byte)Math.Min(value, byte.MaxValue); }
        }

        public int G
        {
            get { return this.g; }
            set { this.g = (byte)Math.Min(value, byte.MaxValue); }
        }

        public int B
        {
            get { return this.b; }
            set { this.b = (byte)Math.Min(value, byte.MaxValue); }
        }

        public string Name
        {
            get { return this.name; }
        }

        public float Af
        {
            get { return this.a / 255.0f; }
        }

        public float Rf
        {
            get { return this.r / 255.0f; }
        }

        public float Gf
        {
            get { return this.g / 255.0f; }
        }

        public float Bf
        {
            get { return this.b / 255.0f; }
        }


        public static readonly GrColor Empty;
        public static readonly GrColor AliceBlue = new GrColor(255, 240, 248, 255, "AliceBlue");
        public static readonly GrColor AntiqueWhite = new GrColor(255, 250, 235, 215, "AntiqueWhite");
        public static readonly GrColor Aqua = new GrColor(255, 0, 255, 255, "Aqua");
        public static readonly GrColor Aquamarine = new GrColor(255, 127, 255, 212, "Aquamarine");
        public static readonly GrColor Azure = new GrColor(255, 240, 255, 255, "Azure");
        public static readonly GrColor Beige = new GrColor(255, 245, 245, 220, "Beige");
        public static readonly GrColor Bisque = new GrColor(255, 255, 228, 196, "Bisque");
        public static readonly GrColor Black = new GrColor(255, 0, 0, 0, "Black");
        public static readonly GrColor BlanchedAlmond = new GrColor(255, 255, 235, 205, "BlanchedAlmond");
        public static readonly GrColor Blue = new GrColor(255, 0, 0, 255, "Blue");
        public static readonly GrColor BlueViolet = new GrColor(255, 138, 43, 226, "BlueViolet");
        public static readonly GrColor Brown = new GrColor(255, 165, 42, 42, "Brown");
        public static readonly GrColor BurlyWood = new GrColor(255, 222, 184, 135, "BurlyWood");
        public static readonly GrColor CadetBlue = new GrColor(255, 95, 158, 160, "CadetBlue");
        public static readonly GrColor Chartreuse = new GrColor(255, 127, 255, 0, "Chartreuse");
        public static readonly GrColor Chocolate = new GrColor(255, 210, 105, 30, "Chocolate");
        public static readonly GrColor Coral = new GrColor(255, 255, 127, 80, "Coral");
        public static readonly GrColor CornflowerBlue = new GrColor(255, 100, 149, 237, "CornflowerBlue");
        public static readonly GrColor Cornsilk = new GrColor(255, 255, 248, 220, "Cornsilk");
        public static readonly GrColor Crimson = new GrColor(255, 220, 20, 60, "Crimson");
        public static readonly GrColor Cyan = new GrColor(255, 0, 255, 255, "Cyan");
        public static readonly GrColor DarkBlue = new GrColor(255, 0, 0, 139, "DarkBlue");
        public static readonly GrColor DarkCyan = new GrColor(255, 0, 139, 139, "DarkCyan");
        public static readonly GrColor DarkGoldenrod = new GrColor(255, 184, 134, 11, "DarkGoldenrod");
        public static readonly GrColor DarkGray = new GrColor(255, 169, 169, 169, "DarkGray");
        public static readonly GrColor DarkGreen = new GrColor(255, 0, 100, 0, "DarkGreen");
        public static readonly GrColor DarkKhaki = new GrColor(255, 189, 183, 107, "DarkKhaki");
        public static readonly GrColor DarkMagenta = new GrColor(255, 139, 0, 139, "DarkMagenta");
        public static readonly GrColor DarkOliveGreen = new GrColor(255, 85, 107, 47, "DarkOliveGreen");
        public static readonly GrColor DarkOrange = new GrColor(255, 255, 140, 0, "DarkOrange");
        public static readonly GrColor DarkOrchid = new GrColor(255, 153, 50, 204, "DarkOrchid");
        public static readonly GrColor DarkRed = new GrColor(255, 139, 0, 0, "DarkRed");
        public static readonly GrColor DarkSalmon = new GrColor(255, 233, 150, 122, "DarkSalmon");
        public static readonly GrColor DarkSeaGreen = new GrColor(255, 143, 188, 139, "DarkSeaGreen");
        public static readonly GrColor DarkSlateBlue = new GrColor(255, 72, 61, 139, "DarkSlateBlue");
        public static readonly GrColor DarkSlateGray = new GrColor(255, 47, 79, 79, "DarkSlateGray");
        public static readonly GrColor DarkTurquoise = new GrColor(255, 0, 206, 209, "DarkTurquoise");
        public static readonly GrColor DarkViolet = new GrColor(255, 148, 0, 211, "DarkViolet");
        public static readonly GrColor DeepPink = new GrColor(255, 255, 20, 147, "DeepPink");
        public static readonly GrColor DeepSkyBlue = new GrColor(255, 0, 191, 255, "DeepSkyBlue");
        public static readonly GrColor DimGray = new GrColor(255, 105, 105, 105, "DimGray");
        public static readonly GrColor DodgerBlue = new GrColor(255, 30, 144, 255, "DodgerBlue");
        public static readonly GrColor Firebrick = new GrColor(255, 178, 34, 34, "Firebrick");
        public static readonly GrColor FloralWhite = new GrColor(255, 255, 250, 240, "FloralWhite");
        public static readonly GrColor ForestGreen = new GrColor(255, 34, 139, 34, "ForestGreen");
        public static readonly GrColor Fuchsia = new GrColor(255, 255, 0, 255, "Fuchsia");
        public static readonly GrColor Gainsboro = new GrColor(255, 220, 220, 220, "Gainsboro");
        public static readonly GrColor GhostWhite = new GrColor(255, 248, 248, 255, "GhostWhite");
        public static readonly GrColor Gold = new GrColor(255, 255, 215, 0, "Gold");
        public static readonly GrColor Goldenrod = new GrColor(255, 218, 165, 32, "Goldenrod");
        public static readonly GrColor Gray = new GrColor(255, 128, 128, 128, "Gray");
        public static readonly GrColor Green = new GrColor(255, 0, 128, 0, "Green");
        public static readonly GrColor GreenYellow = new GrColor(255, 173, 255, 47, "GreenYellow");
        public static readonly GrColor Honeydew = new GrColor(255, 240, 255, 240, "Honeydew");
        public static readonly GrColor HotPink = new GrColor(255, 255, 105, 180, "HotPink");
        public static readonly GrColor IndianRed = new GrColor(255, 205, 92, 92, "IndianRed");
        public static readonly GrColor Indigo = new GrColor(255, 75, 0, 130, "Indigo");
        public static readonly GrColor Ivory = new GrColor(255, 255, 255, 240, "Ivory");
        public static readonly GrColor Khaki = new GrColor(255, 240, 230, 140, "Khaki");
        public static readonly GrColor Lavender = new GrColor(255, 230, 230, 250, "Lavender");
        public static readonly GrColor LavenderBlush = new GrColor(255, 255, 240, 245, "LavenderBlush");
        public static readonly GrColor LawnGreen = new GrColor(255, 124, 252, 0, "LawnGreen");
        public static readonly GrColor LemonChiffon = new GrColor(255, 255, 250, 205, "LemonChiffon");
        public static readonly GrColor LightBlue = new GrColor(255, 173, 216, 230, "LightBlue");
        public static readonly GrColor LightCoral = new GrColor(255, 240, 128, 128, "LightCoral");
        public static readonly GrColor LightCyan = new GrColor(255, 224, 255, 255, "LightCyan");
        public static readonly GrColor LightGoldenrodYellow = new GrColor(255, 250, 250, 210, "LightGoldenrodYellow");
        public static readonly GrColor LightGray = new GrColor(255, 211, 211, 211, "LightGray");
        public static readonly GrColor LightGreen = new GrColor(255, 144, 238, 144, "LightGreen");
        public static readonly GrColor LightPink = new GrColor(255, 255, 182, 193, "LightPink");
        public static readonly GrColor LightSalmon = new GrColor(255, 255, 160, 122, "LightSalmon");
        public static readonly GrColor LightSeaGreen = new GrColor(255, 32, 178, 170, "LightSeaGreen");
        public static readonly GrColor LightSkyBlue = new GrColor(255, 135, 206, 250, "LightSkyBlue");
        public static readonly GrColor LightSlateGray = new GrColor(255, 119, 136, 153, "LightSlateGray");
        public static readonly GrColor LightSteelBlue = new GrColor(255, 176, 196, 222, "LightSteelBlue");
        public static readonly GrColor LightYellow = new GrColor(255, 255, 255, 224, "LightYellow");
        public static readonly GrColor Lime = new GrColor(255, 0, 255, 0, "Lime");
        public static readonly GrColor LimeGreen = new GrColor(255, 50, 205, 50, "LimeGreen");
        public static readonly GrColor Linen = new GrColor(255, 250, 240, 230, "Linen");
        public static readonly GrColor Magenta = new GrColor(255, 255, 0, 255, "Magenta");
        public static readonly GrColor Maroon = new GrColor(255, 128, 0, 0, "Maroon");
        public static readonly GrColor MediumAquamarine = new GrColor(255, 102, 205, 170, "MediumAquamarine");
        public static readonly GrColor MediumBlue = new GrColor(255, 0, 0, 205, "MediumBlue");
        public static readonly GrColor MediumOrchid = new GrColor(255, 186, 85, 211, "MediumOrchid");
        public static readonly GrColor MediumPurple = new GrColor(255, 147, 112, 219, "MediumPurple");
        public static readonly GrColor MediumSeaGreen = new GrColor(255, 60, 179, 113, "MediumSeaGreen");
        public static readonly GrColor MediumSlateBlue = new GrColor(255, 123, 104, 238, "MediumSlateBlue");
        public static readonly GrColor MediumSpringGreen = new GrColor(255, 0, 250, 154, "MediumSpringGreen");
        public static readonly GrColor MediumTurquoise = new GrColor(255, 72, 209, 204, "MediumTurquoise");
        public static readonly GrColor MediumVioletRed = new GrColor(255, 199, 21, 133, "MediumVioletRed");
        public static readonly GrColor MidnightBlue = new GrColor(255, 25, 25, 112, "MidnightBlue");
        public static readonly GrColor MintCream = new GrColor(255, 245, 255, 250, "MintCream");
        public static readonly GrColor MistyRose = new GrColor(255, 255, 228, 225, "MistyRose");
        public static readonly GrColor Moccasin = new GrColor(255, 255, 228, 181, "Moccasin");
        public static readonly GrColor NavajoWhite = new GrColor(255, 255, 222, 173, "NavajoWhite");
        public static readonly GrColor Navy = new GrColor(255, 0, 0, 128, "Navy");
        public static readonly GrColor OldLace = new GrColor(255, 253, 245, 230, "OldLace");
        public static readonly GrColor Olive = new GrColor(255, 128, 128, 0, "Olive");
        public static readonly GrColor OliveDrab = new GrColor(255, 107, 142, 35, "OliveDrab");
        public static readonly GrColor Orange = new GrColor(255, 255, 165, 0, "Orange");
        public static readonly GrColor OrangeRed = new GrColor(255, 255, 69, 0, "OrangeRed");
        public static readonly GrColor Orchid = new GrColor(255, 218, 112, 214, "Orchid");
        public static readonly GrColor PaleGoldenrod = new GrColor(255, 238, 232, 170, "PaleGoldenrod");
        public static readonly GrColor PaleGreen = new GrColor(255, 152, 251, 152, "PaleGreen");
        public static readonly GrColor PaleTurquoise = new GrColor(255, 175, 238, 238, "PaleTurquoise");
        public static readonly GrColor PaleVioletRed = new GrColor(255, 219, 112, 147, "PaleVioletRed");
        public static readonly GrColor PapayaWhip = new GrColor(255, 255, 239, 213, "PapayaWhip");
        public static readonly GrColor PeachPuff = new GrColor(255, 255, 218, 185, "PeachPuff");
        public static readonly GrColor Peru = new GrColor(255, 205, 133, 63, "Peru");
        public static readonly GrColor Pink = new GrColor(255, 255, 192, 203, "Pink");
        public static readonly GrColor Plum = new GrColor(255, 221, 160, 221, "Plum");
        public static readonly GrColor PowderBlue = new GrColor(255, 176, 224, 230, "PowderBlue");
        public static readonly GrColor Purple = new GrColor(255, 128, 0, 128, "Purple");
        public static readonly GrColor Red = new GrColor(255, 255, 0, 0, "Red");
        public static readonly GrColor RosyBrown = new GrColor(255, 188, 143, 143, "RosyBrown");
        public static readonly GrColor RoyalBlue = new GrColor(255, 65, 105, 225, "RoyalBlue");
        public static readonly GrColor SaddleBrown = new GrColor(255, 139, 69, 19, "SaddleBrown");
        public static readonly GrColor Salmon = new GrColor(255, 250, 128, 114, "Salmon");
        public static readonly GrColor SandyBrown = new GrColor(255, 244, 164, 96, "SandyBrown");
        public static readonly GrColor SeaGreen = new GrColor(255, 46, 139, 87, "SeaGreen");
        public static readonly GrColor SeaShell = new GrColor(255, 255, 245, 238, "SeaShell");
        public static readonly GrColor Sienna = new GrColor(255, 160, 82, 45, "Sienna");
        public static readonly GrColor Silver = new GrColor(255, 192, 192, 192, "Silver");
        public static readonly GrColor SkyBlue = new GrColor(255, 135, 206, 235, "SkyBlue");
        public static readonly GrColor SlateBlue = new GrColor(255, 106, 90, 205, "SlateBlue");
        public static readonly GrColor SlateGray = new GrColor(255, 112, 128, 144, "SlateGray");
        public static readonly GrColor Snow = new GrColor(255, 255, 250, 250, "Snow");
        public static readonly GrColor SpringGreen = new GrColor(255, 0, 255, 127, "SpringGreen");
        public static readonly GrColor SteelBlue = new GrColor(255, 70, 130, 180, "SteelBlue");
        public static readonly GrColor Tan = new GrColor(255, 210, 180, 140, "Tan");
        public static readonly GrColor Teal = new GrColor(255, 0, 128, 128, "Teal");
        public static readonly GrColor Thistle = new GrColor(255, 216, 191, 216, "Thistle");
        public static readonly GrColor Tomato = new GrColor(255, 255, 99, 71, "Tomato");
        public static readonly GrColor Turquoise = new GrColor(255, 64, 224, 208, "Turquoise");
        public static readonly GrColor Violet = new GrColor(255, 238, 130, 238, "Violet");
        public static readonly GrColor Wheat = new GrColor(255, 245, 222, 179, "Wheat");
        public static readonly GrColor White = new GrColor(255, 255, 255, 255, "White");
        public static readonly GrColor WhiteSmoke = new GrColor(255, 245, 245, 245, "WhiteSmoke");
        public static readonly GrColor Yellow = new GrColor(255, 255, 255, 0, "Yellow");
        public static readonly GrColor YellowGreen = new GrColor(255, 154, 205, 50, "YellowGreen");

        public static readonly GrColor DefaultLineColor = new GrColor(255, 208, 215, 229);

        class HSL
        {
            private float _h;
            private float _s;
            private float _l;
            public HSL()
            {

            }

            public float H()
            {
                return _h;
            }

            public void H(float value)
            {
                _h = value;
                _h = _h > 1 ? 1 : _h < 0 ? 0 : _h;
            }
            public float S()
            {
                return _s;
            }
            public void S(float value)
            {
                _s = value;
                _s = _s > 1 ? 1 : _s < 0 ? 0 : _s;
            }
            public float L()
            {
                return _l;
            }
            public void L(float value)
            {
                _l = value;
                _l = _l > 1 ? 1 : _l < 0 ? 0 : _l;
            }
        }

#if _WINFORM
        public static implicit operator System.Drawing.Color(GrColor color)
        {
            return System.Drawing.Color.FromArgb(color.a, color.r, color.g, color.b);
        }

        public static implicit operator GrColor(System.Drawing.Color color)
        {
            return new GrColor(color.A, color.R, color.G, color.B);
        }
#endif
    }
}
