// Dependable tests, that cannot be with original code because dependencies 
//==============================================================================
#include "test.h"
namespace uniq {

TEST(Queue){ // ========================================================== Queue
  Queue<int> q(64);
  vector<thread> threads;

  atomic<int> produced(0);
  auto producer = [&produced, &q](int N){
    int i = 0;
    while( ++i <= N && q.push(i) ) 
      produced += i;
    q.push(-1);
  };

  atomic<int> consumed(0);
  auto consumer = [&consumed, &q](){
    int v;
    while (q.pop(v) && v != -1)
      consumed += v;
  };

  auto t = CpuTime();

  for (int i = 0; i < thread::hardware_concurrency()/2; i++) {
    threads.push_back(thread(consumer));
    threads.push_back(thread(producer, 100000));
  };

  for (auto &t : threads) t.join();

  CHECK(produced != 0);
  CHECK(produced == consumed);
  // log("Queue:", double(CpuTime(t)));
}

TEST(OpenQueue){ // ================================================== OpenQueue
  OpenQueue<int> q(64);  
  vector<thread> threads;

  atomic<int> produced(0);
  auto producer = [&produced, &q](int N){
    int i = 0;
    while( ++i <= N && q.push(i) ) 
      produced++;
    q.push(-1);
  };

  atomic<int> consumed(0);
  auto consumer = [&consumed, &q](){
    int v;
    while (q.pop(v) && v != -1)
      consumed++;
  };

  auto t = CpuTime();

  for (int i = 0; i < thread::hardware_concurrency()/2; i++) {
    threads.push_back(thread(consumer));
    threads.push_back(thread(producer, 100'000));
  };
  for (auto &t : threads) t.join();

  CHECK(produced > 0);
  CHECK(produced == consumed);
  // log("OpenQueue:", double(t(CpuTime())));
}

TEST(Log){ //=============================================================== Log
  throw exception(); // to see an exception
  // Log say, err("cerr");
  // say("Hola!");
  // err("Olá");
  // log("To be or not?");
}

TEST(terminal) { // =================================================== terminal
  CHECK(RED+"X"+RST == "\033[31mX\033[0m");
  CHECK(colorcode(1)=="\033[22;31m");
  CHECK(colorcode(13)=="\033[22;37m");
  // for (auto i = 0; i < 20; i++)
  //   log(colorcode(i),i," ",replace(colorcode(i),"\033",""),RST);
}

TEST(numtypes){ // ==================================================== numtypes
  CHECK(sizeof(integer) == __WORDSIZE/CHAR_BIT);
  CHECK(rehash(u64(1)) == 0xfffefffefffeffff);
  CHECK(rehash(rehash(u64(1))) == 1);

  CHECK(i32(-4) + u32(2) == 4294967294);
  CHECK(i64(-4) + u32(2) == -2);
  CHECK(i64(-4) + u32(2) == 18446744073709551614UL);
}

TEST(utils) { // ========================================================= utils
  CHECK(sstr("a",1) == "a1");
  CHECK(trim(" a b\t\n") == "a b" );
  CHECK(tolower("ABC") == "abc" );
  CHECK(format("%d-%s",1,"a") == "1-a" );
  CHECK(join({"a","b","c"},"-") == "a-b-c" );
  CHECK(repeat("12",3) == "121212");
  CHECK(replace("a:=1",":=","=") == "a=1");
};

TEST(Time) { // =========================================================== Time
  CHECK(sizeof(Time) == sizeof(double));
  CHECK(u64(CLOCKS_PER_SEC) == MEGA);

  Time t;
  CHECK(t.str() == t.ctime());
}

TEST(Atomic){ //========================================================= Atomic
  Atomic<int> i; CHECK(i == 0);

  i = 1;   CHECK(i == 1);
  i = i+1; CHECK(i == 2);
  i = i-1; CHECK(i == 1);
  i |= 3;  CHECK(i == 3);
  i &= 1;  CHECK(i == 1);

  CHECK(i.CAS(1,2) && i == 2);
  CHECK(!i.CAS(3,1) && i == 2);

  CHECK(i++ == 2 && i == 3);
  CHECK(i-- == 3 && i == 2);
  CHECK(++i == 3);
  CHECK(--i == 2);
}

// ======================================================================= Actor
TEST(Actor){
  int X = 0;
  // increment X with a lambda call
  auto L = [&]{ X++; }; L(); CHECK(X==1);

  // increment X with the actor, using the same lambda
  Actor A(L); CHECK(A.running());
  A(); CHECK(X==2);

  // increment X with another actor and lambda
  Actor B([&]{ X=X+1; }); 
  B(); CHECK(X==3);

  // Actor<int>C(B); // passing a functor as the callable param copy constructor??
  // C(); CHECK(X==4);

  A.stop(); CHECK(!A.running());
};

//================================================================= TEST(State)
TEST(State) {
  State S;

  int ON = 0, OFF = 0, NONE = 0;
  S.on("off", [&] { OFF++; /*out(CYN,"off");*/ }, [&] { /*out(ORA, "off");*/ });
  S.on("on",  [&] { ON++; /*out(CYN,"on");*/ }, [&] { /*out(ORA, "on");*/ });
  S.on("none",[&] { NONE++; /*out(CYN,"none");*/ }, [&] { /*out(ORA, "none");*/ });

  CHECK(S["off"] && !S["on"] && !S["none"]);
  CHECK(OFF == 1); // side effect of transitions
  
  S("on"); // functor call changes the state
  CHECK(S["on"]); // bracket query the state returning true if match
  CHECK(S[1]); // can query by state id;
  CHECK(ON == 1); 
  CHECK(S.id("opz")==-1); // S.id() to query a state

  S("off");
  CHECK(OFF == 2);
  CHECK(S["off"]);
  CHECK(!S["on"]);

  S(2); // can call functor with stated id;
  CHECK(NONE == 1);
  CHECK(S[2]); CHECK(!S[1]);
  CHECK(S["none"]);
  CHECK(S == "none");
  CHECK(S == 2);

  CHECK(S.name()=="none");
  CHECK(S.name(1)=="on");
  CHECK_EXCEPTION(S["opz"]); // non existent state raises an exception
  CHECK(sstr(S)=="[none:2]"); // ostream operator
}//

}// uniq • Released under GPL 3.0 //*/