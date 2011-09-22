#ifndef VESUNIFORM_H
#define VESUNIFORM_H

#ifdef ANDROID
# include <GLES2/gl2.h>
# include <GLES2/gl2ext.h>
#else
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>
#endif

// C++ includes
#include <string>
#include <vector>

// VES includes
#include "vesGMTL.h"

// Forward declarations
class vesShaderProgram;

#ifndef GL_SAMPLER_1D
    #define GL_SAMPLER_1D               0x8B5D
    #define GL_SAMPLER_2D               0x8B5E
    #define GL_SAMPLER_3D               0x8B5F
    #define GL_SAMPLER_1D_SHADOW        0x8B61
    #define GL_SAMPLER_2D_SHADOW        0x8B62
#endif

template <typename DataType>
class vesUniformDataArray
{
public:

  typedef typename std::vector<DataType>::reference reference;

  vesUniformDataArray(int size)
  {
    m_data.resize(size);
  }

  DataType& operator[](const int index)
  {
    return m_data[index];
  }

  reference front()
  {
    return m_data.front();
  }

  std::vector<DataType> m_data;
};


class vesUniform
{
public:

  typedef vesUniformDataArray<GLint>   IntArray;
  typedef vesUniformDataArray<GLfloat> FloatArray;

  enum Type {
    Float       = GL_FLOAT,
    FloatVec2   = GL_FLOAT_VEC2,
    FloatVec3   = GL_FLOAT_VEC3,
    FloatVec4   = GL_FLOAT_VEC4,
    Int         = GL_INT,
    IntVec2     = GL_INT_VEC2,
    IntVec3     = GL_INT_VEC3,
    IntVec4     = GL_INT_VEC4,
    Bool        = GL_BOOL,
    BoolVec2    = GL_BOOL_VEC2,
    BoolVec3    = GL_BOOL_VEC3,
    BoolVec4    = GL_BOOL_VEC4,
    FloatMat2   = GL_FLOAT_MAT2,
    FloatMat3   = GL_FLOAT_MAT3,
    FloatMat4   = GL_FLOAT_MAT4,
    Sampler1D   = GL_SAMPLER_1D,
    Sampler2D   = GL_SAMPLER_2D,
    Sampler3D   = GL_SAMPLER_3D,
    SamplerCube = GL_SAMPLER_CUBE,

    Sampler1DShadow = GL_SAMPLER_1D_SHADOW,
    Sampler2DShadow = GL_SAMPLER_2D_SHADOW,

    Undefined = 0x0
  };

public:

           vesUniform();
  virtual ~vesUniform();


  /*! Set the type of glUniform, ensuring it is only set once.*/
  bool setType(Type t);

  /*! Get the type of glUniform as enum. */
  Type getType() const { return m_type; }

  /*! Set the name of the uniform, ensuring it is only set once.*/
  virtual void setName(const std::string& name);

  /*! Get the name of the uniform.*/
  virtual const std::string& name() const
  {
    return this->m_name;
  }

  /*! convenient scalar (non-array) constructors w/ assignment */
  explicit vesUniform(const std::string &name, float value);
  explicit vesUniform(const std::string &name, int value);
  explicit vesUniform(const std::string &name, bool value);

  vesUniform(const std::string &name, const vesVector2f &vector);
  vesUniform(const std::string &name, const vesVector3f &vector);
  vesUniform(const std::string &name, const vesMatrix3x3f &matrix);
  vesUniform(const std::string &name, const vesMatrix4x4f &matrix);

  /*! convenient scalar (non-array) value assignment */
  bool set(float value);
  bool set(int value);
  bool set(bool value);
  bool set(const vesVector2f &vector);
  bool set(const vesVector3f &vector);
  bool set(const vesMatrix3x3f &matrix);
  bool set(const vesMatrix4x4f &matrix);

  /*! convenient scalar (non-array) value query */
  bool get(float &value) const;
  bool get(int &value) const;
  bool get(bool &value) const;
  bool get(vesVector2f &vector) const;
  bool get(vesVector3f &vector) const;
  bool get(vesMatrix3x3f &matrix) const;
  bool get(vesMatrix4x4f &matrix) const;

  bool setElement(unsigned int index, float value);
  bool setElement(unsigned int index, int   value);
  bool setElement(unsigned int index, bool  value);
  bool setElement(unsigned int index, const vesVector2f &value);
  bool setElement(unsigned int index, const vesVector3f &value);
  bool setElement(unsigned int index, const vesMatrix3x3f &matrix);
  bool setElement(unsigned int index, const vesMatrix4x4f &matrix);

  bool getElement(unsigned int index, int &value)  const;
  bool getElement(unsigned int index, bool &value) const;
  bool getElement(unsigned int index, float &value) const;
  bool getElement(unsigned int index, vesVector2f &value) const;
  bool getElement(unsigned int index, vesVector3f &value) const;
  bool getElement(unsigned int index, vesMatrix3x3f &value) const;
  bool getElement(unsigned int index, vesMatrix4x4f &value) const;

  int getUniformLocation() const;

  void bind(vesShaderProgram *shaderProgram);

  void callGL() const;


protected:

  vesUniform& operator=(const vesUniform&);

  void setMinimalDefaults();

  bool isCompatibleType(Type type) const;

  inline bool isScalar() const { return m_numberElements==1; }

  unsigned int getInternalArrayNumberOfElements() const;
  int          getTypeNumberOfComponents(Type type) const;
  GLenum       getInternalArrayType(Type type) const;

  void allocateDataArray();

  Type                m_type;
  std::string         m_name;

  unsigned int        m_numberElements;
  int                 m_location;

  IntArray           *m_intArray;
  FloatArray         *m_floatArray;
};

#endif // VESUNIFORM_H