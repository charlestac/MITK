MITK_CREATE_MODULE(
  SUBPROJECTS MITK-DTI
  INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS MitkConnectomics MitkQuantification MitkFiberTracking
  AUTOLOAD_WITH MitkCore
  PACKAGE_DEPENDS PUBLIC DCMTK
)

