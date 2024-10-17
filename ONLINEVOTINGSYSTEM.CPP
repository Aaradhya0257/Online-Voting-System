#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <conio.h>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <vector>
using namespace std;

int StateCount = 0;
int CityCount = 0;
int StateIdNo = 0;
int CityIdNo = 0;
int partyId = 0;
int candidateId = 0;
int VoterId = 0;
int resultId = 0;
// Count functions to count the digits and character in a string
int a[10005],seg[4*100005];
int CountDigits(int number)
{
    if (number == 0)
        return 1;
    int count = 0;
    while (number > 0)
    {
        count++;
        number /= 10;
    }
    return count;
}

int countChar(const char *str)
{
    int count = 0;
    while (*str != '\0')
    {
        count++;
        str++;
    }
    return count;
}

int findStateIdByPartyId(int partyId, const vector<Party>& parties, const vector<State>& states) {
    for (const auto& party : parties) {
        if (party.getPartyId() == partyId) {
            // Assuming the party ID relates directly to state ID
            return partyId; // Modify this as per your actual logic
        }
    }
    return -1; // Return -1 if no matching state is found
}

bool getStateCount()
{
    string fileName = "stateCount.txt";
    ifstream finStateCount(fileName);

    if (!finStateCount)
    {
        cout << "\n   File " << fileName << " does not exist.\n";
        return false;
    }

    if (!(finStateCount >> StateCount))
    {
        cout << "\n   Error reading state count from file.\n";
        return false;
    }

    finStateCount.close();
    return true;
}

bool getCityCount()
{
    string fileName = "cityCount.txt";
    ifstream finCityCount(fileName);
    if (!finCityCount)
    {
        cout << "\n   File " << fileName << " does not exist.\n";
        getch();
        return false;
    }

    if (!(finCityCount >> CityCount))
    {
        cout << "\n   Error reading city count from file.\n";
        getch();
        return false;
    }

    finCityCount.close();
    return true;
}

bool getIdFromFile(string &fileName, int &id)
{
    ifstream fin(fileName);
    if (!fin)
    {
        cout << "\n   File " << fileName << " does not exist.\n";
        getch();
        return false;
    }

    if (!(fin >> id))
    {
        cout << "\n   Error reading ID from file " << fileName << ".\n";
        getch();
        return false;
    }

    fin.close();
    return true;
}

bool setIdToFile(string &fileName, int id)
{
    ofstream fout(fileName);
    if (!fout)
    {
        cout << "\n   Error opening file " << fileName << " for writing.\n";
        getch();
        return false;
    }

    fout << id;
    if (!fout)
    {
        cout << "\n   Error writing to file " << fileName << ".\n";
        getch();
        return false;
    }

    fout.close();
    return true;
}

void funFrontPage()
{
    cout << "\n\n   Hi, Welcome To My Project: Online Voting System";
    getch();
    system("cls");
    cout << "\t\t\t\t  ALGORITHMS AND PROBLEMS SOLVING USING OOPS CONCEPT";
    cout << "\n\n\t\t\t\t\tObjective: To create a secure and user-friendly online voting platform that ensures the integrity and privacy of the voting process.";
    cout << "\n\n\n\t\t\t\t\tDeveloped By: Aaradhya Joshi";

    getch();
}

void funHeading(string Heading)
{
    system("cls");
    cout << "\n";
    cout << string(40, '=') << endl;
    cout << "\t\t\t\t\t Online Voting System \n\n\n";
    transform(Heading.begin(), Heading.end(), Heading.begin(), ::toupper);
    cout << "   " << Heading << "\n";

    cout << string(40, '=') << endl;
    cout << "\n";
}

class  State
{
private:
    int stateId;
    int seats;
    string stateName;

public:
    State() : stateId(0), seats(0), stateName("NoName") {}

    explicit State(const string &sName) : stateName(sName)
    {
        StateCount++;
        stateId = StateCount;
        seats = 0;
    }

    void getStateInfo()
    {
        cout << "Enter the Name: ";
        getline(cin, stateName);
        stateId = ++StateCount; // Increment StateCount and assign to stateId
        seats = 0;
    }

    void showStateInfo() const
    {
        cout << stateName << endl;
    }

    int getSeats() const { return seats; }
    void setSeats(int x) { seats = x; }

