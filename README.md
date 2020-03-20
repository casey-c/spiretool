## SpireTool

![Screenshot](https://raw.githubusercontent.com/casey-c/spiretool/master/spiretool_screenshot.png "Screenshot")

SpireTool is a graphical interface for looking at data extracted from Slay The Spire's beta autosave files. Currently, it supports the tracking of the percent chance each floor has for dropping a potion. It will constantly try and read the most recent autosave and display this information on the main screen. It can also automatically write a formatted string with this variable to a file for OBS to use as a "Text Source" or for other programs to use as needed. It was inspired originally by the twitch streamer [TerrenceMHS](https://twitch.tv/terrencemhs) as a way to automate the display of potion chance on his stream. This tool is still under development but has enough working features for an initial release:

### Features
* Cross-platform (using Qt C++) - [Developed on Linux, but will provide Windows builds to the releases section of this repository]
* Configurable options menu (set the Slay The Spire save folder, change how the output file is formatted, etc.)
* Automatic writing to a file for OBS to read from

### Installation / Use
To use this program, head over to the https://github.com/casey-c/spiretool/releases section of this repository and download the most recent release. Extract the folder to a place on your file system and run the executable. On first use, you'll need to open up the Options menu (Edit->Options or press 
'O') and set the SlayTheSpire saves folder that the program needs to read. Additional details can be found in the Help->Tutorial window of the program.

### Planned Features
* Tracking Uncommon Card drop chances
* Tracking Rare Card drop chances
* Tracking other statistics
* Less minimalist / ugly interface

### Building from source
Current plans are to release Windows builds as I am not sure how to produce standalone executables for other systems yet. To build for your specific platform: clone this repository, open it in QtCreator, and build. No additional libraries beyond Qt should be needed to compile.

### Contributing
Please report bugs / suggestions / ideas in the ISSUES section of this repository. Contributors and merge requests are welcome, but be warned the code is very pre-alpha and is not fully documented yet nor refactored in any meaningful way. I still have some plans for this project, but I am extremely busy with grad school at the moment so releases may be a bit slow.
