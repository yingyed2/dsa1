#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Foobar {
public:
    // constructor functions creates an instance of this class
    Foobar(const string& name) {    // passing by & reference avoids copying the string
        foobarName = name;
        position = 0;
    }

    void setPosition(int pos) { // assigns position
        position = pos;
    }

    string getName() const {    // retrieves name
        return foobarName;
    }

    virtual int getStrength() const {   // retrieves only position for parent class
        return position;
    }

protected:  // access specifiers ensure certain members aren't accessed
    int getPosition() const {
        return position;
    }

private:
    string foobarName;
    int position;
};


class Foo : public Foobar {
public:
    using Foobar::Foobar; // inherit constructor
    int getStrength() const override {  // allows for polymorphic behavior
        return getPosition() * 3;
    }
};


class Bar : public Foobar {
public:
    using Foobar::Foobar;
    int getStrength() const override {
        return getPosition() + 15;
    }
};


vector<Foobar*> readInputFile(const string& inputFile) {
    ifstream inFile(inputFile); // reads data from a file

    vector<Foobar*> foobars;    // declares a vector named foobars to store pointers to objects
    string type, name;

    while (inFile >> type >> name) {    // reads the next two string variables
        if (type == "foo") {
            foobars.push_back(new Foo(name));
        } else if (type == "bar") {
            foobars.push_back(new Bar(name));
        } else {
            foobars.push_back(new Foobar(name));
        }
    }

    return foobars;
}


void assignPositions(vector<Foobar*>& foobars) {
    for (int i = foobars.size() - 1, pos = 1; i >= 0; --i, ++pos) {
        foobars[i]->setPosition(pos);
    }
}


void writeOutputFile(const string& outputFile, const vector<Foobar*>& foobars) {
    ofstream outFile(outputFile);   // writes data to a file

    for (int i = foobars.size() - 1; i >= 0; --i) {
        outFile << foobars[i]->getName() << " strength: " << foobars[i]->getStrength() << endl;
    }
}

// prevents memory leaks in the heap
void cleanUp(vector<Foobar*>& foobars) {
    for (Foobar* foobar : foobars) {
        delete foobar;
    }
    foobars.clear();
}


int main() {
    string inputFile, outputFile;
    cout << "Enter input filename: ";
    cin >> inputFile;
    cout << "Enter output filename: ";
    cin >> outputFile;

    vector<Foobar*> foobars = readInputFile(inputFile);
    assignPositions(foobars);
    writeOutputFile(outputFile, foobars);
    cleanUp(foobars);

    return 0;
}
