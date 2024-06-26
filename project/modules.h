#include "systemc.h"
#include "digit.h"

/*
Full Digit Register with a load
*/
SC_MODULE(reg)
{
    sc_in_clk clk;
    sc_in<NN_DIGIT> input;
    sc_in<bool> load;
    sc_out<NN_DIGIT> out;

    void reg_process();

    SC_CTOR(reg)
    {
        SC_CTHREAD(reg_process, clk.pos());
        // out.initialize(0);
    }
};

/*
Half Digit Register with a load
*/
SC_MODULE(half_digit_reg)
{
    sc_in_clk clk;
    sc_in<NN_HALF_DIGIT> input;
    sc_in<bool> load;
    sc_out<NN_HALF_DIGIT> out;

    void half_digit_reg_process();

    SC_CTOR(half_digit_reg)
    {
        SC_CTHREAD(half_digit_reg_process, clk.pos());
        out.initialize(0);
    }
};

/*
HW Multiplier
out = input1 * input2
*/
SC_MODULE(multiply)
{
    sc_in<NN_DIGIT> input1;
    sc_in<NN_DIGIT> input2;
    sc_out<NN_DIGIT> out;

    void multiply_process();

    SC_CTOR(multiply)
    {
        SC_METHOD(multiply_process);
        sensitive << input1 << input2;
    }
};

/*
HW subtractor
out = input1 - input2
*/
SC_MODULE(subtractor)
{
    sc_in<NN_DIGIT> input1;
    sc_in<NN_DIGIT> input2;
    sc_out<NN_DIGIT> out;

    void subtractor_process();

    SC_CTOR(subtractor)
    {
        SC_METHOD(subtractor_process);
        sensitive << input1 << input2;
    }
};

/*
HW Multiplexor
if control == 0, out = input1
if control == 1, out = input2
*/
SC_MODULE(multiplex)
{
    sc_in<NN_DIGIT> input1;
    sc_in<NN_DIGIT> input2;
    sc_in<bool> control;
    sc_out<NN_DIGIT> out;

    void multiplex_process();

    SC_CTOR(multiplex)
    {
        SC_METHOD(multiplex_process);
        sensitive << input1 << input2 << control;
    }
};

/*
HW comperator implimentation
input1 > input2, return 1
else return 0 <- equal or less returns 0
Here we are checking greater than tho so this is fine
*/
SC_MODULE(comperator)
{
    sc_in<NN_DIGIT> input1;
    sc_in<NN_DIGIT> input2;
    sc_out<bool> ls_Out;

    void comperator_process();

    SC_CTOR(comperator)
    {
        SC_METHOD(comperator_process);
        sensitive << input1 << input2;
    }
};

/*
HW HIGH_HALF inplementation
*/
SC_MODULE(hw_high_half)
{
    sc_in<NN_DIGIT> input;
    sc_out<NN_HALF_DIGIT> out;

    void hw_high_half_process();

    SC_CTOR(hw_high_half)
    {
        SC_METHOD(hw_high_half_process);
        sensitive << input;
    }
};

SC_MODULE(hw_Bhigh_half)
{
    sc_in<NN_DIGIT> input;
    sc_out<NN_DIGIT> out;

    void hw_Bhigh_half_process();

    SC_CTOR(hw_Bhigh_half)
    {
        SC_METHOD(hw_Bhigh_half_process);
        sensitive << input;
    }
};

/*
HW TO_HIGH_HALF implementation
*/
SC_MODULE(hw_to_high_half)
{
    sc_in<NN_DIGIT> input;
    sc_out<NN_DIGIT> out;

    void hw_to_high_half_process();

    SC_CTOR(hw_to_high_half)
    {
        SC_METHOD(hw_to_high_half_process);
        sensitive << input;
    }
};

/*
HW LOW HALF Implementation
*/
SC_MODULE(hw_low_half)
{
    sc_in<NN_DIGIT> input;
    sc_out<NN_HALF_DIGIT> out;

    void hw_low_half_process();

    SC_CTOR(hw_low_half)
    {
        SC_METHOD(hw_low_half_process);
        sensitive << input;
    }
};

SC_MODULE(half_to_full)
{
    sc_in<NN_HALF_DIGIT> input;
    sc_out<NN_DIGIT> out;

    void half_to_full_process();

    SC_CTOR(half_to_full)
    {
        SC_METHOD(half_to_full_process);
        sensitive << input;
    }
};

/*
Bonus Module
*/
SC_MODULE(bonus)
{
    sc_in<NN_DIGIT> T0, T1, C;
    sc_in<NN_HALF_DIGIT> AH;
    sc_out<NN_DIGIT> T0new, T1new;
    sc_out<NN_HALF_DIGIT> AHnew;
    sc_in<bool> ready;
    sc_in_clk clock;
    void bonus_proc()
    {
        NN_DIGIT t[2], c;
        NN_HALF_DIGIT aHigh, cLow, cHigh;
        while (true)
        {
            if (ready.read() == true)
            { // "Required Part" finished? (need valid t[0], t[1])
                t[0] = T0.read();
                t[1] = T1.read();
                c = C.read();
                aHigh = AH.read();
                cHigh = (NN_HALF_DIGIT)HIGH_HALF(c);
                cLow = (NN_HALF_DIGIT)LOW_HALF(c);
                while ((t[1] > cHigh) || ((t[1] == cHigh) && (t[0] >= TO_HIGH_HALF(cLow))))
                {
                    if ((t[0] -= TO_HIGH_HALF(cLow)) > MAX_NN_DIGIT - TO_HIGH_HALF(cLow))
                        t[1]--;
                    t[1] -= cHigh;
                    aHigh++;
                }
                T0new.write(t[0]);
                T1new.write(t[1]);
                AHnew.write(aHigh);
            }
            wait();
        }
    }
    SC_CTOR(bonus) { SC_CTHREAD(bonus_proc, clock.pos()); }
};
