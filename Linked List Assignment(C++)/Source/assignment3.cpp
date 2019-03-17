#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <cstring>

using namespace std;


/*To implement the nodes of doubly linked list:*/
class doubleNode{
private:
    doubleNode *next;
    doubleNode *prev;
    string awayTeam;
    int minuteOfGoal;
    int matchID;
public:
    doubleNode *getNext() const {
        return next;
    }

    void setNext(doubleNode *next) {
        doubleNode::next = next;
    }

    doubleNode *getPrev() const {
        return prev;
    }

    void setPrev(doubleNode *prev) {
        doubleNode::prev = prev;
    }

    const string &getAwayTeam() const {
        return awayTeam;
    }

    void setAwayTeam(const string &awayTeam) {
        doubleNode::awayTeam = awayTeam;
    }

    int getMinuteOfGoal() const {
        return minuteOfGoal;
    }

    void setMinuteOfGoal(int minuteOfGoal) {
        doubleNode::minuteOfGoal = minuteOfGoal;
    }

    int getMatchID() const {
        return matchID;
    }

    void setMatchID(int matchID) {
        doubleNode::matchID = matchID;
    }

    doubleNode(){
        doubleNode::next = nullptr;
        doubleNode::prev = nullptr;
        doubleNode::awayTeam = "";
        doubleNode::matchID = 0;
        doubleNode::minuteOfGoal = 0;

    }

    doubleNode(string awayTeam, int minuteOfGoal, int matchID)
            : awayTeam(move(awayTeam)), minuteOfGoal(minuteOfGoal), matchID(matchID){
        doubleNode::next = nullptr;
        doubleNode::prev = nullptr;
    }
};



/*Implementing a doubly linked list: */
class DoubleLinkedList{
private:
    doubleNode *head;
    doubleNode *tail;
    doubleNode *unsorted_head;
    doubleNode *unsorted_tail;
    int size;

public:
    doubleNode *getHead() const {
        return head;
    }

    void setHead(doubleNode *head) {
        DoubleLinkedList::head = head;
    }

    int getSize() const {
        return size;
    }

    void setSize(int size) {
        DoubleLinkedList::size = size;
    }

    doubleNode *getTail() const {
        return tail;
    }

    doubleNode *getUnsorted_head() const {
        return unsorted_head;
    }

    void insertUnsorted(doubleNode *node_inserted){
        if(unsorted_head == nullptr){
            DoubleLinkedList::unsorted_head = node_inserted;
            DoubleLinkedList::unsorted_tail = node_inserted;
        }
        else{
            node_inserted->setPrev(unsorted_tail);
            unsorted_tail->setNext(node_inserted);
            DoubleLinkedList::unsorted_tail = node_inserted;
        }
    }
    
    
    /*Inserts matches of player in ascending order: */
    void insert(doubleNode *node_inserted){
        doubleNode *current;
        doubleNode *cut;

        /*If there is no node yet: */
        if(head == nullptr){
            DoubleLinkedList::setHead(node_inserted);
            DoubleLinkedList::tail = node_inserted;
            DoubleLinkedList::setSize(DoubleLinkedList::size+1);
        }
        /*If there is one node and inserted node is less than first node: */
        else if(node_inserted->getMatchID() < node_inserted->getMatchID()){
            node_inserted->setNext(DoubleLinkedList::head);
            DoubleLinkedList::head->setPrev(node_inserted);
            DoubleLinkedList::head = node_inserted;
            DoubleLinkedList::setSize(DoubleLinkedList::size+1);

        }

        else{
            current = head;
            cut = current;

            while(node_inserted->getMatchID()> current->getMatchID() && current->getNext() != nullptr){
                cut = current;
                current = current->getNext();
            }
                /*If match ID of given node is greater than all elements, add it to last: */
            if(current->getNext() == nullptr && current->getMatchID() != node_inserted->getMatchID()){
                node_inserted->setNext(nullptr);
                current->setNext(node_inserted);
                node_inserted->setPrev(current);
                DoubleLinkedList::tail = node_inserted;
                DoubleLinkedList::setSize(DoubleLinkedList::size+1);
            }

                /*If there is a corresponding match ID, insert node after it: */
            else if(current->getMatchID() == node_inserted->getMatchID()){
                if(current->getNext() == nullptr){
                    node_inserted->setPrev(current);
                    node_inserted->setNext(nullptr);
                    current->setNext(node_inserted);
                    DoubleLinkedList::tail = node_inserted;
                    DoubleLinkedList::setSize(DoubleLinkedList::size+1);
                }
                else{node_inserted->setPrev(cut);
                    node_inserted->setNext(current);
                    cut->setNext(node_inserted);
                    current->setPrev(node_inserted);
                    DoubleLinkedList::setSize(DoubleLinkedList::size+1);}

            }
                /*Else, insert node to corresponding range: */
            else{
                node_inserted->setPrev(cut);
                node_inserted->setNext(current);
                cut->setNext(node_inserted);
                current->setPrev(node_inserted);
                DoubleLinkedList::setSize(DoubleLinkedList::size+1);

            }

        }
    }

