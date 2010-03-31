/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: 1.12 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef QmitkSPSAOptimizerViewWidgetHIncluded
#define QmitkSPSAOptimizerViewWidgetHIncluded

#include "ui_QmitkSPSAOptimizerControls.h"
#include "MitkRigidRegistrationUIExports.h"
#include "QmitkRigidRegistrationOptimizerGUIBase.h"
#include <itkArray.h>
#include <itkObject.h>
#include <itkImage.h>

/*!
* \brief Widget for rigid registration
*
* Displays options for rigid registration.
*/
class MITK_RIGIDREGISTRATION_UI_EXPORT QmitkSPSAOptimizerView : public QmitkRigidRegistrationOptimizerGUIBase
{

public:

  QmitkSPSAOptimizerView( QWidget* parent = 0, Qt::WindowFlags f = 0 );
  ~QmitkSPSAOptimizerView();


  virtual itk::Object::Pointer GetOptimizer();

  virtual itk::Array<double> GetOptimizerParameters();

  virtual void SetOptimizerParameters(itk::Array<double> metricValues);

  virtual void SetNumberOfTransformParameters(int transformParameters);

  virtual QString GetName();

  virtual void SetupUI(QWidget* parent);

protected:

  Ui::QmitkSPSAOptimizerControls m_Controls;

  int m_NumberTransformParameters;

};

#endif
