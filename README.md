## SpireTool

SpireTool is a graphical interface for looking at data extracted from Slay The Spire's beta autosave files. Currently, it supports the tracking of the percent chance each floor has for dropping a potion. It will constantly try and read the most recent autosave and display this information on the main screen. It can also automatically write a formatted string with this variable to a file for OBS to use as a "Text Source" or for other programs to use as needed. It was inspired originally by the twitch streamer [TerrenceMHS](https://twitch.tv/terrencemhs) as a way to automate the display of potion chance on his stream. This tool is still under development but has enough working features for an initial release.

![Screenshot](https://raw.githubusercontent.com/casey-c/spiretool/master/spiretool_screenshot_v3.png "Main Window")
![Screenshot](https://raw.githubusercontent.com/casey-c/spiretool/master/spiretool_screenshot_stats.png "Statistics Window")

*Note*: this stats screen is currently bugged in that "wins" are computed as reaching floor 57 (a heart kill). The screenshot above was taken on my windows install, which has runs from pre-heart days (before I switched to Linux full time and before A20 was even a thing) so that explains the terrible record (I swear I'm not that bad!). Future updates will hopefully improve the filtering available and allow users to customize what sorts of run streaks they're interested in in particular. (E.g. look at win streaks with Snecko eye or runs without Runic Pyramid or runs containing a certain card obtained before a certain floor, etc.). The idea is to make it as flexible as possible to allow looking at all sorts of interesting and complex statistics. These fixes are planned for the "Run Update" (see Roadmap section below).

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

Parsing through the run data should allow the collection of various statistics. One goal is to visualize these in a useful and appealing way. The following mockup-image is something I would like to have in the program, but may be difficult due to the custom graphics code I would need to write for the histogram visualization:
![Screenshot](https://raw.githubusercontent.com/casey-c/spiretool/master/card_detail.png "Card Statistics")

### Current Roadmap
Although I'm pretty busy with grad school at the moment, the near future has some particular milestones set to try and work towards. These are not set in stone and the order may change (and will likely take months of work), but I think it's a general idea of which way this project is heading. Major updates are **bolded** and are the primary goals I'm working on.

* **The Card Update** -- I'm currently working on a database of cards to track their usage over all runs. The simple database portion is nearly completed, but I would like to start tracking all the occurences of each one in various runs. This update will hopefully finish the Cards section of the reference window (including the costs/types/rarities/etc.), and allow for quick access to look at desirable rare cards (e.g. for Neow). Beyond the reference section improvements, I hope to have some general statistics for picks/skips/purchases/etc. At first these will likely be text form only, but I have started some mockups of the sort of visualizations that might be useful. This piece will likely require some OpenGL graphics code (which I'm not an expert on, but have used a number of times prior), and so the visualization piece may be pushed back indefinitely. With this major update, I hope to have the Unknown/Rare card chance hooked up to keep track of commons seen and all that to help make similar decisions like the currently functional potion chance tracker.
* The Reference Update -- This is a more minor update to bring in the pieces of the spreadsheet into the program itself. Being able to know an enemy's AI pattern, a boss's moveset, and other features like how much potions cost in a store, or what sort of events might exist in the current act, are all details that an A20 player might need to maximize their chances of victory. The goal is to integrate all these useful stats in a cleaner and more intuitive interface and to bring it all into one place so you don't need to open a browser every time.
* **The Run Update** -- I'm planning on overhauling the Run system to make it easier to track runs and choices in the run. The goal is to be able to visualize the current autosave history (solving the age old problem of: what was that question mark I just went to again?) and the run on a whole (with stats like max/current hp, gold at every floor, etc.). These will hopefully be accompanied by the histogram visualization code as before, but the major goal is to just have a human readable history of a run (current autosave or historical) much like Spirelogs has in their data. This major update may be pushed earlier as handling the run history in a more intelligent way is pretty important for most pieces of this project.
* The Data/Vis Update -- This minor update will focus on making the collected data more intuitive to visualize (using things like histograms or other types of charts) and easy to export to other programs (e.g. spit it out into a .csv) to let those other applications (e.g. Google Sheets or Matplotlib) visualize it in different ways. The goal is to make it flexible to allow more superusers to really take advantage of the data collected without needing to edit this program's source code.
* The Aesthetics Update -- The interface is pretty ugly and will remain ugly as it is being built. This update will hopefully unify most of the designs and make them cleaner and more intuitive to use (especially for novice users). Beyond the front end that users see, I'd like to go back and refactor a lot of the subsystems involved, document the code, and improve accessibility for other devs to latch onto the project in the future. Currently the code is pretty messy as I haven't been following my usual code quality steps that I typically follow due to it being more of a hobby project than something more serious. Improving the documentation in particular is a major goal as I have the habit of abandoning projects due to burnout and disinterest after a while.


### Building from source
Current plans are to release Windows builds as I am not sure how to produce standalone executables for other systems yet. To build for your specific platform: clone this repository, open it in QtCreator, and build. No additional libraries beyond Qt should be needed to compile.

### Contributing
Please report bugs / suggestions / ideas in the ISSUES section of this repository. Contributors and merge requests are welcome, but be warned the code is very pre-alpha and is not fully documented yet nor refactored in any meaningful way. I still have some plans for this project, but I am extremely busy with grad school at the moment so releases may be a bit slow.
