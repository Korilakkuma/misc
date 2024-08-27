module m_if(w_a, w_b, r_c);
  input wire w_a, w_b;
  output reg r_c;
  always@(*) if ({ w_a, w_b } == 2'b00) r_c <= 0;
        else if ({ w_a, w_b } == 2'b01) r_c <= 1;
        else if ({ w_a, w_b } == 2'b10) r_c <= 0;
        else                            r_c <= 1;
endmodule