    DoubleLinkedList(){
        DoubleLinkedList::head = nullptr;
        DoubleLinkedList::tail = nullptr;
        DoubleLinkedList::unsorted_tail = nullptr;
        DoubleLinkedList::unsorted_head = nullptr;
        DoubleLinkedList::size = 0;
    }

    explicit DoubleLinkedList(doubleNode *head) : head(head) {DoubleLinkedList::size = 0; DoubleLinkedList::tail = head;
        DoubleLinkedList::unsorted_tail = head;
        DoubleLinkedList::unsorted_head = head; }

};


/*Implements a single node for linked list structure: */
class Node{
    private:
        Node *next;
        DoubleLinkedList *down;
        string footballerName;
        string teamName;
    public:

        Node *getNext() const {
            return next;
        }

        void setNext(Node *next) {
            Node::next = next;
        }

        const string &getFootballerName() const {
            return footballerName;
        }

        void setFootballerName(const string &footballerName) {
            Node::footballerName = footballerName;
        }

        const string &getTeamName() const {
            return teamName;
        }

        void setTeamName(const string &team_name) {
            Node::teamName = team_name;
        }


        DoubleLinkedList *getDown() const {
            return down;
        }

        void setDown(DoubleLinkedList *down) {
            Node::down = down;
        }
        
        /*Returns a string of all matches of a footballer: */
        string print_matches() {
            string of;
            of+="Matches of " + Node::getFootballerName() + "\n";
            auto tempNode = Node::down->getUnsorted_head();
            while (tempNode->getNext() != nullptr) {
                of += "Footballer Name: " + Node::getFootballerName() + ",Away Team: " +
                     tempNode->getAwayTeam() + ",Min of Goal: " +
                        to_string(tempNode->getMinuteOfGoal()) + ",Match ID: " + to_string(tempNode->getMatchID()) + "\n";
                tempNode = tempNode->getNext();
            }
            of += "Footballer Name: " + Node::getFootballerName() + ",Away Team: " +
                  tempNode->getAwayTeam() + ",Min of Goal: " +
                  to_string(tempNode->getMinuteOfGoal()) + ",Match ID: " + to_string(tempNode->getMatchID()) + "\n";
            return of;
        }
	
        /*Returns a string of matches of a footballer in ascending order: */
        string print_ascending(){
            string of;
            auto tempNode = Node::down->getHead();
            int tempID = tempNode->getMatchID();
            while(tempNode->getNext() != nullptr){
                of += "footballer Name: " + Node::getFootballerName() + ",Match ID: " + to_string(tempNode->getMatchID()) + "\n";
                while(tempID==tempNode->getMatchID() && tempNode->getNext() != nullptr){
                    tempNode = tempNode->getNext();
                }
                tempID = tempNode->getMatchID();
            }
            if(tempNode->getPrev()->getMatchID() != tempID)
                of += "footballer Name: " + Node::getFootballerName() + ",Match ID: " + to_string(tempNode->getMatchID()) + "\n";
            return of;
        }
        /*Returns a string of matches of a footballer in descending order: */
        string print_descending(){
            string of;
            auto tempNode = Node::down->getTail();
            int tempID = tempNode->getMatchID();
            while(tempNode->getPrev() != nullptr){
                of += "footballer Name: " + Node::getFootballerName() + ",Match ID: " + to_string(tempNode->getMatchID());
                if(tempNode->getMatchID() != Node::down->getHead()->getMatchID()){of+="\n";}
                while(tempID==tempNode->getMatchID() && tempNode->getPrev() != nullptr){
                    tempNode = tempNode->getPrev();
                }
                tempID = tempNode->getMatchID();
            }
            if(tempNode->getNext()->getMatchID() != tempID)
                of += "footballer Name: " + Node::getFootballerName() + ",Match ID: " + to_string(tempNode->getMatchID());
            return of;
        }
        
