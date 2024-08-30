module m_shift_registers(w_clock, w_in, w_out);
  input wire w_clock, w_in;
  output wire w_out;
  reg [3:0] r_shifts = 0;
  always@(posedge w_clock) r_shifts <= { w_in, r_shifts[3:1] };
  assign w_out = r_shifts[0];
endmodule

`timescale 1ns/100ps
module m_wave_viewer();
  reg r_clock = 0;
  initial #150 forever #50 r_clock = ~r_clock;
  wire w_out;
  reg r_in = 1;
  m_shift_registers m (r_clock, r_in, w_out);
  initial #700 $finish;
  initial $dumpvars(0, m);
endmodule
