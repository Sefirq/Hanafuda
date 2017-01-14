#include <algorithm>
#include <iostream>
#include <fstream>
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

class Points
{
public:
    int sanko = 0;
    int shiko = 0;
    int ame_shiko = 0;
    int goko = 0;
    int inoshikacho = 0;
    int tane = 0;
    int akatan = 0;
    int aotan = 0;
    int akatan_aotan_no_chofuku = 0;
    int tanzaku = 0;
    int tsukimi_zake = 0;
    int hanami_zake = 0;
    int kasu = 0;
    int sum_points();
    void clear_points();
};

int Points::sum_points() {
    return sanko + shiko + ame_shiko + goko + inoshikacho + tane + akatan + aotan + akatan_aotan_no_chofuku + tanzaku
    + tsukimi_zake + hanami_zake + kasu;
}

void Points::clear_points() {
    sanko = 0;
    shiko = 0;
    ame_shiko = 0;
    goko = 0;
    inoshikacho = 0;
    tane = 0;
    akatan = 0;
    aotan = 0;
    akatan_aotan_no_chofuku = 0;
    tanzaku = 0;
    tsukimi_zake = 0;
    hanami_zake = 0;
    kasu = 0;
}

class table
{
public:
    void add_card(card card1);
    card remove_card(unsigned long id);
    void print();
    string month_of_card(unsigned long index);
    unsigned long length();
    table();

protected:
    vector<card>* cards;
    vector<card>::iterator it;
};

void table::add_card(card card1) {
    cards->push_back(card1);
}

