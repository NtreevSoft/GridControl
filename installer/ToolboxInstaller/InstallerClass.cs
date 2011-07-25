﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration.Install;
using System.Linq;
using Microsoft.Win32;
using System.Diagnostics;


namespace NtreevSoft.ToolboxInstaller
{
    [RunInstaller(true)]
    public partial class InstallerClass : Installer
    {
        public InstallerClass()
        {
            InitializeComponent();
        }
        
        protected override void OnBeforeInstall(IDictionary savedState)
        {
            base.OnBeforeInstall(savedState);
        }


        public override void Install(IDictionary stateSaver)
        {
            base.Install(stateSaver);
            using (RegistryKey setupKey = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\VisualStudio\9.0\Setup\VS"))
            {
                if (setupKey != null)
                {
                    string devenv = setupKey.GetValue("EnvironmentPath").ToString();
                    if (!string.IsNullOrEmpty(devenv))
                    {
                        Process.Start(devenv, "/setup").WaitForExit();
                    }
                }
            }
        }
    }
}
