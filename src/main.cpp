#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <queue>
class Airport{
    private:
    //write down all the varioables present in the .dat file
    int airportID;
    std::string name;
    std::string city;
    std::string country;
    std::string iata;
    std::string icao;
    double latitude;
    double longitude;

    
    public:
    //constructors: intialize 0 in ints and doubles c++ auto handles initialization for chars and strings
    Airport() : airportID(0), latitude(0.0), longitude(0.0) {}
    Airport(int id, const std::string& n, const std::string& c, const std::string& cntry, const std::string& it,
        const std::string& ic, double lat, double lon ) : airportID(id), name(n), city(c), country(cntry), iata(it), icao(ic),
        latitude(lat), longitude(lon){}
    
    //getter commands

    int getAirportID() const{
        return airportID;
    }

    double getlatitude() const {
    return latitude;
     }

    double getlongitude() const{
        return longitude;
    } 

    std::string getname() const{
        return name;
    }

    std::string getcity() const{
        return city;
    }

    std::string getcountry() const{
        return country;
    } 

    std::string getiata() const{
        return iata;
    }

    std::string geticao() const{
        return icao;
    }
};


class AirportBST{
        private:
        struct Node{
            Airport data;
            Node* left;
            Node* right;
            Node(const Airport &d) : data(d), left(nullptr), right(nullptr) {}
        };

        /*create a bool function inserted to keep track of whether the airport data has been onserted into 
        the tree or not
        init a root iata should be nodes of bst 
        check if airport iata is less than current node iaata if so insert to left else inser into right
        if null create new node(airport) and swicth inserted to true*/

        Node* root;

        Node* insertBSTrecursive(Node* node, const Airport& airport, bool& inserted){
            if(node==nullptr){
                inserted = true;
                return new Node(airport);
            }

            if(airport.getiata()<node->data.getiata()){
                node->left = insertBSTrecursive(node->left, airport, inserted);
            }
            else if(airport.getiata()>node->data.getiata()){
                node->right = insertBSTrecursive(node->right, airport, inserted);
            }
            else{
                inserted = false;
            }
            return node;
        }

        Node* insertBSTiterative(Node* root, const Airport &airport, bool &inserted) {
             Node* newnode = new Node(airport);
             inserted = true;

             if (root == nullptr) return newnode;

             Node* curr = root;
             while (true) {
                 if (airport.getiata() < curr->data.getiata()) {
                     if (curr->left == nullptr) {
                         curr->left = newnode;   
                         return root;
                         }
                     curr = curr->left;
                }
                  else if (airport.getiata() > curr->data.getiata()) {
                  if (curr->right == nullptr) {
                    curr->right = newnode;  
                    return root;
            }
                  curr = curr->right;
                  }
                 else {
            // duplicate iata found
                    inserted = false;
                    delete newnode;
                    return root;
                 }
            }
        }   
        

        int computeheight(Node* node) const{
            if(node==nullptr) return -1;
            int leftheight = computeheight(node->left);
            int rightheight = computeheight(node->right);
            return 1+ std::max(leftheight, rightheight);
        }

        int countnodes(Node* node) const{
            if(node==nullptr) return 0;
            return 1+ countnodes(node->left)+countnodes(node->right);
        }

        //delete children first then the node itself
        void destroy(Node* node){
            if(node==nullptr) return;
            destroy(node->left);
            destroy(node->right);
            delete node;
        }

        void sumAndCount(Node* root, int depth, int &sum, int &count) const {
             if (root == nullptr) return;
             sum += depth;
             count++;
             sumAndCount(root->left, depth + 1, sum, count);
             sumAndCount(root->right, depth + 1, sum, count);
        }

         double averagedepth(Node* root) const {
         if (root == nullptr) return 0;
         int sum = 0, count = 0;
         sumAndCount(root, 0, sum, count);
         return (double)sum / count;
         }  
         
         //helper for task 4a is valid bst STRAT
         bool checkinorder(Node* root, Node * &prev) const{//left node right
            if(!root) return true;
            if(!checkinorder(root->left, prev)) return false;
            if(prev && prev->data.getiata() >= root->data.getiata()) return false;
            prev = root;
            return checkinorder(root->right, prev);
         }

         bool validRange(Node* node, Node* low, Node* high) const {
            if (node == nullptr) return true;

            // low  = lower bound exclusive
            // high = upper bound exclusive

            if (low  != nullptr && node->data.getiata() <= low->data.getiata())  return false;
            if (high != nullptr && node->data.getiata() >= high->data.getiata()) return false;

            return validRange(node->left,  low, node)   // left subtree upper bound tightens to current
             && validRange(node->right, node, high); // right subtree lower bound tightens to current
        }

        //helper for task 4a is valid bst END

         void printinorderhelper(Node* root) const{
            if(!root) return;
            printinorderhelper(root->left);
            std::cout << root->data.getiata() << " | " << root->data.getname() << " | "
          << root->data.getcity() << ", " << root->data.getcountry() << "\n";
            printinorderhelper(root->right);

         }

         void printpostorderhelper(Node* root) const{//left right node
            if(!root) return;
            printpostorderhelper(root->left);
            printpostorderhelper(root->right);
            std::cout<<root->data.getiata()<<" ";
         }

         void printpreorderhelper(Node* root) const{
            if(!root) return;
            std::cout<<root->data.getiata()<<" ";
            printpreorderhelper(root->left);
            printpreorderhelper(root->right);
         }

