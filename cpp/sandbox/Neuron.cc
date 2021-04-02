#include "../libs/delegate.hpp"
#include "uniq.h"

class Neuron { 
 private:
  static int index;
  int id;
  int charge=0;

 public:
  Neuron() { id = ++index; }

  static constexpr int threshold = 100;

  using Pulse = delegate<void(int)>;

  Pulse fire;

  void pulse(int i) {
    charge += i;
    out("N", id, " charged to ", charge);
    if (charge >= threshold && fire) { 
      log(RED, " fire!");
      fire(charge - threshold / 2);
      charge = 0;
    } else log("");
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
// TEST(Neuron) {
int main() {
  Time t(CpuTime());

  Neuron n1, n2, n3;
  Terminate ground;

  n1.fire = Neuron::Pulse{n2, &Neuron::pulse};
  n2.fire = Neuron::Pulse{n3, &Neuron::pulse};
  n3.fire = Neuron::Pulse{ground, &Terminate::term};

  // // using std::bind to fire pulses
  // Neuron foo;
  // auto f3 = bind(&Neuron::fire, &foo, _2);
  // f3(5);

  constexpr auto threshold = Neuron::threshold;

  for (int i = 0; i < 8; i++) {
    log(GRN, "\nPulse ", i, ", ", threshold);
    n1.pulse(threshold);
  }

  log(RED, "time: ", t(CpuTime()));

  Neuron n4;
  n4.fire = Neuron::Pulse{ground, &Terminate::term};
  n3.fire = Neuron::Pulse{n4, &Neuron::pulse};

  for (int i = 0; i < 8; i++) {
    log(YEL,"\nPulse", i, ", ", threshold);
    n1.pulse(threshold);
  }
  log(GRN,"time: ", t(CpuTime()));

  // CHECK(bob.indent == alice.indent + 1);
  return 0;
}
// int main() { uniq::test_Neuron(); }