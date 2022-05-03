set(SRC
  Wizard97Test.cpp
  stdafx.cpp
  Wizard/TestWizard.cpp
  Wizard/TestWizardCompletionPage.cpp
  Wizard/TestWizardFilePreviewPage.cpp
  Wizard/TestWizardInfo.cpp
  Wizard/TestWizardOutputPage.cpp
  Wizard/TestWizardPathFilterPage.cpp
  Wizard/TestWizardSheet.cpp
  Wizard/TestWizardWelcomePage.cpp
  )
set(RC Wizard97Test.rc)
add_executable(Wizard97Test WIN32 ${SRC} ${RC})
