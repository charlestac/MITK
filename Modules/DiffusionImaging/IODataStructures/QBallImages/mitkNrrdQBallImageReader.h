/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Language:  C++
Date:      $Date: 2007-08-24 13:35:59 $
Version:   $Revision: 1.14 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mitkNrrdQBallImageReader_h
#define __mitkNrrdQBallImageReader_h

#include "mitkCommon.h"
#include "itkVectorContainer.h"
#include "mitkFileReader.h"
#include "vnl/vnl_vector_fixed.h"
#include "mitkQBallImage.h"
#include "mitkQBallImageSource.h"
#include "itkVectorImage.h"

namespace mitk
{

  /** \brief 
  */

  class NrrdQBallImageReader : public mitk::QBallImageSource, public FileReader
  {
  public:

    typedef mitk::QBallImage OutputType;
    typedef mitk::QBallImageSource QBImgSourceType;
 
    mitkClassMacro( NrrdQBallImageReader, QBImgSourceType );
    itkNewMacro(Self);

    const char* GetFileName() const;
    void SetFileName(const char* aFileName);
    const char* GetFilePrefix() const;
    void SetFilePrefix(const char* aFilePrefix);
    const char* GetFilePattern() const;
    void SetFilePattern(const char* aFilePattern);

    static bool CanReadFile(const std::string filename, const std::string filePrefix, const std::string filePattern);

  protected:

    /** Does the real work. */
    virtual void GenerateData();
    virtual void GenerateOutputInformation();

    std::string m_FileName;
    std::string m_FilePrefix;
    std::string m_FilePattern;

  private:
    void operator=(const Self&); //purposely not implemented
  };

} //namespace MITK

#endif // __mitkNrrdQBallImageReader_h