    int getStateId() const { return stateId; }
    void setStateId(int id) { stateId = id; }

    string getStateName() const { return stateName; }

    void setStateName(const string &sName) { stateName = sName; }

    int storeState();
    int getStates(int index);
};

// Read state from the binary file
int State::getStates(int index)
{
    ifstream finStateRecord("States.txt", ios::in | ios::binary);
    if (!finStateRecord)
    {
        cout << "\nFile not Found";
        return 0;
    }
    else
    {
        finStateRecord.seekg(index * sizeof(State), ios::beg);
        finStateRecord.read(reinterpret_cast<char *>(this), sizeof(State));
    }
    return 1; // Return success
}

// Store state into the binary file
int State::storeState()
{
    ofstream foutState("States.txt", ios::app | ios::binary);
    if (foutState)
    {
        foutState.write(reinterpret_cast<const char *>(this), sizeof(State));
        return 1; // Return success
    }
    return 0; // Return failure if file opening fails
}

// Get multiple state input and add to vector
void funStateInput(vector<State> &states, int x)
{
    int y = x + StateCount; // Calculate the new total states
    State obj;
    for (int i = StateCount; i < y; i++)
    {
        if (x > 1)
        {
            cout << "\nEnter State " << (i + 1 - StateCount) << "\n";
        }
        else
        {
            cout << "\n";
        }
        obj.getStateInfo();
        states.push_back(obj); // Add new state to the vector
    }
}

// Insert new states and update file
void insertNewState()
{
    int x;
    cout << "\nEnter the No. of States you want to Insert: ";
    cin >> x;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover input
    if (x <= 0)
        return;

    vector<State> states;
    states.resize(StateCount); // Resize vector to hold current states
    for (int i = 0; i < StateCount; i++)
    {
        states[i].getStates(i);
    }

    funStateInput(states, x);
    StateCount += x; // Update state count

    // Store all states back to the file
    ofstream foutState("States.txt", ios::binary);
    for (auto &state : states)
    {
        state.storeState(); // Call the instance method
    }

    cout << "\n\nRecord Inserted\n\nPress any Key To Update The Record";
    cin.get();
}

// Input for delete choices
int inputDeleteChoices()
{
    int x;
    cout << "1. Delete Via Serial Number (*Convenient)\n";
    cout << "2. Delete Via State Name\n";
    cout << "3. Go Back\n";
    cout << "\nEnter your Choice: ";
    cin >> x;
    return x;
}

// Delete state record
void deleteState()
{
    if (StateCount == 0)
    {
        cout << "\nNo Record Exists";
        cin.get();
        return;
    }

    vector<State> states(StateCount);
    for (int i = 0; i < StateCount; i++)
    {
        states[i].getStates(i);
    }

    while (true)
    {
        cout << "\nDeleting A State Record:\n";
        cout << "\n   S.No.  State Name\n\n";
        for (int i = 0; i < StateCount; i++)
        {
            cout << "   " << i + 1 << ". " << states[i].getStateName() << endl;
        }

        int ch = inputDeleteChoices();
        if (ch == 1)
        {
            int sr;
            cout << "\nEnter the S.No.: ";
            cin >> sr;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover input

            if (sr <= 0 || sr > StateCount)
            {
                cout << "\nInvalid No!";
                continue;
            }

            char cnfm;
            cout << "\nAre you Sure to Delete Record " << states[sr - 1].getStateName() << " (y/n): ";
            cin >> cnfm;

            if (cnfm == 'y' || cnfm == 'Y')
            {
                states.erase(states.begin() + (sr - 1)); // Remove state
                StateCount--;
                cout << "\nRecord Deleted\nPress any Key to Update Record";
                cin.get();
            }
        }
        else if (ch == 2)
        {
            string sName;
            cout << "\nEnter the State Name: ";
            cin.ignore(); // Clear the newline character
            getline(cin, sName);

            auto it = find_if(states.begin(), states.end(), [&sName](const State &state)
                              {
                                  return state.getStateName() == sName; // Compare names
                              });

            if (it != states.end())
            {
                char cnfm;
                cout << "\nAre you Sure to Delete Record " << it->getStateName() << " (y/n): ";
                cin >> cnfm;

                if (cnfm == 'y' || cnfm == 'Y')
                {
                    states.erase(it); // Remove state
                    StateCount--;
                    cout << "\nRecord Deleted\nPress any Key to Update Record";
                    cin.get();
                }
            }
            else
            {
                cout << "\nState Not Found!\n";
                cin.get();
            }
        }
        else if (ch == 3)
        {
            return;
        }
        else
        {
            cout << "\nInvalid choice! Please try again.\n";
        }
    }

    // Store updated states back to the file
    ofstream foutState("States.txt", ios::binary);
    for (auto &state : states)
    {
        state.storeState(); // Call the instance method
    }
}

