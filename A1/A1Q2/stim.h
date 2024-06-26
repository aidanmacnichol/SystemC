#include "systemc.h"
#include <iomanip>

SC_MODULE(stim)
{
  //Keep track of reset and x (different types)
  sc_out<float> x;
  sc_out<bool> reset;
  

  sc_in_clk Clk;

  void stimgen()
  {
    reset.write(true);
    wait();
    reset.write(false);
    x.write(true);
    wait();
    x.write(false);
    wait();

  }

  SC_CTOR(stim)
  {
    SC_CTHREAD(stimgen, Clk.pos());
  }
};