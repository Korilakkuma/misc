module m_case(w_a, w_b, r_c);
  input wire w_a, w_b;
  output reg r_c;
  always@(*) case ({ w_a, w_b })
               2'b00  : r_c <= 0;
               2'b01  : r_c <= 1;
               2'b10  : r_c <= 0;
               default: r_c <= 1;
             endcase
endmodule
