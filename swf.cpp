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

  std::string get_tag_name(uint32_t tag)
  {
    static std::map<uint32_t, std::string> tags =
    {
      { 0, "End" },
      { 1, "ShowFrame" },
      { 2, "DefineShape" },
      { 4, "PlaceObject" },
      { 5, "RemoveObject" },
      { 6, "DefineBits" },
      { 7, "DefineButton" },
      { 8, "JPEGTables" },
      { 9, "SetBackgroundColor" },
      { 10, "DefineFont" },
      { 11, "DefineText" },
      { 12, "DoAction" },
      { 13, "DefineFontInfo" },
      { 14, "DefineSound" },
      { 15, "StartSound" },
      { 17, "DefineButtonSound" },
      { 18, "SoundStreamHead" },
      { 19, "SoundStreamBlock" },
      { 20, "DefineBitsLossless" },
      { 21, "DefineBitsJPEG2" },
      { 22, "DefineShape2" },
      { 23, "DefineButtonCxform" },
      { 24, "Protect" },
      { 26, "PlaceObject2" },
      { 28, "RemoveObject2" },
      { 32, "DefineShape3" },
      { 33, "DefineText2" },
      { 34, "DefineButton2" },
      { 35, "DefineBitsJPEG3" },
      { 36, "DefineBitsLossless2" },
      { 37, "DefineEditText" },
      { 39, "DefineSprite" },
      { 41, "ProductInfo" },
      { 43, "FrameLabel" },
      { 45, "SoundStreamHead2" },
      { 46, "DefineMorphShape" },
      { 48, "DefineFont2" },
      { 56, "ExportAssets" },
      { 57, "ImportAssets" },
      { 58, "EnableDebugger" },
      { 59, "DoInitAction" },
      { 60, "DefineVideoStream" },
      { 61, "VideoFrame" },
      { 62, "DefineFontInfo2" },
      { 64, "EnableDebugger2" },
      { 65, "ScriptLimits" },
      { 66, "SetTabIndex" },
      { 69, "FileAttributes" },
      { 70, "PlaceObject3" },
      { 71, "ImportAssets2" },
      { 73, "DefineFontAlignZones" },
      { 74, "CSMTextSettings" },
      { 75, "DefineFont3" },
      { 76, "SymbolClass" },
      { 77, "Metadata" },
      { 78, "DefineScalingGrid" },
      { 82, "DoABC" },
      { 83, "DefineShape4" },
      { 84, "DefineMorphShape2" },
      { 86, "DefineSceneAndFrameLabelData" },
      { 87, "DefineBinaryData" },
      { 88, "DefineFontName" },
      { 89, "StartSound2" },
      { 90, "DefineBitsJPEG4" },
      { 91, "DefineFont4" }
    };

    if (tags.count(tag))
    {
      return tags[tag];
    }
    else
    {
      return "Invalid (" + std::to_string(tag) +")";
    }
  }
}