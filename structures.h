#include <vector>
#include <cstdint>

#include "util.h"

namespace swf
{
  struct RECORDHEADER
  {
    RECORDHEADER(std::vector<uint8_t>& file, size_t offset = 0)
    {
      /*
        Tag code and length are stored in one 16 bit integer

        High 10 bits are the tag code and low 6 bits are length

        If length == 0x3F then it is an extended record and the
        length is stored in a 32 bit integer following the tag
        code and length value.
      */
      
      m_TagCodeAndLength = util::read<uint16_t>(file, offset);

      if (extended())
      {
        m_Length = util::read<uint32_t>(file, offset + 2);
      }
    }

    inline bool extended()
    {
      //  If length is 0x3F then it is an extended record
      return (m_TagCodeAndLength & 0x3F) == 0x3F;
    }

    inline uint32_t length()
    {
      //  If extended then return full length, else lower 6 bits
      return extended() ? m_Length : m_TagCodeAndLength & 0x3F;
    }

    inline uint16_t code()
    {
      //  Tag code is high 10 bits
      return (m_TagCodeAndLength & 0xFFC0) >> 6;
    }
  private:
    uint16_t m_TagCodeAndLength;
    uint32_t m_Length;
  };

  struct RECT
  {
    RECT(std::vector<uint8_t>& file, size_t offset = 0)
    {
      m_nbits = file[offset] >> 3;
      m_bytes = m_nbits % 8 == 0 ? m_nbits / 8 : m_nbits / 8 + 1;

      uint8_t current_byte = file[offset];
      uint8_t bits_left = 3;

      m_xmin = util::read_bits<uint32_t>(file, m_nbits, bits_left, offset);
      m_xmax = util::read_bits<uint32_t>(file, m_nbits, bits_left, offset + m_bytes);
      m_ymin = util::read_bits<uint32_t>(file, m_nbits, bits_left, offset + m_bytes * 2);
      m_ymax = util::read_bits<uint32_t>(file, m_nbits, bits_left, offset + m_bytes * 3);
    }

    RECT(const RECT& o)
    {
      m_nbits = o.m_nbits;
      m_bytes = o.m_bytes;
      m_xmin = o.m_xmin;
      m_xmax = o.m_xmax;
      m_ymin = o.m_ymin;
      m_ymax = o.m_ymax;
    }

    inline uint8_t nbits()
    {
      return m_nbits;
    }

    inline uint32_t xmin()
    {
      return m_xmin;
    }

    inline uint32_t xmax()
    {
      return m_xmax;
    }

    inline uint32_t ymin()
    {
      return m_ymin;
    }

    inline uint32_t ymax()
    {
      return m_ymax;
    }

    inline uint32_t length()
    {
      return m_bytes * 4 + 1;
    }

    friend std::ostream& operator<<(std::ostream& os, RECT& rect)
    {
      std::cout << std::endl;
      std::cout << "\tNBits: " << static_cast<int>(rect.nbits()) << std::endl;
      std::cout << "\tX Min: " << rect.xmin() << std::endl;
      std::cout << "\tX Max: " << rect.xmax() << std::endl;
      std::cout << "\tY Min: " << rect.ymin() << std::endl;
      std::cout << "\tY Max: " << rect.ymax();
      return os;
    }

  private:
    uint8_t m_nbits;
    uint32_t m_xmin;
    uint32_t m_xmax;
    uint32_t m_ymin;
    uint32_t m_ymax;

    uint32_t m_bytes;
  };
}