# Forsen speedrun analysis
Downloads forsen's Minecraft VODs and analyzes them to get statistics. Written in C++ using the OpenCV library.

# How to use
**Before**: make sure you have cmake and OpenCV installed (you may need to add OpenCV's _lib_ and _bin_ folders to path.

**Compiling**: create a build folder, cd into it. Run ```cmake ..``` to generate build files, and ```cmake --build .``` to build the project.

**Executing**: Run ``` forsen.exe --id STREAM_ID --date YYYY-MM-DD --times [HH:MM:SS]```. You need to specify at least two times (beginning and end), but you can specify more to split the VOD into multiple parts (in case the download fails). _**NOTE**: as of 2023-05-14 the intermediate times need to be between runs (i.e. when a seed if being loaded) otherwise the last run is reset when the video ends._

# How to contribute
**SUMMARY**: fork the repository and create **small** commits on your local repository (include any issue reference in your commit messages). Create a pull request when you're ready. Please use snake_case to keep a consistant code style.

- Fork the repository and clone it on your computer.
- OPTIONAL: keep your fork up to date with this repo
  - Make sure your fork is the origin remote (```git remote -v```, if there's no "origin" then do ```git remote add origin URL_OF_FORK```.
  - Add this repo as the upstream remote: ```git remote add upstream https://github.com/Mynnhos/forsen.git```.
  - You can now pull the latest changes from this repository using ```git pull upstream main```
- Create a new branch using ```git checkout -b BRANCH_NAME``` (or do everything in main).
- Do whatever changes you want
- Push your changes to your fork: ```git add -all```, ```git commit -m "brief description"``` and ```git push origin BRANCH_NAME```
  - **When related to an issue please include the issue reference in the commit message (URL or )**
  - **Keep your commits small**. Create a commit for each issue/feature/idea. 
- Create a pull request: on your fork page on GitHub click on _"Compare and pull request"_ describing what you changed. I'll review it as soon as possible.
