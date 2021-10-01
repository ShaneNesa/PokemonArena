//PokemonArena
//A game which allows the user to choose 4 Pokemon and battle 24 enemies

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <cstdio>
using namespace std;

class Pokemon;
class Attack;

void battle();
void pokechoose();
void usermove();
void enemymove();
int userattack();

class Attack{
	friend Pokemon;
	private:
	string name;
	int cost;
	int damage;
	string special;

	public:
	Attack(string name, string cost, string damage, string special){
		this->name=name;
		this->cost=stoi(cost);
		this->damage=stoi(damage);
		this->special=special;
	}
	
	Attack(){
	}
	
	string getName(){
		return name;
	}
	
	int getCost(){
		return cost;
	}
	
	int getDamage(){
		return damage;
	}
	
	string getSpecial(){
		return special;
	}
		
};

class Pokemon{
	friend Attack;
	private:
		string name;
		int maxhp;
		int hp;
		int energy=50;
		string type;
		string weakness;
		string resistance;
		int attacknum;
		Attack * attacks;
		bool disable=false;
		bool stun=false;
	
	public:
	Pokemon(string line){
		stringstream liness(line);
		getline(liness, name, ',');
		string temphp = "";
		getline(liness, temphp, ',');
		hp = stoi(temphp);
		maxhp = hp;
		getline(liness, type, ',');
		getline(liness, resistance, ',');
		getline(liness, weakness, ',');
		getline(liness, temphp, ',');
		attacknum = stoi(temphp);
		attacks = new Attack[attacknum];
		
		string attName;
		string attCost;
		string attDamage;
		string attSpecial;
		for (int i=0; i<attacknum; i++){ //creates object for each attack
			getline(liness, attName, ',');
			getline(liness, attCost, ',');
			getline(liness, attDamage, ',');
			getline(liness, attSpecial, ',');
			attacks[i] = Attack(attName, attCost, attDamage, attSpecial);
		}	
	}
	
	Pokemon (){
	}

	bool operator == (const Pokemon &p) const{
		return !name.compare(p.getName());
	}
	
	string getName(){
		return name;
	}
	
	string getName() const{
		return name;
	}
	
	int getMaxHP(){
		return maxhp;
	}
	
	int getHP(){
		return hp;
	}
	
	string getType(){
		return type;
	}
	
	string getWeakness(){
		return weakness;
	}
	
	string getResistance(){
		return resistance;
	}
	
	Attack * getAttacks(){
		return attacks;
	}
	
	int getAttackNum(){
		return attacknum;
	}
	
	bool getStun(){
		return stun;
	}
	
	bool getDisable(){
		return disable;
	}
	
	int getEnergy(){
		return energy;
	}
	
	void setEnergy(int num){
		energy=num;
	}
	
	void setStun(bool value){
		stun=value;
	}
	
	void setHP(int num){
		hp=num;
	}
	
	string printStun(){
		if (stun){
			return "stunned,";
		}
		else{
			return "";
		}
	}
	
	string printDisable(){
		if (disable){
			return "disabled";
		}else{
			return "";
		}
	}
	
	//controls damage taken when hit by attack
	void recattack(Pokemon poke, Attack attack){
		
		srand (time(NULL));

		string special= attack.getSpecial();
		int damage= attack.getDamage();

		poke.setEnergy(poke.getEnergy()-attack.getCost());

		if (poke.getDisable()){
			damage-=10;
			if (damage<0){
				damage=0;
			}
		}
		
		if (!resistance.compare(poke.getType())){  //changes damage based on enemy pokemon type
			damage*=0.5;
		}else if (!weakness.compare(poke.getType())){
			damage*=2;
		}
		
		//takes care of special attacks
		if (!special.compare("stun")){
			if (rand() % 2 + 1 == 1){
				stun=true;	
			}
			hp-=damage;
			printf("\n%s used %s. %s took %d damage.\n",poke.getName().c_str(),attack.getName().c_str(),name.c_str(),damage);
			if (stun){
				cout << name << " got stunned!\n" << endl;
			}
		} else if (!special.compare("wild card")) {
			if (rand() % 2 + 1 == 1){
				hp-=damage;
				printf("\n%s used %s. %s took %d damage.\n",poke.getName().c_str(),attack.getName().c_str(),name.c_str(),damage);
			}
			else{
				printf("\n%s used %s. The move failed!\n",poke.getName().c_str(),attack.getName().c_str(),name.c_str(),damage);
			}
		} else if (!special.compare("wild storm")){
			int counter=0;
			while(rand() % 2 + 1 == 1 && hp>0){
				hp-=damage;
				counter+=1;
			}
			printf("\n%s used %s. It hit %d times! %s took %d damage.\n",poke.getName().c_str(),attack.getName().c_str(),counter,name.c_str(),damage*counter);
		} else if (!special.compare("disable")){
			disable=true;
			hp-=damage;
			printf("\n%s used %s. %s took %d damage.\n",poke.getName().c_str(),attack.getName().c_str(),name.c_str(),damage);
			printf("%s got disabled!\n",name.c_str());
		} else if (!special.compare("recharge")){
			poke.setEnergy(poke.getEnergy()+20);
			hp-=damage;
			printf("\n%s used %s. %s took %d damage.\n",poke.getName().c_str(),attack.getName().c_str(),name.c_str(),damage);
			cout << poke.getName() << " recovered 20 energy.\n" << endl;
		} else{
			hp-=damage;
			printf("\n%s used %s. %s took %d damage.\n",poke.getName().c_str(),attack.getName().c_str(),name.c_str(),damage);
		}
		
		if (hp<0){
			hp=0;
		}
	}
};

