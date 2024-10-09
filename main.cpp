#include <iostream>
#include <string>
#include <list>
#include <ranges>
#include <random>

using namespace std;

size_t mod_pow(size_t a, size_t n, size_t m) {
    size_t result = 1;
    a %= m;
    while (n) {
        if (n % 2 == 1)
            result = (result * a) % m;

        a = (a * a) % m;
        n /= 2;
    }
    return result;
}

class StringSet {
    static constexpr size_t BUCKETS = 1e6 + 1;
    static constexpr size_t p = 29;

    vector<list<string>> hash_table;

    size_t hash_string(const string &s) {
        size_t sum = 0;
        for (int i = 0; i < s.size(); i++) {
            sum = (sum + (s[i] - 'a') * mod_pow(p, i, BUCKETS) % BUCKETS) % BUCKETS;
        }
        return sum;
    }

public:

    StringSet() : hash_table(BUCKETS) {}

    void add(const string &s) {
        if (contains(s)) {
            return;
        }
        auto hash = hash_string(s);
        hash_table[hash].push_back(s);
    }

    void remove(const string &s) {
        auto hash = hash_string(s);
        hash_table[hash].remove(s);
    }

    bool contains(const string &s) {
        auto hash = hash_string(s);
        return ranges::find(hash_table[hash], s) != hash_table[hash].end();
    }

    vector<string> get_all_palindromes() {
        vector<string> palindromes{};
        for (auto &bucket: hash_table) {
            for (auto &s: bucket) {
                size_t half_part = s.size() / 2;
                bool is_palindrome = true;
                for (size_t i = 0; i < half_part && is_palindrome; ++i) {
                    if (s[i] != s[s.size() - i - 1]) {
                        is_palindrome = false;
                    }
                }
                if (is_palindrome) {
                    palindromes.emplace_back(s);
                }
            }
        }
        return palindromes;
    }
};

random_device rd;
std::mt19937 gen(rd());

string generate_random_string(int length) {
    string result;
    string letters = "abcdefghijklmnopqrstuvwxyz";
    uniform_int_distribution<> dist(0, int(letters.size()) - 1);

    for (int i = 0; i < length; ++i) {
        result += letters[dist(gen)];
    }
    return result;
}

vector<string> generate_random_strings(int count) {
    int max_length = 15;
    vector<std::string> strings;
    for (int i = 0; i < count; ++i) {
        uniform_int_distribution<> str_length(1, max_length);
        int length = str_length(gen);
        strings.push_back(generate_random_string(length));
    }
    return strings;
}


void set_input(StringSet &ss) {
    char operation;
    string str;
    while (cin >> operation) {
        if (operation == '#') {
            break;
        }
        cin >> str;

        switch (operation) {
            case '+':
                ss.add(str);
                break;
            case '-':
                ss.remove(str);
                break;
            case '?':
                if (ss.contains(str)) {
                    cout << "yes" << "\n";
                } else {
                    cout << "no" << "\n";
                }
                break;
            default:
                break;
        }
    }
}


int main() {
    StringSet ss;
    int count = 1e6;
    vector<string> test_set = generate_random_strings(count);
    for (int i = 0; i < count; ++i) {
        ss.add(test_set[i]);
    }
    set_input(ss);

    vector<string> palindromes = ss.get_all_palindromes();
    for (const auto &palindrome: palindromes) {
        cout << palindrome << '\n';
    }

    return 0;
}
