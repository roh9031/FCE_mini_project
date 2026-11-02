#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <queue>
#include <chrono>    // task 6 timing
#include <random>    // task 6 random searches and shuffle
#include <iomanip>   // task 6 table columns
#include <cctype>    // toupper for the session commands


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


         //helper for task 2 search START
         //loop down from the root and save every code we compare with
         Node* searchhelper(const std::string& code, std::vector<std::string>& path, int& comparisons) const{
            Node* curr = root;
            comparisons = 0;
            path.clear();
            while(curr){
                path.push_back(curr->data.getiata());
                comparisons++;
                if(code == curr->data.getiata()) return curr;
                else if(code < curr->data.getiata()) curr = curr->left;
                else curr = curr->right;
            }
            return nullptr;
         }
         //helper for task 2 search END

         //helper for task 5 range START
         //only go left if key > from, only go right if key < to so we skip subtrees outside the range
         void rangehelper(Node* root, const std::string& from, const std::string& to,
                          std::vector<std::string>& res, int& visited) const{
            if(!root) return;
            visited++;
            std::string key = root->data.getiata();
            if(key > from) rangehelper(root->left, from, to, res, visited);
            if(key >= from && key <= to) res.push_back(key);
            if(key < to) rangehelper(root->right, from, to, res, visited);
         }
         //helper for task 5 range END

         //helper for task 4b save preorder
         void preordervectorhelper(Node* root, std::vector<std::string>& res) const{
            if(!root) return;
            res.push_back(root->data.getiata());
            preordervectorhelper(root->left, res);
            preordervectorhelper(root->right, res);
         }

         //helper for task 6 rebalance inorder into a vector
         void inordervectorhelper(Node* root, std::vector<Airport>& res) const{
            if(!root) return;
            inordervectorhelper(root->left, res);
            res.push_back(root->data);
            inordervectorhelper(root->right, res);
         }

         //helper for task 6 balanced build: middle element is the root
         Node* buildbalancedhelper(const std::vector<Airport>& sorted, int lo, int hi){
            if(lo > hi) return nullptr;
            int mid = (lo + hi) / 2;
            Node* node = new Node(sorted[mid]);
            node->left = buildbalancedhelper(sorted, lo, mid - 1);
            node->right = buildbalancedhelper(sorted, mid + 1, hi);
            return node;
         }

        // recursive versions of search min max and delete

         // recursive search compare with this node then search only the side the code can be on
         Node* searchrecursivehelper(Node* node, const std::string& code, std::vector<std::string>& path, int& comparisons) const{
            if(!node) return nullptr;
            path.push_back(node->data.getiata());
            comparisons++;
            if(code == node->data.getiata()) return node;
            if(code < node->data.getiata()) return searchrecursivehelper(node->left, code, path, comparisons);
            return searchrecursivehelper(node->right, code, path, comparisons);
         }

         // recursive min / max the leftmost / rightmost node
         Node* minnoderecursive(Node* node) const{
            if(!node->left) return node;
            return minnoderecursive(node->left);
         }

         Node* maxnoderecursive(Node* node) const{
            if(!node->right) return node;
            return maxnoderecursive(node->right);
         }

         /* recursive delete returns the new root of this subtree and the parent stores it
         d1 leaf return null, d2 one child: return that child,
         d3 two children copy the successor (min of right subtree) here, then delete it from the right subtree */
         Node* removerecursivehelper(Node* node, const std::string& code, std::string& deletecase){
            if(!node){
                deletecase = "not found";
                return nullptr;
            }
            if(code < node->data.getiata()){
                node->left = removerecursivehelper(node->left, code, deletecase);
                return node;
            }
            if(code > node->data.getiata()){
                node->right = removerecursivehelper(node->right, code, deletecase);
                return node;
            }
            if(!node->left && !node->right){
                deletecase = "case D1: leaf";
                delete node;
                return nullptr;
            }
            if(!node->right){
                Node* child = node->left;
                deletecase = "case D2: only a left child, " + child->data.getiata();
                delete node;
                return child;
            }
            if(!node->left){
                Node* child = node->right;
                deletecase = "case D2: only a right child, " + child->data.getiata();
                delete node;
                return child;
            }
            Node* succ = minnoderecursive(node->right);
            deletecase = "case D3: two children, replaced by successor " + succ->data.getiata();
            node->data = succ->data;
            std::string succcase;   // the successor has no left child so this is d1 or d2
            node->right = removerecursivehelper(node->right, node->data.getiata(), succcase);
            return node;
         }

    
        public:
        AirportBST() : root(nullptr) {}
        ~AirportBST(){
            destroy(root);
        }

        //returns true if inserted false if an airport with the same iata already exists
        bool insert(const Airport& airport){
            bool inserted = false;
            root = insertBSTiterative(root, airport, inserted);
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



        //delete every node so we can build again
        void clear(){
            destroy(root);
            root = nullptr;
        }

        std::string rootiata() const{
            if(!root) return "(empty)";
            return root->data.getiata();
        }

        //  task 2b a code deep in the tree  the last code in level order is on the deepest level
        std::string deepestiata() const{
            std::vector<std::string> levelorder = bfshelper(root);
            if(levelorder.empty()) return "(empty)";
            return levelorder[levelorder.size() - 1];
        }

        //task 2a find CODE prints the path comparisons and the airport
        bool find(const std::string& code) const{
            std::vector<std::string> path;
            int comparisons = 0;
            Node* found = searchhelper(code, path, comparisons);
            std::cout << "Path: ";
            for(int i = 0; i < (int)path.size(); i++){
                std::cout << path[i];
                if(i < (int)path.size() - 1) std::cout << " -> ";
            }
            if(found){
                std::cout << "    (comparisons: " << comparisons << ")\n";
                std::cout << found->data.getiata() << " " << found->data.getname() << ", "
                          << found->data.getcity() << ", " << found->data.getcountry() << "\n";
                return true;
            }
            std::cout << " -> (NULL)    not found   (comparisons: " << comparisons << ")\n";
            return false;
        }

        //same search with no printing used for the timing in task 6
        bool contains(const std::string& code) const{
            Node* curr = root;
            while(curr){
                if(code == curr->data.getiata()) return true;
                if(code < curr->data.getiata()) curr = curr->left;
                else curr = curr->right;
            }
            return false;
        }

        //task 2b min = go left until null, max = go right until null
        std::string findmin() const{
            if(!root) return "(empty)";
            Node* curr = root;
            while(curr->left) curr = curr->left;
            return curr->data.getiata();
        }

        std::string findmax() const{
            if(!root) return "(empty)";
            Node* curr = root;
            while(curr->right) curr = curr->right;
            return curr->data.getiata();
        }

        /*task 3 remove CODE
        find the node and remember the parent
        d1 leaf parent points to null
        d2 one child parent points to the child
        d3 two children copy the inorder successor (min of right subtree) up then delete the successor*/
        bool remove(const std::string& code){
            Node* parent = nullptr;
            Node* curr = root;
            while(curr && curr->data.getiata() != code){
                parent = curr;
                if(code < curr->data.getiata()) curr = curr->left;
                else curr = curr->right;
            }
            if(!curr){
                std::cout << code << " not found, nothing deleted\n";
                return false;
            }
            std::string deletecase;
            if(!curr->left && !curr->right){
                deletecase = "case D1: leaf";
                if(!parent) root = nullptr;
                else if(parent->left == curr) parent->left = nullptr;
                else parent->right = nullptr;
                delete curr;
            }
            else if(!curr->left || !curr->right){
                Node* child;
                if(curr->left){
                    child = curr->left;
                    deletecase = "case D2: only a left child, " + child->data.getiata();
                } else {
                    child = curr->right;
                    deletecase = "case D2: only a right child, " + child->data.getiata();
                }
                if(!parent) root = child;
                else if(parent->left == curr) parent->left = child;
                else parent->right = child;
                delete curr;
            }
            else{
                Node* succparent = curr;
                Node* succ = curr->right;
                while(succ->left){
                    succparent = succ;
                    succ = succ->left;
                }
                deletecase = "case D3: two children, replaced by successor " + succ->data.getiata();
                curr->data = succ->data;
                if(succparent == curr) succparent->right = succ->right;
                else succparent->left = succ->right;
                delete succ;
            }
            if(!parent) deletecase += " (this was the root)";
            std::cout << "Deleted " << code << " (" << deletecase << ")   validate: "
                      << (isvalidBSTrangedsolution() ? "OK" : "FAILED") << "\n";
            return true;
        }

        //task 3b level order of the top few levels one line per level
        void printlevels(int levels) const{
            if(!root) return;
            std::queue<Node*> helper;
            helper.push(root);
            for(int lvl = 0; lvl < levels && !helper.empty(); lvl++){
                int levelsize = (int)helper.size();
                std::cout << "  level " << lvl << ": ";
                for(int i = 0; i < levelsize; i++){
                    Node* curr = helper.front();
                    helper.pop();
                    std::cout << curr->data.getiata() << " ";
                    if(curr->left) helper.push(curr->left);
                    if(curr->right) helper.push(curr->right);
                }
                std::cout << "\n";
            }
        }

        //task 4b save file codes in preorder one per line
        bool savepreorder(const std::string& filename) const{
            std::ofstream file(filename);
            if(!file) return false;
            std::vector<std::string> codes;
            preordervectorhelper(root, codes);
            for(int i = 0; i < (int)codes.size(); i++) file << codes[i] << "\n";
            return true;
        }

        //task 5 range FROM TO
        std::vector<std::string> range(const std::string& from, const std::string& to, int& visited) const{
            std::vector<std::string> res;
            visited = 0;
            rangehelper(root, from, to, res, visited);
            return res;
        }

        //task 6 balanced build from a sorted vector, and rebalance
        void buildbalanced(const std::vector<Airport>& sorted){
            clear();
            root = buildbalancedhelper(sorted, 0, (int)sorted.size() - 1);
        }

        void rebalance(){
            std::vector<Airport> sorted;
            inordervectorhelper(root, sorted);
            buildbalanced(sorted);
        }

                // public recursive versions insert find min max remove

        // recursive insert: uses insertBSTrecursive
        bool insertrecursive(const Airport& airport){
            bool inserted = false;
            root = insertBSTrecursive(root, airport, inserted);
            return inserted;
        }

        // recursive find prints the same path and comparisons as find
        bool findrecursive(const std::string& code) const{
            std::vector<std::string> path;
            int comparisons = 0;
            Node* found = searchrecursivehelper(root, code, path, comparisons);
            std::cout << "Path: ";
            for(int i = 0; i < (int)path.size(); i++){
                std::cout << path[i];
                if(i < (int)path.size() - 1) std::cout << " -> ";
            }
            if(found){
                std::cout << "    (comparisons: " << comparisons << ")\n";
                std::cout << found->data.getiata() << " " << found->data.getname() << ", "
                          << found->data.getcity() << ", " << found->data.getcountry() << "\n";
                return true;
            }
            std::cout << " -> (NULL)    not found   (comparisons: " << comparisons << ")\n";
            return false;
        }

        std::string findminrecursive() const{
            if(!root) return "(empty)";
            return minnoderecursive(root)->data.getiata();
        }

        std::string findmaxrecursive() const{
            if(!root) return "(empty)";
            return maxnoderecursive(root)->data.getiata();
        }

        // recursive remove prints the case and validates like remove
        bool removerecursive(const std::string& code){
            bool wasroot = (root && root->data.getiata() == code);
            std::string deletecase;
            root = removerecursivehelper(root, code, deletecase);
            if(deletecase == "not found"){
                std::cout << code << " not found, nothing deleted\n";
                return false;
            }
            if(wasroot) deletecase += " (this was the root)";
            std::cout << "Deleted " << code << " (" << deletecase << ")   validate: "
                      << (isvalidBSTrangedsolution() ? "OK" : "FAILED") << "\n";
            return true;
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



bool compareiata(const Airport& a, const Airport& b){
    return a.getiata() < b.getiata();
}

// builds a tree by inserting the list in that order (same as task 1b)
void buildtree(AirportBST& tree, const std::vector<Airport>& list){
    tree.clear();
    for(int i = 0; i < (int)list.size(); i++) tree.insert(list[i]);
}

// task 4b load file read the preorder codes and insert them in that order
// sorted is the airports sorted by code so we can binary search the full record
void loadpreorder(AirportBST& tree, const std::string& filename, const std::vector<Airport>& sorted){
    tree.clear();
    std::ifstream file(filename);
    std::string code;
    while(file >> code){
        int lo = 0, hi = (int)sorted.size() - 1;
        while(lo <= hi){
            int mid = (lo + hi) / 2;
            if(sorted[mid].getiata() == code){ tree.insert(sorted[mid]); break; }
            if(sorted[mid].getiata() < code) lo = mid + 1;
            else hi = mid - 1;
        }
    }
}

// task 6 time 10000 searches
double timesearches(const AirportBST& tree, const std::vector<std::string>& queries){
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < (int)queries.size(); i++) tree.contains(queries[i]);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}



// task 5 prints every airport in the rangeso we need the full record for each code
// binary search in the airports sorted by code prints one airport per line
void printairportbycode(const std::vector<Airport>& sorted, const std::string& code){
    int lo = 0, hi = (int)sorted.size() - 1;
    while(lo <= hi){
        int mid = (lo + hi) / 2;
        if(sorted[mid].getiata() == code){
            std::cout << sorted[mid].getiata() << " | " << sorted[mid].getname() << " | "
                      << sorted[mid].getcity() << ", " << sorted[mid].getcountry() << "\n";
            return;
        }
        if(sorted[mid].getiata() < code) lo = mid + 1;
        else hi = mid - 1;
    }
    std::cout << code << "\n";   // not in the data file (eg an airport added in the session)
}

// the command list for the interactive session
void printhelp(){
    std::cout << "Commands:\n"
              << "  find CODE        search, prints the path and number of comparisons\n"
              << "  min | max        smallest / largest code\n"
              << "  remove CODE      delete an airport (prints the case and validates)\n"
              << "  add CODE         add an airport (asks for name, city, country)\n"
              << "  list             every airport in code order\n"
              << "  range FROM TO    every airport from FROM to TO\n"
              << "  save FILE        write the codes in preorder\n"
              << "  load FILE        rebuild the tree from a preorder file\n"
              << "  validate         check the BST property (range method)\n"
              << "  levels           level order of the top 3 levels\n"
              << "  stats            size, height, average depth\n"
              << "  rebalance        rebuild as a balanced tree\n"
              << "  help | quit\n";
}

int main() {
    std::vector<Airport> airports;
//task 1a load file
    // also try data/airports.dat, so it works when run from the repo root
    // with the handouts build command (g++ -std=c++17 -O2 src/*.cpp -o airports)
    if (!loadairports("data/airports.dat", airports) && !loadairports("../data/airports.dat", airports)) {
        std::cout << "could not open the file\n";
        return 1;                            // tell the operating systemthe program failed
    }

 // task 1b build the bst by inserting in file order
    AirportBST bst;
    int insertedCount = 0;
    int duplicateCount = 0;
    int n = (int)airports.size();
    bool alwaysvalid = true;                 // task 4c validate after every insert

    for (int i = 0; i < n; i++) {
        if (bst.insert(airports[i])) {
            insertedCount++;
        } else {
            duplicateCount++;                                   // rule I4 report it
            std::cout << "Duplicate code: " << airports[i].getiata() << "\n";
        }
        if (!bst.isvalidBSTrangedsolution()) alwaysvalid = false;   // task 4c
    }

    std::cout << "Inserted " << insertedCount << " airports, "
              << duplicateCount << " duplicates (expected 0)\n \n";

    // task 1c height and average depth
    std::cout<<"Task 1c \n";
    std::cout << "BST size: " << bst.size() << "\n";
    std::cout << "BST height: " << bst.height() << "\n";
    std::cout << "Average depth: " << bst.avgdepth() << "\n \n";



    // task 2 search every case
    std::cout << "Task 2\n";
    bst.find(bst.rootiata());   // the root (GKA)
    bst.find("AAC");            // a leaf
    bst.find("YZR");            // deep in the tree
    bst.find(bst.deepestiata()); // the deepest node of all
    bst.find("XYZ");            // does not exist
    bst.find("AAA");            // smallest code
    bst.find("000");            // smaller than every code (digits sort before letters)
    bst.find("ZZZZ");           // bigger than every code
    bst.find("BOS");
    std::cout << "Min code: " << bst.findmin() << "   Max code: " << bst.findmax() << "\n \n";

    // task 3 delete every case each on a fresh file order tree
    // codes found by looking at the tree: AAC leaf GJT only left child
    // AAA only right child AEY two children GKA the root
    std::cout << "Task 3\n";
    std::string deletes[5] = {"AAC", "GJT", "AAA", "AEY", "GKA"};
    for (int i = 0; i < 5; i++) {
        buildtree(bst, airports);
        std::cout << "before:\n";
        bst.printlevels(3);
        bst.remove(deletes[i]);
        std::cout << "after:\n";
        bst.printlevels(3);
    }
    buildtree(bst, airports);
    bst.remove("BOS");

    // task 3c made up airport
    Airport fake(0, "Cats Buitano Memorial Airfield", "Boston", "United States", "QQQ", "\\N", 42.34, -71.09);
    bst.insert(fake);
    std::cout << "Added QQQ   validate: " << (bst.isvalidBSTrangedsolution() ? "OK" : "FAILED") << "\n";
    bst.find("QQQ");
    bst.remove("QQQ");
    bst.find("QQQ");
    std::cout << "\n";
    buildtree(bst, airports);   // back to the normal tree for task 4

    // end of tasks 2 and 3 

    // task 4a inorder list and sorted check
    std::cout<<"Task 4a:\n";
    std::cout << "list (every airport in code order):\n";   // label on its own line
    bst.printinorder();
    std::cout << "\nInorder is sorted: " << (bst.isvalidBST() ? "true" : "false") << "\n";

    std::vector<std::string> lo = bst.printbfs();
    std::cout << "Level order (first 7): ";
    for (int i = 0; i < 7 && i < (int)lo.size(); i++) {
         std::cout << lo[i] << " ";
    }  
    std::cout << "\nLevel order count: " << lo.size() << " (expected 6072)\n";

 

    // task 4b save in preorder load it back check the shape is the same
    std::cout<<"\nTask 4b\n";
    std::vector<Airport> sortedairports = airports;
    std::sort(sortedairports.begin(), sortedairports.end(), compareiata);
    bst.savepreorder("preorder.txt");
    AirportBST reloaded;
    loadpreorder(reloaded, "preorder.txt", sortedairports);
    std::cout << "Reloaded height: " << reloaded.height() << " (original " << bst.height() << ")\n";
    std::cout << "Same level order: " << (reloaded.printbfs() == bst.printbfs() ? "true" : "false") << "\n";

    // end of task 4b 

    std::cout<<"\nTask 4c\n";
    std::cout << "validate after every insert (task 1b build): " << (alwaysvalid ? "true" : "false") << "\n";   // ADDED
    std::cout << "Task 4c validate (range method): " << (bst.isvalidBSTrangedsolution() ? "true" : "false") << "\n";


    // task 5 range query
    std::cout<<"\nTask 5\n";
    int visited = 0;
    std::vector<std::string> inrange = bst.range("BOS", "BWI", visited);
    for (int i = 0; i < (int)inrange.size(); i++) {           //  list every airport in the range
        printairportbycode(sortedairports, inrange[i]);
    }
    std::cout << "range BOS BWI: " << inrange.size() << " airports (expected 113), " << visited << " nodes visited\n";

    // recursive vs iterative both versions must build the same tree and give the same answers
    std::cout << "\nRecursive vs iterative\n";
    AirportBST itertree, rectree;
    for (int i = 0; i < n; i++) {
        itertree.insert(airports[i]);            // iterative insert insertBSTiterative
        rectree.insertrecursive(airports[i]);    // recursive insert insertBSTrecursive
    }
    std::cout << "insert: height " << itertree.height() << " vs " << rectree.height()
              << ", same level order: " << (itertree.printbfs() == rectree.printbfs() ? "true" : "false") << "\n";
    std::cout << "find BOS, iterative:\n";  itertree.find("BOS");
    std::cout << "find BOS, recursive:\n";  rectree.findrecursive("BOS");
    std::cout << "find XYZ, iterative:\n";  itertree.find("XYZ");
    std::cout << "find XYZ, recursive:\n";  rectree.findrecursive("XYZ");
    std::cout << "min/max iterative: " << itertree.findmin() << " " << itertree.findmax()
              << "   recursive: " << rectree.findminrecursive() << " " << rectree.findmaxrecursive() << "\n";
    std::string bothdeletes[5] = {"AAC", "GJT", "AAA", "AEY", "GKA"};
    for (int i = 0; i < 5; i++) {
        std::cout << "iterative: ";  itertree.remove(bothdeletes[i]);
        std::cout << "recursive: ";  rectree.removerecursive(bothdeletes[i]);
    }
    std::cout << "after the same 5 deletes: same level order: "
              << (itertree.printbfs() == rectree.printbfs() ? "true" : "false") << "\n\n";


    // task 6 shape experiment same 10000 random codes for every tree
    std::cout<<"\nTask 6\n";
    std::mt19937 rng(2560);
    std::vector<std::string> queries;
    for (int i = 0; i < 10000; i++) queries.push_back(airports[rng() % n].getiata());

    AirportBST t;
    buildtree(t, airports);
    std::cout << "File order:     height " << t.height() << "  avg depth " << t.avgdepth()
              << "  10000 searches: " << timesearches(t, queries) << " ms\n";
    buildtree(t, sortedairports);
    std::cout << "Sorted order:   height " << t.height() << "  avg depth " << t.avgdepth()
              << "  10000 searches: " << timesearches(t, queries) << " ms\n";
    t.buildbalanced(sortedairports);
    std::cout << "Balanced:       height " << t.height() << "  avg depth " << t.avgdepth()
              << "  10000 searches: " << timesearches(t, queries) << " ms\n";
    std::vector<Airport> shuffled = airports;
    std::shuffle(shuffled.begin(), shuffled.end(), std::mt19937(1));
    buildtree(t, shuffled);
    std::cout << "Random shuffle: height " << t.height() << "  avg depth " << t.avgdepth()
              << "  10000 searches: " << timesearches(t, queries) << " ms\n";
    std::cout << "expected heights: 177, 6071, 12, about 25-29\n";   

    bst.rebalance();
    std::cout << "After rebalance: height " << bst.height() << "  avg depth " << bst.avgdepth() << "\n";

    // evidence for the task 6 explanation how ordered is the file already?
    // a random order would have about 50% increasing pairs and only short increasing runs
    int iataup = 0, icaoup = 0, run = 1, longest = 1;
    for (int i = 1; i < n; i++) {
        if (airports[i].geticao() > airports[i - 1].geticao()) icaoup++;
        if (airports[i].getiata() > airports[i - 1].getiata()) {
            iataup++;
            run++;
            if (run > longest) longest = run;
        } else {
            run = 1;
        }
    }
    std::cout << "\nHow the file is ordered (a random order gives about 50%):\n";
    std::cout << "lines with a larger IATA code than the line before: " << 100.0 * iataup / (n - 1) << "%\n";
    std::cout << "lines with a larger ICAO code than the line before: " << 100.0 * icaoup / (n - 1) << "%\n";
    std::cout << "longest run of increasing IATA codes: " << longest << " lines\n";

    // end of tasks 5 and 6

    // interactive session the commands named in the handout
    // find remove list save load validate range rebalance used for the README sample and the GIF
    buildtree(bst, airports);   // start the session from the file order tree
    std::cout << "\n Interactive session (type help for commands) \n";
    printhelp();
    std::string line;
    std::cout << "> ";
    while (std::getline(std::cin, line)) {
        std::istringstream ss(line);
        std::string cmd, a1, a2;
        ss >> cmd >> a1 >> a2;
        std::string file = a1;                         // file names keep their case
        for (int i = 0; i < (int)a1.size(); i++) a1[i] = (char)toupper((unsigned char)a1[i]);
        for (int i = 0; i < (int)a2.size(); i++) a2[i] = (char)toupper((unsigned char)a2[i]);

        if (cmd == "quit" || cmd == "exit") break;
        else if (cmd == "help") printhelp();
        else if (cmd == "find" && !a1.empty()) bst.find(a1);
        else if (cmd == "min") std::cout << "Min code: " << bst.findmin() << "\n";
        else if (cmd == "max") std::cout << "Max code: " << bst.findmax() << "\n";
        else if (cmd == "remove" && !a1.empty()) {
            std::cout << "before:\n";
            bst.printlevels(3);
            bst.remove(a1);
            std::cout << "after:\n";
            bst.printlevels(3);
        }
        else if (cmd == "add" && a1.size() == 3) {
            std::string name, city, country;
            std::cout << "name: ";    std::getline(std::cin, name);
            std::cout << "city: ";    std::getline(std::cin, city);
            std::cout << "country: "; std::getline(std::cin, country);
            Airport added(0, name, city, country, a1, "\\N", 0.0, 0.0);
            if (bst.insert(added)) {
                std::cout << "Added " << a1 << "   validate: "
                          << (bst.isvalidBSTrangedsolution() ? "OK" : "FAILED") << "\n";
                // keep the sorted list up to date so range and load can find it
                sortedairports.push_back(added);
                std::sort(sortedairports.begin(), sortedairports.end(), compareiata);
            } else {
                std::cout << a1 << " already exists (rule I4), not added\n";
            }
        }
        else if (cmd == "list") {
            bst.printinorder();
            std::cout << "Inorder is sorted: " << (bst.isvalidBST() ? "true" : "false") << "\n";
        }
        else if (cmd == "range" && !a2.empty()) {
            std::vector<std::string> res = bst.range(a1, a2, visited);
            for (int i = 0; i < (int)res.size(); i++) printairportbycode(sortedairports, res[i]);
            std::cout << "range " << a1 << " " << a2 << ": " << res.size() << " airports, "
                      << visited << " nodes visited\n";
        }
        else if (cmd == "save" && !file.empty()) {
            if (bst.savepreorder(file)) std::cout << "saved " << bst.size() << " codes to " << file << "\n";
            else std::cout << "could not write " << file << "\n";
        }
        else if (cmd == "load" && !file.empty()) {
            loadpreorder(bst, file, sortedairports);
            std::cout << "loaded " << bst.size() << " airports, height " << bst.height() << "\n";
            bst.printlevels(3);
        }
        else if (cmd == "validate") {
            std::cout << "validate: " << (bst.isvalidBSTrangedsolution() ? "true" : "false") << "\n";
        }
        else if (cmd == "levels") bst.printlevels(3);
        else if (cmd == "stats") {
            std::cout << "size " << bst.size() << ", height " << bst.height()
                      << ", average depth " << bst.avgdepth() << "\n";
        }
        else if (cmd == "rebalance") {
            bst.rebalance();
            std::cout << "rebalanced: height " << bst.height() << ", average depth " << bst.avgdepth() << "\n";
        }
        else if (!cmd.empty()) std::cout << "unknown command or missing argument (type help)\n";
        std::cout << "> ";
    }
    return 0;
}