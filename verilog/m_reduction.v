module m_reduction();
  wire [7:0] w_a = 8'b11110101, w_b = 8'b11111111;
  initial #1 begin
    $display("%b %b %b", &w_a, |w_a, ^w_a);
    $display("%b %b %b", &w_b, |w_b, ^w_b);
  end
endmodule
