
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <list> 
#include "DecisionTree.h"

using namespace std;

list< map<int, bool> > getSampleListFromFile(string, int &, int &);
bool boolFunc(bool []);
void outputTree(DecisionTree&);

int main(int argc, char** argv) {
    
    //No file path provided.
    if (argc < 2) {
        cerr << "Missing argument. Command to run is EXECUTABLE_NAME FILE_PATH."
             << endl;
        exit(1);
    }
    
    //Get sample data set from provided file
    string filePath = argv[1];
    int numVariables, totalData;
    list< map<int, bool> > sampleList = 
        getSampleListFromFile(filePath, numVariables, totalData);
    
    //Make the tree and print it
    DecisionTree dt = DecisionTree(sampleList);
    dt.printTree(dt.getRootNode(), 0);
    
    cout << endl << "TREE FORMAT" << endl;
    cout << " aX <-attribute number\n"
         << " |--------1 <-output value \n"
         << " | ^Negative branch (aX is false)\n"
         << " |\n"
         << " | <-Positive branch (aX is true)\n"
         << " |-------aY <-next attribute\n\n";
    
    cout << "Input boolean values (0 or 1) to test the decision tree.\n";
    cout << "Input a non-boolean character to exit.\n";
    
    //User input loop.
    bool bools[numVariables];
    while (true) {
        for (int i=0; i<numVariables; i++) {
            cout << "Attr " << i+1 << ": ";
            while (!(cin >> bools[i])) {
                cout << "Non-boolean value entered. Exiting now...\n";
                exit(0);
            }
        }
        cout << "Output value: " << dt.evaluate(bools) << endl << endl;
    }
    
    return 0;
}

list< map<int, bool> > getSampleListFromFile(
        string filePath, int & numVariables, int & totalData) {
    
    //Open the sample data set
    ifstream inFile;
    inFile.open(filePath);
    if (!inFile) {
        cerr << "Error: Couldn't open the sample data file. Exiting now...\n";
        exit(1);
    }
    
    //Read first two numbers as number of decision variables and total number of 
    //samples
    if (!(inFile >> numVariables) || !(inFile >> totalData)) {
        cerr << "Read error on initial dataset values...\n";
        exit(1);
    }
    
    //Read data to sampleList
    list< map<int, bool> > sampleList;
    for (int i=0; i<totalData; i++) {
        
        map<int, bool> sample;
        for (int j=0; j<numVariables; j++) {
            if (!(inFile >> sample[j+1])) {
                cerr << "Read error on sample value...\n";
                exit(1);
            }
        }
        
        if (!(inFile >> sample[0])) {
            cerr << "Read error on sample value...\n";
            exit(1);
        }
        
        sampleList.push_back(sample);
    }
    inFile.close();
    
    return sampleList;
}

