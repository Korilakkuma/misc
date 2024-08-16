module m_block();
  initial #200 $display("%d nsec", 200);
  initial begin
    #100 $display("%d nsec", 100);
    #150 $display("%d nsec", (100 + 150));
  end
endmodule
