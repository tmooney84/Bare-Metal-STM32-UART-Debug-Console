*** Variables ***
${BUTTON}     sysbus.gpioc.buttonPC13
${LED}        sysbus.gpioa.greenled2
${UART}       sysbus.usart2
${URI}        @${CURDIR}/UART_debug_console.resc

*** Keywords ***
Setup Simulation
    Execute Command    include ${URI}
    Create Terminal Tester    ${UART}

*** Test Cases ***
### IS A DELAY NEEDED LIKE THE POLLING VERSION FOR
### THE BUTTON DEPRESS LOGIC???
LED Toggles On Via Button Interrupt
    [Setup]    Setup Simulation
    Execute Command    ${BUTTON} Press
    Execute Command	emulation RunFor "0.05"
    #Execute Command    emulation Sleep 50ms
    Execute Command    ${BUTTON} Release
    Execute Command	emulation RunFor "0.05"
    #Execute Command    emulation Sleep 50ms

    Wait For Line On Uart    BTN Pressed...
    Wait For Line On Uart    LED ON
    ${state}=    Execute Command    ${LED} State
    Should Contain    ${state}    True

LED Toggles Off Via Button Interrupt
    [Setup]    Setup Simulation
    # First press to turn on
    Execute Command    ${BUTTON} Press
    Execute Command    ${BUTTON} Release
    Wait For Line On Uart    LED ON
    # Second press to turn off
    Execute Command    ${BUTTON} Press
    Execute Command    ${BUTTON} Release
    Wait For Line On Uart    LED OFF
    ${state}=    Execute Command    ${LED} State
    Should Contain    ${state}    False

UART Command Led On
    [Setup]    Setup Simulation
    Write Line To Uart    led on
    Wait For Line On Uart    LED ON
    ${state}=    Execute Command    ${LED} State
    Should Contain    ${state}    True

UART Command Led Off
    [Setup]    Setup Simulation
    Write Line To Uart    led on
    Write Line To Uart    led off
    Wait For Line On Uart    LED OFF
    ${state}=    Execute Command    ${LED} State
    Should Contain    ${state}    False

UART Command Echo Message
    [Setup]    Setup Simulation
    Write Line To Uart    echo "Hello world!"
    Wait For Line On Uart    Hello world!

UART Command Sysinfo
    [Setup]    Setup Simulation
    Write Line To Uart    sysinfo
    Wait For Line On Uart    processor speed:
    Wait For Line On Uart    SRAM size: 128KB

UART Help Menu Via Ctrl+H
    [Setup]    Setup Simulation
    # Send ASCII Backspace (0x08) for Ctrl+H
    Write Char On Uart    \x08
    Wait For Line On Uart    HELP MENU
    Wait For Line On Uart    led toggle: This will toggle the led switch on/off.
