# OpenPad
 OpenPad editor for OpenHoldem application (https://github.com/OpenHoldem/openholdembot)

Lightweight OpenHoldem (OH) profile file editor (OpenPPL language file) with usefull direct line inputing and autoscroll option for fast OH profile debugging mostly with huge file size and with clear target line highlighting and numbering.
Editor is based on Scintilla edit control (https://www.scintilla.org/) which is very usefull and efficient for line numbering, line scrolling, language code lexering, advanced search, marker manipulations, symboling and much more...

This first version only support basic options and future versions will implement much more like advanced search and marker manipulations.

Final intent for this project is to implement it to my "Action Trace Profile" OH patch which is a kind of OH profile debugging assistant displaying in real-time executing line from profile for an OH bot taken action (logic decision) available here: https://github.com/Mudr0x/openholdembot/commit/63dabc7a14e1792cb9986028cb55258afc815f10 on my other Real-Time Assistant (RTA) for OH bot patch available here: https://github.com/Mudr0x/openholdembot/commit/cad41f0ee102fdfdb33bffdb07e558b104e97f30.
This implementation will display real-time executing line ("Action Trace") as a hyperlink which will open this OpenPad when clicking on it to open the profile and autoscrolling and highlight this target line which could be very usefull for real-time profile debugging.

![image](https://user-images.githubusercontent.com/78977383/116783685-6c110780-aa90-11eb-9c27-f4ba7430b80c.png)


05/14/2021: Release v.1.1

+ Add text search option and markers walker, with a total recast of the app that suits MFC Document/View SDI app recommendations.


And never forget, you can learn more by sharing   ;)
 
 
