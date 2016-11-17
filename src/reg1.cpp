// =========================================================================
// Copyright 2016 Stig Viste, Norwegian University of Science and Technology
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
// http://opensource.org/licenses/MIT
// =========================================================================

#include "registration.h"

// ===================================
// Image registration method 1 
// ===================================
ResampleFilterType::Pointer registration1( 
                                        ImageType* const fixed, 
                                        ImageType* const moving ){

  // Initialize parameters
  // TODO: Read parameters from config
  float angle   = 0.0;                          // Transform angle
  float lrate   = 0.1;                          // Learning rate
  float slength = 0.0001;                       // Minimum step length
  int   niter   = 200;                          // Number of iterations

  const unsigned int numberOfLevels = 1;        // 1:1 transform
  const double translationScale = 1.0 / 1000.0;

  // Optimizer and Registration containers
  OptimizerType::Pointer      optimizer     = OptimizerType::New();
  RegistrationType::Pointer registration = registrationContainer(
                                        fixed,
                                        moving,
                                        optimizer );

  // Construction of the transform object
  TransformType::Pointer  transform = TransformType::New();
  TransformInitializerType::Pointer initializer = initializerContainer(
                                        fixed,
                                        moving,
                                        transform );

  // Set parameters
  transform->SetAngle( angle );

  registration->SetInitialTransform( transform );
  registration->InPlaceOn();

  OptimizerScalesType optimizerScales( transform->GetNumberOfParameters() );

  optimizerScales[0] = 1.0;
  optimizerScales[1] = translationScale;
  optimizerScales[2] = translationScale;
  optimizerScales[3] = translationScale;
  optimizerScales[4] = translationScale;

  optimizer->SetScales(   optimizerScales  );
  optimizer->SetLearningRate(     lrate    );
  optimizer->SetMinimumStepLength( slength );
  optimizer->SetNumberOfIterations( niter  );

  // Create the command observer and register it with the optimizer
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  optimizer->AddObserver( itk::IterationEvent(), observer );

  // Optional: Shrinking and/or smoothing, set to 0
  RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
  shrinkFactorsPerLevel.SetSize( 1 );
  shrinkFactorsPerLevel[0] = 1;

  RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
  smoothingSigmasPerLevel.SetSize ( 1 );
  smoothingSigmasPerLevel[0] = 0;

  registration->SetNumberOfLevels(          numberOfLevels          );
  registration->SetSmoothingSigmasPerLevel( smoothingSigmasPerLevel );
  registration->SetShrinkFactorsPerLevel(   shrinkFactorsPerLevel   );

  // Start registration process
  try {
    registration->Update();
    std::cout << "Optimizer stop condition: "
              << registration->GetOptimizer()->GetStopConditionDescription()
              << std::endl;
  }
  catch( itk::ExceptionObject & err ){
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    exit(1);
  }

  // Resample new image
  ResampleFilterType::Pointer resample = resamplePointer(
                                        fixed,
                                        moving,
                                        transform );

  // Print results
  finalParameters( transform, optimizer );

  return resample;
};
