#include <GL/glew.h>
#include <fstream>
#include <iterator>
#include <iostream>

namespace gl {

  struct ShaderInData
  {
    GLenum type;
    std::string dir;
  };
  
  class Shader
  {
  public:
    Shader(void) = default;

    // initialize at construction
    Shader(const ShaderInData& data)
      : m_type(data.type)
      {
	std::string src;
	ExtractSource(src, data.dir);
        CreateShader(src);
      }

    void Init(const ShaderInData& data)
    {
      m_type = data.type;
      std::string src;
      ExtractSource(src, data.dir);
      CreateShader(src);
    }

    inline
    uint32_t ID(void) { return m_id; }
  private:
    void ExtractSource(std::string& src, const std::string& dir)
    {
      std::ifstream file(dir);
      VerifyFStream(file);
      src = std::string(
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>() );
    }

    /* checking if the file stream is ok */
    void VerifyFStream(std::ifstream& file, std::string& dir)
    {
      if(!file.open()) std::cerr << "unable to load : " << dir << "\n";
      std::cout << "succeeded to load : " << dir << "\n";
    }
    void CreateShader(const std::string& src)
    {
      m_id = glCreateShader(m_type);
      const char* srcs[1] = { src.c_str() };
      glShaderSource(m_id, 1, srcs, 0);
      glCompileShader(m_id);
    }
  private:
    union
    {
      GLenum m_type;
      uint32_t m_id;
    };
  };


  /* shader program */
  template<uint32_t _Shs> /* number of shaders */
  class Program
  {
  public:
    Program(void) = default;
    template<typename... _Sh>
      Program(_Sh&&... bases)
      : m_shaders { std::forward<_Sh>(bases)... }
      {
      }
  public:
      void Compile(void)
      {
	
      }
  private:
    Shader m_shaders[_Shs];
  };
  
}