void choiceAdmin()
{
    while (true)
    {
        funHeading("ADMIN CHOICES");
        cout << "1. Insert New State\n";
        cout << "2. Delete State\n";
        cout << "3. Show State\n";
        cout << "4. Exit\n";
        cout << "\nEnter your Choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            insertNewState();
            break;
        case 2:
            deleteState();
            break;
        case 3:
            // Implement show state functionality here
            cout << "Functionality to show states is not implemented yet.\n";
            cin.get();
            break;
        case 4:
            return;
        default:
            cout << "\nInvalid choice. Please try again.\n";
        }
    }
}
class City : public State
{
private:
    int cityId;
    int leadCandidateId;
    string cityName;

public:
    City() : State(), cityId(0), leadCandidateId(0), cityName("NoName") {}

    int getCityInfo(int sId, const string &sName)
    {
        setStateId(sId);
        setStateName(sName);
        cout << "Enter the Name: ";
        getline(cin, cityName);
        int s = 1;
        setSeats(s);
        cityId = ++CityIdNo;
        leadCandidateId = 0;
        return s;
    }

    void showCityInfo() const
    {
        cout << "City ID: " << cityId << ", City Name: " << cityName << ", Lead Candidate ID: " << leadCandidateId << endl;
    }

    int getCityId() const { return cityId; }
    const string &getCityName() const { return cityName; }
    int getLeadCandidateId() const { return leadCandidateId; }

    void setCityId(int n) { cityId = n; }
    void setCityName(const string &name) { cityName = name; }
    void setLeadCandidateId(int n) { leadCandidateId = n; }

    int storeCity();
    int getCity(int x);
};

int City::getCity(int x)
{
    ifstream finCityRecord("Cities.txt", ios::in | ios::binary);
    if (!finCityRecord)
    {
        cout << "\n   File not Found";
        return -1;
    }

    finCityRecord.seekg(x * sizeof(City), ios::beg);
    finCityRecord.read(reinterpret_cast<char *>(this), sizeof(City));
    if (finCityRecord)
    {
        return 1;
    }
    else
    {
        cout << "\n   Error reading city record.";
        return -1;
    }
}

int City::storeCity()
{
    ofstream foutCity("Cities.txt", ios::app | ios::binary);
    if (!foutCity)
    {
        cout << "\n   Error opening file for writing.";
        return -1;
    }
    foutCity.write(reinterpret_cast<const char *>(this), sizeof(City));
    return 1;
}

int funCityInput(int stateId, const string &stateName, vector<City> &cities, int x)
{
    int y = x + CityCount;
    City obj;
    int stateSeats = 0;

    for (int i = CityCount; i < y; ++i)
    {
        cout << "\n   Enter City " << (i + 1) << "\n";
        stateSeats += obj.getCityInfo(stateId, stateName);

        cities.push_back(obj);
    }

    return stateSeats;
}

void insertNewCity()
{
    int x;
    getCityCount();
    getStateCount();

    vector<State> bs1(StateCount);
    funHeading("Inserting City's Record: ");
    cout << endl
         << "   S.No.  StateName\n\n";

    for (int i = 0; i < StateCount; ++i)
    {
        bs1[i].getStates(i);
        cout << "   " << (i + 1) << ".";
        // Display logic here
        bs1[i].showStateInfo();
    }

    int st;
    cout << "\n   Select the State via serialNo. : ";
    cin >> st;
    cin.ignore();
    cout << "\n   Enter the No. of Cities you want to Insert: ";
    cin >> x;
    cin.ignore();

    if (x == 0)
        return;

    vector<City> cities(CityCount + x);
    int seatsCount = funCityInput(bs1[st - 1].getStateId(), bs1[st - 1].getStateName(), cities, x);
    seatsCount += bs1[st - 1].getSeats();
    bs1[st - 1].setSeats(seatsCount);

    // Logic to store states and cities, and handle file removal.
    cout << "\n\n   Record Inserted\n\n   Press any Key To Update The Record";
    getch();
}
void deleteCity()
{
    funHeading("Deleting A City Record:");
    cout << "   Available Soon";
    cin.get(); // Replaced getch() with cin.get() for better portability
}