card table::remove_card(unsigned long id) {
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

string table::month_of_card(unsigned long index){
    return cards->at(index).month;
}

class player : public table
{
public:
    void add_to_acquired_cards(card card_to_be_added);
    void print_acquired_cards();
    bool check_for_sanko_or_shiko(bool sanko);
    bool check_for_ameshiko_or_goko(bool ameshiko);
    bool check_for_inoshikacho();
    bool check_for_tane_tanzaku_kasu(int points, int how_many_to_start_counting);
    bool check_for_akatan_or_aotan(bool akatan);
    bool check_for_akatan_aotan_no_chofuku();
    bool check_for_tsukimi_zake_or_hanami_zake(bool tsukimi_zake);
    player();
    Points player_points;

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

bool player::check_for_sanko_or_shiko(bool sanko){
    int counter_of_cards = 0;
    if(this->player_points.sanko)
        return false;
    else{
        for(it = acquired_cards->begin(); it != acquired_cards->end(); it++){
            string month = it->month, november = "listopad";
            if (it->points == 20 && month.compare(november)){
                counter_of_cards++;
            }
        }
        if(counter_of_cards == 3 && sanko){
            cout<<"You have made Sanko! +6 points!"<<endl;
            player_points.shiko = 6;
            return true;
        }
        else if(counter_of_cards == 4 && !sanko){
            cout<<"You have made Shiko! +8 points!"<<endl;
            player_points.sanko = 8;
            return true;
        }
        else
            return false;
    }
}

bool player::check_for_ameshiko_or_goko(bool ameshiko){
    int counter_of_cards = 0;
    if(this->player_points.sanko)
        return false;
    else{
        for(it = acquired_cards->begin(); it != acquired_cards->end(); it++){
            if (it->points == 20){
                counter_of_cards++;
            }
        }
        if(counter_of_cards == 4 && ameshiko){
            cout<<"You have made Ame-shiko! +7 points!"<<endl;
            player_points.ame_shiko = 7;
            return true;
        }
        else if(counter_of_cards == 5 && !ameshiko){
            cout<<"You have made Goko! +10 points!"<<endl;
            player_points.goko = 10;
            return true;
        }
        else
            return false;
    }
}

bool player::check_for_inoshikacho() {
    int counter_of_isc_cards = 0, counter_of_cards = 0, current_points_from_inoshikacho = player_points.inoshikacho;
    for(it = acquired_cards->begin(); it != acquired_cards->end(); it++){
        if(it->points == 10){
            counter_of_cards++;
            if(!it->month.compare("czerwiec") || !it->month.compare("lipiec") || it->month.compare("pazdziernik")){
                counter_of_isc_cards++;
            }
        }
    }
    if(counter_of_isc_cards == 3 && current_points_from_inoshikacho < counter_of_cards + 2){
        cout << "You have made Inoshikacho! +" << 2 + counter_of_cards << " points!"<<endl;
        player_points.inoshikacho = counter_of_cards + 2;
        return true;
    }
    else{
        return false;
    }
}

bool player::check_for_tane_tanzaku_kasu(int points, int how_many_to_start_counting){
    int counter_of_cards = 0;
    int current_points_from_tane = player_points.tane;
    int current_points_from_tanzaku = player_points.tanzaku;
    int current_points_from_kasu = player_points.kasu;

    for(it = acquired_cards->begin(); it != acquired_cards->end(); it++){
        if(it->points == points){
            counter_of_cards++;
        }
    }
    if(points == 10 && counter_of_cards >= how_many_to_start_counting && current_points_from_tane < counter_of_cards - 5){
        cout << "You have made Tane! +" << counter_of_cards - 5 << " points!" << endl;
        player_points.tane = counter_of_cards - 5;
        return true;
    }
    else if(points == 5  && counter_of_cards >= how_many_to_start_counting && current_points_from_tanzaku < counter_of_cards - 5){
        cout << "You have made Tanzaku! +" << counter_of_cards - 5 << " points!" << endl;
        player_points.tanzaku = counter_of_cards - 5;
        return true;
    }
    else if(points == 1  && counter_of_cards >= how_many_to_start_counting && current_points_from_kasu < counter_of_cards - 10){
        cout << "You have made Kasu! +" << counter_of_cards - 10 << " points!" << endl;
        player_points.kasu = counter_of_cards - 10;
        return true;
    }
    else {
        return false;
    }
}

bool player::check_for_akatan_or_aotan(bool akatan){
    int counter_of_cards = 0, counter_for_akatan_or_aotan = 0;
    int current_points_from_akatan = player_points.akatan;
    int current_points_from_aotan = player_points.aotan;

    for(it = acquired_cards->begin(); it != acquired_cards->end(); it++){
        if(it->points == 5){
            string month = it->month;
            counter_of_cards++;
            if(akatan && (!month.compare("styczen") || !month.compare("luty") || !month.compare("marzec"))){
                counter_for_akatan_or_aotan++;
            }
            else if(!akatan && (!month.compare("czerwiec") || !month.compare("wrzesien") || !month.compare("pazdziernik"))){
                counter_for_akatan_or_aotan++;
            }
        }
    }

    if(akatan && counter_for_akatan_or_aotan == 3 && current_points_from_akatan < counter_of_cards + 2){
        cout << "You have made Akatan! +" << counter_of_cards + 2 << " points!" << endl;
        player_points.akatan = counter_of_cards + 2;
        return true;
    }
    else if(!akatan && counter_for_akatan_or_aotan == 3 && current_points_from_aotan < counter_of_cards + 2){
        cout << "You have made Aotan! +" << counter_of_cards + 2 << " points!" << endl;
        player_points.aotan = counter_of_cards + 2;
        return true;
    }
    else{
        return false;
    }
}

bool player::check_for_akatan_aotan_no_chofuku(){
    int counter_of_cards = 0, current_points_from_akatan_aotan_no_chofuku = player_points.akatan_aotan_no_chofuku;
    int counter_of_cards_for_aanc = 0;
    for(it = acquired_cards->begin(); it != acquired_cards->end(); it++){
        if(it->points == 5){
            string month = it->month;
            counter_of_cards++;
            if(!month.compare("styczen") || !month.compare("luty") || !month.compare("marzec")
               || !month.compare("czerwiec") || !month.compare("wrzesien") || !month.compare("pazdziernik")){
                counter_of_cards_for_aanc++;
            }
        }
    }
    if(counter_of_cards_for_aanc == 6 && current_points_from_akatan_aotan_no_chofuku < counter_of_cards + 4){
        cout << "You have made Akatan, Aotan no Chofuku! +" << counter_of_cards + 4 << " points!" << endl;
        player_points.akatan_aotan_no_chofuku = counter_of_cards + 4;
        return true;
    }
    else{
        return false;
    }
}

bool player::check_for_tsukimi_zake_or_hanami_zake(bool tsukimi_zake){
    int counter_of_cards = 0;
    int current_points_from_tsukimi_zake = player_points.tsukimi_zake;
    int current_points_from_hanami_zake = player_points.hanami_zake;
    for(it = acquired_cards->begin(); it != acquired_cards->end(); it++){
        if(it->points == 10 && !it->month.compare("wrzesien")){
            counter_of_cards++;
        }
        if(tsukimi_zake && it->points == 20 && !it->month.compare("sierpien")){
            counter_of_cards++;
        }
        else if(!tsukimi_zake && it->points == 20 && !it->month.compare("marzec")){
            counter_of_cards++;
        }
    }
    if(counter_of_cards == 2){
        if(tsukimi_zake && !current_points_from_tsukimi_zake){
            cout << "You have made Tsukimi-zake! +5 points!" << endl;
            player_points.tsukimi_zake = 5;
            return true;
        }
        else if(!tsukimi_zake && !current_points_from_hanami_zake){
            cout << "You have made Hanami-zake! +5 points!" << endl;
            player_points.hanami_zake = 5;
            return true;
        }
        else{
            return false;
        }
    }
    return false;
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

bool check_if_has_new_points(player* playerx){
    bool change = playerx->check_for_sanko_or_shiko(true);
    change |= playerx->check_for_sanko_or_shiko(false); // that means that we are checking for shiko
    change |= playerx->check_for_ameshiko_or_goko(true);
    change |= playerx->check_for_ameshiko_or_goko(false); // we are checking for goko
    change |= playerx->check_for_inoshikacho();
    change |= playerx->check_for_tane_tanzaku_kasu(10, 5); // tane
    change |= playerx->check_for_tane_tanzaku_kasu(5, 5); // tanzaku
    change |= playerx->check_for_tane_tanzaku_kasu(1, 10); // kasu
    change |= playerx->check_for_akatan_or_aotan(true);
    change |= playerx->check_for_akatan_or_aotan(false); // aotan
    change |= playerx->check_for_akatan_aotan_no_chofuku();
    change |= playerx->check_for_tsukimi_zake_or_hanami_zake(true);
    change |= playerx->check_for_tsukimi_zake_or_hanami_zake(false);
    return change;
}

bool check_second_move(table* table1, player* player1, card card1, unsigned long chosen_card_from_table){
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
            bool if_has_new_points = check_if_has_new_points(player1);
            return true;
        }
        else{
            cout << "These cards are not from the same month" << endl;
            return false;
        }
    }
}

bool check_if_correct(table* table1, player* player1, unsigned long chosen_card_from_hand, unsigned long chosen_card_from_table){
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
    for(unsigned long i = 0 ; i < player1->length(); i++){
        string month = player1->month_of_card(i);
        for(unsigned long j = 0; j < table1->length(); j++){
            string month_from_table = table1->month_of_card(j);
            if(!month.compare(month_from_table)){
                return true;
            }
        }
    }
    return false;
}

bool try_to_discard(player* player1, table* table1, unsigned long index_to_be_discarded){
    if(index_to_be_discarded < 1 || index_to_be_discarded > player1->length()){
        cout<<"You don't have a card with such number, pick one from 1-"<<player1->length()<<")"<<endl;
        return false;
    }
    else{
        card discarded_card = player1->remove_card(index_to_be_discarded-1);
        table1->add_card(discarded_card);
        return true;
    }
}

bool check_if_any_move_possible_with_top_card(table* table1, card top_card){
    for(unsigned long i = 0; i < table1->length(); i++){
        string month = table1->month_of_card(i);
        if(!month.compare(top_card.month))
            return true;
    }
    return false;
}

bool after_acquiring_new_points(int points_now, int points_then) {
    string reply;
    cout << "Your score is now " << points_now << ", you scored  " << points_now - points_then << " points."
         << "Type koi-koi to continue or shobu to end this game." << endl;
    cin >> reply;
    cout << reply.compare("koi-koi") << endl;
    if (!reply.compare("shobu")) {
        cout << "You won!" << endl;
        return false;
    }
    return true;
}

bool players_move(bool* player1plays, player* playerx, table* table1, pile_of_cards* deck1) {
    unsigned long chosen_card_from_hand = 0, chosen_card_from_table = 0;
    bool if_correct_move = false, if_any_move_possible, if_has_new_points;
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
        int points_then = playerx->player_points.sum_points();
        if_has_new_points = check_if_has_new_points(playerx);
        int points_now = playerx->player_points.sum_points();
        if(if_has_new_points){
            if(!after_acquiring_new_points(points_now, points_then))
                return false;
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
            }
            else {
                int points_before_second_move = playerx->player_points.sum_points();
                if_correct_move = check_second_move(table1, playerx, top_card, chosen_card_from_table);
                int points_after_second_move = playerx->player_points.sum_points();
                if(points_after_second_move != points_before_second_move){
                    if(!after_acquiring_new_points(points_after_second_move, points_before_second_move)){
                        return false;
                    }
                }
            }
        }
    *player1plays = !*player1plays;
    return true;
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
                if_play = players_move(&whose_turn, player1, table_of_game, deck);
            }
            else{
                if_play = players_move(&whose_turn, player2, table_of_game, deck);
            }
            if(!if_play){
                if(whose_turn){
                    player2->player_points.clear_points();
                }
                else{
                    player1->player_points.clear_points();
                }
                cout << "Score is : " << player1->player_points.sum_points() << ":" << player2->player_points.sum_points()<<endl;
            }
        }
    }
    else{
        cout << "nie to nie" << endl;
    }
    cout << "KONIEC GRY" << endl;
    return 0;
}