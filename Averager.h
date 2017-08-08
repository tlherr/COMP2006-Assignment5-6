#ifndef COMP2005_ASSIGNMENT5_6_AVERAGER_H
#define COMP2005_ASSIGNMENT5_6_AVERAGER_H

#include <vector>
#include <map>
#include <iostream>
#include <sstream>

using namespace std;

class month {
public:
    int monthNum;
    vector<float> temps;

    month(int monthNum) : monthNum(monthNum) {}
    void addTemp(float temp) {
        temps.push_back(temp);
    }

    string getMonthName(int monthNum) {
        switch(monthNum) {
            case 1:
                return "January";
            case 2:
                return "February";
            case 3:
                return "March";
            case 4:
                return "April";
            case 5:
                return "May";
            case 6:
                return "June";
            case 7:
                return "July";
            case 8:
                return "August";
            case 9:
                return "September";
            case 10:
                return "October";
            case 11:
                return "November";
            case 12:
                return "December";
        }

        return "Invalid Month";
    }

    /**
     * Calculate average temperate for the month, include how many temperatures were skipped
     * @return string expected format: July: 2.5 (4 of 5)
     */
    string calcAvg() {
        float sum = 0;
        int skipped = 0;
        for(int i=0; i<temps.size(); i++) {
            if(temps.at((unsigned long) i) == -1000) {
                skipped++;
            } else {
                sum+=temps.at((unsigned long) i);
            }
        }
        ostringstream rtrnStream;
        rtrnStream << getMonthName(this->monthNum) << ": " << " Sum: " << sum << " Size: " << temps.size()
                   << " Average: " << (sum/temps.size()) << " (" << temps.size()-skipped << " of " << temps.size() << ")"
                   << endl;
        return rtrnStream.str();
    }
};

class year {
    public:
        int yearNum;
        vector<month> months;

    year(int yearNum) : yearNum(yearNum) {
        for(int i=1; i<=12; i++) {
            month current = month(i);
            months.push_back(current);
        }
    }

    month& getMonth(int monthNum) {
        return months.at((unsigned long) monthNum-1);
    }

    void display() {
        for(auto mnth : months) {
            cout << mnth.calcAvg();
        }
    }

};


class averager {
    public:
        map<int, year> years;

        void insert(int yearNum, int month, float temp, bool emptyTemp) {
            if(month<=0 || month>12) {
                cout << "Invalid Month given" << endl;
                return;
            }

            //Check if the requested year exists in the map
            if(years.count(yearNum)==0) {
                //Requested year does not exist, must be created
                year yearToAdd = year(yearNum);
                years.insert(pair<int, year>(yearNum, yearToAdd));
            }

            if(emptyTemp) {
                years.at(yearNum).getMonth(month).addTemp(-1000);
            } else {
                years.at(yearNum).getMonth(month).addTemp(temp);
            }
        }

        void getAll() {
            for(auto& iter : years) {
                cout << "========" << iter.first << "========" << endl;
                iter.second.display();
                cout << "================" << endl;
            }
        }
};



#endif //COMP2005_ASSIGNMENT5_6_AVERAGER_H