void updateCity()
{
    funHeading("Updating A City Record:");
    cout << "   Available Soon";
    cin.get(); // Replaced getch() with cin.get() for better portability
}

void viewCityRecord()
{
    getCityCount();
    if (CityCount == 0)
    {
        cout << "\n   No Record Found";
        cin.get(); // Replaced getch() with cin.get()
        return;
    }

    vector<City> cities(CityCount); // Use std::vector for dynamic sizing
    cout << endl
         << "   S.No.  StateName    CityName    Seats\n\n";

    for (int i = 0; i < CityCount; ++i)
    {
        cities[i].getCity(i); // Assuming this retrieves city data
        cout << "   " << (i + 1) << ".";
        int j = CountDigits(i + 1);

        // Handle spacing
        for (int k = j; k < 8; k++)
            cout << " ";
        cout << cities[i].getStateName(); // Assuming this returns the state name

        j = countChar(cities[i].getStateName().c_str());
        for (int k = j; k < 12; k++)
            cout << " ";

        cout << cities[i].getCityName(); // Assuming this returns the city name

        j = countChar(cities[i].getCityName().c_str());
        for (int k = j; k < 14; k++)
            cout << " ";

        cout << cities[i].getSeats() << endl; // Assuming this returns the number of seats
    }

    cin.get(); // Replaced getch() with cin.get()
}
class Password
{
private:
    string password; // Using std::string instead of char array

public:
    void setPassword(const string &pwd)
    {
        password = pwd;
    }

    string getPassword() const
    {
        return password;
    }
};
class Party: public Password
{
private:
    int PartyID, votes, Members, seats;
    string partyName, headName;
    static int partyId; // Static variable to ensure unique IDs

public:
    Party()
    {
        PartyID = 0;
        votes = 0;
        Members = 0;
        seats = 0;
        partyName = "NoName";
        headName = "NoName";
        setPassword("1234"); // Default password
    }

    void getPartyInfo()
    {
        PartyID = ++partyId; // Increment and assign a unique PartyID
        votes = 0;
        seats = 0;
        Members = 1;

        cin.ignore(); // Clear input buffer

        cout << "Enter the Party Name: ";
        getline(cin, partyName); // Input party name

        cout << "Enter the Head Name: ";
        getline(cin, headName); // Input head name

        string arr;
        char cr;
        cout << "Enter the Password: ";

        // Mask password input using standard cin (without getch)
        while (true)
        {
            cr = cin.get(); // Get each character
            if (cr == '\n')
            {
                setPassword(arr); // Store the password
                break;
            }
            else
            {
                cout << "*"; // Print '*' for each character typed
                arr += cr;   // Append character to string
            }
        }
    }

    // Getters for party details
    string getPartyName() const { return partyName; }
    string getHeadName() const { return headName; }
    int getPartyId() const { return PartyID; }
    int getVotes() const { return votes; }
    int getMembers() const { return Members; }
    int getSeats() const { return seats; }

    // Setters for party details
    void setPartyName(const string &newName) { partyName = newName; }
    void setHeadName(const string &newName) { headName = newName; }
    void setVotes(int n) { votes = n; }
    void setPartyId(int n) { PartyID = n; }
    void setMembers(int n) { Members = n; }
    void setSeats(int n) { seats = n; }

    // Methods to store and retrieve party details (placeholders, define based on your requirements)
    int storeParty()
    {
        // Placeholder: Define logic to store party info, maybe in a file
        cout << "Storing party info for " << partyName << endl;
        return 1; // Success
    }

    int getParty(int x)
    {
        // Placeholder: Define logic to retrieve party info based on PartyID or another identifier
        cout << "Getting party info for PartyID " << x << endl;
        return 1; // Success
    }
};

