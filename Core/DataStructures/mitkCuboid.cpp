#include "mitkCuboid.h"
#include "vtkTransform.h"
#include "mitkVector.h"
#include "vtkCubeSource.h"

#include <vtkSTLReader.h>

mitk::Cuboid::Cuboid()
{
  m_Geometry3D->Initialize();
  vtkCubeSource* cube = vtkCubeSource::New();
  cube->SetXLength(2.0);
  cube->SetYLength(2.0);
  cube->SetZLength(2.0);
  SetVtkPolyData(cube->GetOutput());
  cube->Delete();
}

mitk::Cuboid::~Cuboid()
{

}

bool mitk::Cuboid::IsInside(ITKPoint3D worldPoint)
{
  // transform point from world to object coordinates
  ScalarType p[4];
  p[0] = worldPoint[0];
  p[1] = worldPoint[1];
  p[2] = worldPoint[2];
  p[3] = 1;
  m_Geometry3D->GetVtkTransform()->GetInverse()->TransformPoint(p, p);
	mitk::ITKPoint3D itkPoint;

  bool retval =(p[0] >= -1) && (p[0] <= 1) 
            && (p[1] >= -1) && (p[1] <= 1) 
            && (p[2] >= -1) && (p[2] <= 1);
  return retval;
}

mitk::ScalarType mitk::Cuboid::GetVolume()
{
  return   2 * m_Geometry3D->GetXAxis().GetNorm() * 2 * m_Geometry3D->GetYAxis().GetNorm() 
         * 2 * m_Geometry3D->GetZAxis().GetNorm();
}
