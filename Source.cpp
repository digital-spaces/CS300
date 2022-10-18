#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Course object class
class Course {
private:
    string ID;
    string name;
    vector<string> prerequisites;

public:
    Course();
    Course(string courseID, string courseName, vector<string> coursePrerequisites);
    virtual ~Course();
    string getID();
    string getName();
    vector<string> getPrerequisites();

    void setID(string courseID);
    void setName(string courseName);
    void setPrerequisites(vector<string> coursePrerequisites);
    void addPrerequisite(string coursePrerequisite);
};

// Course constructors & deconstructor
Course::Course() {
    ID = "";
    name = "";
    prerequisites.resize(0);
}

Course::Course(string courseID, string courseName, vector<string> coursePrerequisites) {
    ID = courseID;
    name = courseName;
    prerequisites = coursePrerequisites;
}

Course::~Course() {
    ID = "";
    name = "";
    prerequisites.resize(0);
}

// Course methods
string Course::getID() {
    return ID;
}

string Course::getName() {
    return name;
}

vector<string> Course::getPrerequisites() {
    return prerequisites;
}

void Course::setID(string courseID) {
    ID = courseID;
}

void Course::setName(string courseName) {
    name = courseName;
}

void Course::setPrerequisites(vector<string> coursePrerequisites) {
    prerequisites = coursePrerequisites;
}

void Course::addPrerequisite(string coursePrerequisite) {
    prerequisites.push_back(coursePrerequisite);
}

// Reads courses from a text file and returns a vector of Course objects
vector<Course> readCoursesFile(string file) {
    vector<Course> tempCourses;
    fstream fileStream; // Declare a new file stream
    fileStream.open(file, ios::in); // Open file in read mode

    // If file is open, read it
    if (fileStream.is_open()) {
        string line;
        vector<string> courseIDs;
        int comma;

        // Add all course IDs to vector for later validation
        while (getline(fileStream, line)) {
            comma = line.find(",");
            courseIDs.push_back(line.substr(0, comma));
        }

        // Reopen file to reset line pointer
        fileStream.close();
        fileStream.open(file, ios::in); 

        // Create all the Course objects and add them to a vector
        while (getline(fileStream, line)) {
            vector<string> courseInfo;

            stringstream lineStream(line);
            string temp;
            while (lineStream.good()) {
                getline(lineStream, temp, ',');
                courseInfo.push_back(temp);
                //cout << temp << endl;
            }

            // Verify that the line is a course
            if (courseInfo.size() >= 2) {
                Course tempCourse;
                tempCourse.setID(courseInfo[0]);
                //cout << courseInfo[0] << endl;
                tempCourse.setName(courseInfo[1]);
                //cout << courseInfo[1] << endl;
                
                // Get any prerequisites
                for (size_t i = 2; i < courseInfo.size(); i++) {
                    vector<string>::iterator it;
                    it = find(courseIDs.begin(), courseIDs.end(), courseInfo[i]);

                    if (it != courseIDs.end()) {
                        tempCourse.addPrerequisite(courseInfo[i]);
                        //cout << courseInfo[i] << ",";
                    }
                }
                //cout << endl;

                tempCourses.push_back(tempCourse); // Add course to vector
            }
        }
    }
    else {
        cout << file << " was not found." << endl;
    }

    // Close file when no longer in use.
    fileStream.close();

    return tempCourses;
}

// Compares courses for alphabetical sorting
bool compareCourse(Course a, Course b) {
    return a.getID() < b.getID();
}

// Sorts courses alphabetically
vector<Course> sortCourses(vector<Course> courses) {
    sort(courses.begin(), courses.end(), compareCourse);

    return courses;
}

// Prints courses alphabetically
void printAllCourses(vector<Course> courses) {
    vector<Course> sorted = sortCourses(courses);

    cout << "Here is a sample schedule:" << endl << endl;

    for (Course i : sorted) {
        cout << i.getID() << ", " << i.getName() << endl;
    }
}

// Print single course
void printCourseInformation(vector<Course> courses, string courseNumber) {
    for (Course i : courses) {
        if (i.getID() == courseNumber) {
            // Print course basic info
            cout << i.getID() << ", " << i.getName() << endl;

            // Print course prerequisites
            if (i.getPrerequisites().size() >= 1) {
                cout << "Prerequisites: ";

                for (size_t j = 0; j < i.getPrerequisites().size(); j++) {
                    cout << i.getPrerequisites()[j];

                    if (i.getPrerequisites().size() - 1 > j) {
                        cout << ", ";
                    }
                }

                cout << endl;
            }

            return;
        }
    }

    cout << "Course " << courseNumber << " not found." << endl;
}

void menu(string file) {
    // Declare variables
    vector<Course> courses;
    string courseNumber;
    int choice = 0;

    while (choice != 9) {
        system("cls"); // Clear screeen

        cout << "Welcome to the course planner." << endl << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        cout << endl; // Adding some whitespace

        switch (choice) {
            // Read courses file and load into the data structure
            case 1:
                courses = readCoursesFile(file);
                break;

            // Print an alphabetical list of courses
            case 2:
                printAllCourses(courses);
                break;

            // Print a specific course's information
            case 3:
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                cout << endl;
                printCourseInformation(courses, courseNumber);

            // Quit application
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;

            // Invalid command
            default:
                cout << choice << " is not a valid option." << endl;
                break;
        }

        cout << endl; // Adding some whitespace
        system("pause"); // Pause the screen before clearing
    }
}

int main() {
    string file = "ABCU_Advising_Program_Input.txt";

    menu(file);
}