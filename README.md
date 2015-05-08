Nebulo Mod
==========
A Flanger and Phaser plug-in with a customizable LFO.
-----------------------------------------------------
![alt tag](https://github.com/tlongabaugh/nebulo-mod/blob/master/Pictures/NebuloModPicture.png)

Authors: Ryan Foo and Tom Longabaugh

School: New York University, Music Technology Program.

AU and VST formats supported. 

## Installation and Building the Project
This plugin uses the JUCE library, AU, and VST SDK to build properly. The user must have XCode 6.0.1 and above to build the project. In order to build the project, please follow the directions in (this folder)/VST_AU_SDKs/README.txt to install the AU and VST SDKs on your machine, or follow the below link to install JUCE for VST development: Intro to JUCE Development
 
After installing the AU and SDK libraries, the project can be built by simply opening /nebulo_mod_code/nebuloMod/Builds/MacOSX/NebuloMod.xcodeproj and pressing "Build." The XCode project will build an executable in your ~/Library/Audio/Plug-Ins/VST, ~/Library/Audio/Plug-Ins/VST3, and ~/Library/Audio/Plug-Ins/Components directories. Both Ableton and Logic (Ableton prefereed) should be able to use our plug-in, as well as other (untested) DAWs excluding Pro Tools. Make sure you build the project in the corresponding 32- or 64-bit format for your DAW.
 
These DAWs have been tested, but plug-in should work in any DAW that supports VST or AU:
Ableton Live
Logic Pro  

