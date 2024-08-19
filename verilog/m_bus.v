module m_bus();
  reg [3:0] r_a, r_b;
  wire [3:0] w_c;
  assign w_c = r_a | r_b;
  initial begin
    r_a <= 4'b1010;
    r_b <= 4'b1100;
  end
  always@(*) #1 $display("%3d %b %b %b", $time, r_a, r_b, w_c);
endmodule