         std::vector<std::string> bfshelper(Node* root) const{
            std::vector<std::string> res;
            if(!root) return res;
            std::queue<Node*> helper;
            helper.push(root);
            while(!helper.empty()){
                Node* curr = helper.front();
                helper.pop();
                res.push_back(curr->data.getiata());
                if(curr->left) helper.push(curr->left); 
                if(curr->right) helper.push(curr->right);
                /*empty/null will bepushed into vector but it will be marked as null if we do if(curr->left) etc 
                we push only the present value pushing verything will help visualize bst struct better
                */

            }

            return res;


         }
    

        public:
        AirportBST() : root(nullptr) {}
        ~AirportBST(){
            destroy(root);
        }

        //returns true if inserted, false if an airport with the same iata already exists
        bool insert(const Airport& airport){
            bool inserted = false;
            root = insertBSTrecursive(root, airport, inserted);
            return inserted;
        }

        int height() const{
            return computeheight(root);
        }

        int size() const{
            return countnodes(root);
        }

        double avgdepth() const{
            return averagedepth(root);
        }

        bool isvalidBST() const{
            Node* prev = nullptr;
            return checkinorder(root, prev);
        }

        bool isvalidBSTrangedsolution() const {
            return validRange(root, nullptr, nullptr);
        }

        void printinorder() const{
            return printinorderhelper(root);
        }

        void printpreorder() const{
            return printpreorderhelper(root);
        }

        void printpostorder() const{
            return printpostorderhelper(root);
        }

        std::vector<std::string> printbfs() const{
            return bfshelper(root);
        } 

        
};

// splits one line of airports.dat into its fields
// commas inside double quotes are part of the text not separators
std::vector<std::string> splitline(const std::string& line) {
    std::vector<std::string> fields;   // the finished pieces go here
    std::string current;               // the piece we're building right now
    bool inquotes = false;             // are we inside a quoted part

    for (int i = 0; i < (int)line.size(); i++) {
        char ch = line[i];             // look at one character

        if (ch == '"') {
            inquotes = !inquotes;      // entering or leaving quotes don't keep the quote itself
        }
        else if (ch == ',' && !inquotes) {
            fields.push_back(current); // a real separator this piece is finished
            current.clear();           // start the next piece empty
        }
        else {
            current += ch;             // any other character including commas inside quotes
        }
    }

    fields.push_back(current);         // the last piece has no comma after it so save it here

    return fields;
}

bool loadairports(const std::string& filename, std::vector<Airport>& airports) {
    std::ifstream file(filename);            // open whichever file main asked for

    if (!file) {                             // couldnt open it
        return false;                        // tell mainit failed
    }

    std::string line;
    int lines = 0;
    int skipped = 0;

    while (std::getline(file, line)) {       // read one line each loop
        lines++;

        // cut the line into pieces
        std::vector<std::string> fields = splitline(line);

        // skip lines we cant use too few pieces or no iata code (\N)
        if ((int)fields.size() < 8 || fields[4] == "\\N") {
            skipped++;
            continue;                        // jump straight to the next line
        }

        // fill out one Airport card from the pieces
        //    text -> number stoi for the int ID stod for the doubles
        Airport a(std::stoi(fields[0]),      // airport ID
                  fields[1],                 // name
                  fields[2],                 // city
                  fields[3],                 // country
                  fields[4],                 // IATA code
                  fields[5],                 // ICAO code
                  std::stod(fields[6]),      // latitude
                  std::stod(fields[7]));     // longitude

        // add it to the vector
        airports.push_back(a);
    }

    std::cout << "lines read: " << lines << " \n";
    std::cout << "airports loaded: " << airports.size() << " \n";
    std::cout << "lines skipped: " << skipped << "\n";
    return true;                             // tell main it worked
}

int main() {
    std::vector<Airport> airports;
//task 1a load file
    if (!loadairports("../data/airports.dat", airports)) {
        std::cout << "could not open the file\n";
        return 1;                            // tell the operating systemthe program failed
    }

 // task 1b build the BST by inserting in file order
    AirportBST bst;
    int insertedCount = 0;
    int duplicateCount = 0;
    int n = (int)airports.size();

    for (int i = 0; i < n; i++) {
        if (bst.insert(airports[i])) {
            insertedCount++;
        } else {
            duplicateCount++;                                   // rule I4 report it
            std::cout << "Duplicate code: " << airports[i].getiata() << "\n";
        }
    }

    std::cout << "Inserted " << insertedCount << " airports, "
              << duplicateCount << " duplicates (expected 0)\n \n";

    // task 1c height and average depth
    std::cout<<"Task 1c \n";
    std::cout << "BST size: " << bst.size() << "\n";
    std::cout << "BST height: " << bst.height() << "\n";
    std::cout << "Average depth: " << bst.avgdepth() << "\n \n";

    // task 4a inorder list and sorted check
    std::cout<<"Task 4a:\n";
    std::cout << "Inorder: ";
    //temporarily commented for clean terminal output bst.printinorder();
    std::cout << "\nInorder is sorted: " << (bst.isvalidBST() ? "true" : "false") << "\n";

    std::vector<std::string> lo = bst.printbfs();
    std::cout << "Level order (first 7): ";
    for (int i = 0; i < 7 && i < (int)lo.size(); i++) {
         std::cout << lo[i] << " ";
    }  
    std::cout << "\nLevel order count: " << lo.size() << " (expected 6072)\n";
    std::cout<<"\nTask 4c\n";
    std::cout << "Task 4c validate (range method): " << (bst.isvalidBSTrangedsolution() ? "true" : "false") << "\n";

    return 0;
}