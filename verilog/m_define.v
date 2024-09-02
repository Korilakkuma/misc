`define D_INTERVAL 999999
module m_define(w_clock, r_out);
  input wire w_clock;
  output reg r_out = 0;
  reg [31:0] r_count = 0;
  always@(posedge w_clock) begin
    r_count <= (r_count == `D_INTERVAL) ? 0 : r_count + 1;
    r_out   <= (r_count == 0) ? -r_out : r_out;
  end
endmodule
