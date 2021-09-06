## Setting up the ESP-32 WROOM32 board with ESP-IDF (Linux/MacOs)

  

### Prerequisites:

1) [Python3](https://www.python.org/downloads/)
2) [CMake](https://cmake.org/)
3) [Ninja](https://ninja-build.org/)
4) [dfu-util](http://dfu-util.sourceforge.net/)
5) [VSCode](https://code.visualstudio.com/download)

### Setup:

1) Get the [ESP-IDF framework for C/C++](https://github.com/espressif/esp-idf)
	```
	mkdir -p ~/esp
	cd ~/esp
	git clone --recursive https://github.com/espressif/esp-idf.git
	```
2) Install required tools needed by ESP-IDF for development
	```
	cd ~/esp/esp-idf
	./install.sh
	```
3) Set up environment variables required for build, flash and monitor
	```
	. $HOME/esp/esp-idf/export.sh
	```
	The ESP-IDF man-page recommends setting up an alias for frequent users. To do this, open __~/.bash_profile__ and append the following line: 
	```
	alias get_idf='. $HOME/esp/esp-idf/export.sh'
	```	  
	After this step, refresh the configuration by typing __source ~/.bashrc__ in the terminal.

4) Install the [VSCode Extension for ESP-IDF](https://marketplace.visualstudio.com/items?itemName=espressif.esp-idf-extension)

5) Then go to __View__ -> __Command Palette__ and search for Configure ESP-IDF extension

6) Select the correct PythonPath and follow the instructions.

#### More Information : https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/#get-started-connect
  
  ## Building and running the Remote-controlled car project

1) Navigate to the project root directory.

2) Set-up the necessary environment variables needed for the build. Follow step 3 from the __Setup__ guide above. You'll see a similar result in your terminal:

	```
	$. $HOME/esp/esp-idf/export.sh
	
	Setting IDF_PATH to '/Users/dev/esp/esp-idf'
	Detecting the Python interpreter
	Checking "python" ...
	Checking "python3" ...
	Python 3.7.4
	"python3" has been detected
	Adding ESP-IDF tools to PATH...
	Using Python interpreter in /Users/dev/.espressif/python_env/idf4.4_py3.7_env/bin/python
	Checking if Python packages are up to date...
	Python requirements from /Users/dev/esp/esp-idf/requirements.txt are satisfied.
	Added the following directories to PATH:
	  /Users/dev/esp/esp-idf/components/esptool_py/esptool
	  /Users/dev/esp/esp-idf/components/espcoredump
	  /Users/dev/esp/esp-idf/components/partition_table
	  /Users/dev/esp/esp-idf/components/app_update
	  /Users/dev/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin
	  /Users/dev/.espressif/tools/xtensa-esp32s2-elf/esp-2020r3-8.4.0/xtensa-esp32s2-elf/bin
	  /Users/dev/.espressif/tools/xtensa-esp32s3-elf/esp-2020r3-8.4.0/xtensa-esp32s3-elf/bin
	  /Users/dev/.espressif/tools/riscv32-esp-elf/1.24.0.123_64eb9ff-8.4.0/riscv32-esp-elf/bin
	  /Users/dev/.espressif/tools/esp32ulp-elf/2.28.51-esp-20191205/esp32ulp-elf-binutils/bin
	  /Users/dev/.espressif/tools/esp32s2ulp-elf/2.28.51-esp-20191205/esp32s2ulp-elf-binutils/bin
	  /Users/dev/.espressif/tools/openocd-esp32/v0.10.0-esp32-20210401/openocd-esp32/bin
	  /Users/dev/.espressif/python_env/idf4.4_py3.7_env/bin
	  /Users/dev/esp/esp-idf/tools
	Done! You can now compile ESP-IDF projects.
	Go to the project directory and run:

	  idf.py build
	```

3) Build the project
	```
	$ idf.py build
	```

4) Connect the ESP-32 WROOM32 board to PC

	* The WROOM32 series uses the CP2102 driver, so [install it](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) so that the PC can recognize the port.
	* To get the PORT the esp-32 is connected to, type in the following:
		```
		ls /dev/tty.*
		```
	
		 __Common Problems (MacOS)__
		 
		* [MacOs doesn't recognize the port](https://github.com/espressif/arduino-esp32/issues/1084)
		
		* Trying out different micro-USB cable or even a USB-hub could do the trick sometimes.
		
		* In my case, I had to download the latest stable version of the [ch340g-ch34g-ch34x-mac-os-x-driver](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver)



5) Flash the project to the esp-32 chip using a micro-USB cable
	```
	$ idf.py -p PORT flash
	```
6) Connect to the "Remote-controlled-car" WiFi network from a desktop or a phone. No password is required.

7) In the browser, type in http://192.168.1.1 . You should see a simple UI with several buttons for control.

8) Enjoy!