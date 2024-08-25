module m_arithmetic_operators();
  wire [31:0] w_a = 8, w_b = 5;
  initial #1 $display("%d %d %d %d %d", (w_a + w_b), (w_a - w_b), (w_a * w_b), (w_a / w_b), (w_a % w_b));
endmodule
