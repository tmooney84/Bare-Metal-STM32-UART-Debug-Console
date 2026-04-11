*** Variables ***
${BUTTON}    sysbus.gpioc.buttonPC13
${LED}       sysbus.gpioa.greenled2

*** Test Cases ***
LED Toggles 'On' Button Press
      [Setup]    Execute Command    include @${CURDIR}/proj_1.resc
      Execute Command    ${BUTTON} Press
      Execute Command    pause
      Execute Command    emulation RunFor "0.2"
      Execute Command    ${BUTTON} Release
      ${state}=    Execute Command   ${LED} State
      Should Contain    ${state}    True

LED Toggles 'Off' Button Press
      [Setup]    Execute Command    include @${CURDIR}/proj_1.resc
      Execute Command    ${BUTTON} Press
      Execute Command    pause
      Execute Command    emulation RunFor "0.2"
      Execute Command    ${BUTTON} Release
      Execute Command    pause
      Execute Command    emulation RunFor "0.5"
      Execute Command    ${BUTTON} Press
      Execute Command    pause
      Execute Command    emulation RunFor "0.2"
      Execute Command    ${BUTTON} Release
      Execute Command    pause
      Execute Command    emulation RunFor "0.5"
      ${state}=    Execute Command   ${LED} State
      Should Contain    ${state}    False
