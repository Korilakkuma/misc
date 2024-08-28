module m_counter_circuit(w_clock, w_count);
  input  wire w_clock;
  output wire[1:0] w_count;
  reg [1:0] r_count = 0;
  always@(posedge w_clock) r_count <= w_count + 1;
  assign w_count = r_count;
endmodule

module m_clock();
  reg r_clock = 0;
  initial #150 forever #50 r_clock = ~r_clock;
  wire [1:0] w_count;
  m_counter_circuit m (r_clock, w_count);
  always@(posedge r_clock) #1
    $display("%3d %d %d", $time, w_count, m.r_count);
  initial #510 $finish;
endmodule
