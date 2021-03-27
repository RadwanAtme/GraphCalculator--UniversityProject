# GraphCalculator--UniversityProject
this is a project that was given to us as a subtitute for the final exam during the corona virus pandemic in the course Introduction to Systems Programming
(מבוא לתכנות מערכות - מת"ם)<br>
we were asked to build a graph calculator that runs similarly to a shell<br>
the graph has 2 diffrent modes:<br>
  batch mode where it can recive commands via a text file<br>
  interactive mode where it recives commands similar to a shell<br>
the calculator has a number of diffrent functions: (more details in Product.txt file)<br>
  defining a graph example: G1={v1,v2|<v1,v2>}, G2=G1<br>
  operator based functions for example: G1=!G2 , G3 = G4 + G5 , G3 = G4 * G5, G2 = G1 ^ G2, G4 = G1- G2<br>
  other functions such as print,delete,reset,who,quit<br>
  function for saving and loading a graph from a binary file and many more<br>
 
all the files were written by me from scratch, we were given 6.5 days to complete the project<br>

the project was a big learning experince for me as it was my first big project in c++<br>
since it was my first time writing in full oop i kind of made the main function look more like c style where i should have implemented another class to hold my graphs<br>
also i wasn't consistant in using const functions espicially when it came to dealing with commands and strings which made some proplems for me and i didn't choose the best data<br>
structure to handle the graphs( i have only learnt about lists,sets and vectors at the time but i should of researched more and i should of went with a map)<br>

overall it was a great learning experince and it was a fun project to do<br>

the details of the project and what was asked of us can be found in Project.txt<br>

you need these commands to run the project in a bash shell:<br>
CXX=/usr/local/bin/g++<br>
export LD_LIBRARY_PATH="/usr/local/lib64:${LD_LIBRARY_PATH}"



  
