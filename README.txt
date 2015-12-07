How to run the code.

1. Access/go into the edge directory (cd Edge)
2. make a new directory for the build of the application. (mkdir build)
3. go into the build directory. (cd build)
4. run cmake.  eg. (cmake ..)
5. once you run cmake, a directory that has the executable should be created in the edge directory. The name of the new directory is bin. Change to the bin directory and navigate to the executable file.  On my machine, here is how it's done (cd ../bin/macosx/)

6. the executable is called coloring_book.  To run it type the following command. (./coloring_book ../../images/Statue_of_Liberty_color-wiki.png).  This should work on any png or jpeg image. 

7. Have fun...
