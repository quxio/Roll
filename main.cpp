#include <iostream>
#include <getopt.h>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <limits>
#include <iterator>
#include <memory>

static void version() { std::cerr << "Roll version 0.1.1\n"; }
static void help() {
    std::cerr << "Usage:\n"
                 "  cointoss / roll -c             |  print HEADS/TAILS\n"
                 "  roll <number>                  |  roll a d<number>\n"
                 "  roll [-n <amount>] <items>     |  sample n items (1) from the list\n"
                 "  roll [-n <amount>] [-d delim]  |  sample n items from stdin\n"
                 "Options:\n"
                 "  -a    (sample mode) only shuffle the items\n"
                 "  -z    (die mode) zero-index the die\n";
}

static void printSampleStdin(std::mt19937& mt, const char delim, const long long k) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    auto rejectTrim { [delim](std::string& str){
            if (str.empty()||str.front()=='\0'||str.front()=='\n') { return true; }
            while (str.back()==delim||str.back()=='\n') { str.pop_back(); }
            return false;
        }};

    std::vector<std::string> select{}; select.reserve(std::min(k, 1024ll));

    std::size_t i{0};
    std::uniform_int_distribution<std::size_t> dist;
    std::string in; while (delim ? std::getline(std::cin, in, delim) : std::cin >> in) {
        if (rejectTrim(in)) { continue; }
        if (i < k) { select.push_back(in); }
        else {
            dist.param( std::uniform_int_distribution<std::size_t>::param_type {0, i});
            if (auto idx{ dist(mt) }; idx < k) { select[idx] = in; }
        }
        ++i;
    }

    std::shuffle(select.begin(), select.end(), mt);

    for (const auto& str : select) { std::cout << str << '\n'; }
}

static void printSampleArray(std::mt19937& mt, std::vector<const char *>& ar, const long long k) {
    if (k >= ar.size()) {
        std::shuffle(ar.begin(), ar.end(), mt);
        for (const auto str : ar) { std::cout << str << '\n'; }
        return;
    }
    std::vector<std::string_view> sample;
    std::sample(ar.begin(), ar.end(), std::back_inserter(sample), k, mt);
    for (const auto& str : sample) { std::cout << str << '\n'; }
}

static void printToss(std::mt19937& mt) {
    std::cout << ( mt() & 1 ? "HEADS":"TAILS" );
}

static void printDie(std::mt19937& mt, long long sides, bool zero) {
    if (zero) {
        std::uniform_int_distribution dist{0ll, sides-1};
        std::cout << dist(mt) << '\n';
        return;
    }
    std::uniform_int_distribution dist{1ll, sides};
    std::cout << dist(mt) << '\n';
}

int main(int argc, char **argv) {

    std::mt19937 mt{std::random_device{}()};

    if (strcmp(argv[0], "cointoss")==0) { printToss(mt); return 0; }

    std::vector<const char *> list;
    long long amount{1};
    char delim{};
    bool zero{false};

    while (true) {
        int option_index{0};
        static struct option long_options[] {
            {"help", no_argument, nullptr, 'h'},
            {"version", no_argument, nullptr, 'v'},
        };
        int c{ getopt_long(argc, argv, "-n:d:chaz", long_options, &option_index) };
        if (c == -1 ) { break; }

        switch (c) {
            case 'n': {
                auto a{ std::strtoll(optarg, nullptr, 0) };
                if (a <= 0) { std::cerr << "roll: invalid amount argument"; return 1; }
                if (amount != std::numeric_limits<long long>::max()) { amount = a; }
                break;
            }
            case 'd':
                if (strlen(optarg) == 1) { delim = optarg[0]; break; }
                if (strcmp(optarg,"\\n")!=0) {
                    std::cerr << "roll: delimiter must be a single character or '\\n'\n"; return 1;
                }
                delim = '\n'; break;
            case 'a':
                amount = std::numeric_limits<long long>::max(); break;
            case 'c':
                printToss(mt); return 0;
            case 'z':
                zero = true; break;
            case 1:
                list.push_back(optarg); break;
            case 'h': version(); help(); return 0;
            case 'v': version(); return 0;
            case '?': help(); return 1;
            default: std::cerr << "roll: error parsing arguments"; return 1;
        }
    }

    if (list.size() == 1) {
        auto f{ list.front() };
        auto d{ std::strtoll(f, nullptr, 0) };
        if (d <= 0) { std::cerr << "roll: invalid number of die sides '" << f << "'\n"; return 1; }
        printDie(mt, d, zero);
        return 0;
    }

    if (list.empty()) { printSampleStdin(mt, delim, amount); return 0; }

    printSampleArray(mt, list, amount);

    return 0;
}