# ==========================
# A collection of unit tests
# ==========================

configure_file(globals.cpp.in globals.cpp @ONLY)

# Enable testing functionality
enable_testing()

# Add test cases
setup_test(test_weno)
setup_test(test_glog)
setup_test(test_absorber)
setup_test(test_yaml_read)

if (PVFMM_OPTION STREQUAL "ENABLE_PVFMM")
  setup_test(test_nbody)
endif()

if (${NVAPOR} EQUAL 0)
  setup_test(test_mesh)
endif()

if (${NVAPOR} EQUAL 1 AND ${NCLOUD} EQUAL 1 AND ${NPHASE_LEGACY} EQUAL 2)
  setup_test(test_moist_adiabat)
endif()

if (${NVAPOR} EQUAL 2)
  if (${NCLOUD} EQUAL 4)
    setup_test(test_impl)
    setup_test(test_air_parcel)
    setup_test(test_radiation)
    setup_test(test_microwave_opacity)
    setup_test(test_thermodynamics)
    setup_test(test_microphysics)
  elseif (${NCLOUD} EQUAL 5)
    setup_test(test_ammonium_hydrosulfide)
  endif()
endif()

# copy yaml files
file(GLOB inputs *.inp *.yaml)
foreach(input ${inputs})
    file(COPY ${input} DESTINATION ${CMAKE_BINARY_DIR}/tests)
endforeach()
