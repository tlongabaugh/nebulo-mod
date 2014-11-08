#include <cstdint>
#include <fstream>

inline void GetLE32(std::ifstream& file, int32_t* val)    
{ 
  uint8_t chars[4];
  file.read(reinterpret_cast<char*>(chars), 4);
  *val = (chars[0] << 0) + (chars[1] << 8) + (chars[2] << 16) + (chars[3] << 24);
}

inline void GetBE32(std::ifstream& file, int32_t* val) 
{ 
  uint8_t chars[4];
  file.read(reinterpret_cast<char*>(chars), 4);
  *val = (chars[3] << 0) + (chars[2] << 8) + (chars[1] << 16) + (chars[0] << 24);
}

inline void GetLE16(std::ifstream& file, int16_t* val)    
{ 
  uint8_t chars[2];
  file.read(reinterpret_cast<char*>(chars), 2);
  *val = (chars[0] << 0)  + (chars[1] << 8);
}

inline void GetBE16(std::ifstream& file, int16_t* val) 
{ 
  uint8_t chars[2];
  file.read(reinterpret_cast<char*>(chars), 2);
  *val = (chars[1] << 16) + (chars[0] << 24);
}

inline void PutLE32(std::ofstream& file, int32_t val)    
{ 
  file << static_cast<uint8_t>(((val & 0x000000FF)>> 0))
    << static_cast<uint8_t>(((val & 0x0000FF00)>> 8))
    << static_cast<uint8_t>(((val & 0x00FF0000)>>16))
    << static_cast<uint8_t>(((val & 0xFF000000)>>24));
}

inline void PutBE32(std::ofstream& file, int32_t val) 
{ 
  file << static_cast<uint8_t>(((val & 0xFF000000)>>24))
    << static_cast<uint8_t>(((val & 0x00FF0000)>>16))
    << static_cast<uint8_t>(((val & 0x0000FF00)>> 8))
    << static_cast<uint8_t>(((val & 0x000000FF)>> 0));
}

inline void PutLE16(std::ofstream& file, int16_t val)    
{ 
  file << static_cast<uint8_t>(((val & 0x00FF)>> 0))
    << static_cast<uint8_t>(((val & 0xFF00)>> 8));
}

inline void PutBE16(std::ofstream& file, int16_t val) 
{ 
  file << static_cast<uint8_t>(((val & 0xFF00)>> 8))
    << static_cast<uint8_t>(((val & 0x00FF)>> 0));
}