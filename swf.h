#ifndef _MDSWF_DECOMPRESS_H
#define _MDSWF_DECOMPRESS_H

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <fstream>
#include <memory>
#include <map>

#include "util.h"

#include "structures.h"
#include "tags.h"

namespace swf
{
  std::vector<uint8_t> decompress(std::string);

  struct SWFHeader
  {
  public:
    SWFHeader(std::vector<uint8_t>& file, size_t offset = 0)
    {
      m_signature = util::read(file, offset, 3);
      m_version = file[offset + 3];
      m_length = util::read<uint32_t>(file, offset + 4);
      m_framesize = std::make_shared<RECT>(file, offset + 8);
      m_framerate_fixed = util::read<uint16_t>(file, offset + 8 + m_framesize->length());
      m_framecount = util::read<uint16_t>(file, offset + 8 + m_framesize->length() + 2);

      if ((m_framerate_fixed & 0xFF) != 0)
      {
        m_framerate = static_cast<float>((m_framerate_fixed >> 8) / (m_framerate_fixed & 0xFF));
      }
      else
      {
        m_framerate = static_cast<float>(m_framerate_fixed >> 8);
      }
    }

    inline std::string signature()
    {
      return m_signature;
    }

    inline uint8_t version()
    {
      return m_version;
    }

    inline uint32_t file_size()
    {
      return m_length;
    }

    inline RECT frame_size()
    {
      return RECT(*m_framesize);
    }

    inline float frame_rate()
    {
      return m_framerate;
    }

    inline uint16_t frame_count()
    {
      return m_framecount;
    }

    inline uint32_t size()
    {
      return 12 + m_framesize->length();
    }

    friend std::ostream& operator<<(std::ostream& os, SWFHeader& header)
    {
      std::cout << "Signature:   " << header.signature() << std::endl;
      std::cout << "Version:     " << static_cast<int>(header.version()) << std::endl;
      std::cout << "File Length: " << header.file_size() << std::endl;
      std::cout << "Frame Size:  " << header.frame_size() << std::endl;
      std::cout << "Frame Rate:  " << header.frame_rate() << std::endl;
      std::cout << "Frame Count: " << static_cast<int>(header.frame_count());
      return os;
    }

  private:
    std::string m_signature;
    uint8_t m_version;
    uint32_t m_length;
    std::shared_ptr<RECT> m_framesize;
    uint16_t m_framerate_fixed;
    float m_framerate;
    uint16_t m_framecount;
  };

  class SWFFile
  {
  public:
    SWFFile(std::string filename)
    {
      m_content = util::read_file(filename);

      if (m_content[0] == 'C')
      {
        m_content = decompress(filename);
      }

      m_header = std::make_shared<SWFHeader>(m_content);

      for (uint32_t i = m_header->size(); i < m_content.size();)
      {
        Tag newtag(m_content, i);
        m_tags.push_back(newtag);

        std::cout << "Tag: " << newtag.tag_name() << std::endl;
        i += newtag.size();

      }
    }

    inline SWFHeader header()
    {
      return SWFHeader(*m_header);
    }

    inline void write(std::string out)
    {
      util::write_file(out, m_content);
    }

    friend std::ostream& operator<<(std::ostream& os, SWFFile& file)
    {
      std::cout << file.header();
      return os;
    }
  private:
    std::vector<uint8_t> m_content;
    std::shared_ptr<SWFHeader> m_header;
    std::vector<Tag> m_tags;
  };
}

#endif
