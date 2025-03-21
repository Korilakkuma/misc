module m_bimodal(w_clock, w_ra, w_prediction, w_wa, w_we, w_token);
  input wire w_clock, w_we;
  input wire [4:0] w_ra, w_wa;
  input wire w_token;

  output wire w_prediction;

  reg [1:0] mem [0:31];

  wire [1:0] w_data = mem[w_ra];

  assign w_prediction = w_data[1];

  reg [1:0] P1_counter = 0, P2_counter = 0;

  always @(posedge w_clock) { P1_counter, P2_counter } <= { w_data, P1_counter };
  always @(posedge w_clock) if (w_we)
    mem[w_wa] <= ((P2_counter < 3) &  w_token) ? (P2_counter + 1) :
                 ((P2_counter > 0) & !w_token) ? (P2_counter - 1) : P2_counter;

  integer i;
  initial for (i = 0; i < 32; i = i + 1) mem[i] = 1;
endmodule
