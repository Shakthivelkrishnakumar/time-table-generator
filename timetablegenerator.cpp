#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct Subjects
{
    char name[50];
    char teacher_name[50];
};

int randomIndex(int no_of_subjects)
{
    int rand_index = rand() % no_of_subjects;

    return rand_index;
}

int getSlotIndex(int no_of_subjects, int class_per_day, int labs_per_subject, int* temp_week_sub, int* temp_week_labs, bool is_lab)
{
    int index = randomIndex(no_of_subjects + 1);  //call for random number generation

    if (index >= no_of_subjects)	//random number validation
    {
        return -1;
    }

    if (is_lab)
    {
        if (temp_week_labs[index] < labs_per_subject)
        {
            temp_week_labs[index]++;
            return index;
        }
    }

    if (temp_week_sub[index] < class_per_day)
    {
        temp_week_sub[index]++;
        return index;
    }

    return -1;
}

void print()
{
    string line;

    fstream timetable;
    timetable.open("timetable.csv", ios::out | ios::in);
    while (getline(timetable, line))
    {
        cout << line << "\n";
    }
}

int main()
{
    int no_of_subjects;
    int class_hour = 1;
    int class_per_day = 4;
    int labs_per_subject = 3;
    string delimiter = ",";

    cout << "---------------------------\n";
    cout << "-------- Timetable --------\n";
    cout << "---------------------------\n\n";

    cout << "Enter the number of subjects: ";
    cin >> no_of_subjects;

    struct Subjects subject[no_of_subjects]; //creating array of structures
    string days[5] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" }; //string array

    for (int i = 0; i < no_of_subjects; i++)  //stacking the subjects into the array of subjects
    {
        cout << "Enter the subject " << i + 1 << " name: ";
        cin >> subject[i].name;

        cout << "Enter the subject '" << subject[i].name << "' teacher name: ";
        cin >> subject[i].teacher_name;
        cout << "\n";
    }

    fstream timetable;  //csv file object creation
    timetable.open("timetable.csv", ios::out | ios::in); //opening csv file

    // Write the header
    timetable << delimiter;  //since csv is being used delimiter=,
    for (int hour = 9; hour < 17; hour += class_hour)  
    {
        timetable << "'" << hour << " - " << hour + 1 << "'" << delimiter;
    }
    timetable << "\n";

    int temp_week_sub[no_of_subjects]; //to store the details of subject and lab period count
    int temp_week_labs[no_of_subjects];

    for (int i = 0; i < no_of_subjects; i++)  //assigning 0 to the arrays
    {
        temp_week_sub[i] = 0;
        temp_week_labs[i] = 0;
    }

    srand(time(NULL)); //the seed for random number generation is begun with the computer's inbuild clk

    for (int j = 0; j < 5; j++)
    {
        timetable << days[j] << delimiter; //printing the days in column

        for (int hour = 8; hour < 16; hour += class_hour)
        {
            // twm association
            if(j == 4 && (hour >= 13 && hour <= 14))
            {
                timetable << "twm/association" << delimiter;
                continue;
            }

            int slot = getSlotIndex(no_of_subjects, class_per_day, labs_per_subject, temp_week_sub, temp_week_labs, false);

            if (slot == -1)
            {
                slot = getSlotIndex(no_of_subjects, class_per_day, labs_per_subject, temp_week_sub,
                    temp_week_labs, true);

                if (slot != -1)
                {
                    timetable << subject[slot].name << " LAB / Tutorial(" << subject[slot].teacher_name << ")"
                              << delimiter;
                }
                else
                {
                    timetable << "Library" << delimiter;
                }
            }
            else
            {
                timetable << subject[slot].name << " (" << subject[slot].teacher_name << ")"
                          << delimiter;
            }
        }

        timetable << "\n";
    }

    timetable.close();

    print();

    return 0;
}
