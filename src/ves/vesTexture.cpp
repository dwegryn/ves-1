/*========================================================================
  VES --- VTK OpenGL ES Rendering Toolkit

      http://www.kitware.com/ves

  Copyright 2011 Kitware, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 ========================================================================*/

#include "vesTexture.h"
#include "vesShaderProgram.h"

// IMPORTANT: Make sure that this struct has no pointers.  All pointers should
// be put in the class declaration. For all newly defined pointers make sure
// to update constructor and destructor methods.
struct vesTextureInternal
{
  double value; // sample
};

static const GLfloat squareVertices[] = {
  -1.0f, -1.0f,
  1.0f, -1.0f,
  -1.0f,  1.0f,
  1.0f,  1.0f,
};

static const GLfloat textureVertices[] = {
  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f,  1.0f,
  0.0f,  0.0f,
};

vesTexture::vesTexture(vesShaderProgram *shader,
                       SFImage image)
{
  _internal = new vesTextureInternal();
  this->ShaderProgram = shader;
  this->Image = image;
  this->loaded = false;
}

vesTexture::~vesTexture()
{
  delete _internal;
}

void vesTexture::Render()
{

  if(!loaded)
  {
    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_2D,texID);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 this->Image.width,
                 this->Image.height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 this->Image.data);
    loaded = true;
  }
  this->ShaderProgram->Use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texID);

  // Set uniforms
  vesMatrix4x4f orthoProjection = vesOrtho(-1,1,-1,1,-1,1000);
  this->ShaderProgram->SetUniformMatrix4x4f("u_ortho",orthoProjection);

  // Set Attributes
  // Enable Vertex Attribs


  // Assign data
  glVertexAttribPointer(this->ShaderProgram->GetAttribute("a_position"),
                        2,
                        GL_FLOAT,
                        0,
                        0,
                        squareVertices);
  this->ShaderProgram->EnableVertexArray("a_position");
  glVertexAttribPointer(this->ShaderProgram->GetAttribute("a_texCoord"),
                        2,
                        GL_FLOAT,
                        0,
                        0,
                        textureVertices);
  this->ShaderProgram->EnableVertexArray("a_texCoord");
  // Draw arrays
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  // Disable vertex attributes
  this->ShaderProgram->DisableVertexArray("a_position");
  this->ShaderProgram->DisableVertexArray("a_texCoord");
}