vector<Pokemon> userlist;// = *(new vector <Pokemon>());
vector<Pokemon> enemylist = *(new vector <Pokemon>());
Pokemon currpoke;
Pokemon currenemy;

int main() {
	srand (time(NULL));

	//creating objects from pokemon in file
	ifstream infile;
	infile.open("pokemon.txt");      //opening file
	if (!infile.is_open()){
	  //cout << "sdf" << endl;
	  return -1;
	}else{
	  //cout << "sdf" << endl;
	}
	int pokenum = 0;
	string pokenumBuffer = "";
	getline(infile, pokenumBuffer); //checking number of pokemon and creating array for it
	pokenum = stoi(pokenumBuffer);
	Pokemon * pokelist = new Pokemon [pokenum];
	
	//filling pokelist with pokemon objects
	string line = "";
	for (int i=0; i<pokenum; i++){		//iterating through pokelist
		getline(infile, line, '\n'); //setting 'line' to each line in file
		//cout << line << endl;
		pokelist[i]= *(new Pokemon(line));  //inserting new pokemon object into pokelist
		//cout << pokelist[max(i-5,0)].getName() << endl;
	}
	infile.close();
	
	//user choosing 4 pokemon
	
	int attemptChoice = 0;
	do{
		userlist.clear();
		cout << "Choose 4 Pokemon:" << endl;//display names
		for (int i = 0; i < 14; i++){
			string test = "aushdf";
			//printf("%s", test);
			printf("%2d. %-15s %d. %s\n",i+1, pokelist[i].getName().c_str() ,i+15,pokelist[i+14].getName().c_str());
			//string test = pokelist[i].getName();
			//cout << i+1 << ". " << test << endl;
		}
		
		for (int i=0; i<4; i++){
			while (userlist.size()<5){
				int attemptchoice = 0;
				string input = "";
				while (true) {
					cout << "Please enter a number." << endl;
					getline(cin, input);
					// This code converts from string to number safely.
					stringstream myStream(input);
					if (myStream >> attemptchoice)
						break;
				}
				if (attemptchoice>0 && attemptchoice<29){
					bool alreadyChose = false;
					for (int i = 0; i < userlist.size(); i++){
						if (!userlist[i].getName().compare(pokelist[attemptchoice-1].getName()) ){
							cout << "You already chose " << pokelist[attemptchoice-1].getName() << ". Pick a different Pokemon." << endl;
							alreadyChose = true;
							break;
						} 
					}
					if (!alreadyChose){
						userlist.push_back(pokelist[attemptchoice-1]);
						break;
					}
				}else{
					cout <<"Choose a number from 1 to 28." << endl;;
				}
			}	
		}
		
		cout << "Your team is:\n|-----------------------------------------------------------------------------|\n";
		for (int i = 0; i < userlist.size(); i++){
			Pokemon p = userlist[i];
			printf(" %-12sHP:%-5dType:%-12sWeakness:%-12sResistance:%-8s\n",p.getName().c_str(),p.getHP(),p.getType().c_str(),p.getWeakness().c_str(),p.getResistance().c_str());
			cout << "|-----------------------------------------------------------------------------|" << endl;
		}
		
		cout << "\nIs this okay?\n1. Yes\n2. No\n";
		attemptChoice = 0;
		while (true){
			string answer = "";
			getline(cin, answer);
			// This code converts from string to number safely.
			stringstream answerStream(answer);
			if (answerStream >> attemptChoice)
				break;
			cout << "type a number" << endl;
		}
		
	}while(attemptChoice!=1);
	
	for (int i = 0; i < pokenum; i++){	//make enemy pokemon list
		bool userPoke = false;
		for (int j = 0; j < userlist.size(); j++){
			if (!userlist[j].getName().compare(pokelist[i].getName())){
				userPoke = true;
				break;
			} 
		}
		if (!userPoke){
			enemylist.push_back(pokelist[i]);
		}
	}
	
	//choose starter pokemon and enemy
	int currenemynum = rand() % 24 + 1;
	currenemy = enemylist[currenemynum];
	cout << "\nThe enemy is " << enemylist[currenemynum].getName() << endl;
	cout << "Choose your Pokemon:\n|-------------------------------------------------------------------------------|\n";		
	pokechoose();
	
	//main game
	while(true){	
		battle();
		
		cout << currenemy.getName() << " fainted!\n" << endl;//will start battle with new enemy when previous one dies
		enemylist.erase(find(enemylist.begin(), enemylist.end(), currenemy));
		
		if (enemylist.size()>0){
			//Collections.shuffle(enemylist);
			currenemy=enemylist[0];
			
			if (userlist.size()>1){
				cout << currenemy.getName() << " is up next. Would you like to switch Pokemon?\n" << endl;
				cout << "1. Yes\n2. No\n"<< endl;
				int switchPokeAnswer = 0;
				cin >> switchPokeAnswer;
				if (switchPokeAnswer == 1){
					cout << "\nChoose a Pokemon:" << endl;
					cout << "\n|-------------------------------------------------------------------------------|" << endl;
					pokechoose();
				}
			}	
		}
		else{
			cout << "The enemy Pokemon have all been slaughtered! You Win!" << endl;
			return 0;
		}
	}
}

