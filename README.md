allegro_gripper_windows
====================

This application is lightweight control and communication software for the Allegro Gripper on Windows.

myAllegroGripper.exe:
 Allegro Gripper control and CAN communication module.

rPanelManipulator.exe 
 GUI Interface for executing different control and grasp modes
 
 
Instructions
============ 

 1. Open up the solution, myAllegroGripper.sln, in Visual Studio
 2. Right click the project 'myAllegroGripper' in the Solution Explorer and click 'Properties'
 3. At the top of the 'Property Pages' window, set Configuration to 'All Configurations'
 4. Navigate to Configuration Properties > Debugging and set the Working Directory to 'bin'
 5. Open myAllegroGripper.cpp and, near the top, find comment '// SET CORRECT PARAMETER HERE BEFORE RUNNING THIS PROGRAM.' and the constants below it.
 	- const bool GRIPPER_9DOF: Set to 'true' if using a 9-DOF gripper. If not set to 'false'.
	- const int GRIPPER_VERSION: Set to '4' (not used).
 
You are now ready to compile, plug in and turn on your Allegro Gripper, and test the program.

Keyboard commands can be used to execute grasps and other joint configurations. 
See the instructions printed at the beginning of the application.

=====

Allegro Gripper Standalone Visual Studio Project and Source

This file contains a summary of what you will find in each of the files that make up your myAllegroGripper application.



**myAllegroGripper.vcproj:**

This is the main project file for VC++ projects generated using an Application Wizard. It contains information about the version of Visual C++ that generated the file, and information about the platforms, configurations, and project features selected with the Application Wizard.

	
	
**myAllegroGripper.cpp:**

This is the main application source file.

	
	
**Other standard files:**

StdAfx.h, StdAfx.cpp

These files are used to build a precompiled header (PCH) file named myAllegroGripper.pch and a precompiled types file named StdAfx.obj.

	
	
**Other notes:**

AppWizard uses "TODO:" comments to indicate parts of the source code you should add to or customize.

For more information regarding setting the project properties, etc., please see the included document Allegro_Gripper_MSVS_Project_*.pdf.

**Note:** This document is currently provided only in Korean. All important setup information can be gathered deirectly from the screenshots.



**Questions?**

Please email Sean Yi
seanyi@wonik.com
