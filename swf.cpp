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
}