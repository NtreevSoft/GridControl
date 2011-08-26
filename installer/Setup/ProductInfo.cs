using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Setup
{
    class ProductInfo
    {
        [DllImport("msi.dll", CharSet = CharSet.Unicode)]
        static extern Int32 MsiGetProductInfo(string product, string property,
            [Out] StringBuilder valueBuf, ref Int32 len);

        [DllImport("msi.dll", SetLastError = true)]
        static extern int MsiEnumProducts(int iProductIndex,
            StringBuilder lpProductBuf);

        string productName;
        Guid id;

        public static ProductInfo Find(Guid guid)
        {
            try
            {
                StringBuilder sbProductCode = new StringBuilder(39);
                int iIdx = 0;
                while (0 == MsiEnumProducts(iIdx++, sbProductCode))
                {
                    Int32 productNameLen = 512;
                    StringBuilder sbProductName = new StringBuilder(productNameLen);
                    MsiGetProductInfo(sbProductCode.ToString(), "ProductName", sbProductName, ref productNameLen);

                    Guid productID = new Guid(sbProductCode.ToString());

                    if (productID == guid)
                    {
                        ProductInfo product = new ProductInfo();
                        product.productName = sbProductName.ToString();
                        product.id = guid;
                        return product;
                    }
                }
            }
            catch (Exception)
            {
                return null;
            }

            return null;
        }

        public string Name
        {
            get { return this.productName; }
        }

        public Guid ID
        {
            get { return this.id; }
        }

        public string Version
        {
            get
            {
                Int32 productNameLen = 512;
                StringBuilder versionString = new StringBuilder(productNameLen);
                
                MsiGetProductInfo("{" + ID.ToString() + "}", "VersionString", versionString, ref productNameLen);

                return versionString.ToString();
            }
        }
    }
}
