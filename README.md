# Decision Tree

## SUMMARY
    This program uses the decision tree learning algorithm to create a binary 
    decision tree from a data set in a file provided by the user. It will accept
    any list of samples consisting of binary attributes and outputs as long as 
    it fits this format:

        x y
        a1,1 a1,2 a1,3 . . . a1,x o1
        a2,1 a2,2 a2,3 . . . a2,x o2
        a3,1 a3,2 a3,3 . . . a3,x o3
                    .
                    .
                    .
        ay,1 ay,2 ay,3 . . . ay,x oy

    where x is the number of attributes in each sample, y is the number of
    samples, and each attribute value and output is a binary value.

    After creating the tree, the user is given the option to input sets of 
    binary data to get an output from the decision tree.


## COMPILING AND RUNNING
    This program consists of two files:
        DecisionTree.cpp
        main.cpp        

    It was written using the c++11 standard on a Linux machine.
    To compile enter the following command:
    
        g++ -std=c++11 main.cpp DecisionTree.cpp

    Set the executable permission on the resulting executable file.
    
    The program takes in as an argument the file path of the file containing 
    the data set. So to run the program:
    
        EXECUTABLE_NAME PATH_TO_FILE


## OPERATION
    Upon executing the program, as long as the data is read in from the file,
    the decision tree will be automatically created and displayed on the screen.
    It displays in the following format:

        a1 <--root attribute
        |--------1 <--output value 
        |  ^Negative branch (a1 is false)
        |
        |
        |-------a2 <--next attribute to split on
            ^Positive branch (a1 is true)

    Basically negative branches are printed above positive branches. In the
    small example above, if a1 is negative, the output is 1. If it is positive,
    then check a2.

    After displaying the program will prompt for some attribute values. Simply
    type 1 or 0 followed by enter until an output value is displayed.

    The program repeats until a non-boolean value is entered.