                /*Checks whether a given footballer has hat-trick or not: */
        bool has_hattrick(){
            auto tempNode = Node::getDown()->getHead();
            while(tempNode->getNext() != nullptr){
                if(tempNode->getNext()->getNext()!= nullptr){
                    if(tempNode->getNext()->getNext()->getMatchID() == tempNode->getMatchID()){
                        return true;
                    }
                }
                tempNode = tempNode->getNext();
            }
            return false;
        }

        Node(){
                Node::footballerName = "";
                Node::teamName = "";
                Node::next = nullptr;
                Node::down = nullptr;}

        Node(string footballerName, string teamName) :
                footballerName(move(footballerName)), teamName(move(teamName)){
            Node::next = nullptr;
            Node::down = nullptr;}
};


/*Linked List defined: */
class LinkedList{
    private:
        Node *head;
        int size;
    public:
    Node *getHead() const {
        return head;
    }

    void setHead(Node *head) {
        LinkedList::head = head;
    }

    int getSize() const {
        return size;
    }

    void setSize(int size) {
        LinkedList::size = size;
    }
	/*Inserts a node to linked list in ascending order: */
    void insert(Node *node_inserted, doubleNode *match, doubleNode *matchUnsorted){
            Node *current;
            Node *cut;
                /*If there is no node yet.*/
            if(head == nullptr){
                node_inserted->setDown(new DoubleLinkedList(match));
                //node_inserted->getDown()->insert(match);
                LinkedList::head = node_inserted;
                LinkedList::setSize(LinkedList::size+1);
                //cout<<"first node inserted"<< endl;
            }

                /*If inserted footballer name is less than first node*/
            else if(node_inserted->getFootballerName().compare(head->getFootballerName())<0){
                node_inserted->setNext(head);
                node_inserted->setDown(new DoubleLinkedList(match));
                //node_inserted->getDown()->insert(match);
                LinkedList::head = node_inserted;
                LinkedList::setSize(LinkedList::size+1);
                //cout <<"Smaller value than head is inserted"<< endl;
            }


            else{
            current = head;
            cut = current;

                while(node_inserted->getFootballerName().compare(current->getFootballerName()) > 0 && current->getNext() != nullptr){
                    cut = current;
                    current = current->getNext();
                }
                    /*Inserted footballer name is already in the list, so insert it to list of matches*/
                if(current->getFootballerName() == node_inserted->getFootballerName()){
                    current->getDown()->insert(match);
                    current->getDown()->insertUnsorted(matchUnsorted);
                }
                    /*Inserted footballer name is greater than all elements in the list*/
                else if(node_inserted->getFootballerName().compare(current->getFootballerName()) > 0
                        && current->getNext() == nullptr){
                    node_inserted->setNext(nullptr);
                    node_inserted->setDown(new DoubleLinkedList(match));
                    current->setNext(node_inserted);
                    LinkedList::setSize(LinkedList::size+1);
                }
                    /*Inserted footballer name is neither the smallest nor the greatest*/
                else{
                    node_inserted->setDown(new DoubleLinkedList(match));
                    cut->setNext(node_inserted);
                    node_inserted->setNext(current);
                    LinkedList::setSize(LinkedList::size+1);
                }
            }

        }
        
        /*Prints elements of linked list, i.e. list of footballers: */
        string print_list(){
            string of;
            auto tempNode = LinkedList::getHead();
            while(tempNode->getNext() != nullptr){
                of += tempNode->getFootballerName() + "\n";
                tempNode = tempNode->getNext();
            }
            of += tempNode->getFootballerName() + "\n";
            return of;
        }
        
        /*Returns a string of the players who scored a hat-trick*/
        string print_hattrick() {
            string of;
            auto tempNode = LinkedList::getHead();
            while(tempNode->getNext() != nullptr){
                if(tempNode->has_hattrick())
                    of+=tempNode->getFootballerName()+"\n";
                tempNode = tempNode->getNext();
            }
            if(tempNode->has_hattrick())
                of+=tempNode->getFootballerName()+"\n";
            return of;
        }
	/*Searchs a player with given string in the list and returns its pointer: */
        Node* search_by_name(const string &name){
            auto tempNode = LinkedList::getHead();
            while(tempNode->getNext() != nullptr){
                if(tempNode->getFootballerName() == name)
                    return tempNode;
                tempNode = tempNode->getNext();
            }
            if(tempNode->getFootballerName() == name)
                return tempNode;
            else
                return nullptr;
        }

