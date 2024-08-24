module m_multiplexer();
  reg r_a, r_b, r_c;
  wire w_d;
  // assign w_d = r_c ? r_b : r_a;
  assign w_d = (r_c & r_b) | (!r_c & r_a);
  initial begin
         r_c <= 0; r_a <= 0; r_b <= 0;
    #100 r_c <= 0; r_a <= 0; r_b <= 1;
    #100 r_c <= 0; r_a <= 1; r_b <= 0;
    #100 r_c <= 0; r_a <= 1; r_b <= 1;
    #100 r_c <= 1; r_a <= 0; r_b <= 1;
    #100 r_c <= 1; r_a <= 1; r_b <= 0;
    #100 r_c <= 1; r_a <= 1; r_b <= 1;
  end
  always@(*) #1 $display("%d nsec:  %b %b %b %b", $time, r_c, r_a, r_b, w_d);
endmodule
