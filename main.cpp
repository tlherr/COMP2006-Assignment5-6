/**
 * Student Name: Thomas Herr
 * Student Number: 200325519
 * Assignment 5/6
 * Platform: OSX
 * Apple LLVM version 7.3.0 (clang-703.0.31)
 * Target: x86_64-apple-darwin15.6.0
 */
#include <iostream>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <vector>
#include "Averager.h"

using namespace std;

//Structure to hold data
class dataPoint {
    public:
        int year;
        int month;
        int day;
        float max_temp;
        bool emptyTemp;

        void toString() {
            cout << "Year: " << year << " Month: " << month << " Day: " << day << " Max Temp: " << max_temp << " Empty Temp: "
                 << emptyTemp << endl;
        }
};

//Vector to hold data
vector<dataPoint> dataContainer;

//Averager to perform calculations on our data
averager avgContainer;

/**
 * Check if a given file can be accessed
 * @param name name of the file to check
 * @return boolean true if file can be accessed
 */
bool fileExists (const string& name) {
    //Attempt to access the file, expecting a non error code response (anything not -1)
    return ( access( name.c_str(), F_OK ) != -1 );
}
/**
 * Turn a line of data into a useable datapoint object
 *
 * Fields (in order):
 * Date/Time,"Year","Month","Day","Data Quality","Max Temp (�C)","Max Temp Flag","Min Temp (�C)",
 * "Min Temp Flag","Mean Temp (�C)","Mean Temp Flag","Heat Deg Days (�C)","Heat Deg Days Flag","Cool Deg Days (�C)",
 * "Cool Deg Days Flag","Total Rain (mm)","Total Rain Flag","Total Snow (cm)","Total Snow Flag","Total Precip (mm)",
 * "Total Precip Flag","Snow on Grnd (cm)","Snow on Grnd Flag","Dir of Max Gust (10s deg)","Dir of Max Gust Flag",
 * "Spd of Max Gust (km/h)","Spd of Max Gust Flag"
 *
 * @param line
 * @param delimiter
 * @return
 */
dataPoint parseLine(string line, char delimiter) {
    //Make a new datapoint to represent this row
    int index = 0;
    std::stringstream  lineStream(line);
    std::string        cell;
    dataPoint current = dataPoint();
    //Iterate through cells in the row
    while(getline(lineStream,cell, delimiter))
    {
        //Remove double quoted strings
        cell.erase(remove( cell.begin(), cell.end(), '\"' ), cell.end());

        if(cell.length()!=0) {
            switch(index) {
                case 1:
                    current.year = stoi(cell);
                    break;
                case 2:
                    current.month = stoi(cell);
                    break;
                case 3:
                    current.day = stoi(cell);
                    break;
                case 5:
                    current.max_temp = stoi(cell);
                    break;
                case 6:
                    if(cell=="M") {
                        current.emptyTemp = true;
                    }
                    break;
            }
        }
        index++;
    }
    current.toString();
    return current;
}


/**
 * Read the data file.  The data file contains a significant amount of information.
 * We will only be working with the “Max Temp (°C)”. Refer to the legend to determine what position this holds.
 * Use dynamic memory allocation to place the “Max Temp (°C)” raw data into memory based on the amount of available data
 * from the data file.
 * @param fileName Name of the file to be opened
 * @param delimiter character that seperates records
 * @return int number of records read
 */
int readFile(string fileName, char delimiter) {

    if(!fileExists(fileName)) {
        cout << "Unable to access file" << endl;
        return -1;
    }

    ifstream  data(fileName);

    string line;
    //Iterate through lines in the file
    while(getline(data,line))
    {
        dataContainer.push_back(parseLine(line, delimiter));
    }
    cout << "Processed: " << dataContainer.size() << " elements" << endl;

    return (int) dataContainer.size();
}
/**
 * Using this stored daily data, and a function call, calculate the monthly average for each month of the year.
 * If the data is non-numeric, ie. “M”, as per the legend we will ignore this in our averaging.
 * Keep track of how many days were “skipped” in this calculation.
 *
 * Ex. If July had only 5 days (it won`t but for sample purposes only) and the data was 2, 3, M, 3, 2
 * the average would be: (2+3+3+2)/4 = 10/4 = 2.5.  We could display:
 * July: 2.5 (4 of 5)		//Month: Average (Number Sampled of Total for Period)
 *
 * Store these calculated monthly averages and the number of sample points into a dynamically created section of memory.
 *
 */
void calcMonthlyAvgTemp() {
    for (auto element : dataContainer) {
        avgContainer.insert(element.year, element.month, element.max_temp, element.emptyTemp);
    }
}



int main() {
    //Create our objects
    avgContainer = averager();

    //1. Print your name and student number at the start of your code
    cout << "Hi Thomas Herr (200325519)," << endl;
    cout << "Please note when specifying file paths USE FULL PATHS" << endl;

    //2. Provide a menu to the user. This will continue to loop until the user exits through option 3.
    int userInputOption = 0;

    while(userInputOption!=3) {
        cout << "Please make a selection from the following:" << endl;
        cout << "1.	Open a Climate File to Process" << endl;
        cout << "2.	Save the Climate Data" << endl;
        cout << "3.	Exit" << endl;

        cout << "What is your selection?";
        cin >> userInputOption;

        if(userInputOption==1) {
            //Open file to process
            string fname;
            cout << "What is the name of the file you would like to open: " << endl;
            cout << "/Users/tom/CLionProjects/COMP2005_Assignment5_6/2014-Barrie-Oro-Daily.csv" << endl;
            cin >> fname;
            readFile(fname, ',');

        } else if(userInputOption==2) {
            //Ask user for a filename
            string fName;
            cout << "What is the name of the file you would like to save" << endl;

            cin >> fName;

            if(cin.good()) {
                //Generate averages
                calcMonthlyAvgTemp();

                //Output to console
                avgContainer.getAll();

                //Redirect stdout to file
                ofstream out(fName.c_str());
                streambuf *coutbuf = cout.rdbuf(); //save old buf
                cout.rdbuf(out.rdbuf()); //redirect std::cout to file

                //Output
                avgContainer.getAll();

                cout.rdbuf(coutbuf); //reset to standard output again

                exit(0);
            } else {
                //Restart
                userInputOption=2;
            }
        } else if(userInputOption==3) {
            cout << "Exiting..." << endl;
            exit(0);
        } else {
            cout << "Invalid Option Entered, resetting" << endl;
            userInputOption=0;
        }
    }


    return 0;
}