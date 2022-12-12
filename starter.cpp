#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include <cstdlib>
#include <queue>
using namespace std;

#define IS_QUIT(s) (s == "X" || s == "x")
#define FILENAME "facebook.csv"
using namespace std;

struct Person {
    vector<string> friends;
    bool visited;
};

bool BFS(map<string, Person> everyone, string start, string end, map<string,bool> visited){
    queue<string> q;
    Person tmp;

    if(start == end){
        return true;
    }

    visited[start] = true;
    q.push(start);

    while (!q.empty()){
        start = q.front();
        q.pop();
        tmp = everyone[start];
        for(int i = 0; i < tmp.friends.size(); i++){
            if(tmp.friends[i] == end){
                return true;
            }

            if(!visited[tmp.friends[i]]){
                visited[tmp.friends[i]] = true;
                q.push(tmp.friends[i]);
            }
        }
    }
        return false;
}

void parse_line(const string &str,
    vector<string> &line) {
    istringstream istr(str);
    string tmp;
    while (getline(istr, tmp, ',')) {
        line.push_back(tmp);
    }
}

int main() {
    ifstream inFile(FILENAME);
    vector<string> row;
    vector<string> names;
    map<string, Person> everyone;
    string startingLine, endingLine;
    map<string, bool> visited;
    string added;

    // Verify that the file open was OK
    if (!inFile.good()) {
        cerr << "Invalid file." << endl;
        return (-1);
    }

    // Read the header line of the file (first line, contains column labels).
    // We save this line (names) so we can lookup the string names in the
    // below loop.
    getline(inFile, startingLine);
    parse_line(startingLine, names);

    // Reach each subsequent entry
    while (getline(inFile, startingLine)) {
        if (inFile.eof())
            break;
        vector<string> row;
        Person p;
        parse_line(startingLine, row);
        // Start at 1 (0th field is the string name)
        for (size_t i = 1; i < row.size(); i++) {
            int adj_status = atoi(row[i].c_str());
            // A '1' indicates an adjacency, so skip if we get a '0'
            // If there is an adjacency to this person, push the string name
            // of that person on the adjacency list.
            if (adj_status == 1)
                p.friends.push_back(names[i]);
        }
        // Add this (new) person to the map.
        // In this map, the key is the string name of the person, and
        // the value is their Person structure (adjacency list).
        everyone.insert(make_pair(row[0], p));
    }

    for (;;) {
        cout << "Enter the starting name (X to quit) : ";
        getline(cin, startingLine);
        if (IS_QUIT(startingLine))
            break;

        // Look up the adjacency list of this person in the map
        if (everyone.count(startingLine) == 0) {
            cout << "That person is not in the map." << endl;
            continue;
        }

        cout << endl << "Enter the ending name (X to quit): ";
        getline(cin,endingLine );
        if (IS_QUIT(endingLine))
            break;
        
        if(BFS(everyone, startingLine, endingLine, visited)){
            cout << "There IS a path between these two people." << endl;
        } else {
            cout << "There is NOT a path between these two people." << endl;
        }
        
    }
    cout << "Exiting..." << endl;
    return (0);
}