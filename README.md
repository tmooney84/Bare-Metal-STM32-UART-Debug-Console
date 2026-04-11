# Register Level LED Controller

This is a bare-metal C firmware program that controls an LED with an on-board button using direct register access and is written for the STM Nucleo-F411RE Development Board and a virtualized version of that board in Renode.

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
    git clone https://github.com/tmooney84/Register_Level-LED-Controller.git
    ```

using SSH:
    ```sh
    git clone git@github.com:tmooney84/Register_Level-LED-Controller.git
    ```

2. Navigate to the project directory:
    ```sh
    cd /Register_Level-LED-Controller
    ```

## Usage

### 1. If running on a physical STM Nucleo-F411RE Board 

A. open the STM32CubeIDE and connect the board using the microUSB cable. If you need to install STM32CubeIDE visit     https://www.st.com/en/development-tools/stm32cubeide.html.
    
B. Once you have opened the IDE, make sure you are in the correct workspace and navigate to the Register_Level-LED-Controller project and the proj_1_led_controller sub-directory. The main project code can be found in the Src and Inc sub-directories.

C. Next build the project and download the firmware to the STM board. Look to IDE Help guide for further details.
    
D. Once the firmware is loaded on the board, the on-board PC-13 button should toggle the PA-5 LED on and off. 

[![LED Demo](https://i.ytimg.com/an_webp/q3SN33JRCC0/mqdefault_6s.webp?du=3000&sqp=CIGY-MwG&rs=AOn4CLB_RjRVUVDbnDqcjIYLr7qQ0KYNaQ)](https://www.youtube.com/watch?v=q3SN33JRCC0)


### 2. If emulating the the board using Renode 
  A. Open Renode and start the Renode Script (.resc) for the virtual board and firmware.
  
  ```renode
  start @/absolute-path-to-repository-folder/renode-emulation/proj_1.resc
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

## Future Improvements
  -Interrupt-based logic connecting the button and LED 
  
  -Include UART debugging and Robot Framework Testing

## License

Copyright <2026> <Trevor Mooney>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


---

Thank you for visiting!