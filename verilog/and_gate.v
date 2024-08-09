module m_AND_gate(w_a, w_b, w_c);
  input  wire w_a, w_b;
  output wire w_c;
  assign w_c = w_a & w_b;
endmodule