// Initialize static variable
int Party::partyId = 0;
class Candidate
{
private:
    int candidateID;
    char candidateName[25];
    char stateName[25];
    char cityName[25];
    int votes;
    int seats;
    char password[10];
    int partyId;

public:
    // Getters
    int getCandidateID() { return candidateID; }
    char *getCandidateName() { return candidateName; }
    char *getstateName2() { return stateName; }
    char *getcityName2() { return cityName; }
    int getVotes() { return votes; }
    int getSeats() { return seats; }
    int getPartyId() { return partyId; }
    char *getPassword2() { return password; }

    // Setters
    void setCandidateID(int id) { candidateID = id; }
    void setCandidateName(const char *name) { strcpy(candidateName, name); }
    void setstateName(const char *state) { strcpy(stateName, state); }
    void setcityName(const char *city) { strcpy(cityName, city); }
    void setVotes(int v) { votes = v; }
    void setSeats(int s) { seats = s; }
    void setPartyId(int id) { partyId = id; }
    void setPassword(const char *pass) { strcpy(password, pass); }

    // Member functions
    void getCandidateInfo()
    {
        cout << "Enter Candidate ID: ";
        cin >> candidateID;
        cin.ignore();
        cout << "Enter Candidate Name: ";
        cin.getline(candidateName, 25);
        cout << "Enter State: ";
        cin.getline(stateName, 25);
        cout << "Enter City: ";
        cin.getline(cityName, 25);
        cout << "Enter Votes: ";
        cin >> votes;
        cout << "Enter Seats: ";
        cin >> seats;
        cout << "Enter Password: ";
        cin >> password;
        cout << "Enter Party ID: ";
        cin >> partyId;
    }

    int getCandidate(int x);
    int storeCandidate();
};

int Candidate::getCandidate(int x)
{
    int i = 0;
    ifstream finCandidateRecord("candidates.txt", ios::in | ios::binary);
    if (!finCandidateRecord)
    {
        cout << "\nFile not Found";
    }
    else
    {
        while (!finCandidateRecord.eof() && i != x + 1)
        {
            finCandidateRecord.read((char *)this, sizeof(*this));
            i++;
        }
    }
    return i; // Returning the count of candidates read
}

int Candidate::storeCandidate()
{
    ofstream foutCandidate("candidates.txt", ios::app | ios::binary);
    foutCandidate.write((char *)this, sizeof(*this));
    foutCandidate.close();
    return 0;
}

int funCandidateInput(Candidate *bs1, int x)
{
    Candidate obj;
    cout << "   ";
    obj.getCandidateInfo();
    bs1[x - 1] = obj; // Store candidate at the correct index
    return obj.getCandidateID();
}

int insertNewCandidate()
{
    int x = 1, Id = 0;
    // Replace with actual implementation to get the number of candidates
    // getCandidateId();

    if (x == 0)
    {
        return Id;
    }
    Candidate *bs1 = new Candidate[candidateId + x]; // Dynamic allocation

    // Sample heading function
    cout << "Creating Candidate Account: \n";
    for (int i = 0; i < candidateId; i++)
    {
        bs1[i].getCandidate(i);
    }
    Id = funCandidateInput(bs1, x);
    // Sample function to save count, implement as needed
    // setCandidateId();

    remove("candidates.txt");
    for (int i = 0; i < candidateId; i++)
    {
        bs1[i].storeCandidate();
    }
    cout << "\n\nRecord Inserted\n\nPress any Key To Insert The Record";
    cin.ignore(); // Use cin.ignore() instead of getch() for better compatibility
    delete[] bs1; // Free allocated memory
    return Id;
}

int CandidateLogin()
{
    // Assume you already have a method to get the candidate ID count
    if (candidateId == 0)
    {
        cout << "\nNo Record Found";
        return 0;
    }

    Candidate *bs1 = new Candidate[candidateId];
    for (int i = 0; i < candidateId; i++)
    {
        bs1[i].getCandidate(i);
    }

    // Sample login interface
    cout << "Candidate Login Page: ";
    int Id;
    cout << "\nEnter Your Id: ";
    cin >> Id; // Get candidate ID
    bool found = false;
    int index = Id - 1;

    // Check if candidate ID exists
    if (Id == bs1[index].getCandidateID())
    {
        found = true;
    }

    if (!found)
    {
        cout << "\nNo Account exists with this Id!";
        delete[] bs1; // Free allocated memory
        return 0;
    }

    char paswrd[10], cr;
    string cmpPaswrd = bs1[index].getPassword2();
    cout << "\nEnter Your Password: ";
    for (int i = 0; i < 10; i++)
    {
        cr = getch();
        if (cr != '\r')
        {
            paswrd[i] = cr;
            cout << "*";
        }
        else
        {
            paswrd[i] = '\0';
            break;
        }
    }
    int match = (strcmp(paswrd, cmpPaswrd.c_str()) == 0); // Compare passwords
    if (match)
    {
        delete[] bs1; // Free allocated memory
        return bs1[index].getCandidateID();
    }
    else
    {
        cout << "\nWrong Password!";
    }

    delete[] bs1; // Free allocated memory
    return 0;
}
class Interval
{
public:
    int low;
    int high;
    int PID;

