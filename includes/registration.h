//==========================================================================
// Copyright 2016 Stig Viste, Norwegian University of Science and Technology
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
// http://opensource.org/licenses/MIT
// =========================================================================

#ifndef REGISTRATION_H_DEFINED
#define REGISTRATION_H_DEFINED

#include "itkImage.h"

// Image registration
#include "itkImageRegistrationMethodv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"

// Edge detection
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"

// Transform
#include "itkAffineTransform.h"
#include "itkCenteredSimilarity2DTransform.h"
#include "itkCenteredRigid2DTransform.h"
#include "itkCenteredTransformInitializer.h"

// Image I/O
#include "itkResampleImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkIdentityTransform.h"
#include "itkImageMaskSpatialObject.h"

// Image operations
#include "itkMedianImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkSquaredDifferenceImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

// Introduce a class that will keep track of the iterations
#include "itkCommand.h"
class CommandIterationUpdate : public itk::Command {
public:
  typedef  CommandIterationUpdate   Self;
  typedef  itk::Command             Superclass;
  typedef  itk::SmartPointer<Self>  Pointer;
  itkNewMacro( Self );

protected:
  CommandIterationUpdate() {};

public:
  typedef itk::RegularStepGradientDescentOptimizerv4<double>  OptimizerType;
  typedef const OptimizerType *                               OptimizerPointer;

  void Execute(itk::Object *caller, const itk::EventObject & event) ITK_OVERRIDE;
  void Execute(const itk::Object * object, const itk::EventObject & event) ITK_OVERRIDE;
};

// Instantiation of input images
const   unsigned int  Dimension = 2;
typedef float         PixelType;
typedef unsigned char CharPixelType;
typedef uint16_t      UintPixelType;

typedef itk::Image< PixelType, Dimension >                  ImageType;
typedef itk::Image< CharPixelType, Dimension >              CharImageType;
typedef itk::Image< UintPixelType, Dimension >              UintImageType;
typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<
                            ImageType,
                            ImageType >                     GradientFilterType;
typedef itk::MedianImageFilter<
                            ImageType,
                            ImageType >                     MedianFilterType;


// Instantiation of transform types
typedef itk::CenteredRigid2DTransform<
                            double >                        TransformRigidType;
typedef itk::CenteredTransformInitializer<
                            TransformRigidType,
                            ImageType,
                            ImageType >                     TransformRigidInitializerType;
typedef itk::CenteredSimilarity2DTransform<
                            double >                        TransformSimilarityType;
typedef itk::CenteredTransformInitializer<
                            TransformSimilarityType,
                            ImageType,
                            ImageType >                     TransformSimilarityInitializerType;
typedef itk::AffineTransform<
                            double,
                            Dimension >                     TransformAffineType;
typedef itk::CenteredTransformInitializer<
                            TransformAffineType,
                            ImageType,
                            ImageType >                     TransformAffineInitializerType;
typedef itk::CompositeTransform<
                            double,
                            Dimension >                     CompositeTransformType;

typedef itk::RegularStepGradientDescentOptimizerv4<
                            double>                         OptimizerType;
typedef itk::MeanSquaresImageToImageMetricv4<
                            ImageType,
                            ImageType >                     MetricType;

typedef itk::ImageRegistrationMethodv4<
                            ImageType,
                            ImageType,
                            TransformRigidType >            RegistrationRigidType;
typedef itk::ImageRegistrationMethodv4<
                            ImageType,
                            ImageType,
                            TransformSimilarityType >       RegistrationSimilarityType;
typedef itk::ImageRegistrationMethodv4<
                            ImageType,
                            ImageType,
                            TransformAffineType >           RegistrationAffineType;
typedef itk::SubtractImageFilter<
                            ImageType,
                            ImageType,
                            ImageType >                     DifferenceFilterType;
typedef itk::RescaleIntensityImageFilter<
                            ImageType,
                            ImageType >                     RescalerType;
typedef itk::ResampleImageFilter<
                            ImageType,
                            ImageType >                     ResampleFilterType;
