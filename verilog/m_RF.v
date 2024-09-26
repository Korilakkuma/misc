module m_RF(w_clock, w_ra1, w_ra2, w_rd1, w_rd2, w_wa, w_we, w_wd);
  input wire w_clock, w_we;
  input wire [4:0] w_ra1, w_ra2, w_wa;
  input wire [31:0] w_wd;
  output wire [31:0] w_rd1, w_rd2;

  reg [31:0] mem [0:31];

  assign w_rd1 = (w_ra1 == 5'd0) ? 32'd0 : mem[w_ra1];
  assign w_rd2 = (w_ra2 == 5'd0) ? 32'd0 : mem[w_ra2];

  always@(posedge w_clock) if (w_we) mem[w_wa] <= w_wd;
  always@(posedge w_clock) if (w_we & (w_wa == 5'd30)) $finish;

  integer i;
  initial for (i = 0; i < 32; i = i + 1) mem[i] = 0;
endmodule