    Interval() : low(0), high(0), PID(0) {}

    Interval(int l, int h, int p) : low(l), high(h), PID(p) {}

    void getIntervalInfo()
    {
        cout << "\n   Enter Low : ";
        cin >> low;
        cout << "\n   Enter High : ";
        cin >> high;

        // Simulating fetching party data (you need to define how you fetch partyId)
        int partyId = 5;    // Dummy value
        Party ps1[partyId]; // Assuming Party array size as partyId for now

        cout << endl
             << "   ID  Party\n\n";
        for (int i = 0; i < partyId; i++)
        {
            ps1[i].getParty(i);
            cout << "   " << ps1[i].getPartyId() << ".";
            int j = CountDigits(i + 1);
            for (int k = j; k < 8; k++)
                cout << " ";
            cout << ps1[i].getPartyName() << endl;
        }

        cout << "\n   Choose The Party (ID) : ";
        cin >> PID;
    }

    // Getter and Setter methods
    int getLow() { return low; }
    int getHigh() { return high; }
    int getpId() { return PID; }

    void setLow(int l) { low = l; }
    void setHigh(int h) { high = h; }
    void setpId(int p) { PID = p; }

    int storeInterval();
    int getInterval(int x);
};

// Read the interval data from the file based on xth record
int Interval::getInterval(int x)
{
    int i = 0;
    ifstream finIntervalRecord;
    finIntervalRecord.open("Record.txt", ios::in | ios::binary);
    if (!finIntervalRecord)
    {
        cout << "\n   File not Found";
        return -1;
    }
    while (!finIntervalRecord.eof() && i != x + 1)
    {
        finIntervalRecord.read((char *)this, sizeof(*this));
        i++;
    }
    return 0;
}

// Store interval data to the file
int Interval::storeInterval()
{
    ofstream foutInterval;
    foutInterval.open("Record.txt", ios::app | ios::binary);
    foutInterval.write((char *)this, sizeof(*this));
    foutInterval.close();
    return 0;
}

// Interval Tree Node Structure
struct ITNode
{
    Interval *i;
    int max;
    ITNode *left, *right;
};

// Create a new node with the given interval
ITNode *newNode(Interval i)
{
    ITNode *temp = new ITNode;
    temp->i = new Interval(i);
    temp->max = i.getHigh();
    temp->left = temp->right = NULL;
    return temp;
}

// Insert a new interval into the Interval Tree
ITNode *insert(ITNode *root, Interval i)
{
    if (root == NULL)
        return newNode(i);

    int l = root->i->getLow();

    if (i.getLow() < l)
        root->left = insert(root->left, i);
    else
        root->right = insert(root->right, i);

    if (root->max < i.getHigh())
        root->max = i.getHigh();

    return root;
}

// Check if two intervals overlap
bool doOverlap(Interval i1, Interval i2)
{
    return (i1.getLow() < i2.getHigh() && i2.getLow() < i1.getHigh());
}

