# Getting setup to run PnET on Windows10
[**note:** we need someone to translate this to Mac and Linux OS] <br/>
for Jack: https://www.youtube.com/watch?v=I0oNm6u4zUM
### Table of Contents
+ [Download PnET](#download-pnet)
+ [Install MinGW compiler](#install-mingw-compiler)
+ [Install and configure Visual Studio Code](#install-and-configure-visual-studio-code)
+ [Compiling and executing](#compiling-and-executing) 
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
7. Scoll down to `IntelliSense mode` and set it to `gcc-x64`. All other settings should be fine. Now, on the left hand *Explorer*, there should be a folder called .vscode, and within it the *c_cpp_properties.json* file. This json file holds the configurations we just set, and we could edit this to make changes, too. 
8. Next, click the `Terminal` tab at the top of VScode. Click on `Configure Default Build Task...`
9. A drop-down should appear where you have the option to click on `shell: g++.exe build active file`. This will open a tasks.json file. If you do not have this option, go to step 13.
10. Confirm that `command` and `cwd` are pointing towards the compiler, paths we have previously set. They should look something like: <br/>
`"command": "C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\bin\\g++.exe"` <br/>
`"cwd": "C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\bin"`
11. Under `"Args:`, replace `${file}` with `${workspaceFolder}\\pnet_linux\\*.cpp` to tell vscode to build all .cpp files in the pnet_linux folder. [note to self: there must be a more generic way to include the pnet_linux in the workspace or paths.]
12. Also under `Args:`, modify the line that says `${fileDirname}\\${fileBasenameNoExtension}.exe` to say `${fileDirname}\\pnet_executable.exe` to compile the pnet_executable. You can name the executable whatever you want.
13. If under `Configure Default Build Task...` you are given the option to `Create a new tasks.json file from a template`, select this to open a tasks.json. In the pnet github repository there is a folder called `vscode_config_example`. Copy this tasks.json file to replace the one in vscode. Change command and cwd to point towards your compiler (see step 10)

### Compiling and executing
Any time the PnET code or input files are change, we have to compile a new executable program. The section below will step through compiling from VScode or from the the shell.
1. In VScode, click `Terminal`, and then click `Run Build Task...` Alternatively, just press `Ctrl+Shift+B`.
2. To run the model, there are several options. In the shell navigate into the `~pnet-master/pnet_linux` folder and type the command, `./pnet_executable` to run the compiled model. Or, with old-fashioned mouse clicking, go to the `~pnet-master/pnet_linux`folder. You would see the `pnet_executable.exe` file. You can click on this to run the program. [Note to self: is there way to run the model GUI style from VScode? Some extension?]
3. Go look in `~/pnet-master/results/site`. You should see the generated output csv files.

##### A note on compiling from the shell
We could also easily compile from the shell:
1. Navigate into ~/pnet-master/pnet-linux.
2. Type `g++ -o pnet_executable *.cpp` and press enter. The \*.cpp indicates that the compiler should select *all* .cpp files in the directory. We could compile a single .cpp file just calling `g++ somefile.cpp`, but PnET is comprised of many .cpp files linked together, so they must be compiled together.
3. Run the exectuatable like described above: `./pnet_executable`



#### Shell Navigation Crashcourse
The shell allows us to access operating system services via command line tools. In Windows, we can use **PowerShell**. Another good option is [Git Bash](https://git-scm.com/downloads). Powershell can also be access from within VScode by clicking on the `Terminal` tab and then click `New terminal`. Here are some commands that are good to know: <br/>
`pwd` **print working directy** <br/>
`cd` **change directory** e.g.) `cd C:/Users/` will bring you from **C:/** to **C:/Users/** <br/>
`cd ..` **change directory to parent directory.** e.g.) from **C:/Users** to **C:/** <br/>
`ls` **list files** in the directory <br/>
`mkdir` **make new directory**
