#include "../libs/delegate.hpp"
#include "uniq.h"

class Neuron { 
 private:
  static int index;
  int id;
  int charge=0;

 public:
  Neuron() { id = ++index; }

  using Pulse = delegate<void(int)>;

  static constexpr int threshold = 100;

  Pulse fire;

  void pulse(int i) {
    charge += i;
    log("N", id, " charged to ", charge);
    if (charge >= threshold && fire) {
      // log("- fire!");
      fire(charge - threshold / 2);
      charge = 0;
    }
  };
};

int Neuron::index = 0;

struct Terminate {
  int charge=0;
  void term(int i) {
    charge += i;
    log("Termination, charge ", charge);
  }
};

// void say(string msg) { return Neuron().say(msg); }

//=============================================== test
// void test_Neuron() {
int main() {
  Time t(procTime());

  Neuron n1, n2, n3;
  Terminate ground;

  n1.fire = Neuron::Pulse{n2, &Neuron::pulse};
  n2.fire = Neuron::Pulse{n3, &Neuron::pulse};
  n3.fire = Neuron::Pulse{ground, &Terminate::term};

  // // using std::bind to fire pulses
  // Neuron foo;
  // auto f3 = bind(&Neuron::fire, &foo, _2);
  // f3(5);

  constexpr auto pulse = Neuron::threshold;

  for (int i = 0; i < 8; i++) {
    log(GRN, "\nPulse", i, ", ", pulse);
    n1.pulse(pulse);
  }

  log(RED, "time: ", t(procTime()));

  Neuron n4;
  n4.fire = Neuron::Pulse{ground, &Terminate::term};
  n3.fire = Neuron::Pulse{n4, &Neuron::pulse};

  for (int i = 0; i < 8; i++) {
    log(YEL,"\nPulse", i, ", ", pulse);
    n1.pulse(pulse);
  }
  log(RED,"time: ", t(procTime()));

  // CHECK(bob.indent == alice.indent + 1);
  return 0;
}
// int main() { uniq::test_Neuron(); }