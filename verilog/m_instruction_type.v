module m_instruction_type(opcode5, r, i, s, b, u, j);
  input wire [4:0] opcode5;
  output wire r, i, s, b, u, j;
  assign j = (opcode5 == 5'b11011);
  assign b = (opcode5 == 5'b11000);
  assign s = (opcode5 == 5'b01000);
  assign r = (opcode5 == 5'b01100);
  assign u = ((opcode5 == 5'b01101) || (opcode5 == 5'b00101));
  assign i = ~(j | b | s | r | u);
endmodule

module m_get_instruction_type();
  wire [4:0] opcode5 = 5'b01101;
  wire r, i, s, b, u, j;
  m_instruction_type m (opcode5, r, i, s, b, u, j);
  initial #100 $display("%b r %b i %b s %b b %b u %b j %b", opcode5, r, i, s, b, u, j);
endmodule
