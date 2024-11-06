module m_pht(w_clock, w_ra, w_rd, w_wa, w_we, w_wd);
  input wire w_clock, w_we;
  input wire [4:0] w_ra, w_wa;
  input wire w_wd;

  output wire w_rd;

  reg [0:0] mem [0:31];

  assign w_rd = mem[w_ra];

  always @(posedge w_clock) if (w_we) mem[w_wa] <= w_wd;

  integer i;
  initial for (i = 0; i < 32; i = i + 1) mem[i] = 0;
endmodule
