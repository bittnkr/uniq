//==============================================================================
// Node • Tree parent/children node using shared_ptr
//==============================================================================
#pragma once
#include "uniq.h"
namespace uniq {

struct Node : public Named {
  any value;
  Node* parent = nullptr;
  vector<shared_ptr<Node>> children;

  Node(string name, any v) : Named(name), value(v) {}

  shared_ptr<Node> addChild(string name, any v) {
    shared_ptr<Node> child(make_shared<Node>(name, v));
    child->parent = this;
    children.push_back(child);
    return child;
  }

  vector<string> path() {
    vector<string> v;
    Node* n = this;
    while (n){
      v.push_back(n->name);
      n = n->parent;
    };
    reverse(v.begin(), v.end());
    return v;
  }

  // ~Node(){ log("~Node()", name); } 

  // shared_ptr<Node> operator[](size_t i) { return children[i]; }
  Node& operator[](size_t i) { return *children[i]; }
};

// ostream& operator<<(ostream& os, Node& t) { return os << t.name; }

//========================================================================= test
TEST(Node) {
  Node tree("root", 0);
  auto a = tree.addChild("a", 1);
  auto b = tree.addChild("b", 3.14);
  auto c = b->addChild("c", string("abc"));

  CHECK(tree.name == "root");
  CHECK(tree.children.size() == 2);
  CHECK(b->children.size() == 1);
  CHECK(c->children.size() == 0);
  CHECK(tree.children[0]->name == "a");
  CHECK((*tree.children[0].get()).name == "a");

  CHECK(any_cast<double>(tree.children[1]->value) > 3.1);
  CHECK(any_cast<string>(tree.children[1]->children[0]->value) == "abc");

  CHECK(join(c->path(),"/")=="root/b/c");
}
}  // namespace uniq
