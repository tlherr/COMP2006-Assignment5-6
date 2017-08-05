/**
 * Student Name: Thomas Herr
 * Student Number: 200325519
 * Assignment 5/6
 */
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <vector>

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
        //Make a new datapoint to represent this row
        dataPoint current = dataPoint();
        int index = 0;
        std::stringstream  lineStream(line);
        std::string        cell;
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
                        if(cell=="M") {
                            current.emptyTemp = true;
                        } else {
                            current.max_temp = stoi(cell);
                            current.emptyTemp = false;
                        }
                        break;
                    default:break;
                }
            }

            index++;
        }

        current.toString();
        dataContainer.push_back(current);
    }

    cout << "Processed: " << dataContainer.size() << " elements" << endl;

    return 0;
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
 * @return
 */
int calcMonthlyAvgTemp() {
    //Iterate over the data and group months
}



int main() {
    //1. Print your name and student number at the start of your code
    cout << "Hi Thomas Herr (200325519)," << endl;

    //2. Provide a menu to the user. This will continue to loop until the user exits through option 3.
    int userInputOption = 0;

    while(userInputOption!=3) {
        cout << "Please make a selection from the following:" << endl;
        cout << "1.	Open a Climate File to Process		//will call a function for opening" << endl;
        cout << "2.	Save the Climate Data			//will call a function for saving" << endl;
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
            //Save climate data
            calcMonthlyAvgTemp();


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