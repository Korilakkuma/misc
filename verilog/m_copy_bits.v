module m_copy_bits();
  wire [3:0] w_a = 4'b1101;
  initial #1 begin
    $display("{ 4{ w_a } }: %b", { 4{ w_a } });
    $display("{ 4{ w_a[2:0] } }: %b", { 4{ w_a[2:0] } });
  end
endmodule
