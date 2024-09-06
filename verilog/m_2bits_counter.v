module m_2bits_counter(w_clock, r_count);
  input wire w_clock;
  output reg [1:0] r_count = 0;
  always@(posedge w_clock) r_count[0] <= ~r_count[0];
  always@(posedge w_clock) r_count[1] <= r_count[0] ^ r_count[1];
endmodule
