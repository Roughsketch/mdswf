#ifndef _MDSWF_STRUCTURES_H
#define _MDSWF_STRUCTURES_H

#include <vector>
#include <cstdint>

#include "util.h"

namespace swf
{
  class SWFSection
  {
  public:
    virtual std::vector<uint8_t> raw() = 0;
  };

  struct RECORDHEADER : public SWFSection
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

    inline uint32_t size()
    {
      return extended() ? 6 : 2;
    }

    inline std::vector<uint8_t> raw()
    {
      std::vector<uint8_t> rawdata;

      rawdata.push_back(m_TagCodeAndLength & 0xFF);
      rawdata.push_back(m_TagCodeAndLength >> 8);

      if (extended())
      {
        util::push_int(rawdata, m_Length);
      }

      return rawdata;
    }
  private:
    uint16_t m_TagCodeAndLength;
    uint32_t m_Length;
  };

  struct RECT : public SWFSection
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

      std::copy(file.begin() + offset, file.begin() + offset + length(), std::back_inserter(m_raw));
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

    inline std::vector<uint8_t> raw()
    {
      return m_raw;
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
    std::vector<uint8_t> m_raw;
  };

  struct RGB : public SWFSection
  {
    RGB(std::vector<uint8_t>& file, size_t offset = 0)
    {
      m_red = file[offset];
      m_green = file[offset + 1];
      m_blue = file[offset + 2];
    }
  private:
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
  };

  struct RGBA : public SWFSection
  {
    RGBA(std::vector<uint8_t>& file, size_t offset = 0, bool argb = false)
    {
      if (argb)
      {
        m_alpha = file[offset];
        m_red = file[offset + 1];
        m_green = file[offset + 2];
        m_blue = file[offset + 3];
      }
      else
      {
        m_red = file[offset];
        m_green = file[offset + 1];
        m_blue = file[offset + 2];
        m_alpha = file[offset + 3];
      }
    }

    inline uint32_t get_rgba()
    {
      uint32_t rgba = m_red;

      rgba = rgba << 8;
      rgba |= m_green;
      rgba = rgba << 8;
      rgba |= m_blue;
      rgba = rgba << 8;
      rgba |= m_alpha;

      return rgba;
    }

    inline uint32_t get_argb()
    {
      uint32_t argb = m_alpha;

      argb = argb << 8;
      argb |= m_red;
      argb = argb << 8;
      argb |= m_green;
      argb = argb << 8;
      argb |= m_blue;

      return argb;
    }

    inline std::vector<uint8_t> raw()
    {
      return m_raw;
    }
  private:
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
    uint8_t m_alpha;
  };

  struct MATRIX : public SWFSection
  {
    MATRIX(std::vector<uint8_t>& file, size_t offset = 0)
    {

    }

    inline std::vector<uint8_t> raw()
    {
      return m_raw;
    }
  private:
  };

  struct GRADIENT : public SWFSection
  {
    GRADIENT(std::vector<uint8_t>& file, size_t offset = 0)
    {

    }

    inline std::vector<uint8_t> raw()
    {
      return m_raw;
    }
  private:
  };

  struct FOCALGRADIENT : public SWFSection
  {
    FOCALGRADIENT(std::vector<uint8_t>& file, size_t offset = 0)
    {

    }

    inline std::vector<uint8_t> raw()
    {
      return m_raw;
    }
  private:
  };

  struct FILLSTYLE : public SWFSection
  {
    FILLSTYLE(std::vector<uint8_t>& file, size_t offset = 0)
    {

    }

    inline std::vector<uint8_t> raw()
    {
      return m_raw;
    }
  private:
    uint8_t m_style_type;
    std::shared_ptr<RGB> m_rgb_color;
    std::shared_ptr<RGBA> m_rgba_color;
    std::shared_ptr<MATRIX> m_matrix;
    std::shared_ptr<GRADIENT> m_gradient;
    std::shared_ptr<FOCALGRADIENT> m_focal_gradient;
    uint16_t m_bitmap_id;
  };

  struct FILLSTYLEARRAY : public SWFSection
  {
    FILLSTYLEARRAY(std::vector<uint8_t>& file, size_t offset = 0)
    {

    }

    inline std::vector<uint8_t> raw()
    {
      return m_raw;
    }
  private:
    bool m_extended;
    uint16_t m_file_style_count;
    std::vector<FILLSTYLE> m_fill_styles;
  };

  struct LINESTYLEARRAY : public SWFSection
  {
    LINESTYLEARRAY(std::vector<uint8_t>& file, size_t offset = 0)
    {

    }

    inline std::vector<uint8_t> raw()
    {
      return m_raw;
    }
  private:
  };

  struct ENDSHAPERECORD : public SWFSection
  {
    ENDSHAPERECORD();
    inline std::vector<uint8_t> raw()
    {
      return std::vector<uint8_t>(1);
    }
  };

  struct STYLECHANGERECORD : public SWFSection
  {
    STYLECHANGERECORD(std::vector<uint8_t>& file, size_t offset)
    {

    }

    inline std::vector<uint8_t> raw()
    {
      return m_raw;
    }

  private:
    bool m_state_new_styles;
    bool m_state_line_style;
    bool m_state_fill_style_1;
    bool m_state_fill_style_0;
    bool m_state_move_to;


    std::vector<uint8_t> m_raw;
  };

  struct STRAIGHTEDGERECORD : public SWFSection
  {

  };

  struct CURVEDEDGERECORD : public SWFSection
  {

  };

  struct SHAPERECORD : public SWFSection
  {
    enum Type
    {
      End,
      Style,
      Straight,
      Curved
    };

    SHAPERECORD(std::vector<uint8_t>& file, size_t offset = 0)
    {
      //  If high 6 bits are 0 then it's an End record
      if ((file[offset] >> 2) == 0)
      {
        m_type = Type::End;
      }
      //  If it's not an end record and highest bit is 0, then it's a Style record
      else if ((file[offset] >> 7) == 0)
      {
        m_type = Type::Style;
      }
      //  If the highest 2 bits are 1 then it's a Straight Edge record
      else if ((file[offset] >> 6) == 3)
      {
        m_type == Type::Straight;
      }
      //  If nothing else fit then it's a Curved Edge record
      else
      {
        m_type == Type::Curved;
      }
    }

    inline std::vector<uint8_t> raw()
    {
      return m_raw;
    }
  private:
    Type m_type;
    std::shared_ptr<ENDSHAPERECORD> m_end;
    std::shared_ptr<STYLECHANGERECORD> m_style;
    std::shared_ptr<STRAIGHTEDGERECORD> m_straight;
    std::shared_ptr<CURVEDEDGERECORD> m_curved;

    std::vector<uint8_t> m_raw;
  };

  struct SHAPE : public SWFSection
  {
    SHAPE() {};
    SHAPE(std::vector<uint8_t>& file, size_t offset = 0)
    {
      m_num_fill_bits = util::read<uint32_t>(file, offset);
      m_num_line_bits = util::read<uint32_t>(file, offset + 4);

      offset += 8;

      //  While no end record found (Six 0 bytes)
      while (util::read<uint64_t>(file, offset) & 0xFFFFFF00 != 0)
      {
        SHAPERECORD record(file, offset);
        m_shape_records.push_back(record);

        offset += record.size();
      }
    }

  protected:
    uint32_t m_num_fill_bits;
    uint32_t m_num_line_bits;
    std::vector<SHAPERECORD> m_shape_records;
  };

  struct SHAPEWITHSTYLE : public SHAPE
  {
    SHAPEWITHSTYLE(std::vector<uint8_t>& file, size_t offset = 0)
    {

    }

    inline std::vector<uint8_t> raw()
    {
      return m_raw;
    }
  private:
    std::shared_ptr<FILLSTYLEARRAY> m_file_styles;
    std::shared_ptr<LINESTYLEARRAY> m_line_styles;
  };
}

#endif
