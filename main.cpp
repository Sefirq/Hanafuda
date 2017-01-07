#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <set>
#include <unordered_set>
using namespace std;

int myrandom (int i) { return std::rand()%i;}

class card
{
public:
    void print();
    int id;
    string month;
    int points;
    card(int, string, int);
};

card::card(int id_of_card, string month_of_card, int points_of_card)
{
    id = id_of_card;
    month = month_of_card;
    points = points_of_card;
}

void card::print() {
    cout << id <<" "<< month << points << endl;
}

class table
{
public:
    void add_card(card card1);
    card remove_card(int id);
    void print();
    string month_of_card(int index);
    unsigned long length();
    table();

protected:
    vector<card>* cards;
    vector<card>::iterator it;
};

void table::add_card(card card1) {
    cards->push_back(card1);
}

card table::remove_card(int id) {
    card card_to_be_erased = cards->at(id);
    cards->erase(cards->begin()+id);
    return card_to_be_erased;
}

table::table(){
    cards = new vector<card>;
}
void table::print(){
    for (it = cards->begin(); it != cards->end(); ++it){
        cout<<it->id<<" "<<it->month<<" "<<it->points<<" || ";
    }
    cout<<endl;
}

unsigned long table::length() {
    return cards->size();
}

string table::month_of_card(int index){
    return cards->at(index).month;
}

class player : public table
{
public:
    void add_to_acquired_cards(card card_to_be_added);
    void print_acquired_cards();
    player();
private:
    vector<card>* acquired_cards;
};

player::player(){
    acquired_cards = new vector<card>;
}

void player::add_to_acquired_cards(card card_to_be_added) {
    acquired_cards->push_back(card_to_be_added);
}

void player::print_acquired_cards() {
    for (it = acquired_cards->begin(); it != acquired_cards->end(); ++it){
        cout<<it->id<<" "<<it->month<<" "<<it->points<<" || ";
    }
    cout<<endl;
}

class pile_of_cards : public table
{
public:
    void create_pile();
    void shuffle();
    card pop_a_card();
};

void pile_of_cards::create_pile() {
    it = cards->begin();
    string line;
    ifstream read_file;
    read_file.open("list_of_cards.txt");
    while(getline(read_file, line)){
        string tab[3];
        for(int i = 0; i < 3; i++) {
            tab[i] = line.substr(0, line.find("\t"));
            line = line.substr(line.find("\t") + 1, line.length() - line.find("\t"));
        }
        cards->push_back(card(stoi(tab[0], nullptr), tab[1], stoi(tab[2], nullptr)));
        cout<<"lol"<<endl;
    }
    read_file.close();
}

void pile_of_cards::shuffle() {
    random_shuffle(cards->begin(), cards->end(), myrandom);
}


card pile_of_cards::pop_a_card() {
    card card_to_return = cards->front();
    cards->erase(cards->begin());
    return card_to_return;
}

void first_deal(pile_of_cards* deck, player* player1, player* player2, table* table1){
    for(int i  = 0; i < 4; i++){
        player1->add_card(deck->pop_a_card());
        player1->add_card(deck->pop_a_card());
        player2->add_card(deck->pop_a_card());
        player2->add_card(deck->pop_a_card());
        table1->add_card(deck->pop_a_card());
        table1->add_card(deck->pop_a_card());
    }
}

bool check_second_move(table* table1, player* player1, card card1, int chosen_card_from_table){
    if(chosen_card_from_table < 1 || chosen_card_from_table > table1->length()){
        cout<<"There is no such card on the table. Pick one from 1-"<<table1->length()<<")"<<endl;
        return false;
    }
    else{
        string month_of_card = card1.month;
        string month_of_chosen_card = table1->month_of_card(chosen_card_from_table-1);
        cout<<month_of_card << " " << month_of_chosen_card << endl;
        if(!month_of_card.compare(month_of_chosen_card)){
            cout<<"That's a proper move!"<<endl;
            player1->add_to_acquired_cards(card1);
            card card_acquired = table1->remove_card(chosen_card_from_table-1);
            player1->add_to_acquired_cards(card_acquired);
            player1->print_acquired_cards();
            return true;
        }
        else{
            cout << "These cards are not from the same month" << endl;
            return false;
        }
    }
}

bool check_if_correct(table* table1, player* player1, int chosen_card_from_hand, int chosen_card_from_table){
    cout << chosen_card_from_hand << " " << chosen_card_from_table << endl;
    if(chosen_card_from_hand < 1 || chosen_card_from_hand > player1->length()){
        cout << "You don't have a card with such number, pick one from 1-" << player1->length() << ")" << endl;
        return false;
    }
    else if(chosen_card_from_table < 1 || chosen_card_from_table > table1->length()){
        cout << "There is no such card on the table. Pick one from 1-" << table1->length() << ")" << endl;
        return false;
    }
    else{
        string month_of_players_card = table1->month_of_card(chosen_card_from_table-1);
        string month_of_card_from_table = player1->month_of_card(chosen_card_from_hand-1);
        cout << month_of_card_from_table<<" " << month_of_players_card << endl;
        if(!month_of_card_from_table.compare(month_of_players_card)){
            cout << "That's a proper move!" << endl;
            card card_acquired = table1->remove_card(chosen_card_from_table-1);
            player1->add_to_acquired_cards(card_acquired);
            card_acquired = player1->remove_card(chosen_card_from_hand-1);
            player1->add_to_acquired_cards(card_acquired);
            player1->print_acquired_cards();
            return true;
        }
        else{
            cout << "These cards are not from the same month" << endl;
            return false;
        }
    }
}

