module m_clock();
  reg r_clock = 0;
  initial #150 forever #50 r_clock = ~r_clock;
  always@(*) $display("%3d: %d", $time, r_clock);
  initial #410 $finish;
endmodule
