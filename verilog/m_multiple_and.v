module m_multiple_and();
  reg r_in1, r_in2, r_in3;
  wire w_t, w_out;
  initial r_in1 <= 1;
  initial r_in2 <= 1;
  initial r_in3 <= 0;
  m_AND_gate m1 (r_in1, r_in2, w_t);
  m_AND_gate m2 (w_t, r_in3, w_out);
  initial #1 $display("%d %d %d %d", r_in1, r_in2, r_in3, w_out);
endmodule
