#include <gtest/gtest.h>

#include <sstream>

#include "BST.hpp"

TEST(BST, empty) {
  BST<int> bst1;
  EXPECT_TRUE(bst1.empty());
  BST<int> bst2 = {1, 2, 3};
  EXPECT_FALSE(bst2.empty());
}

TEST(BST, equal) {
  BST<int> bst1, bst2;
  EXPECT_TRUE(bst1 == bst2);
  bst1.insert(5);
  EXPECT_TRUE(bst1 != bst2);
  bst2.insert(5);
  EXPECT_TRUE(bst1 == bst2);
}

TEST(BST, insert) {
  BST<int> bst;
  bst.insert(4);
  bst.insert(2);
  bst.insert(6);
  bst.insert(1);
  bst.insert(3);
  bst.insert(5);
  bst.insert(7);
  EXPECT_FALSE(bst.empty());

  std::string expected = " 1 2 3 4 5 6 7";
  std::string actual;
  for (auto i = bst.begin(); i != bst.end(); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  expected = " 4 2 1 3 6 5 7";
  actual = "";
  for (auto i = bst.begin(Preorder()); i != bst.end(Preorder()); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  expected = " 1 3 2 5 7 6 4";
  actual = "";
  for (auto i = bst.begin(Postorder()); i != bst.end(Postorder()); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);
}

TEST(BST, insert_it) {
  BST<int> bst;
  std::vector<int> cont = {4, 2, 6, 1, 3, 5, 7};
  bst.insert(cont.begin(), cont.end());
  EXPECT_FALSE(bst.empty());

  std::string expected = " 1 2 3 4 5 6 7";
  std::string actual;
  for (auto i = bst.begin(); i != bst.end(); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  expected = " 4 2 1 3 6 5 7";
  actual = "";
  for (auto i = bst.begin(Preorder()); i != bst.end(Preorder()); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  expected = " 1 3 2 5 7 6 4";
  actual = "";
  for (auto i = bst.begin(Postorder()); i != bst.end(Postorder()); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);
}

TEST(BST, insert_range) {
  BST<int> bst;
  std::vector<int> cont = {4, 2, 6, 1, 3, 5, 7};
  bst.insert_range(cont);
  EXPECT_FALSE(bst.empty());

  std::string expected = " 1 2 3 4 5 6 7";
  std::string actual;
  for (auto i = bst.begin(); i != bst.end(); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  expected = " 4 2 1 3 6 5 7";
  actual = "";
  for (auto i = bst.begin(Preorder()); i != bst.end(Preorder()); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  expected = " 1 3 2 5 7 6 4";
  actual = "";
  for (auto i = bst.begin(Postorder()); i != bst.end(Postorder()); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);
}

TEST(BST, insert_iList) {
  BST<int> bst;
  bst.insert({4, 2, 6, 1, 3, 5, 7});
  EXPECT_FALSE(bst.empty());
  
  std::string expected = " 1 2 3 4 5 6 7";
  std::string actual;
  for (auto i = bst.begin(); i != bst.end(); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  expected = " 4 2 1 3 6 5 7";
  actual = "";
  for (auto i = bst.begin(Preorder()); i != bst.end(Preorder()); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  expected = " 1 3 2 5 7 6 4";
  actual = "";
  for (auto i = bst.begin(Postorder()); i != bst.end(Postorder()); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);
}

TEST(BST, init_List) {
  BST<int> bst = {4, 2, 6, 1, 3, 5, 7};
  EXPECT_FALSE(bst.empty());
  
  std::string expected = " 1 2 3 4 5 6 7";
  std::string actual;
  for (auto i = bst.begin(); i != bst.end(); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  expected = " 4 2 1 3 6 5 7";
  actual = "";
  for (auto i = bst.begin(Preorder()); i != bst.end(Preorder()); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  expected = " 1 3 2 5 7 6 4";
  actual = "";
  for (auto i = bst.begin(Postorder()); i != bst.end(Postorder()); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);
}

TEST(BST, erase) {
  BST<int> bst = {4, 2, 6, 1, 3, 5, 7, 1, 0};
  std::string expected = " 0 1 1 2 3 4 5 6 7";
  std::string actual;
  for (auto i = bst.begin(); i != bst.end(); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  bst.erase(1);
  expected = " 0 2 3 4 5 6 7";
  actual.clear();
  for (auto i = bst.begin(); i != bst.end(); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  bst.erase(bst.begin());
  expected = " 2 3 4 5 6 7";
  actual.clear();
  for (auto i = bst.begin(); i != bst.end(); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  bst.erase(bst.cbegin());
  expected = " 3 4 5 6 7";
  actual.clear();
  for (auto i = bst.begin(); i != bst.end(); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);

  bst.erase(bst.cbegin(), bst.cend());
  EXPECT_TRUE(bst.empty());
}

TEST(BST, find) {
  BST<int> bst1 = {4, 2, 6, 1, 3, 5, 7};
  EXPECT_EQ(bst1.find(1), bst1.begin());
  EXPECT_EQ(bst1.find(8), bst1.end());
  EXPECT_NE(bst1.find(5), bst1.begin());

  const BST<int> bst2 = {4, 2, 6, 1, 3, 5, 7};
  EXPECT_EQ(bst2.find(1), bst2.cbegin());
  EXPECT_EQ(bst2.find(8), bst2.cend());
  EXPECT_NE(bst2.find(5), bst2.cbegin());
}

TEST(BST, contains) {
  BST<int> bst = {4, 2, 6, 1, 3, 5, 7};
  EXPECT_TRUE(bst.contains(1));
  EXPECT_FALSE(bst.contains(8));
}

TEST(BST, extract) {
  BST<int> bst;
  bst.insert(5);
  EXPECT_EQ(bst.extract(5), 5);
  EXPECT_TRUE(bst.empty());
}

TEST(BST, swap) {
  BST<int> bst1 = {4, 2, 6, 1, 3, 5, 7};
  BST<int> bst2 = {1, 2, 3, 4, 5, 6, 7};
  bst1.swap(bst2);
  EXPECT_EQ(bst1, bst2);
  EXPECT_EQ(bst2, bst1);
}

TEST(BST, merge) {
  BST<int> bst1 = {4, 2, 6, 1, 3, 5, 7};
  BST<int> bst2 = {1, 2, 3, 4, 5, 6, 7};
  bst1.merge(bst2);
  EXPECT_NE(bst1, bst2);

  std::string expected = " 1 1 2 2 3 3 4 4 5 5 6 6 7 7";
  std::string actual;
  for (auto i = bst1.begin(); i != bst1.end(); ++i) {
	actual += ' ' + std::to_string(*i);
  }
  EXPECT_EQ(expected, actual);
}

TEST(BST, equal_range) {
  BST<int> bst = {4, 2, 6, 1, 3, 5, 7};
  auto [a, b] = bst.equal_range(3);
  EXPECT_EQ(*a, 3);
  EXPECT_EQ(*b, 4);

  auto [c, d] = bst.equal_range(3, Preorder());
  EXPECT_EQ(*c, 3);
  EXPECT_EQ(*d, 6);

  auto [e, f] = bst.equal_range(3, Postorder());
  EXPECT_EQ(*e, 3);
  EXPECT_EQ(*f, 2);
}