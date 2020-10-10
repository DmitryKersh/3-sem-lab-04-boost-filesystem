// Copyright 2018 Your Name <your_email>

#include <header.hpp>

map<string, vector<path>> read_contents(const path& dir_path) {
  map<string, vector<path>> content;
  directory_iterator iter(dir_path);

  for (directory_entry& entry : iter) {
    if (is_directory(entry)) {
      for (directory_entry& inner_entry : directory_iterator(entry)) {
        if (is_regular_file(inner_entry)) {
          string filename = inner_entry.path().filename().string();

          if (std::regex_match(filename.begin(), filename.end(),
                               filename_pattern)) {
            content[entry.path().filename().string()].push_back(
                inner_entry.path());
          }
        }
      }
    } /*else if (is_symlink(entry)){
      path link_path = read_symlink(entry);
      for (directory_entry& link_entry : directory_iterator(link_path)){
        if (is_regular_file(link_entry)) {
          string filename = link_entry.path().filename().string();

          if (std::regex_match(filename.begin(), filename.end(),
                               filename_pattern)) {
            content[entry.path().filename().string()].push_back(
                link_entry.path());
          }
        }
      }
    }*/
  }
  return content;
}

void print_contents(const map<string, vector<path>>& content, ostream& os) {
 /* os << "_________________\n"
        "| C O N T E N T |\n"
        "|_______________|"
     << std::endl;*/
  for (const auto& item : content) {
    for (const path& p : item.second) {
      os << item.first << " " << p.filename().string() << std::endl;
    }
  }
}

size_t get_date(const path& p) {
  //regex filename_pattern("balance_\\d{8}_\\d{8}\\.txt");

  std::string s = p.string();
  if (!std::regex_match(s.begin(), s.end(), filename_pattern)) {
    throw std::runtime_error("Wrong filename format! Can't get date!");
  }
  //                  |       |
  // balance_00000000_00000000.txt //
  return std::stol(s.substr(17, 8));
}

size_t get_account(const path& p) {
  //regex filename_pattern("balance_\\d{8}_\\d{8}\\.txt");

  std::string s = p.string();
  if (!std::regex_match(s.begin(), s.end(), filename_pattern)) {
    throw std::runtime_error("Wrong filename format! Can't get account!");
  }
  //         |       |
  // balance_00000000_00000000.txt //
  return std::stol(s.substr(8, 8));
}

bool operator<(const account_key& lhs, const account_key& rhs) {
  if (lhs.broker != rhs.broker) {
    return lhs.broker < rhs.broker;
  }
  return lhs.account < rhs.account;
}

map<account_key, account_info> make_infos(
    const map<string, vector<path>>& content) {
  map<account_key, account_info> res;
  for (const auto& item : content) {
    for (const auto& file : item.second) {
      size_t acc = get_account(file.filename());

      res[{item.first, acc}].number_of_files++;

      if (get_date(file.filename()) > res[{item.first, acc}].last_date) {
        res[{item.first, acc}].last_date = get_date(file.filename());
      }
    }
  }
  return res;
}

void print_infos(const map<account_key, account_info>& infos, ostream& os) {
  /*os << "_________________\n"
        "|    I N F O    |\n"
        "|_______________|"
     << std::endl;*/
  for (const auto& item : infos) {
    os << "broker:" << item.first.broker << " account:" << std::setw(8)
       << std::setfill('0') << item.first.account
       << " files:" << item.second.number_of_files
       << " last date:" << item.second.last_date << std::endl;
  }
}
