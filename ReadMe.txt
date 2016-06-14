Logitech|G Arx Control SDK
Copyright (C) 2015 Logitech Inc. All Rights Reserved


Introduction
--------------------------------------------------------------------------

This package enables developers to easily add support in their games or 
applications for Logitech|G Arx Control App. 


Contents of the package
--------------------------------------------------------------------------

- Logitech|G Arx Control SDK header, libs 32 and 64 bit
- Logitech|G Arx Control SDK Game engines wrapper dll, 32 and 64 bit
- Demo game executable
- Documentation
- Samples using the SDK.
	-MFC_Sample as application sample
	-DirectX_Sample as game sample


The environment for use of the package
--------------------------------------------------------------------------

1. Visual Studio 2012 to build and run the sample program. Logitech Gaming
Software 8.55+ installed on the development machine


Disclaimer
--------------------------------------------------------------------------

This is work in progress. If you find anything wrong with either
documentation or code, please let us know so we can improve on it.


Where to start
--------------------------------------------------------------------------

For a demo program to display content in Arx Control:

Execute Demo/SDK_SampleGame.exe

Or:

1. Go to Samples/DirectX_Sample  folder and open the project in 
   Visual Studio 2012

2. Compile and run it.

3. Run Arx Control installed on any compatible mobile device.

5. Enjoy the Sample game's applet on Gamepanel App.


To implement game controller support in your game:

1. Include the following header file in your game:

- Include/LogitechGArxControlLib.h

2. Include the following library in your game:

- Lib\x86\LogitechGArxControlLib.lib
- Lib\x64\LogitechGArxControlLib.lib

3. Read and follow instructions from Doc/LogitechGArxControl.pdf

For more specific details on the samples, please refer to the ReadMe.txt in the folder Samples/.

For questions/problems/suggestions email to:
devtechsupport@logitech.com
