#ifndef LAB_TEXT_HPP
#define LAB_TEXT_HPP

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <filesystem>
#include <string>
#include <map>

#include "lab/shaders.hpp"

class Text final
{
 public:
  Text(FT_Library & ft, const std::filesystem::path & font_path);
  Text(const Text &) = delete;
  ~Text();

  void render(const Shader::shared & shader, const std::string & text, float x, float y, float scale, const glm::vec3 & color);

 private:
  struct Character
  {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Offset to advance to next glyph
  };

  std::map< char, Character > _characters;
  unsigned int _VAO, _VBO;
};

#endif
