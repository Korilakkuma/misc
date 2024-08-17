module m_finish();
  initial #200 $display("%d nsec", 200);
  initial begin
    #100 $display("%d nsec", 100);
    #150 $display("%d nsec", (100 + 150));
  end
  initial #210 $finish;
endmodule
