#include "ladder.h"
#include <algorithm>

using namespace std;

void error(string word1, string word2, string msg) {
    cout << "Cannot create word ladder from " << word1 << " to " << word2 << ": " << msg << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    if (abs(static_cast<int>(str1.length()) - static_cast<int>(str2.length()))> d) {
        return false;
    }

    int len1 = str1.length();
    int len2 = str2.length();

    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1, 0));

    for (int i = 0; i <= len1; i++) 
        dp[i][0] = i;
    for (int j = 0; j <= len2; j++) 
        dp[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            if (tolower(str1[i-1]) == tolower(str2[j-1])) 
                dp[i][j] = dp[i-1][j-1];
            else 
                dp[i][j] = 1 + min(dp[i-1][j-1], min(dp[i-1][j], dp[i][j-1]));
        }
    }

    return dp[len1][len2] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    string start = begin_word;
    string end = end_word;
    transform(start.begin(), start.end(), start.begin(), ::tolower);
    transform(end.begin(), end.end(), end.begin(), ::tolower);

    if (start == end) {
        return {};
    }

    if (word_list.find(start) == word_list.end()) {
        error(begin_word, end_word, "start word not in list");
        return {};
    }
    if (word_list.find(end) == word_list.end()) {
        error(begin_word, end_word, "end word not in list");
        return {};
    }

    queue<vector<string>> ladder_queue;
    ladder_queue.push({start});

    set<string> visited;
    visited.insert(start);

    while (!ladder_queue.empty()) {
        auto current_ladder = ladder_queue.front();
        ladder_queue.pop();
        string last = current_ladder.back();

        if (last == end) 
            return current_ladder;
        
        vector<string> neighbors;

        for (size_t i = 0; i < last.size(); ++i) {
            string temp = last;
            for (char c = 'a'; c <= 'z'; ++c) {
                if (c == last[i]) continue;
                temp[i] = c;
                if (word_list.count(temp) && !visited.count(temp)) {
                    neighbors.push_back(temp);
                    visited.insert(temp); 
                }
            }
        }

        for (size_t i = 0; i < last.size(); ++i) {
            string temp = last.substr(0, i) + last.substr(i + 1);
            if (word_list.count(temp) && !visited.count(temp)) {
                neighbors.push_back(temp);
                visited.insert(temp);
            }
        }

        for (size_t i = 0; i <= last.size(); ++i) {
            for (char c = 'a'; c <= 'z'; ++c) {
                string temp = last.substr(0, i) + c + last.substr(i);
                if (word_list.count(temp) && !visited.count(temp)) {
                    neighbors.push_back(temp);
                    visited.insert(temp);
                }
            }
        }

        sort(neighbors.begin(), neighbors.end());
        
        for (const string& neighbor : neighbors) {
            vector<string> new_ladder = current_ladder;
            new_ladder.push_back(neighbor);
            if (neighbor == end) 
                return new_ladder;
            
            ladder_queue.push(new_ladder);
        }
    }

    error(begin_word, end_word, "no ladder found");
    return {};
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << file_name << endl;
        return;
    }

    string word;
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        word_list.insert(word);
    }

    file.close();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }

    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i < ladder.size() - 1) 
            cout << " ";
    }
    cout << endl;
}

#define my_assert(e) { cout << #e << ((e) ? " passed" : " failed") << endl; }

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}