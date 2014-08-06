#include "swf.h"

namespace swf
{
  std::vector<uint8_t> decompress(std::string filename)
  {
    std::vector<uint8_t> contents(8);

    std::ifstream file(filename, std::ios_base::in | std::ios_base::binary);

    std::string start(8, ' ');
    file.read(&start[0], 8);

    if (boost::starts_with(start, "CWS") == false)
    {
      return std::vector<uint8_t>(0);
    }

    contents[0] = 'F';
    std::copy(start.begin() + 1, start.end(), contents.begin() + 1);

    file.seekg(8, std::ios::beg);

    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::zlib_decompressor());
    in.push(file);
    boost::iostreams::copy(in, std::back_inserter(contents));

    file.close();

    return contents;
  }

  void SWFFile::write(std::string out)
  {
    std::vector<uint8_t> swfdata = m_header->raw();

    for (auto& tag : m_tags)
    {
      auto tagdata = tag.raw();
      std::copy(tagdata.begin(), tagdata.end(), std::back_inserter(swfdata));
    }

    util::write_file(out, swfdata);
  }

  void SWFFile::randomize()
  {
    std::random_device rd;
    std::mt19937 random;
    std::uniform_int_distribution<int> dist(0, 0xFF);

    random.seed(rd());

    for (auto& swftag : m_tags)
    {
      if (swftag.tag() != Tags::DefineShape)
      {
        continue;
      }

      auto data = swftag.data();

      for (int i = 20; i < data.size(); i += 50)
      {
        data[i] = dist(random);
      }

      swftag.set_data(data);
    }
  }
}