bool check_if_any_move_possible(table* table1, player* player1){
    for(int i = 0 ; i < player1->length(); i++){
        string month = player1->month_of_card(i);
        for(int j = 0; j < table1->length(); j++){
            string month_from_table = table1->month_of_card(j);
            if(!month.compare(month_from_table)){
                return true;
            }
        }
    }
    return false;
}

bool try_to_discard(player* player1, table* table1, int index_to_be_discarded){
    if(index_to_be_discarded < 1 || index_to_be_discarded > player1->length()){
        cout<<"You don't have a card with such number, pick one from 1-"<<player1->length()<<")"<<endl;
        return false;
    }
    else{
        card discarded_card = player1->remove_card(index_to_be_discarded);
        table1->add_card(discarded_card);
        return true;
    }
}

bool check_if_any_move_possible_with_top_card(table* table1, card top_card){
    for(int i = 0; i < table1->length(); i++){
        string month = table1->month_of_card(i);
        if(!month.compare(top_card.month))
            return true;
    }
    return false;
}

void players_move(bool* player1plays, player* playerx, table* table1, pile_of_cards* deck1) {
    int chosen_card_from_hand = 0;
    int chosen_card_from_table = 0;
    bool if_correct_move = false, if_any_move_possible;
    if(*player1plays) {
        cout << "Player 1's move!" << endl << "If you can't find a proper move, please type '0'" << endl;
        cout << "Player 1:" << endl;
    }
    else{
        cout << "Player 2's move!" << endl << "If you can't find a proper move, please type '0'" << endl;
        cout << "Player 2:" << endl;
    }
        playerx->print();
        cout << "On the table:" << endl;
        table1->print();
        while (!if_correct_move) {
            cout << "Choose cards from the same month - first your card, then card from the table (1-"
                 << table1->length() << ")" << endl;
            cin >> chosen_card_from_hand;
            if (!chosen_card_from_hand) {
                if_any_move_possible = check_if_any_move_possible(table1, playerx);
                if (if_any_move_possible) {
                    cout << "There is at least one possible move" << endl;
                    continue;
                } else {
                    cout << "No possible moves" << endl << "Choose a card to discard it to the table (1-"
                         << playerx->length() << ")" << endl;
                    if_correct_move = false;
                    while (!if_correct_move) {
                        cin >> chosen_card_from_hand;
                        if_correct_move = try_to_discard(playerx, table1, chosen_card_from_hand);
                    }
                    break;
                }
            }
            cin >> chosen_card_from_table;
            cout << chosen_card_from_hand << chosen_card_from_table << endl;
            if_correct_move = check_if_correct(table1, playerx, chosen_card_from_hand,
                                               chosen_card_from_table);
        }
        //now the 2nd move in a turn
        card top_card = deck1->pop_a_card();
        top_card.print();
        if_correct_move = false;
        while (!if_correct_move) {
            cout << "Try to find card from the same month as this one. If it is not possible, type '0'" << endl;
            table1->print();
            cout << "Type a number from 1 to " << table1->length() << endl;
            cin >> chosen_card_from_table;//
            if (!chosen_card_from_table) {
                if_any_move_possible = check_if_any_move_possible_with_top_card(table1, top_card);
                if (if_any_move_possible) {
                    cout << "There is at least one possible move" << endl;
                    continue;
                } else {
                    cout << "No possible moves" << endl << "Placing the card on the table..." << endl;
                    table1->add_card(top_card);
                    break;
                }
            }//
            else {
                if_correct_move = check_second_move(table1, playerx, top_card, chosen_card_from_table);
            }
        }
    cout<<*player1plays<<endl;
    *player1plays = !*player1plays;
    cout<<*player1plays<<endl;
}



int main()
{
    srand(unsigned (time(0)));
    bool if_play = false;
    char reply;
    cout<<"Czy chcesz zagrać w Hanafuda [T/N]?"<<endl;
    cin >> reply;
    if (reply == 'T'){
        if_play = true;
        pile_of_cards* deck = new pile_of_cards();
        deck->create_pile();
        cout<<"cus"<<endl;
        //deck->print();
        deck->shuffle();
        //deck->print();
        cout<<"ok!"<<endl;
        table* table_of_game = new table();
        player* player1 = new player;
        player* player2 = new player;
        first_deal(deck, player1, player2, table_of_game);
        bool whose_turn = true; // if true then Player1, else Player2
        while(if_play) {
            cout<<whose_turn<<endl;
            if (whose_turn) {
                //player 1's move
                players_move(&whose_turn, player1, table_of_game, deck);
            }
            else{
                players_move(&whose_turn, player2, table_of_game, deck);
            }
        }
    }
    else{
        cout << "nie to nie" << endl;
    }
    return 0;
}