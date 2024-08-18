module m_high_impedance();
  reg r_a, r_b;
  wire w_c, w_d;
  assign w_c = r_a & r_b;
  initial begin
    r_a <= 0;
    #100 r_a <= 0; r_b <= 1;
    #100 r_a <= 1; r_b <= 0;
    #100 r_a <= 1; r_b <= 1;
  end
  always@(*) #1 $display("%d %d %d %d %d", $time, r_a, r_b, w_c, w_d);
endmodule
