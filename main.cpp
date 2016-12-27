#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
class card
{
public:
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

class pile_of_cards
{
public:
    vector<card> pile;
    vector<card>::iterator it;
    void create_pile();
    void shuffle();
};

void pile_of_cards::create_pile() {
    it = pile.begin();
    string line;
    ifstream read_file;
    read_file.open("list_of_cards.txt");
    while(getline(read_file, line)){
        string tab[3];
        for(int i = 0; i < 3; i++) {
            tab[i] = line.substr(0, line.find("\t"));
            line = line.substr(line.find("\t") + 1, line.length() - line.find("\t"));
        }
        pile.push_back(card(stoi(tab[0], nullptr), tab[1], stoi(tab[2], nullptr)));
        cout<<"lol"<<endl;
    }
    read_file.close();
}

void pile_of_cards::shuffle() {

}

int main()
{
    bool if_play = false;
    char reply;
    card gru_4(2, "grudzien", 1);
    cout<<gru_4.id<<gru_4.month<<gru_4.points<<endl;
    cout<<"Czy chcesz zagrać w Hanafuda [T/N]?"<<endl;
    cin >> reply;
    if (reply == 'T'){
        if_play = true;
        pile_of_cards deck;
        deck.create_pile();
        for (deck.it = deck.pile.begin(); deck.it != deck.pile.end(); ++deck.it){
            cout<<deck.it->id<<" "<<deck.it->month<<" "<<deck.it->points<<endl;
        }
        cout<<"ok!"<<endl;
    }
    else{
        cout<<"nie to nie"<<endl;
    }
    return 0;
}