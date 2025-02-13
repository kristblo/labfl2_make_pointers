TTK4235 student?
================

Then read this document.

This repo contains the necessary files for following me in the 18.02 lecture, which will cover Makefiles and pointers in C. However, you are now at the master branch, which isn't really where you want to be. The main.c file is a bit more chaotic than what I'll be presenting, with some personal notes and such. No, you should download the branch called `lecture_start`, which contains a more or less empty main.c file as well as a module called `motor_driver`. If you're coming here after the lecture and want to see the code as it was when the lecture ended (with annotations to tell you what's going on), download the branch called `lecture_end` instead.

Simple solution:
----------------

Find the branch button at the top of the repo currently set to `master`, just below the repo name and way left of the green Code button. Select your preferred branch, then download the code as a zip file by pressing the green Code button and select Download ZIP. Unpack it and put it in your local TTK4235 directory (which you have, right? You're not using the Downloads directory as a permanent storage, right?), and you're ready for the lecture.

Correct solution:
-----------------

Clone the repo and call `git checkout lecture_start`. Cloning is done by pressing the green Code button and copying the HTTPS URL (not the SSH URL), then calling `git clone <url>`. The `checkout` command then lets you switch branches. Be sure to call `clone` from your preferred directory, such as your local TTK4235 directory, as git will create a copy of the entire repo inside that directory.
