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
#ifndef __itkShCoefficientImageImporter_cpp
#define __itkShCoefficientImageImporter_cpp

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "itkShCoefficientImageImporter.h"
#include <itkImageRegionIterator.h>
#include <boost/math/special_functions.hpp>

using namespace boost::math;

namespace itk {

template< class PixelType, int ShOrder >
ShCoefficientImageImporter< PixelType, ShOrder >::ShCoefficientImageImporter()
    : m_Toolkit(FSL)
{
    m_ShBasis.set_size(ODF_SAMPLING_SIZE, (ShOrder+1)*(ShOrder+2)/2);
}

template< class PixelType, int ShOrder >
void ShCoefficientImageImporter< PixelType, ShOrder >
::GenerateData()
{
    CalcShBasis();
    if (m_InputImage.IsNull())
        return;

    Vector<float, 4> spacing4 = m_InputImage->GetSpacing();
    Point<float, 4> origin4 = m_InputImage->GetOrigin();
    Matrix<double, 4, 4> direction4 = m_InputImage->GetDirection();
    ImageRegion<4> imageRegion4 = m_InputImage->GetLargestPossibleRegion();

    Vector<double, 3> spacing3;
    Point<float, 3> origin3;
    Matrix<double, 3, 3> direction3;
    ImageRegion<3> imageRegion3;

    spacing3[0] = spacing4[0]; spacing3[1] = spacing4[1]; spacing3[2] = spacing4[2];
    origin3[0] = origin4[0]; origin3[1] = origin4[1]; origin3[2] = origin4[2];
    for (int r=0; r<3; r++)
        for (int c=0; c<3; c++)
            direction3[r][c] = direction4[r][c];
    imageRegion3.SetSize(0, imageRegion4.GetSize()[0]);
    imageRegion3.SetSize(1, imageRegion4.GetSize()[1]);
    imageRegion3.SetSize(2, imageRegion4.GetSize()[2]);

    m_OdfImage = OdfImageType::New();
    m_OdfImage->SetSpacing( spacing3 );
    m_OdfImage->SetOrigin( origin3 );
    m_OdfImage->SetDirection( direction3 );
    m_OdfImage->SetRegions( imageRegion3 );
    m_OdfImage->Allocate();
    Vector< PixelType, ODF_SAMPLING_SIZE > nullVec1; nullVec1.Fill(0.0);
    m_OdfImage->FillBuffer(nullVec1);

    m_CoefficientImage = CoefficientImageType::New();
    m_CoefficientImage->SetSpacing( spacing3 );
    m_CoefficientImage->SetOrigin( origin3 );
    m_CoefficientImage->SetDirection( direction3 );
    m_CoefficientImage->SetRegions( imageRegion3 );
    m_CoefficientImage->Allocate();
    Vector< PixelType, (ShOrder*ShOrder + ShOrder + 2)/2 + ShOrder > nullVec2; nullVec2.Fill(0.0);
    m_CoefficientImage->FillBuffer(nullVec2);

    int x = imageRegion4.GetSize(0);
    int y = imageRegion4.GetSize(1);
    int z = imageRegion4.GetSize(2);
    int numCoeffs = imageRegion4.GetSize(3);

    for (int a=0; a<x; a++)
        for (int b=0; b<y; b++)
            for (int c=0; c<z; c++)
            {
                vnl_matrix<double> coeffs((ShOrder*ShOrder + ShOrder + 2)/2 + ShOrder,1);

                typename InputImageType::IndexType index;
                index.SetElement(0,a);
                index.SetElement(1,b);
                index.SetElement(2,c);
                typename CoefficientImageType::PixelType pix;
                for (int d=0; d<numCoeffs; d++)
                {
                    index.SetElement(3,d);
                    pix[d] = m_InputImage->GetPixel(index);
                    coeffs[d][0] = pix[d];
                }
                typename CoefficientImageType::IndexType index2;
                index2.SetElement(0,a);
                index2.SetElement(1,b);
                index2.SetElement(2,c);
                m_CoefficientImage->SetPixel(index2, pix);

                typename OdfImageType::PixelType pix2;
                vnl_matrix<double> odf = m_ShBasis*coeffs;
                for (int d=0; d<ODF_SAMPLING_SIZE; d++)
                    pix2[d] = odf(d,0);

                m_OdfImage->SetPixel(index2,pix2);
            }
}

// generate spherical harmonic values of the desired order for each input direction
template< class PixelType, int ShOrder >
void ShCoefficientImageImporter< PixelType, ShOrder >
::CalcShBasis()
{
    vnl_matrix_fixed<double, 2, ODF_SAMPLING_SIZE> sphCoords = GetSphericalOdfDirections();
    int j, m; double mag, plm;

    for (int p=0; p<ODF_SAMPLING_SIZE; p++)
    {
        j=0;
        for (int l=0; l<=ShOrder; l=l+2)
            for (m=-l; m<=l; m++)
            {
                switch (m_Toolkit)
                {
                case FSL:
                    plm = legendre_p<double>(l,abs(m),cos(sphCoords(0,p)));
                    mag = sqrt((double)(2*l+1)/(4.0*M_PI)*factorial<double>(l-abs(m))/factorial<double>(l+abs(m)))*plm;
                    if (m<0)
                        m_ShBasis(p,j) = sqrt(2.0)*mag*cos(-m*sphCoords(1,p));
                    else if (m==0)
                        m_ShBasis(p,j) = mag;
                    else
                        m_ShBasis(p,j) = pow(-1.0, m)*sqrt(2.0)*mag*sin(m*sphCoords(1,p));
                    break;
                case MRTRIX:
                    plm = legendre_p<double>(l,abs(m),-cos(sphCoords(0,p)));
                    mag = sqrt((double)(2*l+1)/(4.0*M_PI)*factorial<double>(l-abs(m))/factorial<double>(l+abs(m)))*plm;
                    if (m>0)
                        m_ShBasis(p,j) = mag*cos(m*sphCoords(1,p));
                    else if (m==0)
                        m_ShBasis(p,j) = mag;
                    else
                        m_ShBasis(p,j) = mag*sin(-m*sphCoords(1,p));
                    break;
                }

                j++;
            }
    }
}

// convert cartesian to spherical coordinates
template< class PixelType, int ShOrder >
vnl_matrix_fixed<double, 2, ODF_SAMPLING_SIZE> ShCoefficientImageImporter< PixelType, ShOrder >
::GetSphericalOdfDirections()
{
    itk::OrientationDistributionFunction< PixelType, ODF_SAMPLING_SIZE > odf;
    vnl_matrix_fixed<double, 3, ODF_SAMPLING_SIZE>* dir = odf.GetDirections();
    vnl_matrix_fixed<double, 2, ODF_SAMPLING_SIZE> sphCoords;

    for (int i=0; i<ODF_SAMPLING_SIZE; i++)
    {
        double mag = dir->get_column(i).magnitude();

        if( mag<mitk::eps )
        {
            sphCoords(0,i) = M_PI/2; // theta
            sphCoords(1,i) = M_PI/2; // phi
        }
        else
        {
            sphCoords(0,i) = acos(dir->get(2,i)/mag); // theta
            sphCoords(1,i) = atan2(dir->get(1,i), dir->get(0,i)); // phi
        }
    }
    return sphCoords;
}

}

#endif // __itkShCoefficientImageImporter_cpp
