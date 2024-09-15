module m_adder(w_in1, w_in2, w_out);
  input wire [31:0] w_in1, w_in2;
  output wire [31:0] w_out;
  assign w_out = w_in1 + w_in2;
endmodule
