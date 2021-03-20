#include <set>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <math.h>
#include <iterator>
#include <chrono>
#include <ctime>

using namespace std;

double calculateDistance(vector<double> currRow, vector<double> tempRow) {

    if (currRow.size() != tempRow.size()) {
        cout << "VECTORS ARE NOT EQUAL" << endl; //test case
    }

    for (unsigned i = 0; i < currRow.size(); ++i) {

        currRow.at(i) -= tempRow.at(i);
        currRow.at(i) *= currRow.at(i);
    }

    double distance = 0;

    for (unsigned i = 0; i < currRow.size(); ++i) {

        distance += currRow.at(i);

    }

    return sqrt(distance);

}

double crossValidation(vector<vector<double> > data, set<int> currentFeatures, int tempFeature, bool isForward) {

    if (isForward) {
        currentFeatures.insert(tempFeature);
    }
    else {
        currentFeatures.erase(tempFeature);
    }

    for (unsigned i = 0; i < data.size(); ++i) {
        for (unsigned k = 1; k < data.at(i).size(); ++k) {
            if (currentFeatures.count(k) == 0) {
                data.at(i).at(k) = 0;
            }
        }
    }


    int numCorrect = 0; // the number correctly classified

    for (unsigned i = 0; i < data.size(); ++i) {
        int currLabel = data.at(i).at(0); //1 or 2
        vector<double> currRow = data.at(i);
        currRow.erase(currRow.begin());

        double nearestNeighborDistance = INT_MAX;
        double nearestNeighborLocation = INT_MAX;
        int nearestNeighborLabel = INT_MAX;

        for (unsigned k = 0; k < data.size(); ++k) {
            if (k != i) {
                int tempLabel = data.at(k).at(0); //label of the temp row
                vector<double> tempRow = data.at(k);
                tempRow.erase(tempRow.begin());

                double distance = calculateDistance(currRow, tempRow);

                if (distance < nearestNeighborDistance) {
                    nearestNeighborDistance = distance;
                    nearestNeighborLocation = k;
                    nearestNeighborLabel = tempLabel;
                }
            }
        }

        if (currLabel == nearestNeighborLabel) {
            numCorrect++;
        }

    }

    double currAccuracy = (numCorrect * 1.0) / data.size();

    return currAccuracy;



}

void forwardSearch(vector<vector<double> > & data) {

    set<int> currentFeatures;
    set<int> bestFeatures;
    double bestAccuracy = 0;

    for (unsigned i = 1; i < data.at(0).size(); ++i) { //iterate through num "columns"
        cout << endl << "On the " << i << "th level of the search tree" << endl;
        int tempFeature = 0; //this feature will be removed or added
        double maxAccuracy = 0;

        for (unsigned k = 1; k < data.at(i).size(); ++k) {
            if (currentFeatures.count(k) == 0) { 
                cout << "--Considering adding the " << k << " feature" << endl;
                double currAccuracy = crossValidation(data, currentFeatures, k, true);

                if (currAccuracy > maxAccuracy) {
                    maxAccuracy = currAccuracy;
                    tempFeature = k;
                }
            }
        }

        currentFeatures.insert(tempFeature);
        cout << endl << "On level " << i << " I added feature " << tempFeature << " to current set" << endl;
        cout << "Accuracy: " << fixed << setprecision(4) << maxAccuracy << endl;
        cout << "------------------------------------------------" << endl;

        if (maxAccuracy > bestAccuracy) {
            bestAccuracy = maxAccuracy;
            bestFeatures = currentFeatures;
        }
    }

    cout << endl << "The best features are: ";


    set<int>::iterator itr;
    for (itr = bestFeatures.begin(); itr != bestFeatures.end(); itr++) { 
        cout << *itr << " ";
    }

    cout << endl;
    cout << "Accuracy: " << fixed << setprecision(4) << bestAccuracy << endl;

}

void backwardSearch(vector<vector<double> > & data) {

    set<int> currentFeatures;

    for (unsigned i = 1; i < data.at(0).size(); i++) {

        currentFeatures.insert(i);

    }

    set<int> bestFeatures;
    double bestAccuracy = 0;

    for (unsigned i = 1; i < data.at(0).size(); ++i) { //iterate through num "columns"
        cout << endl << "On the " << i << "th level of the search tree" << endl;
        int tempFeature = 0; //this feature will be removed or added
        double maxAccuracy = 0;

        for (unsigned k = 1; k < data.at(i).size(); ++k) {
            if (currentFeatures.count(k) == 1) { 
                cout << "--Considering removing the " << k << " feature" << endl;
                double currAccuracy = crossValidation(data, currentFeatures, k, false);

                if (currAccuracy > maxAccuracy) {
                    maxAccuracy = currAccuracy;
                    tempFeature = k;
                }
            }
        }

        currentFeatures.erase(tempFeature);
        cout << endl << "On level " << i << " I erased feature " << tempFeature << " from current set" << endl;
        cout << "Accuracy: " << fixed << setprecision(4) << maxAccuracy << endl;

        // set<int>::iterator itr;

        // cout << "Current Features: ";

        // for (itr = currentFeatures.begin(); itr != currentFeatures.end(); itr++) { 
        //     cout << *itr << " ";
        // }

        // cout << endl;

        cout << "------------------------------------------------" << endl;

        if (maxAccuracy > bestAccuracy) {
            bestAccuracy = maxAccuracy;
            bestFeatures = currentFeatures;
        }
    }

    cout << endl << "The best features are: ";


    set<int>::iterator itr;
    for (itr = bestFeatures.begin(); itr != bestFeatures.end(); itr++) { 
        cout << *itr << " ";
    }

    cout << endl;
    cout << "Accuracy: " << fixed << setprecision(4) << bestAccuracy << endl;

}


int main () {

    cout << "Please enter the filename" << endl;

    string filename;

    getline(cin, filename);

    ifstream infile;
    infile.open(filename);

    if (!infile.is_open()) {
        cout << "The file does not exist" << endl;
        cout << "Goodbye" << endl;

        return 0;
    }

    vector<vector<double> > data;
    string line;
    while (getline(infile, line) ) {
        vector<double> row;
        double num;
        istringstream ss(line);

        while ( ss >> num ) {
            row.push_back(num);
        }

        data.push_back(row);
    }

    int num;

    cout << "Type the number of the algorithm you want to run" << endl;

    cout << "   1) Forward Selection" << endl;
    cout << "   2) Backward Elimination" << endl;

    cin >> num;

    chrono::time_point<chrono::system_clock> start, end; 
  
    start = chrono::system_clock::now(); 

    if (num == 1) {
        forwardSearch(data);
    }
    else if (num == 2) {
        backwardSearch(data);
    }

    else {

        cout << "Try again" << endl;
    }

    end = chrono::system_clock::now(); 
  
    chrono::duration<double> elapsed_seconds = end - start; 
    time_t end_time = chrono::system_clock::to_time_t(end); 
  
    std::cout << "Elapsed time: " << elapsed_seconds.count() << " seconds" << endl;;

    return 0;
}