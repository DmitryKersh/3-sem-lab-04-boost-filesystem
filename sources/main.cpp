// Copyright 2020 Dmitry Kersh d.karpukhin@mail.ru

#include <header.hpp>

int main(int argc, char** argv){
  path path_to_ftp;

  if (argc >= 2){
    path_to_ftp = argv[1];
  } else {
    path_to_ftp = "./";
  }

  map<string, set<path>> content = read_contents(path_to_ftp);

  print_contents(content, std::cout);

  map<account_key, account_info> infos = make_infos(content);

  print_infos(infos, std::cout);



  return 0;
}
