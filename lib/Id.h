//==============================================================================
// Id • Add id property to a class
//==============================================================================
#pragma once
namespace uniq {

//======================================================================= Id
class Id{ 
  inline static map<string, Atomic<long>> Ids = {};
  inline static std::mutex mutex = {};
public:
  long id=0;

  Id(string group=""){ 
    lock_guard<std::mutex> lock(Id::mutex);
    if(group=="") 
      group = anyType(*this);
    id = ++Ids[group];
  };

  inline operator long() const { return id; }
  
  bool operator<(const Id &other) const { return id < other.id; };
  bool operator>(const Id &other) const { return id > other.id; };
  bool operator==(const Id &other) const { return id == other.id; };

  bool operator<(const long other) const { return id < other; };
  bool operator>(const long other) const { return id > other; };
  bool operator==(const long other) const { return id == other; };
};

/*/===================================================================== TEST(Id)
TEST(Id){
  Id a("test_id1") , b("test_id1");
  CHECK(a == 1L);
  CHECK(b == 2L);

  Id c("test_id2");
  CHECK(c.id == 1L);
}//*///

} // UniQ • Released under GPL 3 licence