typedef itk::ImageMaskSpatialObject<
                            Dimension >                     MaskType;
typedef itk:: LinearInterpolateImageFunction<
                            ImageType,
                            double >    										InterpolatorType;

// Image casting, because masking only supports unsigned char
typedef itk::CastImageFilter<
                            ImageType,
                            CharImageType >                 CastFilterType;


// Set up writer
typedef itk::ImageFileWriter<
                            ImageType >                     WriterType;
typedef itk::ImageFileWriter<
                            UintImageType >                 UintWriterType;

// Set up optimizer
typedef OptimizerType::ScalesType                           OptimizerScalesType;

// Generic handlers
RegistrationRigidType::Pointer registrationRigidContainer(
                            ImageType* const fixed,
                            ImageType* const moving,
                            OptimizerType::Pointer optimizer );
RegistrationSimilarityType::Pointer registrationSimilarityContainer(
                            ImageType* const fixed,
                            ImageType* const moving,
                            OptimizerType::Pointer optimizer );
RegistrationAffineType::Pointer registrationAffineContainer(
                            ImageType* const fixed,
                            ImageType* const moving,
                            OptimizerType::Pointer optimizer );
RegistrationRigidType::Pointer registrationMaskContainer(
                            ImageType* const fixed,
                            ImageType* const moving,
                            MetricType::Pointer metric,
                            OptimizerType::Pointer optimizer );
TransformRigidInitializerType::Pointer initializerRigidContainer(
                            ImageType* const fixed,
                            ImageType* const moving,
                            TransformRigidType::Pointer transform );
TransformSimilarityInitializerType::Pointer initializerSimilarityContainer(
                            ImageType* const fixed,
                            ImageType* const moving,
                            TransformSimilarityType::Pointer transform );
TransformAffineInitializerType::Pointer initializerAffineContainer(
                            ImageType* const fixed,
                            ImageType* const moving,
                            TransformAffineType::Pointer transform );
ResampleFilterType::Pointer resampleRigidPointer(
                            ImageType* const fixed,
                            ImageType* const moving,
                            TransformRigidType::Pointer transform );
ResampleFilterType::Pointer resampleSimilarityPointer(
                            ImageType* const fixed,
                            ImageType* const moving,
                            TransformSimilarityType::Pointer transform );
ResampleFilterType::Pointer resampleAffinePointer(
                            ImageType* const fixed,
                            ImageType* const moving,
                            TransformAffineType::Pointer transform );
DifferenceFilterType::Pointer diffFilter(
                            ImageType* const moving,
                            ResampleFilterType::Pointer resample );

// Image I/O
GradientFilterType::Pointer gradientFilter(
                            ImageType* const fixed,
                            int sigma );
CastFilterType::Pointer     castImage(
                            ImageType* const img );
ImageType::Pointer          medianFilter(
                            ImageType* const fixed,
                            int radius );

// Printing parameters
void finalRigidParameters( TransformRigidType::Pointer transform,
                      OptimizerType::Pointer optimizer);
void finalSimilarityParameters( TransformSimilarityType::Pointer transform,
                      OptimizerType::Pointer optimizer);
void finalAffineParameters( TransformAffineType::Pointer transform,
                      OptimizerType::Pointer optimizer);
void finalMaskParameters( TransformRigidType::Pointer transform,
                          RegistrationRigidType::Pointer registration,
                          OptimizerType::Pointer optimizer );

// Image registrations
TransformRigidType::Pointer registration1(
                            ImageType* const fixed,
                            ImageType* const moving );
ResampleFilterType::Pointer registration2(
                            ImageType* const fixed,
                            ImageType* const moving );
TransformAffineType::Pointer registration3(
                            ImageType* const fixed,
                            ImageType* const moving );
ResampleFilterType::Pointer registration4(
                            ImageType* const fixed,
                            ImageType* const moving,
                            CharImageType* const gradient );

#endif // REGISTRATION_H_DEFINED
