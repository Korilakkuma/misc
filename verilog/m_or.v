module m_or();
  reg r_in1, r_in2;
  wire w_out;
  initial r_in1 <= 0;
  initial r_in2 <= 0;
  initial #100 r_in1 <= 1;
  initial #100 r_in2 <= 0;
  initial #200 r_in1 <= 0;
  initial #200 r_in2 <= 1;
  initial #300 r_in1 <= 1;
  initial #300 r_in2 <= 1;
  m_OR_gate m (r_in1, r_in2, w_out);
  always@(*) #1 $display("%d %d %d", r_in1, r_in2, w_out);
endmodule