//battle method
void battle(){
	
	srand (time(NULL));
	
	bool userfirst=false;
	
	if (rand() % 2 + 1 == 1){//decide who attacks first
		userfirst=true;
	}
	
	while (currenemy.getHP()>0){
		currenemy.setEnergy(min(currenemy.getEnergy()+10,50));//increase 10 energy
		for (int i = 0; i < userlist.size(); i++){
			Pokemon p = userlist[i];
			p.setEnergy(min(p.getEnergy()+10,50));
		}
		
		//display scoreboard
		cout << "\n|---------------------------------------------------------------------|" << endl;
		printf(" You  | %-12s HP:%-5d Energy:%-5d Status:%s %-8s\n",currpoke.getName().c_str(),currpoke.getHP(),currpoke.getEnergy(),currpoke.printStun().c_str(),currpoke.printDisable().c_str());
		cout << "|---------------------------------------------------------------------|" << endl;
		printf(" Enemy| %-12s HP:%-5d Energy:%-5d Status:%s %-8s\n",currenemy.getName().c_str(),currenemy.getHP(),currenemy.getEnergy(),currenemy.printStun().c_str(),currenemy.printDisable().c_str());
		cout << "|---------------------------------------------------------------------|\n";
		
		if (userfirst){  //choose action
			usermove();
			if (currenemy.getHP()<=0){
				break;
			}
			enemymove();
		}else{
			enemymove();
			usermove();
		}
	}
	
	for (int i = 0; i < userlist.size(); i++){ //heal 20 hp
		Pokemon p = userlist[i];
		p.setHP(min(p.getHP()+20,p.getMaxHP()));
	}	
}

//user chooses his action
void usermove(){
	srand (time(NULL));
	
	if (currpoke.getStun()){     //user cant do anything if pokemon is stunned
		cout << currpoke.getName()+" was stunned! It coulnd't move.\n";
		currpoke.setStun(false);
		cout << currpoke.getName()+" is no longer stunned.";
		
	}else{
		cout << "\nWhat will you do?\n";
		cout << "1. Attack\n";
		cout << "2. Retreat\n";
		cout << "3. Pass\n";
		
		int currmove = 0;
		cin >> currmove;
		
		while (currmove<2 || currmove>3 || (currmove == 2 && userlist.size()<2)){
			bool pass=false;
			
			if (currmove<1 || currmove>3 ){
				cout << "Choose a number from 1 to 3.\n";
			}else if (currmove==2 && userlist.size()<2){
				cout << "You don't have any Pokemon left to call in! Choose a different option.\n";
			}else{
				for (int i = 0; i < currpoke.getAttackNum(); i++){
					Attack a = currpoke.getAttacks()[i];
					if (a.getCost()<=currpoke.getEnergy()){   //checks if user can attack
						pass=true;
						break;
					}
				}
			}
			if (pass!=true){
				if (currmove==1){
					cout << currpoke.getName() << " doesn't have enough energy to attack. Choose a different option.\n";
				}
				
			}else{
				if (currmove==1){
					int attnum=userattack();
					if (attnum!=0){ //allows user to stop attacking
						currenemy.recattack(currpoke, currpoke.getAttacks()[attnum-1]);
						break;
					}
					cout << "\nWhat will you do?\n" << endl;
					cout << "1. Attack" << endl;
					cout << "2. Retreat" << endl;
					cout << "3. Pass\n" << endl;
				}
			}
			cin >> currmove;
		}
		
		if(currmove==2){
			cout << "\nChoose a Pokemon:\n|-------------------------------------------------------------------------------|" << endl;
			pokechoose();
		}else if (currmove==3){
			cout << "\n" << currpoke.getName() << " did nothing." << endl;
		}
	}
}

