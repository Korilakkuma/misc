module m_shift_operators();
  wire [7:0] w_a = 8'b11110101;
  wire [2:0] w_b = 3;

  wire signed [7:0] w_sa = 8'b10101010;
  wire signed [7:0] w_sb = 8'd4;

  initial #1 begin
    $display("logical shift %b", (w_a >> 0));
    $display("logical shift %b", (w_a >> 1));
    $display("logical shift %b", (w_a << 1));
    $display("logical shift %b", (w_a << w_b));
    $display("logical shift %b", (w_a >> w_b));

    $display("logical shift    ( 10101010 >>  4) %b", (w_sa >> w_sb));
    $display("arithmetic shift ( 10101010 >>> 4) %b", (w_sa >>> w_sb));
    $display("arithmetic shift ( 10101010 <<  4) %b", (w_sa << w_sb));
  end
endmodule
