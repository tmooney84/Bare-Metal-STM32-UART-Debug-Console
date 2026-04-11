# Bare-Metal UART Debug Console

This is a bare-metal interrupt-driven UART Debug Console written in C includes Button-LED logic, UART-based control of the on-board LED echoing strings from the host PC terminal, and supports debug messages from the firmware to a host PC terminal using programs such as PuTTY or minicom. The debug console uses direct register access and is written for the STM Nucleo-F411RE Development Board and a virtualized version of that board in Renode.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Future Improvements](#future-improvements)
- [License](#license)

## Installation

To run this repository locally, follow these steps:

1. Clone the repository 
using HTTPS:
    ```sh
    git clone https://github.com/tmooney84/Bare-Metal-STM32-UART-Debug-Console.git
    ```

using SSH:
    ```sh
    git clone git@github.com:tmooney84/Bare-Metal-STM32-UART-Debug-Console.git 
    ```

2. Navigate to the project directory:
    ```sh
    cd /Bare-Metal-STM32-UART-Debug-Console
    ```

## Usage

### 1. If running on a physical STM Nucleo-F411RE Board 

A. open the STM32CubeIDE and connect the board using the microUSB cable. If you need to install STM32CubeIDE visit     https://www.st.com/en/development-tools/stm32cubeide.html.
    
B. Once you have opened the IDE, make sure you are in the correct workspace and navigate to the Bare-Metal-STM32-UART-Debug-Console project. The main project code can be found in the Src and Inc sub-directories.

C. Next build the project and download the firmware to the STM board. Look to IDE Help guide for further details.
    
D. Once the firmware is loaded on the board, the on-board PC-13 button should toggle the PA-5 LED on and off. 

### 2. If emulating the the board using Renode 
  A. Open Renode and start the Renode Script (.resc) for the virtual board and firmware.
  
  ```renode
  start @/absolute-path-to-repository-folder/renode-emulation/UART_debug_console.resc
  ```

  B. To simulate Pressing and Releasing the button type Press and Release commands in succession and then check the virtual LED's state.
  
  ```renode
  (nucleo_f411re) gpioc.buttonPC13 Press
  ```
 
  ```renode
  (nucleo_f411re) gpioc.buttonPC13 Release
  ```
  
  ```renode
  (nucleo_f411re) gpioa.greenled2 State
  ```

  C. To simulate typing into a host computer terminal editor, type commands such as "led on", "led off", "led toggle", "echo \"string\" "ctrl + H" and "sysinfo" into the Terminal Analyzer window.

  1) Turning LED on

  ```terminal analyzer
  input: led on
  output: LED ON
  ```

  ```renode
  (nucleo_f411re) gpioa.greenled2 State
  ```

  2) Turning LED off
  ```terminal analyzer
  input: led off
  output: LED OFF
  ```

  ```renode
  (nucleo_f411re) gpioa.greenled2 State
  ```

  3) Toggling LED
  ```terminal analyzer
  input: led toggle
  output: ["LED ON" or "LED OFF"]
  ```
  ```renode
  (nucleo_f411re) gpioa.greenled2 State
  ```


  4) Echoing String
  ```terminal analyzer
  input: echo "Hello World!"
  output: Hello World!
  ```

  5) Ctrl + H for HELP MENU
  ```terminal analyzer
  input: ctrl + H
  output: HELP MENU
	   echo \"<message>\": This display a message back on host machine.
	   led on: This command will turn the led on.
     led off: This will turn the led off.
	   led toggle: This will toggle the led switch on/off.
     sysinfo: This will display vital system information including board name, processor speed, baud rate, memory size
  ```

  6) sysinfo for SYSTEM INFORMATION
  ```terminal analyzer
  input: sysinfo
  output: processor speed: $ps
		   flash size: $fsKB
		   "SRAM size: 128KB
		   "baud rate: $br
  ```
  

### 3. If running the Automated Testing using Robot Framework
A. Navigate to the renode-emulation subdirectory
    ```bash
    cd renode-emulation
    ```
B. Run run_tests.py script
  ```powershell
  python run_tests.py  (Windows)
  ```
  
  ```bash
  python3 run_tests.py (Linux)
  ```

## License

Copyright <2026> <Trevor Mooney>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


---

Thank you for visiting!