        LinkedList(){
            LinkedList::head = nullptr;
            LinkedList::size = 0;
        }
};

int main(int argc, char* argv[]) {

    int half_1 = 0;
    int half_2 = 0;
    int cnt;
    int line_number = 1;
    string line;
    string command;
    string commandsplit;
    string splitted;
    vector <string> tokens;
    vector <string> newtokens;
    vector <string> teams;
    vector <Node*> hat_trick;
    vector <Node*> most_scored;
    ifstream in_file (argv[1]);
    ifstream command_file(argv[2]);
    ofstream outfile;
    outfile.open(argv[3]);
    auto players = new LinkedList();
    if(in_file.is_open()){
        while(getline(in_file, line)){
            stringstream split(line);
            while(getline(split, splitted, ',')){ tokens.push_back(splitted); }
            (stoi(tokens[3])>45) ? half_2++ : half_1++;
            for(cnt=0; cnt<teams.size(); cnt++){ if(teams[cnt] == tokens[1]){ break; }}
            if(cnt == teams.size()){ teams.push_back(tokens[1]); }
            for(cnt=0; cnt<teams.size(); cnt++){ if(teams[cnt] == tokens[2]){ break; }}
            if(cnt == teams.size()){ teams.push_back(tokens[2]); }
            auto sNode = new Node(tokens[0], tokens[1]);
            auto dNode = new doubleNode(tokens[2], stoi(tokens[3]), stoi(tokens[4]));
            auto dUNode = new doubleNode(tokens[2], stoi(tokens[3]), stoi(tokens[4]));

            players->insert(sNode, dNode, dUNode);
            tokens.clear();
        }
    }
    else{
        outfile << "unable to open file: input.txt"<<endl;
    }

    auto tempNode = players->getHead();
    if(tempNode->getDown()->getSize()>=3)
        hat_trick.push_back(tempNode);
    most_scored.push_back(tempNode);
    while(tempNode->getNext() != nullptr){
        tempNode = tempNode->getNext();
        if(tempNode->getDown()->getSize()>=3)
            hat_trick.push_back(tempNode);

        if(most_scored[0]->getDown()->getSize() < tempNode->getDown()->getSize()){
            most_scored.clear();
            most_scored.push_back(tempNode);
        }
        else if(most_scored[0]->getDown()->getSize() == tempNode->getDown()->getSize()){
            most_scored.push_back(tempNode);
        }
        else{
            continue;
        }
    }

    outfile<<line_number++<<")THE MOST SCORED HALF"<< endl;
    if(half_1 == half_2){
        outfile<<"-1"<<endl;
        }
    else{    (half_1>half_2)?outfile<<"0"<<endl:outfile<<"1"<<endl; }

    outfile<<line_number++<<")GOAL SCORER"<<endl;
    while(!most_scored.empty()){
        outfile<<most_scored.front()->getFootballerName()<<endl;
        most_scored.erase(most_scored.begin());
    }

    outfile<<line_number++<<")THE NAMES OF FOOTBALLERS WHO SCORED HAT-TRICK"<<endl;
    outfile<<players->print_hattrick();

    outfile<<line_number++<<")LIST OF TEAMS"<<endl;
    while(!teams.empty()){
        outfile<<teams.front()<<endl;
        teams.erase(teams.begin());
    }

    outfile<<line_number++<<")LIST OF FOOTBALLERS"<<endl;
    outfile << players->print_list();

    if(command_file.is_open()) {
        while (getline(command_file, line, ',')) {
            stringstream split(line);
                while (getline(split, splitted, '\n')){
                    newtokens.push_back(splitted);
                }
        }

        outfile<<line_number++<<")MATCHES OF GIVEN FOOTBALLER"<<endl;
        outfile<<players->search_by_name(newtokens[0])->print_matches();
        outfile<<players->search_by_name(newtokens[1])->print_matches();
        outfile<<line_number++<<")ASCENDING ORDER ACCORDING TO MATCH ID"<<endl;
        outfile<<players->search_by_name(newtokens[2])->print_ascending();
        outfile<<players->search_by_name(newtokens[3])->print_ascending();
        outfile<<line_number++<<")DESCENDING ORDER ACCORDING TO MATCH ID"<<endl;
        outfile<<players->search_by_name(newtokens[4])->print_descending()<<endl;
        outfile<<players->search_by_name(newtokens[5])->print_descending();

    }
    else{
        outfile<<"Unable to open file \"operations.txt\""<<endl;
    }

    in_file.close();
    command_file.close();
    outfile.close();
    return 0;
}
