module m_concat_bits();
  wire [3:0] w_a = 4'b1101;
  initial #1 begin
    $display("{ w_a, w_a }: %b", { w_a, w_a });
    $display("{ w_a, w_a, w_a }: %b", { w_a, w_a, w_a });
  end
endmodule
