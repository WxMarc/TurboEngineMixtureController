# TurboEngineMixtureController
Mixture Controller for Turbocharged Piston Engines in MSFS 2020

Microsoft Flight Simulator's fuel mixture logic for turbocharged piston engines produces excessively rich fuel mixtures
and loss of power at high altitudes. The default logic appears to calculate fuel/air mixture based on ambient density. 
In a real-world turbocharged engine, the turbocharger compresses air before it enters the cylinders. This increases the 
density of air in the engine significantly above ambient density. As such, the default logic in MSFS does not properly 
represent the behavior of a turbocharged engine. A more detailed description of the problem, with data extracted from 
MSFS, is available here:

https://forums.flightsimulator.com/t/inaccurate-mixture-behavior-on-turbocharged-piston-engines/411095

This package includes mixture controllers for the three turbocharged piston aircraft that are currently available for
MSFS: 

- Robert Young's Turbonormalized Bonanza
- Just Flight Turbo Arrow
- Carenado Seneca V  

The controllers calculate a target fuel/air mixture based on the user's hardware mixture lever setting and the air density 
in the intake manifold (rather than the ambient density). This allows the mixture to be managed according to the POH, 
setting mixture at full-rich throughout the climb instead of leaning the mixture to maintain proper power.

Please see the included instructions.pdf for detailed instructions on how to install the controllers for any of the three
aircraft mentioned above.

DEPENDENCIES: 
Successful installtion of the mixture controller requires updating the layout.json file for the airplane to include a 
reference to the .wasm file which is added to the airplane's panel folder. I recommend using the MSFS Layout Generator for
this. It is a quick drag-and-drop utility that updates layout files painlessly. See the link below for more information.

https://github.com/HughesMDflyer4/MSFSLayoutGenerator

CAUTION: 
Successfully installing these controllers will require copying and pasting the files to the proper folders and updating 
each airplane's panel.cfg and layout.json files. If you are not comfortable taking these steps, do not use this code. 

These controllers are designed to work with a hardware axis (throttle quadrant, HOTAS with mixture axis, etc). If you do 
not have a hardware mixture axis, or if you have automixture enabled in MSFS settings, this controller will not add much
value to your experience.

KNOWN ISSUES:
The mixture lever displayed in the virtual cockpit will not reflect the mixture setting on your hardware axis. The virtual 
cockpit lever displays the mixture setting in the sim, which the controller code calculates based on your hardware axis 
position and the density in the intake manifold. At high altitudes, it is quite possible to see the virtual cockpit lever 
set to 25% mixture while the hardware axis is set to full-rich. This is an artifact of the incorrect mixture logic in the 
core sim. It is possible to map the virtual cockpit mixture lever to the hardware axis, but this controller currently does
not implement that feature.
