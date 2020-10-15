#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

using core::Shader;

Shader::Shader(const std::string& aVertexPath,
               const std::string& aFragmentPath)
  : mProgramId(0)
{
  // First, load the shader source into strings.
  std::string vertexCodeString;
  std::string fragmentCodeString;
  LoadShaderAsString(vertexCodeString, aVertexPath);
  LoadShaderAsString(fragmentCodeString, aFragmentPath);

  // Next, compile the shaders.
  unsigned int vertexID, fragmentID;

  CompileShader(vertexID, vertexCodeString, GL_VERTEX_SHADER);
  CompileShader(fragmentID, fragmentCodeString, GL_FRAGMENT_SHADER);

  // Finally, create and link the shader program.
  CreateProgram(vertexID, fragmentID);

  // Delete the shaders as they have been linked and are no longer needed.
  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);
}

void Shader::Activate() const
{
  glUseProgram(mProgramId);
}

void Shader::SetBool(const std::string& aName, bool aValue) const
{
  glUniform1i(glGetUniformLocation(mProgramId, aName.c_str()), (int)aValue);
}

void Shader::SetInt(const std::string& aName, int aValue) const
{
  glUniform1i(glGetUniformLocation(mProgramId, aName.c_str()), aValue);
}

void Shader::SetFloat(const std::string& aName, float aValue) const
{
  glUniform1f(glGetUniformLocation(mProgramId, aName.c_str()), aValue);
}

void Shader::LoadShaderAsString(std::string& aString,
                                const std::string& aShaderPath)
{
  std::ifstream shaderFile;
  std::stringstream shaderStream;

  // Try to open the given file.
  try
  {
    // Read in the shader source code.
    shaderFile.open(aShaderPath.c_str());
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    aString = shaderStream.str();
  }
  catch(std::ifstream::failure e)
  {
    std::cout << "Error loading shader!\n" << e.what() << std::endl;
  }
}

void Shader::CompileShader(unsigned int& aShaderID,
                           const std::string& aShaderSource,
                           GLenum aShaderType)
{
  const char* shaderCode = aShaderSource.c_str();

  // Error variables.
  int success;
  char infoLog[512];

  aShaderID = glCreateShader(aShaderType);
  glShaderSource(aShaderID, 1, &shaderCode, NULL);
  glCompileShader(aShaderID);

  // Check for compile errors.
  glGetShaderiv(aShaderID, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(aShaderID, 512, NULL, infoLog);
    std::cout << "Error compiling shader!\n" << infoLog << std::endl;
  }
}

void Shader::CreateProgram(unsigned int aVertexID,
                           unsigned int aFragmentID)
{
  mProgramId = glCreateProgram();
  glAttachShader(mProgramId, aVertexID);
  glAttachShader(mProgramId, aFragmentID);
  glLinkProgram(mProgramId);

  // Error variables.
  int success;
  char infoLog[512];

  // Check for linking errors.
  glGetProgramiv(mProgramId, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(mProgramId, 512, NULL, infoLog);
    std::cout << "Error linking program!\n" << infoLog << std::endl;
  }
}