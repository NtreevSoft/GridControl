using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Reflection;

namespace Setup
{
    static class Program
    {
        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Guid id = new Guid("{51f2fb0b-b25a-4833-a9b4-a299d2e1f6a8}");

            ProductInfo productInfo = ProductInfo.Find(id);

            string filePath = Environment.GetEnvironmentVariable("Temp") + @"\" + Guid.NewGuid().ToString() + ".msi";

            FileStream fileStream = new FileStream(filePath, FileMode.Create);
            if (fileStream == null)
                return;

            BinaryWriter writer = new BinaryWriter(fileStream);
            writer.Write(Properties.Resources.SetupGridControl);
            writer.Close();

            try
            {
                string version = Assembly.GetExecutingAssembly().GetName().Version.ToString();
                Process process = null;
                if (productInfo != null && productInfo.Version != version)
                {
                    process = Process.Start("msiexec.exe", string.Format("/i {0} REINSTALL=ALL REINSTALLMODE=vmous", filePath));
                }
                else
                {
                    process = Process.Start("msiexec.exe", string.Format("/i {0}", filePath));
                }

                if (process != null)
                {
                    process.WaitForExit();
                }
            }
            finally
            {
                File.Delete(filePath);
            }
        }
    }
}


