module m_async_imem(w_addr, w_ir);
  input wire [31:0] w_addr;
  output wire [31:0] w_ir;
  assign w_ir = (w_addr == 0) ? { 7'd0, 5'd1, 5'd0, 3'd0, 5'd1, 7'b0110011 } :
                (w_addr == 4) ? { 7'd0, 5'd0, 5'd1, 3'd0, 5'd1, 7'b0110011 } :
                                { 7'd0, 5'd1, 5'd1, 3'd0, 5'd1, 7'b0110011 };
endmodule
