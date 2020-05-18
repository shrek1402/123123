typedef struct tCHS { // 20
  uint8_t Head     : 4;
  uint8_t Sector   : 6;
  uint8_t Cvlinder : 10;
};

typedef struct tLARGE { // 24
  uint8_t Head     : 8;
  uint8_t Sector   : 6;
  uint8_t Cvlinder : 10;
};

typedef struct tIDECHS { // 28
  uint8_t Head     : 4;
  uint8_t Sector   : 8;
  uint8_t Cvlinder : 16;
};

typedef uint32_t tLBA;
