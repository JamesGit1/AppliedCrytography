#include <iostream>
#include <fstream>
#include "authlib.h"
#include <sstream>
#include <stdio.h>
#include <vector> 
#include <string.h>

using namespace std;

#include "openssl/sha.h"

//Global variables for username and passwords cause it's easy
vector<string> user;
vector<string> hashedPasses;
static char buffer[65]; //Output buffer to be filled with hased version of entered password

#define BuffSize 1024

//Print Statement (Because we like python :)
void print(){cout << "Error" << endl;}

int checkFile(){ //Open file and translates to users and hashed passwords
  //opens file input stream
  ifstream hashedPass;
  hashedPass.open("3791570.txt");
  string line;

  if(hashedPass.is_open()){
    int count = 0;

    //Repeat till end of file
    while(getline(hashedPass,line)){
      vector<string> vect; //temporary vector for storing output of string split probably overkill but does the job
      stringstream ss(line);

      // While input stream isn't empty (i.e not end of line) look until next delimiter and add to vector
      while(ss.good()){
        string substr;
        getline(ss, substr, ':');
        vect.push_back(substr);
      }
      
      // We want first bit username and bit after : to be the hashed password
      user.push_back(vect[0]);
      hashedPasses.push_back(vect[1]);

      count++;
    }
  }
  else cout << "Failed to open file 3791570.txt";
  hashedPass.close();

  return 0;
}

//FROM stackoverflow example https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
void sha256_string(string in, char outputBuffer[65]) {
  char charpassEntered[256]; //Function takes array char so this used instead of straight string
  strcpy(charpassEntered, in.c_str()); 
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init( & sha256);
  SHA256_Update( & sha256, charpassEntered, strlen(charpassEntered));
  SHA256_Final(hash, & sha256);
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
  }
  outputBuffer[64] = 0;
}

int main() {
  /*
  //DO NOT OPEN
  //https://bit.ly/3k3gxAh
  */
  
  checkFile(); //initalise file into variables correctly

  // User gets 3 attempts to get user and password correct
  bool auth = false;
  int attempts=3;
  string userEntered;
  string passEntered;

  while(attempts>0 && !auth){
    try {
      cout << "Please enter your username: ";
      cin >> userEntered;
      cout << "Please enter your password: ";
      cin >> passEntered;

      sha256_string(passEntered, buffer); //Converts password to hash

      for (long unsigned int i=0; i < sizeof(user); i++) // Loop for length of user
      {
        if(userEntered==user[i] && buffer==hashedPasses[i]){ //Checks username and hashed version of entered password against assigned variables
          auth = true;
          break; // stop looking through list if found
        }
      }
    } 
    catch (const exception & e) { //Handles generic errors
      print();
    }

    attempts--;
    if (auth){
      authenticated(userEntered);
    } 
    else rejected(userEntered);

    if(!auth){
      cout << "Attempts left: " << attempts << "\n\n";
    }
  }
  return 0;
}
