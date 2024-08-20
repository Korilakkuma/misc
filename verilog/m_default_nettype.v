`default_nettype none
module m_default_nettype();
  wire [3:0] w_a;
  assign w_a = 7;
  initial #1 $display("%d", w_a);
endmodule
