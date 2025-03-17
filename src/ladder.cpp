#include "ladder.h"

void error(string word1, string word2, string msg)
    cout << "Cannot create word ladder from " << word1 << " to " << word2 << ": " << msg << endl;

bool edit_distance_within(const string& str1, const string& str2, int max_dist) {
    if (abs(static_cast<int>(str1.length()) - static_cast<int>(str2.length())) > max_dist)
        return false;
    
    if (str1.length() == str2.length()) {
        int diff_count = 0;
        for (size_t i = 0; i < str1.length(); i++) {
            if (tolower(str1[i]) != tolower(str2[i])) {
                diff_count++;
                if (diff_count > max_dist) {
                    return false;
                }
            }
        }
        return true;
    }
    
    const int len1 = str1.length();
    const int len2 = str2.length();
    
    vector<int> prev_row(len2 + 1);
    vector<int> curr_row(len2 + 1);
    
    for (int j = 0; j <= len2; j++) 
        prev_row[j] = j;
    
    bool possible = false;
    for (int i = 1; i <= len1; i++) {
        curr_row[0] = i;
        
        possible = false;
        
        for (int j = 1; j <= len2; j++) {
            if (tolower(str1[i-1]) == tolower(str2[j-1])) 
                curr_row[j] = prev_row[j-1];
            else 
                curr_row[j] = 1 + min(prev_row[j-1], min(prev_row[j], curr_row[j-1]));
            
            if (curr_row[j] <= max_dist) 
                possible = true;
        }
        
        if (!possible) 
            return false;
        
        swap(prev_row, curr_row);
    }
    
    return prev_row[len2] <= max_dist;
}

inline bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) 
        return vector<string>();

    string lower_begin = begin_word;
    string lower_end = end_word;
    for (char& c : lower_begin) c = tolower(c);
    for (char& c : lower_end) c = tolower(c);
    
    set<string> lowercase_word_set;
    for (const auto& word : word_list) {
        string lower_word = word;
        for (char& c : lower_word) c = tolower(c);
        lowercase_word_set.insert(lower_word);
    }
    
    if (lowercase_word_set.find(lower_begin) == lowercase_word_set.end() || 
        lowercase_word_set.find(lower_end) == lowercase_word_set.end()) {
        return vector<string>();
    }
    
    map<size_t, vector<string>> words_by_length;
    for (const auto& word : word_list)
        words_by_length[word.length()].push_back(word);
    
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    
    set<string> visited;
    visited.insert(lower_begin);
    
    int iterations = 0;
    const int MAX_ITERATIONS = 1000000;
    
    while (!ladder_queue.empty() && iterations < MAX_ITERATIONS) {
        iterations++;
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = current_ladder.back();
        string lower_last = last_word;
        for (char& c : lower_last) c = tolower(c);

        for (int len_diff = -1; len_diff <= 1; len_diff++) {
            size_t target_len = last_word.length() + len_diff;
            if (target_len == 0) continue;

            if (words_by_length.find(target_len) == words_by_length.end())
                continue;

            for (const string& candidate : words_by_length[target_len]) {
                string lower_candidate = candidate;
                for (char& c : lower_candidate) c = tolower(c);

                if (visited.find(lower_candidate) != visited.end()) 
                    continue;
                
                if (is_adjacent(last_word, candidate)) {
                    if (lower_candidate == lower_end) {
                        vector<string> result = current_ladder;
                        result.push_back(candidate);
                        return result;
                    }

                    visited.insert(lower_candidate);
                    vector<string> new_ladder = current_ladder;
                    new_ladder.push_back(candidate);
                    ladder_queue.push(new_ladder);
                }
            }
        }
    }

    return vector<string>();
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << file_name << endl;
        return;
    }
    
    string word;
    while (file >> word) {
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
    cout << " " << endl;
}

#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}

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