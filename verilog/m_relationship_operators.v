module m_relationship_operators();
  wire [31:0] w_a = 7, w_b = 8;
  initial #1 begin
    $display("%d %d %d %d", (w_a < w_b), (w_a <= w_b), (w_a > w_b), (w_a <= w_b));
    $display("%d %d", (w_a == w_b), (w_a != w_b));
  end
endmodule
