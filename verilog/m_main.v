`timescale 1ns/100ps
`default_nettype none
module m_main();
  reg r_clock = 0;
  initial #150 forever #50 r_clock = ~r_clock;

  reg [31:0] r_cc = 1;
  always@(posedge r_clock) r_cc <= r_cc + 1;

  initial #1000000 begin
    $display("Timeout");
    $finish;
  end

  m_simulator m (r_clock, r_cc);

  initial $dumpvars(0, m);
endmodule
