*** Variables ***
${LED}                          sysbus.gpioa.greenled2

*** Test Cases ***
blinky on nucleo_f411re
    ${x}=                       Execute Command         include @${CURDIR}/blinky.resc
    Create Terminal Tester      sysbus.usart2    timeout=15  defaultPauseEmulation=true
    Create LED Tester           ${LED}

    Wait For Line On Uart       *** Booting Zephyr OS build f931c5a44f65 ***
    Wait For Line On Uart       LED state: (ON|OFF)      treatAsRegex=true
    Assert LED Is Blinking      testDuration=4  onDuration=1  offDuration=1