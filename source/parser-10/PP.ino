
int16_t AY, DY, AX, DX;
long int SCY, SCX;
uint32_t stpdtx, stpdty, dtx, dty;
uint16_t stpdelx, stpdelx;
bool enabled = true; 
uint8_t recv;
message nw, pack;
motion mX(MXSTP, MXDIR, MXENA);
motion mY(MYSTP, MYDIR, MYENA);


void loop() {
  // =============RS485============
  if (Serial.available() and !pack.mready) {
    recv = Serial.read();
    if (recv == 0x50 or recv == 0x55) {
      nw.len = 0;
    }
    nw.add(recv);
    if ( nw.len > 3 and nw.len == ((nw.data[2] == 0) ? 8 : nw.data[2] + 5)) {
      //for (int i = 0; i < nw.len; i++) {
      pack = nw;
      //}
      pack.mready = true;
      pack.check();
      nw.len = 0;
    }
  }

  // =============PID============
  if (pack.mready and pack.type == 2) {
    AX = pack.getX();
    AY = pack.getY();
    SCX = (abs(AX) / TOSTEPX);
    SCY = (abs(AX) / TOSTEPX);
    if (SCX >= 200) stpdelX = 40;
    else if (SCX <= 10) stpdelX = 1000;
    else stpdelx = 10000 / SCX;
    
    if (SCY >= 200) stpdely = 40;
    else if (SCY <= 10) stpdely = 1000;
    else stpdely = 10000 / SCY;
  }

  // ============ step ============



}
