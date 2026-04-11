*** Settings ***
### THIS MAY NOT WORK FOR IMPORTING THE LIBRARY
### IS THIS EVEN NEEDED???
Library    RenodeLibrary

*** Variables ***
${BUTTON}    sysbus.gpioc.buttonPC13
${LED}       sysbus.gpioa.greenled2
${UART}      sysbus.usart2

### DO I NEED TO HAVE A DELAY LIKE THE POLLING VERSION
### FOR THE BUTTON DEPRESS LOGIC???
*** Test Cases ***
LED Toggles On Button Press
    [Setup]    Execute Command    include @${CURDIR}/proj_1.resc
    ${state0}=    Execute Command    ${LED} State
    Should Contain    ${state0}    False

    Execute Command    ${BUTTON} Press
    Execute Command    emulation RunFor "0.05"
    Execute Command    ${BUTTON} Release
    Execute Command    emulation RunFor "0.05"

    ${state1}=    Execute Command    ${LED} State
    Should Contain    ${state1}    True
    ${uart1}=    Execute Command    logLevel 4
    # better: check UART output with terminal tester if your suite has it

LED Toggles Off Button Press
    [Setup]    Execute Command    include @${CURDIR}/proj_1.resc

    Execute Command    ${BUTTON} Press
    Execute Command    emulation RunFor "0.05"
    Execute Command    ${BUTTON} Release
    Execute Command    emulation RunFor "0.05"

    Execute Command    ${BUTTON} Press
    Execute Command    emulation RunFor "0.05"
    Execute Command    ${BUTTON} Release
    Execute Command    emulation RunFor "0.05"

    ${state}=    Execute Command    ${LED} State
    Should Contain    ${state}    False

Echo Command
    [Setup]    Execute Command    include @${CURDIR}/proj_1.resc
    Create Terminal Tester    ${UART}    115200
    Write Line To Uart    ${UART}    echo "Hello world!"
    Wait For Line On Uart    Hello world!

LED On Command
    [Setup]    Execute Command    include @${CURDIR}/proj_1.resc
    Create Terminal Tester    ${UART}    115200
    Write Line To Uart    ${UART}    led on
    Wait For Line On Uart    LED ON

LED Off Command
    [Setup]    Execute Command    include @${CURDIR}/proj_1.resc
    Create Terminal Tester    ${UART}    115200
    Write Line To Uart    ${UART}    led off
    Wait For Line On Uart    LED OFF

LED Toggle Command
    [Setup]    Execute Command    include @${CURDIR}/proj_1.resc
    Create Terminal Tester    ${UART}    115200
    Write Line To Uart    ${UART}    led toggle
    Wait For Line On Uart    LED ON

Sysinfo Command
    [Setup]    Execute Command    include @${CURDIR}/proj_1.resc
    Create Terminal Tester    ${UART}    115200
    Write Line To Uart    ${UART}    sysinfo
    Wait For Line On Uart    processor speed:
    Wait For Line On Uart    flash size:
    Wait For Line On Uart    SRAM size:
    Wait For Line On Uart    baud rate:

Help Menu With CtrlH
    [Setup]    Execute Command    include @${CURDIR}/proj_1.resc
    Create Terminal Tester    ${UART}    115200
    Write Char To Uart    ${UART}    \x08
    Wait For Line On Uart    HELP MENU
