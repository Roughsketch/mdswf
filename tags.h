#ifndef _MDSWF_TAGS_H
#define _MDSWF_TAGS_H

#include <cstdint>
#include <memory>
#include <vector>

#include "structures.h"

namespace swf
{
  enum Tags
  {
    End = 0,
    ShowFrame = 1,
    DefineShape = 2,
    PlaceObject = 4,
    RemoveObject = 5,
    DefineBits = 6,
    DefineButton = 7,
    JPEGTables = 8,
    SetBackgroundColor = 9,
    DefineFont = 10,
    DefineText = 11,
    DoAction = 12,
    DefineFontInfo = 13,
    DefineSound = 14,
    StartSound = 15,
    DefineButtonSound = 17,
    SoundStreamHead = 18,
    SoundStreamBlock = 19,
    DefineBitsLossless = 20,
    DefineBitsJPEG2 = 21,
    DefineShape2 = 22,
    DefineButtonCxform = 23,
    Protect = 24,
    PlaceObject2 = 26,
    RemoveObject2 = 28,
    DefineShape3 = 32,
    DefineText2 = 33,
    DefineButton2 = 34,
    DefineBitsJPEG3 = 35,
    DefineBitsLossless2 = 36,
    DefineEditText = 37,
    DefineSprite = 39,
    ProductInfo = 41,
    FrameLabel = 43,
    SoundStreamHead2 = 45,
    DefineMorphShape = 46,
    DefineFont2 = 48,
    ExportAssets = 56,
    ImportAssets = 57,
    EnableDebugger = 58,
    DoInitAction = 59,
    DefineVideoStream = 60,
    VideoFrame = 61,
    DefineFontInfo2 = 62,
    EnableDebugger2 = 64,
    ScriptLimits = 65,
    SetTabIndex = 66,
    FileAttributes = 69,
    PlaceObject3 = 70,
    ImportAssets2 = 71,
    DefineFontAlignZones = 73,
    CSMTextSettings = 74,
    DefineFont3 = 75,
    SymbolClass = 76,
    Metadata = 77,
    DefineScalingGrid = 78,
    DoABC = 82,
    DefineShape4 = 83,
    DefineMorphShape2 = 84,
    DefineSceneAndFrameLabelData = 86,
    DefineBinaryData = 87,
    DefineFontName = 88,
    StartSound2 = 89,
    DefineBitsJPEG4 = 90,
    DefineFont4 = 91,
  };

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

    inline std::vector<uint8_t> raw()
    {
      std::vector<uint8_t> rawdata = m_header->raw();
      
      rawdata.insert(rawdata.end(), m_data.begin(), m_data.end());

      return rawdata;
    }

    inline std::vector<uint8_t> data()
    {
      return m_data;
    }

    inline void set_data(std::vector<uint8_t>& data)
    {
      m_data = std::vector<uint8_t>(data);
    }

    inline std::string tag_name()
    {
      static std::map<uint32_t, std::string> tags =
      { 
        { Tags::End, "End" },
        { Tags::ShowFrame, "ShowFrame" },
        { Tags::DefineShape, "DefineShape" },
        { Tags::PlaceObject, "PlaceObject" },
        { Tags::RemoveObject, "RemoveObject" },
        { Tags::DefineBits, "DefineBits" },
        { Tags::DefineButton, "DefineButton" },
        { Tags::JPEGTables, "JPEGTables" },
        { Tags::SetBackgroundColor, "SetBackgroundColor" },
        { Tags::DefineFont, "DefineFont" },
        { Tags::DefineText, "DefineText" },
        { Tags::DoAction, "DoAction" },
        { Tags::DefineFontInfo, "DefineFontInfo" },
        { Tags::DefineSound, "DefineSound" },
        { Tags::StartSound, "StartSound" },
        { Tags::DefineButtonSound, "DefineButtonSound" },
        { Tags::SoundStreamHead, "SoundStreamHead" },
        { Tags::SoundStreamBlock, "SoundStreamBlock" },
        { Tags::DefineBitsLossless, "DefineBitsLossless" },
        { Tags::DefineBitsJPEG2, "DefineBitsJPEG2" },
        { Tags::DefineShape2, "DefineShape2" },
        { Tags::DefineButtonCxform, "DefineButtonCxform" },
        { Tags::Protect, "Protect" },
        { Tags::PlaceObject2, "PlaceObject2" },
        { Tags::RemoveObject2, "RemoveObject2" },
        { Tags::DefineShape3, "DefineShape3" },
        { Tags::DefineText2, "DefineText2" },
        { Tags::DefineButton2, "DefineButton2" },
        { Tags::DefineBitsJPEG3, "DefineBitsJPEG3" },
        { Tags::DefineBitsLossless2, "DefineBitsLossless2" },
        { Tags::DefineEditText, "DefineEditText" },
        { Tags::DefineSprite, "DefineSprite" },
        { Tags::ProductInfo, "ProductInfo" },
        { Tags::FrameLabel, "FrameLabel" },
        { Tags::SoundStreamHead2, "SoundStreamHead2" },
        { Tags::DefineMorphShape, "DefineMorphShape" },
        { Tags::DefineFont2, "DefineFont2" },
        { Tags::ExportAssets, "ExportAssets" },
        { Tags::ImportAssets, "ImportAssets" },
        { Tags::EnableDebugger, "EnableDebugger" },
        { Tags::DoInitAction, "DoInitAction" },
        { Tags::DefineVideoStream, "DefineVideoStream" },
        { Tags::VideoFrame, "VideoFrame" },
        { Tags::DefineFontInfo2, "DefineFontInfo2" },
        { Tags::EnableDebugger2, "EnableDebugger2" },
        { Tags::ScriptLimits, "ScriptLimits" },
        { Tags::SetTabIndex, "SetTabIndex" },
        { Tags::FileAttributes, "FileAttributes" },
        { Tags::PlaceObject3, "PlaceObject3" },
        { Tags::ImportAssets2, "ImportAssets2" },
        { Tags::DefineFontAlignZones, "DefineFontAlignZones" },
        { Tags::CSMTextSettings, "CSMTextSettings" },
        { Tags::DefineFont3, "DefineFont3" },
        { Tags::SymbolClass, "SymbolClass" },
        { Tags::Metadata, "Metadata" },
        { Tags::DefineScalingGrid, "DefineScalingGrid" },
        { Tags::DoABC, "DoABC" },
        { Tags::DefineShape4, "DefineShape4" },
        { Tags::DefineMorphShape2, "DefineMorphShape2" },
        { Tags::DefineSceneAndFrameLabelData, "DefineSceneAndFrameLabelData" },
        { Tags::DefineBinaryData, "DefineBinaryData" },
        { Tags::DefineFontName, "DefineFontName" },
        { Tags::StartSound2, "StartSound2" },
        { Tags::DefineBitsJPEG4, "DefineBitsJPEG4" },
        { Tags::DefineFont4, "DefineFont4" },
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

  class DefineShapeTag
  {
  public:
    DefineShapeTag(Tag& t)
    {

    }
  private:
    std::shared_ptr<RECORDHEADER> m_header;
    uint16_t m_shape_id;
    std::shared_ptr<RECT> m_shape_bounds;
    std::shared_ptr<SHAPEWITHSTYLE> m_shapes;
  };
}

#endif
