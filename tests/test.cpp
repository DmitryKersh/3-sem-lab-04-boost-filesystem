// Copyright 2018 Your Name <your_email>

#include <gtest/gtest.h>

#include <header.hpp>

TEST(GetDate, MinorFunctions) {
  EXPECT_THROW(get_date(path("Balance_00000000_00000000.txt")), runtime_error);
  EXPECT_THROW(get_date(path("balance_002130_00000000.txt")), runtime_error);
  EXPECT_THROW(get_date(path("balance_00OOO000_00000000.txt")), runtime_error);
  EXPECT_THROW(get_date(path("balance_00000000_00000000.old.txt")),
               runtime_error);

  EXPECT_EQ(get_date(path("balance_00321200_00332220.txt")), 332220);
  EXPECT_EQ(get_date(path("balance_00321200_12345678.txt")), 12345678);
  EXPECT_EQ(get_date(path("balance_00321200_11111111.txt")), 11111111);
}

TEST(GetAccount, MinorFunctions) {
  EXPECT_THROW(get_account(path("Balance_00000000_00000000.txt")),
               runtime_error);
  EXPECT_THROW(get_account(path("balance_00213023_02220.txt")),
               runtime_error);
  EXPECT_THROW(get_account(path("balance_00OOO000_00000000.txt")),
               runtime_error);
  EXPECT_THROW(get_account(path("balance_00000000_00123400.old.txt")),
               runtime_error);

  EXPECT_EQ(get_account(path("balance_00000001_00000000.txt")), 1);
  EXPECT_EQ(get_account(path("balance_00321200_02332220.txt")), 321200);
  EXPECT_EQ(get_account(path("balance_12312312_02332220.txt")), 12312312);
}

TEST(Operators, MinorFunctions) {
  // in ascending order
  account_key ak1 = {"broker1", 11111111};
  account_key ak2 = {"broker1", 12345678};
  account_key ak3 = {"broker2", 12345678};
  account_key ak4 = {"broker3", 87654321};
  account_key ak5 = ak1;

  account_info ai1 = {12, 12345};
  account_info ai2 = {12345, 12};
  account_info ai3 = {12, 12345};

  EXPECT_TRUE(ak1 < ak2);
  EXPECT_TRUE(ak2 < ak3);
  EXPECT_TRUE(ak3 < ak4);
  EXPECT_FALSE(ak1 < ak1);

  EXPECT_TRUE(ak1 == ak5);
  EXPECT_TRUE(ak3 == ak3);
  EXPECT_FALSE(ak1 == ak2);

  EXPECT_TRUE(ai1 == ai1);
  EXPECT_TRUE(ai1 == ai3);
  EXPECT_FALSE(ai1 == ai2);
}

TEST(AllInOne, MajorFunctions) {
  map<string, set<path>> test_map_expected = {
      // not sorted there
      {"broker1",
       {"balance_03934523_20180927.txt", "balance_03934520_20181004.txt",
        "balance_03934520_20180928.txt"}},
      {"broker2",
       {"balance_00100002_20180904.txt", "balance_00100002_20180905.txt"}},
      {"broker3",
       {"balance_00100002_20181003.txt", "balance_00100001_20180917.txt",
        "balance_00100003_20180918.txt", "balance_00100003_20180913.txt"}}};

  // map<string, set<path>> test_map_got = read_contents("./../ftp");

  // EXPECT_EQ(test_map_expected, test_map_got);

  ostringstream expected(
      "broker1 balance_03934520_20180928.txt\n"
      "broker1 balance_03934520_20181004.txt\n"
      "broker1 balance_03934523_20180927.txt\n"
      "broker2 balance_00100002_20180904.txt\n"
      "broker2 balance_00100002_20180905.txt\n"
      "broker3 balance_00100001_20180917.txt\n"
      "broker3 balance_00100002_20181003.txt\n"
      "broker3 balance_00100003_20180913.txt\n"
      "broker3 balance_00100003_20180918.txt\n");

  ostringstream got_contents;
  print_contents(test_map_expected, got_contents);
  EXPECT_EQ(got_contents.str(), expected.str());

  map<account_key, account_info> test_infos = {
      {{"broker1", 3934520}, {2, 20181004}},
      {{"broker1", 3934523}, {1, 20180927}},
      {{"broker2", 100002}, {2, 20180905}},
      {{"broker3", 100001}, {1, 20180917}},
      {{"broker3", 100002}, {1, 20181003}},
      {{"broker3", 100003}, {2, 20180918}}};

  EXPECT_EQ(make_infos(test_map_expected), test_infos);

  ostringstream got_infos;
  ostringstream expected_infos(
      "broker:broker1 account:03934520 files:2 last date:20181004\n"
      "broker:broker1 account:03934523 files:1 last date:20180927\n"
      "broker:broker2 account:00100002 files:2 last date:20180905\n"
      "broker:broker3 account:00100001 files:1 last date:20180917\n"
      "broker:broker3 account:00100002 files:1 last date:20181003\n"
      "broker:broker3 account:00100003 files:2 last date:20180918\n");

  print_infos(test_infos, got_infos);

  EXPECT_EQ(got_infos.str(), expected_infos.str());
}
