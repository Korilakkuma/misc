module m_time();
  initial #200 $display("%3d nsec", $time);
  initial begin
    #100 $display("%3d nsec", $time);
    #150 $display("%3d nsec", $time);
  end
endmodule
