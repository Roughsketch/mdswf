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

    inline std::string tag_name()
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

      if (tags.count(tag()))
      {
        return tags[tag()];
      }
      else
      {
        return "Invalid (" + std::to_string(tag()) + ")";
      }
    }
  private:
    std::shared_ptr<RECORDHEADER> m_header;
    std::vector<uint8_t> m_data;
  };
}

#endif