//user chooses to attack
int userattack(){
	srand (time(NULL));

	cout << "\nChoose an attack (or press 0 to go back):" << endl;
	for (int i=0; i<currpoke.getAttackNum(); i++){
		Attack a =currpoke.getAttacks()[i];     //displaying attacks
		cout << i+1 << ". ";
		printf("%s  Cost:%d Damage:%d Special:%s\n\n",a.getName().c_str(),a.getCost(),a.getDamage(),a.getSpecial().c_str());
	}
	cout << "\n";
	
	int attackchoice; //allows user to stop attacking
	cin >> attackchoice;
	if (attackchoice==0){
		return attackchoice;
	}else{
		while (attackchoice<0 || attackchoice>currpoke.getAttackNum()){
			cout << "Choose a number from 1 to " << currpoke.getAttackNum() << ".\n";
			cin >> attackchoice;
		}
		
		Attack attack=currpoke.getAttacks()[attackchoice-1];
		
		while (attack.getCost() > currpoke.getEnergy()){	//checks if user can perform chosen attack
			cout << "\n" << currpoke.getName() << " doesn't have enough energy for that attack.\n" << endl;
			cout << "Choose a different attack:\n" << endl;
			for (int i=0; i<currpoke.getAttackNum(); i++){
				cout << i << 1 << ". ";
				cout << currpoke.getAttacks()[i].getName() << endl;
			}
			cout << "" << endl;
			cin >> attackchoice;
			while (attackchoice<0 || attackchoice>currpoke.getAttackNum()){
				cout << "Choose a number from 1 to " << currpoke.getAttackNum() << ".\n" << endl;
				cin >> attackchoice;
			}
			attack=currpoke.getAttacks()[attackchoice-1]; //if unable to do attack, choose different one
		}
		return attackchoice;
	}
	
}

//enemy's attack
void enemymove(){
	srand (time(NULL));
	bool pass=true;
	
	if (currenemy.getStun()){  //wont attack if stunned
		cout << currenemy.getName() << " is stunned! It can't move.\n" << endl;
		currenemy.setStun(false);
		
	}else{
		for (int i = 0; i < currenemy.getAttackNum(); i++){
			Attack a = currenemy.getAttacks()[i];
			if (a.getCost()<=currenemy.getEnergy()){ //wont attack if low energy
				pass=false;
				break;
			}
		}
		
		if (pass==false){
			Attack attack=currenemy.getAttacks()[(rand() % currenemy.getAttackNum())];
			while (attack.getCost()>currenemy.getEnergy()){
				attack=currenemy.getAttacks()[(rand() % currenemy.getAttackNum())];
			}
			currpoke.recattack(currenemy,attack);
		}else{
			cout << currenemy.getName() << " doesn't have enough energy to attack.\n" << endl;
		}
	}
	
	if (currpoke.getHP()<=0){ //checks if user died, allows to choose new pokemon
		if (userlist.size()>1){
			userlist.erase(find(userlist.begin(), userlist.end(), currpoke));
			cout << "\n" << currpoke.getName() << " fainted! Choose a new Pokemon:\n|-------------------------------------------------------------------------------|" << endl;
			pokechoose();
		}else{
			cout << currpoke.getName() << " fainted!" << endl;
			cout << "\nAll your Pokemon have fainted! You Lose!" << endl;
			exit(0);
		}
		
	}
}

//allows user to choose a pokemon to use
void pokechoose(){
	srand (time(NULL));
	
	for (int i=0; i<userlist.size(); i++){ //displays pokemon in party
		Pokemon p=userlist[i];
		printf(" %d.%-12sHP:%-5dType:%-12sWeakness:%-12sResistance:%-8s\n",i+1,p.getName().c_str(),p.getHP(),p.getType().c_str(),p.getWeakness().c_str(),p.getResistance().c_str());
		cout << "|-------------------------------------------------------------------------------|" << endl;
	}
	
	int attemptchoice = 0;
	while (true){
		attemptchoice = 0;
		string answer = "";
		getline(cin, answer);
		// This code converts from string to number safely.
		stringstream answerStream(answer);
		if (!(answerStream >> attemptchoice)){
			cout << "type a number" << endl;
		}else{
			if (attemptchoice<1 || attemptchoice > userlist.size()){
				cout << "\nChoose a number from 1 to " << userlist.size() << ".\n";
			}else{
				break;
			}
		
		}
	}
	currpoke=userlist[attemptchoice-1];
	cout << "\n" << currpoke.getName() << ", I choose you!" << endl;
}