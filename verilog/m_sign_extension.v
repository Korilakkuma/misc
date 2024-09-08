module m_sign_extension();
  wire [7:0] w_a = 8'b11111110;
  wire [31:0] w_b = { { 24{ w_a[7] } }, w_a };
  initial #1 $display("%b %b", w_a, w_b);

  wire signed [7:0] w_sa = 8'b11111110;
  wire signed [31:0] w_sb = { { 24{ w_sa[7] } }, w_sa };
  initial #1 $display("%d %d %b %b", w_sa, w_sb, w_sa, w_sb);
endmodule
