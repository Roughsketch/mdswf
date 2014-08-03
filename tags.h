#ifndef _MDSWF_TAGS_H
#define _MDSWF_TAGS_H

#include <cstdint>
#include <memory>
#include <vector>

#include "structures.h"

namespace swf
{
  class Tag
  {
  public:
    Tag(std::vector<uint8_t>& file, size_t offset = 0)
    {
      m_header = std::make_shared<RECORDHEADER>(file, offset);

      uint32_t data_offset = offset + m_header->size();
      std::copy(file.begin() + data_offset, file.begin() + data_offset + m_header->length(), std::back_inserter(m_data));
    }

    inline uint32_t size()
    {
      return m_header->size() + m_header->length();
    }

    inline uint32_t tag()
    {
      return m_header->code();
    }

    inline uint32_t tag_length()
    {
      return m_header->length();
    }
  private:
    std::shared_ptr<RECORDHEADER> m_header;
    std::vector<uint8_t> m_data;
  };
}

#endif
