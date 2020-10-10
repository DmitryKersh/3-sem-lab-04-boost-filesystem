// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <boost/filesystem.hpp>

using boost::filesystem::path;
using boost::filesystem::directory_iterator;
using boost::filesystem::directory_entry;

#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using std::ostream;
using std::map;
using std::regex;
using std::string;
using std::vector;

static regex filename_pattern("balance_\\d{8}_\\d{8}\\.txt");

struct account_key{
  string broker;
  size_t account;
};

struct account_info{
  size_t number_of_files;
  size_t last_date;
};

map<string, vector<path>> read_contents(const path& dir_path);

void print_contents(const map<string, vector<path>>& content, ostream& os);

size_t get_date(const path& p);

size_t get_account(const path& p);

bool operator<(const account_key& lhs, const account_key& rhs);

map<account_key, account_info> make_infos(
    const map<string, vector<path>>& content);

void print_infos(const map<account_key, account_info>& infos, ostream& os);


#endif // INCLUDE_HEADER_HPP_