// Function to display overlapping intervals (sorted)
void displayResultRecord(ITNode *root)
{
    // Assuming getPartyIdNo() is implemented elsewhere and partyId is available
    int partyId = 5;    // Dummy value
    Party bs1[partyId]; // Assuming an array of parties

    for (int i = 0; i < partyId; i++)
    {
        bs1[i].getParty(i); // Assuming this method initializes or sets party data
    }

    int RpId = root->i->PID; // Assuming RpId corresponds to party ID in array
    int c = 0;
    cout << "   " << root->i->low << " - " << root->i->high; // Printing range

    // Formatting and output spacing
    for (int i = 0; i < 7; i++)
        cout << " ";

    // Count digits in Party ID and print it
    c = CountDigits(bs1[RpId - 1].getPartyId());
    cout << bs1[RpId - 1].getPartyId();
    for (int i = c; i < 7; i++)
        cout << " ";

    // Count characters in Party Name and print it (convert to const char* if needed)
    c = countChar(bs1[RpId - 1].getPartyName().c_str());
    cout << bs1[RpId - 1].getPartyName();
    for (int i = c; i < 30; i++)
        cout << " ";

    // Count characters in Head Name and print it (convert to const char* if needed)
    c = countChar(bs1[RpId - 1].getHeadName().c_str());
    cout << bs1[RpId - 1].getHeadName() << "\n";
}
// Overlap search function for intervals
void overlapSearchAllSorted(ITNode *root, Interval i)
{
    if (root == NULL)
        return;

    if (root->left != NULL && root->left->max >= i.low)
        overlapSearchAllSorted(root->left, i);

    if (doOverlap(*(root->i), i))
        displayResultRecord(root);

    overlapSearchAllSorted(root->right, i);
}

// In-order traversal for the Interval Tree
void inorder(ITNode *root)
{
    if (root == NULL)
        return;

    inorder(root->left);
    cout << "[" << root->i->getLow() << ", " << root->i->getHigh() << "]"
         << " max = " << root->max << " PartyId = " << root->i->getpId() << endl;
    inorder(root->right);
}
void displayHeading(const string &heading)
{
    cout << "\n\n   " << heading << "\n";
    cout << "--------------------------------------------\n";
}

// Function to display party results
void displayPartyResults(const vector<Party> &p1)
{
    cout << "\n   PartyName                     HeadName               Votes     Seats\n\n";

    for (const auto &party : p1)
    {
        // Get lengths using std::string::length() method
        int c = party.getPartyName().length();
        cout << "   " << party.getPartyName();

        // Adjust spacing for party name
        for (int k = c; k < 30; k++)
            cout << " ";

        c = party.getHeadName().length();
        cout << party.getHeadName();

        // Adjust spacing for head name
        for (int k = c; k < 25; k++)
            cout << " ";

        c = CountDigits(party.getVotes());
        cout << party.getVotes();

        // Adjust spacing for votes
        for (int k = c; k < 10; k++)
            cout << " ";

        cout << party.getSeats() << "\n";
    }
}
// Dummy function for party count input
void getPartyIdNo()
{
    cout << "\nEnter the number of parties: ";
}

// Dummy function for candidate count input
void getCandidateId()
{
    cout << "\nEnter the number of candidates: ";
}

// Dummy function for state count input
void getStateCount2()
{
    cout << "\nEnter the number of states: ";
}

// Function to show the overall election result
void showResult1()
{
    int partyId;
    getPartyIdNo();
    cin >> partyId; // Input partyId

    vector<Party> p1(partyId);
    int candidateId;
    getCandidateId();
    cin >> candidateId; // Input candidateId

    vector<Candidate> c1(candidateId);

    for (int j = 0; j < partyId; j++)
    {
        p1[j].getParty(j);
        p1[j].setSeats(0);
        p1[j].setVotes(0);
    }

    for (int i = 0; i < candidateId; i++)
    {
        c1[i].getCandidate(i);
        int pId = c1[i].getPartyId();
        p1[pId - 1].setSeats(p1[pId - 1].getSeats() + c1[i].getVotes());
        p1[pId - 1].setVotes(p1[pId - 1].getVotes() + c1[i].getVotes());
    }

    displayPartyResults(p1);
}

