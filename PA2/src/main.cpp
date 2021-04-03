#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

using namespace std;

void help_message() {
    cout << "how to compile: make" << endl;
    cout << "how to run: "
    "./bin/hash_password" << endl;
    cout << "how to clean: make clean" << endl;
}

void generate_dictionary(fstream &fin_password, fstream &fout_dictionary, vector<string> &dictionary, vector < vector < long int > > &hash_value_dictionary){
    string str;
    int nth_password = -1;
    while (getline(fin_password, str)) {
        
        nth_password++;

        //=== store the password into dictionary===//

        dictionary.push_back(str.substr(0, str.size() - 1));  // last char is " ", need to ignore it

        //===turn the password into ASCII===//

        string password_ascii_string;
        int char_ascii_int;
        string char_ascii_string;
        for (int i = 0; i < str.size() - 1; i++) {  // last char is " ", need to ignore it
            char_ascii_int = (int)str[i];
            char_ascii_string = to_string(char_ascii_int);
            password_ascii_string += char_ascii_string;
        }

        //=== add salt, compute hash_value, and store ===//

        for (int i = 0; i <= 9; i++) {
            string salt_password_ascii_string = "00" + to_string(i) + password_ascii_string;
            string left_salt_password_ascii_string = salt_password_ascii_string.substr(0, 8);
            string right_salt_password_ascii_string = salt_password_ascii_string.substr(8, 7);
            long int left_salt_password_ascii_int = stol(left_salt_password_ascii_string);
            long int right_salt_password_ascii_int = stol(right_salt_password_ascii_string);
            long int hash_value = ((243 * left_salt_password_ascii_int) + right_salt_password_ascii_int) % 85767489;
            hash_value_dictionary[nth_password].push_back(hash_value);
        }

        for (int i = 10; i <= 99; i++) {
            string salt_password_ascii_string = "0" + to_string(i) + password_ascii_string;
            string left_salt_password_ascii_string = salt_password_ascii_string.substr(0, 8);
            string right_salt_password_ascii_string = salt_password_ascii_string.substr(8, 7);
            long int left_salt_password_ascii_int = stol(left_salt_password_ascii_string);
            long int right_salt_password_ascii_int = stol(right_salt_password_ascii_string);
            long int hash_value = ((243 * left_salt_password_ascii_int) + right_salt_password_ascii_int) % 85767489;
            hash_value_dictionary[nth_password].push_back(hash_value);
        }

        for (int i = 100; i <= 999; i++) {
            string salt_password_ascii_string = to_string(i) + password_ascii_string;
            string left_salt_password_ascii_string = salt_password_ascii_string.substr(0, 8);
            string right_salt_password_ascii_string = salt_password_ascii_string.substr(8, 7);
            long int left_salt_password_ascii_int = stol(left_salt_password_ascii_string);
            long int right_salt_password_ascii_int = stol(right_salt_password_ascii_string);
            long int hash_value = ((243 * left_salt_password_ascii_int) + right_salt_password_ascii_int) % 85767489;
            hash_value_dictionary[nth_password].push_back(hash_value);
        }
    }

    //===write the result to dictionary.txt===//

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 1000; j++) {
            fout_dictionary << dictionary[i] << " ";
            fout_dictionary << setfill('0') << setw(3) << j << " ";
            fout_dictionary << hash_value_dictionary[i][j] << '\n';
        }
    }
}

void search_hash_value(string hash_value_input, vector<string> &dictionary, vector < vector < long int > > &hash_value_dictionary) {
    
    //======//

    long int hash_value = stol(hash_value_input);
    bool found_flag = 0;

    //======//
    
    for (int i = 0; i < hash_value_dictionary.size(); i++) {
        for (int j = 0; j < hash_value_dictionary[i].size(); j++) {
            if (hash_value_dictionary[i][j] == hash_value) {
                found_flag = 1;
                cout << "The hash value is found!" << '\n' << "The recovered password is: " << dictionary[i] << '\n';
                cout << "The salt is: " << setfill('0') << setw(3) << j << '\n';
                cout << "The number of entries have been searched: " << (i+1) * (j+1) << '\n';
            }
        }
    }

    if (found_flag == 0) {
        cout << "Password recovery failed!" << '\n' << "No such hash valus exists!" << '\n';
        cout << "The number of entries have been searched: 100000" << '\n'; 
    }

    cout << '\n';
}

//==========main function==========//

int main(int argc, char *argv[]) {

    if (argc != 1) {
        help_message();
        return 0;
    }

    string password_file_name;
    cout << "Please enter the name of the password file: ";
    cin >> password_file_name;
    fstream fin_password("input/" + password_file_name);
    fstream fout_dictionary;
    fout_dictionary.open("output/Dictionary.txt", ios::out);

    vector<string> dictionary;
    vector < vector < long int > > hash_value_dictionary(100);

    generate_dictionary(fin_password, fout_dictionary, dictionary, hash_value_dictionary);
    cout << "password dictionary is generated" << '\n';

    //===warning! If add infinite loop, then Dictionary.txt can only be generated til "ZFXOML 786 4489773" for some unknown reason===//
        
        cout << "Please enter the hash value: ";
        string hash_value_input;
        cin >> hash_value_input;
        cout << '\n';
        search_hash_value(hash_value_input, dictionary, hash_value_dictionary);

    fin_password.close();
    fout_dictionary.close();
    return 0;
}