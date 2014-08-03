//  Originally created by Maiddog <maiddogsrl@gmail.com> 8/2/2014

#include <iostream>

#include "swf.h"

int main(int argc, char *argv[]) 
{
  if (argc != 2)
  {
    std::cout << "Invalid file." << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename(argv[1]);
  std::string outputname = boost::filesystem::path(filename).replace_extension("_decompressed.swf").string();

  swf::SWFFile file(filename);

  std::cout << file << std::endl;

  file.write(outputname);

  return EXIT_SUCCESS;
}