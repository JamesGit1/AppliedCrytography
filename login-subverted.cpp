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
string user[2];
string hashedPasses[2];

#define BuffSize 1024

int checkFile(){ //Open file and translates to users and hashed passwords
  //opens file input stream
  ifstream hashedPass;
  hashedPass.open("3791570.txt");
  string line;

  if(hashedPass.is_open()){
    int count = 0;

    //Repeat till end of file
    while(getline(hashedPass,line)){
      vector<string> vect; //tempory vector for storing output of string split probably overkill but does the job
      stringstream ss(line);

      // While input stream isn't empty look until next delimiter and add to vector
      while(ss.good()){
        string substr;
        getline(ss, substr, ':');
        vect.push_back(substr);
      }
      
      // We want first bit username and bit after : to be the hased password
      user[count] = vect[0];
      hashedPasses[count] = vect[1];

      count++;
    }
  }
  else cout << "Failed to open file 3791570.txt";
  
  hashedPass.close();

  //cout << "user1 = " << user[0] << "  pass1 = " << hashedPasses[0] <<"\n";
  //cout << "user2 = " << user[1] << "  pass2 = " << hashedPasses[1] <<"\n\n";
  
  return 0;
}

//FROM stackoverflow example https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
void sha256_string(char *string, char outputBuffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
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

  while(attempts>0 && !auth){
    string userEntered;
    string passEntered;

    cout << "Please enter your username: ";
    cin >> userEntered;
    cout << "Please enter your password: ";
    cin >> passEntered;

    static char buffer[65]; //Output buffer to be filled with hased version of entered password
    char charpassEntered[256]; //Function takes array char so this used instead of straight string
    strcpy(charpassEntered, passEntered.c_str()); //Converts string to char

    sha256_string(charpassEntered, buffer); //Converts password to hash

    for (long unsigned int i=0; i < sizeof(user); i++) // Loop for length of user
    {
      if(userEntered==user[i] && buffer==hashedPasses[i]){ //Checks username and hashed version of entered password against assigned variables
        auth = true;
        break; // stop looking through list if found
      }
    }

    if(!auth){
      cout << "TRY AGAIN, Attempts left: " << attempts << "\n\n";
    }

    attempts--;
  }

  
  if (auth){
    authenticated("user");
  } 
  else rejected("user");

  return 0;
}
