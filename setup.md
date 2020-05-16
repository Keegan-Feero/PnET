# Getting setup to run PnET on Windows10
[**note:** we need someone to translate this to Mac and Linux OS] <br/>
for Jack: https://www.youtube.com/watch?v=I0oNm6u4zUM
### Table of Contents
+ [Download PnET](#download-pnet)
+ [Install MinGW compiler](#install-mingw-compiler)
+ [Install and configure Visual Studio Code](#install-and-configure-visual-studio-code)
+ [Compiling and executing](#compiling-and-executing) 
  + [From VSCode](#from-vscode)
  + [From the shell](#from-the-shell)
+ [Shell Navigation Crashcourse](#shell-navigation-crashcourse)
### Download PnET 
These directions are for the C++ Version of PnET-Daily. For more model choices, and more information go to the [PnET Website](http://www.pnet.sr.unh.edu/). 

1. Download the PnET repository by clicking the green `Clone or download` button on the PnET repo page. Similarly, you can clone this repo to your own GitHub page. 
2.  The downloaded zip will be called **PnET-master.zip**. Extract the contents in your location of choice on your computer.
### Install MinGW compiler
We will also need a compiler to convert *human-readable* source code into *computer-executable* machine code. For Windows, a common choice is a version of MinGW called **Mingw-w64**. 

*Note that here we are following along with a [VScode setup tutoral](https://code.visualstudio.com/docs/cpp/config-mingw).*
1. Go to [SOURCEFORGE](https://sourceforge.net/projects/mingw-w64/files/). Scroll down to **MinGW-W64 Online Installer** and download `MinGW-W64-install.exe`
2. Run the installation file. We can proceed with default settings. However, on the **Settings** page, under the **Architecture** drop down, we can select either *i686* or *x86-64*. If you have a 64-bit processor, you can select *x86-64*. If you miss this, it shouldn't matter, though.  Also, pay attention to where the files are installed -- likely program files.
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
### Install and configure Visual Studio Code
VScode is a light-weight and modular code editor that can be configured nicely for PnET model development. [This section should probably be broken up somehow]
1. Download the [Windows](https://code.visualstudio.com/download) installer, and install with the generic settings.
2. From the shell, navigate *into* the **Pnet-master** directory. Launch VScode by using the command `code .` 
3. Once VScode is running, search for and download the `C/C++ extension` from the Extension View (Ctrl+Shift+X). 
4. On the left side of VScode, use the Explorer drop down to navigate to `pnet_linux` and open `pnet_main.cpp`
5. Open the `Command Palette` (Ctrl+Shift+P), and in the search bar, type *C/C++: Edit Configurations (UI)*
6. Scroll down to `Compiler path`, and where it says `specify a compiler path...` paste in the path to the g++.exe compiler. This is the same path we included for our environment (see above) plus `g++.exe`. Altogether, this will look something like `C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe`
7. Scoll down to `IntelliSense mode` and set it to `gcc-x64`. All other settings should be fine. Now, on the left hand *Explorer*, there should be a folder called .vscode, and within it a file c_cpp_properties.json. This json file holds the configurations we just set, and we could edit this to make changes, too. 
8. Next, click the `Terminal` tab at the top of VScode. Click on `Configure Default Build Task...`
6. A drop-down should appear. Click `shell: g++.exe build active file`. This will open a *tasks.json* file
7. Confirm that `command` and `cwd` are pointing towards the compiler, paths we have previously set. They should look something like: <br/>
`"command": "C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\bin\\g++.exe"` <br/>
`"cwd": "C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\bin"`


### Compiling and executing
Any time the PnET code or input files are change, we have to compile a new executable program. The section below will step through compiling from VScode or from the terminal.
#### From VSCode
#### From the shell 


1. In VScode, click on the `Terminal` tab and then click `New terminal`. This will launch a new **Windows Powershell** within VScode. Alternatively, this could be done in Powershell outside VScode or in [Git Bash](https://git-scm.com/downloads). 
2. Within your terminal of choice (Powershell or Bash), navigate to the **~/PNET_C1/pnet_linux** directory, which contains all the **.cpp** files we need to compile to run PnET. See [Navigate the terminal](#navigate-the-terminal) for need-to-know terminal commands.
3. In the terminal, type `g++ -o executable *.cpp` and press enter. <br/>
*executable* can be changed. It's the name assigned to the compiled **.exe.**. Currently, as downloaded there is an executable called **pnet.exe** in **./Pnet_C1/pnet_linux**. *If we change any Pnet Code we need to re-compile before running the model*. <br/>
The \*.cpp indicates that the compiler should select *all* .cpp files in the directory. We could compile a single .cpp file just calling `g++ somefile.cpp`, but PnET is comprised of many .cpp files linked together, so they must be compiled together. 
4. To execute the compiled script, type `./executable` and press enter. You'll see the printed output of PnET in the terminal, and if you go to the **./PNET_C1/Result/Site** you'll see the generated **Output_monthly.csv**.

#### Shell Navigation Crashcourse
The shell allows us to access operating system services via command line tools. In Windows, we can use **PowerShell**. Another good option is [Git Bash](https://git-scm.com/downloads). Powershell can also be access from within VScode by clicking on the `Terminal` tab and then click `New terminal`. Here are some commands that are good to know: <br/>
`pwd` **print working directy** <br/>
`cd` **change directory** e.g.) `cd C:/Users/` will bring you from **C:/** to **C:/Users/** <br/>
`cd ..` **change directory to parent directory.** e.g.) from **C:/Users** to **C:/** <br/>
`ls` **list files** in the directory <br/>
`mkdir` **make new directory**
