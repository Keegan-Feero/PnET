# PnET --
### Table of Contents
* [What is PnET](#what-is-pnet)
* [Getting setup to run PnET on Windows10](#getting-setup-to-run-pnet-on-windows10)
  + [Download PnET](#download-pnet)
  + [Install Visual Studio Code editor](#install-visual-studio-code-editor)
  + [Install MinGW compiler](#install-mingw-compiler)
* [Compiling and executing](#compiling-and-executing) 
  + [Navigate the terminal](#navigate-the-terminal)
***
## What is PnET

PnET model description goes here:

***
## Getting setup to run PnET on Windows10
### Download PnET 
PnET can be downloaded from the [PnET Website](http://www.pnet.sr.unh.edu/). There are multiple versions of PnET available for download. There are also different source codes available. PnET can be downloaded in C++, MATLAB, or Visual Basic. However, C++ is the most supported source code and that's what we'll proceed with here:
1. Click on the `Download` tab at the top of the homescreen
2. Click on the model version that you're interested in. For now, `PnET-Daily (C++ Version)` is a good choice to download. This will download a .zip containing the files we'll need to get the model running. 
3. Extract the .zip contents to the directory of your choice. 

In exploring the contents of PnET you'll notice there are multiple folders...
***
### Install Visual Studio Code editor
If we want to modify the PnET C++ code we will want some sort of code editor or integrated development environment (IDE). A good choice is **Visual Studio Code**.
1. Download the [Windows](https://code.visualstudio.com/download) installer, and install with the generic settings.
2. Once VScode is up and running, search for and download the `C/C++ extension` from the Extension View (Ctrl+Shift+X). 
***
### Install MinGW compiler
We will also need a compiler to convert *human-readable* source code into *computer-executable* machine code. For Windows, a common choice is a version of MinGW called **Mingw-w64**. 

*Note that here we are following along with a [VScode setup tutoral](https://code.visualstudio.com/docs/cpp/config-mingw).*
1. Go to [SOURCEFORGE](https://sourceforge.net/projects/mingw-w64/files/). Scroll down to **MinGW-W64 Online Installer** and download `MinGW-W64-install.exe`
2. Run the installation file. Defauly settings are fine, but on the **Settings** page, under the **Architecture** drop down, we can select either *i686* for 32-bit, or *x86-64* for 64-bit. If you have a 64-bit system, you can select the latter, but it'll run the 32-bit version fine. Also, pay attention to where the files are installed -- likely program files.
3. Add MinGW `bin` to the PATH environment
   + Type `settings` into the Windows search bar
   + Search for `Edit environmental variables for your account`
   + Double-click `Path`
   + Click `New`, then `Browse...`, and navigate to where you downloaded MinGW. The path will depend on where you've put the files, but it will look something like: `C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin`
+ Add this path, and click `OK`. 
4. Check MinGW installation
   + Type `Windows Powershell` into the Windows search bar. 
   + In the powershell, type:
    `g++ --version`
     and
    `gdb --version`.
    + For each of these, a disclaimer about the software should pop up. If this fails, doublecheck the environment path settings described in step 2. 
***
## Compiling and executing
*Not sure where this should live, but it's important.* In brief, these instruction describe how to compile multiple C++ code files (.cpp) in an executable, and then execute. We'll need to do this once we modify the PnET code.  

To successfully navigate the Windows Powershell
1. In VScode, click on the `Terminal` tab and then click `New terminal`. This will launch a new **Windows Powershell** within VScode. Alternatively, this could be done in Powershell outside VScode or in [Git Bash](https://git-scm.com/downloads). 
2. Within your terminal of choice (Powershell or Bash), navigate to the **~/PNET_C1/pnet_linux** directory, which contains all the **.cpp** files we need to compile to run PnET. See [Navigate the terminal](#navigate-the-terminal) for need-to-know terminal commands.
3. In the terminal, type `g++ -o executable *.cpp` and press enter. <br/>
*executable* can be changed. It's the name assigned to the compiled **.exe.**. Currently, as downloaded there is an executable called **pnet.exe** in **./Pnet_C1/pnet_linux**. *If we change any Pnet Code we need to re-compile before running the model*. <br/>
The \*.cpp indicates that the compiler should select *all* .cpp files in the directory. We could compile a single .cpp file just calling `g++ somefile.cpp`, but PnET is comprised of many .cpp files linked together, so they must be compiled together. 
4. To execute the compiled script, type `./executable` and press enter. You'll see the printed output of PnET in the terminal, and if you go to the **./PNET_C1/Result/Site** you'll see the generated **Output_monthly.csv**.
##### Navigate the terminal
To navigate Windows Powershell or Git Bash we will need to know some basic commands:<br/>
`pwd` **print working directy** <br/>
`cd` **change directory** e.g.) `cd C:/Users/` will bring you from **C:/** to **C:/Users/** <br/>
`cd ..` **change directory to parent directory.** e.g.) from **C:/Users** to **C:/** <br/>
`ls` **list files** in the directory <br/>
`mkdir` **make new directory**
***
