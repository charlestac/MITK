/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "mitkIGTL3DImageDeviceSource.h"

#include "mitkIGTLMessage.h"

//Microservices
#include <usGetModuleContext.h>
#include <usModule.h>
#include <usServiceProperties.h>
#include <usModuleContext.h>

//itk
#include <itkCommand.h>

mitk::IGTL3DImageDeviceSource::IGTL3DImageDeviceSource()
  : mitk::IGTLDeviceSource()
{
  this->SetName("IGTLDeviceSource (2D Images)");
}

mitk::IGTL3DImageDeviceSource::~IGTL3DImageDeviceSource()
{
}

void mitk::IGTL3DImageDeviceSource::GenerateData()
{
  if (m_IGTLDevice.IsNull())
    return;

  /* update output with message from the device */
  IGTLMessage* msgOut = this->GetOutput();
  assert(msgOut);
  igtl::MessageBase::Pointer msgIn = dynamic_cast<igtl::MessageBase*>(m_IGTLDevice->GetNextImage3dMessage().GetPointer());
  if (msgIn.IsNotNull())
  {
    assert(msgIn);

    msgOut->SetMessage(msgIn);
    msgOut->SetName(msgIn->GetDeviceName());
  }
}