// Function to show the result for a specific state
void showResult2()
{
    int stateCount;
    getStateCount();
    cin >> stateCount;
    string stateName;

    vector<State> bs1(stateCount);
cout << endl << "   S.No.  State\n\n";
for (int i = 0; i < stateCount; i++)
{
    bs1[i].getStates(i);
    cout << "   " << i + 1 << ".";
    int j = CountDigits(i + 1);
    for (int k = j; k < 8; k++)
        cout << " ";
    bs1[i].showStateInfo();
}

int sr = 0;
do
{
    cout << "\n   Choose Your State (S.No) : ";
    cin >> sr;
    if (sr <= 0 || sr > stateCount)
    {
        cout << "\n   Invalid Choice! Please choose a valid state.\n";
    }
} while (sr <= 0 || sr > stateCount);

cout << "\n\n   Selected State: " << bs1[sr - 1].getStateName() << " (" << bs1[sr - 1].getSeats() << ")\n\n";

    int partyId;
    getPartyIdNo();
    cin >> partyId;

    vector<Party> p1(partyId);
    int candidateId;
    getCandidateId();
    cin >> candidateId;

vector<Candidate> c1(candidateId);

for (int j = 0; j < partyId; j++)
{
    p1[j].getParty(j);
    p1[j].setSeats(0);
    p1[j].setVotes(0);
}

for (int i = 0; i < candidateId; i++)
{
    c1[i].getCandidate(i);

    int pId = c1[i].getPartyId();
    int candidateStateId = findStateIdByPartyId(pId, p1, bs1); // Now passing p1 and bs1 vectors

    if (candidateStateId == bs1[sr - 1].getStateId())
    {
        p1[pId - 1].setSeats(p1[pId - 1].getSeats() + c1[i].getVotes());
        p1[pId - 1].setVotes(p1[pId - 1].getVotes() + c1[i].getVotes());
    }
}

displayPartyResults(p1);


// Main function
int main(){
   //setCityLeadCandidate();
   funFrontPage();
   int ch,Id=0;

   choice:;
   ch = choiceAdmin(0);
   switch (ch-1){
   case 0:
      Id = VoterLogin();
      if(Id!=0){viewVoterInfo(Id);}
      goto choice;
   case 1:
      Id = CandidateLogin();
      if(Id!=0){viewCandidateInfo(Id);}
      goto choice;
   case 2:
      Id = partyLogin();
      if(Id!=0){viewPartyInfo(Id);}
      goto choice;
   case 3:
      goto choice3;
   case 4:
      showResults();
      goto choice;
   case 5:
      goto choice5;
   case 6:
      goto Exit;
   default:
      cout<<"\n   Enter the Correct Choice!";
      getch();
      goto choice;
   }

   choice3:;
   ch = choiceAdmin(5);
   switch (ch){
   case 1:
      Id = insertNewVoter();
      if(Id!=0){viewVoterInfo(Id);}
      goto choice;
   case 2:
      Id = insertNewCandidate();
      if(Id!=0){viewCandidateInfo(Id);}
      goto choice;
   case 3:
      Id = insertNewParty();
      if(Id!=0){viewPartyInfo(Id);}
      goto choice;
   case 0:
      goto choice;
   default:
      cout<<"\n   Enter the Correct Choice!";
      getch();
      goto choice;
   }

   //choice4:;
   //goto Exit;

   choice5:;
   ch = choiceAdmin(2);
   switch (ch){
      case 1:
         goto choice5a;
      case 2:
         goto choice5b;
      case 3:
         goto choice5c;
      case 4:
         setResultRecord();
         goto choice5;
      case 0:
         goto choice;
      default:
         cout<<"\n   Enter the Correct Choice!";
         getch();
         goto choice5;

   }

   choice5a:;
   ch = choiceAdmin(3);
   switch (ch){
   case 1:
      insertNewState();
      goto choice5a;
   case 2:
      viewStateRecord();
      goto choice5a;
   case 3:
      deleteState();
      goto choice5a;
   case 4:
      updateState();
      goto choice5a;
   case 0:
      goto choice5;
   default:
      cout<<"\n   Enter the Correct Choice!";
      getch();
      goto choice5a;
   }

   choice5b:;
   ch = choiceAdmin(4);
   switch (ch){
   case 1:
      insertNewCity();
      goto choice5b;
   case 2:
      viewCityRecord();
      goto choice5b;
   case 3:
      deleteCity();
      goto choice5b;
   case 4:
      updateCity();
      goto choice5b;
   case 0:
      goto choice5;
   default:
      cout<<"\n   Enter the Correct Choice!";
      getch();
      goto choice5b;
   }

   choice5c:;
   ch = choiceAdmin(1);
   switch (ch){
   case 1:
      viewVoterRecord();
      goto choice5c;
   case 2:
      viewCandidateRecord();
      goto choice5c;
   case 3:
      viewPartyRecord();
      goto choice5c;
   case 4:
      ResetVotes();
      goto choice5c;
   case 0:
      goto choice5;
   default:
      cout<<"\n   Enter the Correct Choice!";
      getch();
      goto choice5c;
   }


   Exit:;
   return 0;
}
