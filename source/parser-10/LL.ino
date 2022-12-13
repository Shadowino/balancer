class message {
  public:
    // packet struct
    // 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
    // 50 xx xx AX AX AY AY AZ AZ CR CR 
    uint8_t type;
    uint8_t data[20];
    uint8_t len;
    bool mready;
    void add(uint8_t d) {
      data[len] = d;
      len++;
    }
    uint8_t check() {
      if (data[0] == 0x50 and data[2] == 0x06) type = 2;
      else if (data[0] == 0x50 and data[2] == 0x00) type = 1;
      else if (data[0] == 0x55) type = 3;
      else type = 0;
      return type;
    }

    int16_t getY() {
      if (type == 2) {
        return (data[6] | (data[5] << 8));
      } else {
        return (data[6] | (data[5] << 8));
      }

    }

    int16_t getX() {
      if (type == 2) {
        return (data[4] | (data[3] << 8));
      } else {
        return (data[4] | (data[3] << 8));
      }

    }

};

class motion {
  public:
    uint8_t STP;
    uint8_t DIR;
    uint8_t ENA;
    // uint32_t dt;
    uint32_t stpdt;
    uint32_t stpdel;
    motion(uint8_t _STP,uint8_t _DIR, uint8_t _ENA){
      STP = _STP; DIR = _DIR; ENA = _ENA;
    }
    void ini(){
      PORTD |= (1 << STP);
      PORTD |= (1 << DIR);
      PORTD |= (1 << ENA);
    }
    void check(uint32_t dt){
      // dt = micros();
      if (dt - stpdt > stpdel){
        stpdt = dt;
        if(PINB & (1<<STP)){
          PBoff(STP);
        }else{
          PBon(STP);
        }
      }
    }
    
    
};
