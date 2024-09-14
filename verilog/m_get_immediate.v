module m_get_immediate(ir, i, s, b, u, j, imm);
  input wire [31:0] ir;
  input wire i, s, b, u, j;
  output wire [31:0] imm;
  assign imm = i ? { { 20{ ir[31] } }, ir[31:20] } :
               s ? { { 20{ ir[31] } }, ir[31:25], ir[11:7] } :
               b ? { { 20{ ir[31] } }, ir[7], ir[30:25], ir[11:8], 1'b0 } :
               u ? { ir[31:12], 12'b0 } :
               j ? { { 12{ ir[31] } }, ir[19:12], ir[20], ir[30:21], 1'b0 } : 0;
endmodule
